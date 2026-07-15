"""Regression tests for the measured network-load adapter."""

from __future__ import annotations

import importlib.util
import unittest
from pathlib import Path
from unittest.mock import patch


MODULE_PATH = Path(__file__).with_name("live_network_load.py")
SPEC = importlib.util.spec_from_file_location("world_live_network_load", MODULE_PATH)
if SPEC is None or SPEC.loader is None:
    raise RuntimeError(f"Unable to load {MODULE_PATH}")
MODULE = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(MODULE)


class LoadSamplerTests(unittest.TestCase):
    def test_motion_regime_thresholds(self) -> None:
        sampler = MODULE.LoadSampler(capacity_mbps=20.0, interval_ms=250)
        self.assertEqual(sampler._classify(0.00), "CALM")
        self.assertEqual(sampler._classify(0.19), "CALM")
        self.assertEqual(sampler._classify(0.20), "ATTUNE")
        self.assertEqual(sampler._classify(0.54), "ATTUNE")
        self.assertEqual(sampler._classify(0.55), "STORM")
        self.assertEqual(sampler._classify(1.00), "STORM")

    def test_counter_delta_maps_to_declared_capacity(self) -> None:
        sampler = MODULE.LoadSampler(capacity_mbps=20.0, interval_ms=250)
        counters = iter([(1_000_000, 500_000), (1_500_000, 625_000)])
        clocks = iter([10.0, 10.25])

        with (
            patch.object(MODULE, "read_interface_bytes", side_effect=lambda: next(counters)),
            patch.object(MODULE.time, "perf_counter", side_effect=lambda: next(clocks)),
            patch.object(MODULE.time, "time", return_value=1_720_000_000.0),
        ):
            first = sampler.sample_once()
            second = sampler.sample_once()

        self.assertEqual(first["load"], 0.0)
        self.assertEqual(second["throughput_mbps"], 20.0)
        self.assertEqual(second["load"], 1.0)
        self.assertEqual(second["load_q16"], 65_535)
        self.assertEqual(second["behavior"], "STORM")
        self.assertRegex(second["frame_hash"], r"^[0-9a-f]{16}$")

    def test_invalid_frame_never_fabricates_measurements(self) -> None:
        frame = MODULE.LoadSampler(20.0, 250)._invalid("counter_unavailable")
        self.assertFalse(frame["valid"])
        self.assertIsNone(frame["throughput_mbps"])
        self.assertIsNone(frame["load"])
        self.assertIsNone(frame["load_q16"])
        self.assertEqual(frame["behavior"], "STALE")


if __name__ == "__main__":
    unittest.main()
