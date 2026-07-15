#!/usr/bin/env python3
"""Serve the World 2.0 demo with measured Windows interface load.

The adapter reads the aggregate byte counters exposed by ``netstat -e`` and
derives throughput from the counter delta.  It never reads packet payloads,
SSID, BSSID, account identity, or person identity.
"""

from __future__ import annotations

import argparse
import hashlib
import json
import re
import subprocess
import threading
import time
from collections import deque
from http import HTTPStatus
from http.server import SimpleHTTPRequestHandler, ThreadingHTTPServer
from pathlib import Path
from typing import Any


Q16_MAX = 65_535


def read_interface_bytes() -> tuple[int, int]:
    """Return aggregate received/sent interface byte counters on Windows."""
    completed = subprocess.run(
        ["netstat", "-e"],
        check=True,
        capture_output=True,
        text=True,
        timeout=3,
        creationflags=getattr(subprocess, "CREATE_NO_WINDOW", 0),
    )
    for line in completed.stdout.splitlines():
        values = re.findall(r"\d+", line)
        if len(values) == 2:
            return int(values[0]), int(values[1])
    raise RuntimeError("Windows interface byte counters were not found")


class LoadSampler:
    def __init__(self, capacity_mbps: float, interval_ms: int) -> None:
        self.capacity_mbps = capacity_mbps
        self.interval_s = interval_ms / 1000.0
        self.lock = threading.Lock()
        self.sequence = 0
        self.previous_total: int | None = None
        self.previous_time: float | None = None
        self.history: deque[dict[str, Any]] = deque(maxlen=360)
        self.latest = self._invalid("waiting_for_first_counter")

    def _invalid(self, reason: str) -> dict[str, Any]:
        return {
            "schema": "world.network-load/0.1",
            "sequence": self.sequence,
            "timestamp_unix_ms": int(time.time() * 1000),
            "source": "WINDOWS_INTERFACE_BYTES",
            "valid": False,
            "error": reason,
            "capacity_mbps": self.capacity_mbps,
            "throughput_mbps": None,
            "load": None,
            "load_q16": None,
            "behavior": "STALE",
        }

    def _classify(self, load: float) -> str:
        # This live proof exposes the current motion regime directly.  The
        # normative WorldEngine state machine applies separate dwell rules.
        return "STORM" if load >= 0.55 else "ATTUNE" if load >= 0.20 else "CALM"

    def sample_once(self) -> dict[str, Any]:
        now = time.perf_counter()
        timestamp_ms = int(time.time() * 1000)
        received, sent = read_interface_bytes()
        total = received + sent

        if self.previous_total is None or self.previous_time is None:
            throughput_bps = 0.0
            delta_s = self.interval_s
        else:
            delta_s = max(now - self.previous_time, 1e-6)
            throughput_bps = max(total - self.previous_total, 0) / delta_s

        self.previous_total = total
        self.previous_time = now
        throughput_mbps = throughput_bps * 8.0 / 1_000_000.0
        load = min(max(throughput_mbps / self.capacity_mbps, 0.0), 1.0)
        load_q16 = round(load * Q16_MAX)
        behavior = self._classify(load)

        core = {
            "schema": "world.network-load/0.1",
            "sequence": self.sequence,
            "timestamp_unix_ms": timestamp_ms,
            "source": "WINDOWS_INTERFACE_BYTES",
            "valid": True,
            "sample_period_ms": round(delta_s * 1000),
            "received_bytes": received,
            "sent_bytes": sent,
            "throughput_mbps": round(throughput_mbps, 6),
            "capacity_mbps": self.capacity_mbps,
            "load": round(load, 6),
            "load_q16": load_q16,
            "behavior": behavior,
        }
        canonical = json.dumps(core, sort_keys=True, separators=(",", ":")).encode()
        core["frame_hash"] = hashlib.sha256(canonical).hexdigest()[:16]
        self.sequence += 1
        return core

    def run(self) -> None:
        while True:
            started = time.perf_counter()
            try:
                frame = self.sample_once()
            except Exception as exc:  # preserve a visible invalid state
                frame = self._invalid(str(exc))
            with self.lock:
                self.latest = frame
                self.history.append(frame)
            remaining = self.interval_s - (time.perf_counter() - started)
            time.sleep(max(remaining, 0.01))

    def snapshot(self) -> dict[str, Any]:
        with self.lock:
            return dict(self.latest)

    def trace(self) -> list[dict[str, Any]]:
        with self.lock:
            return list(self.history)


def build_handler(demo_dir: Path, sampler: LoadSampler):
    class Handler(SimpleHTTPRequestHandler):
        def __init__(self, *args, **kwargs):
            super().__init__(*args, directory=str(demo_dir), **kwargs)

        def _json(self, value: Any) -> None:
            payload = json.dumps(value, separators=(",", ":")).encode()
            self.send_response(HTTPStatus.OK)
            self.send_header("Content-Type", "application/json; charset=utf-8")
            self.send_header("Content-Length", str(len(payload)))
            self.send_header("Cache-Control", "no-store")
            self.send_header("Access-Control-Allow-Origin", "*")
            self.end_headers()
            self.wfile.write(payload)

        def do_GET(self) -> None:  # noqa: N802 - stdlib API
            if self.path.split("?", 1)[0] == "/api/state":
                self._json(sampler.snapshot())
                return
            if self.path.split("?", 1)[0] == "/api/trace":
                self._json(sampler.trace())
                return
            super().do_GET()

        def log_message(self, format: str, *args: object) -> None:
            if not self.path.startswith("/api/state"):
                super().log_message(format, *args)

    return Handler


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--capacity-mbps", type=float, default=20.0)
    parser.add_argument("--interval-ms", type=int, default=250)
    parser.add_argument("--port", type=int, default=8787)
    args = parser.parse_args()
    if args.capacity_mbps <= 0:
        parser.error("--capacity-mbps must be positive")
    if args.interval_ms < 100:
        parser.error("--interval-ms must be at least 100")

    demo_dir = Path(__file__).resolve().parents[1] / "demo"
    sampler = LoadSampler(args.capacity_mbps, args.interval_ms)
    threading.Thread(target=sampler.run, name="network-load-sampler", daemon=True).start()
    server = ThreadingHTTPServer(("127.0.0.1", args.port), build_handler(demo_dir, sampler))
    print(f"World 2.0 live proof: http://127.0.0.1:{args.port}/?live=1")
    print(f"Declared capacity: {args.capacity_mbps:g} Mbps; sample interval: {args.interval_ms} ms")
    print("Press Ctrl+C to stop. Recent evidence: /api/trace")
    try:
        server.serve_forever()
    except KeyboardInterrupt:
        pass
    finally:
        server.server_close()


if __name__ == "__main__":
    main()
