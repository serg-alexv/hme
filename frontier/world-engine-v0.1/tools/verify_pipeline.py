#!/usr/bin/env python3
"""Verify deterministic pose traces and isolate the first divergent field."""

from __future__ import annotations

import argparse
import hashlib
import json
import sys
from dataclasses import dataclass
from pathlib import Path
from typing import Any


EXPECTED_FRAMES = 720
EXPECTED_SHA256 = "d965aa4a6397378a9385073e0b69657d9eb049cf577e3912e2c15c1af6be173d"
EXPECTED_TRANSITIONS = [
    {"sequence": 0, "behavior": "CALM"},
    {"sequence": 187, "behavior": "ATTUNE"},
    {"sequence": 365, "behavior": "STORM"},
    {"sequence": 540, "behavior": "STALE"},
]


@dataclass(frozen=True)
class Trace:
    label: str
    path: Path
    payload: bytes
    frames: list[dict[str, Any]]
    sha256: str
    transitions: list[dict[str, Any]]


def load_trace(spec: str) -> Trace:
    label, separator, raw_path = spec.partition("=")
    if not separator or not label or not raw_path:
        raise ValueError(f"trace must use LABEL=PATH syntax: {spec!r}")
    path = Path(raw_path)
    payload = path.read_bytes()
    if not payload.endswith(b"\n"):
        raise ValueError(f"{label}: trace must end with a newline")

    frames: list[dict[str, Any]] = []
    for line_number, line in enumerate(payload.splitlines(), start=1):
        try:
            frame = json.loads(line)
        except json.JSONDecodeError as error:
            raise ValueError(f"{label}: invalid JSON on line {line_number}: {error}") from error
        if frame.get("schema") != "world.pose/0.1":
            raise ValueError(f"{label}: unsupported schema on line {line_number}")
        if frame.get("sequence") != line_number - 1:
            raise ValueError(f"{label}: non-monotonic sequence on line {line_number}")
        frames.append(frame)

    transitions: list[dict[str, Any]] = []
    previous: str | None = None
    for frame in frames:
        behavior = frame.get("behavior")
        if behavior != previous:
            transitions.append({"sequence": frame["sequence"], "behavior": behavior})
            previous = behavior

    return Trace(
        label=label,
        path=path,
        payload=payload,
        frames=frames,
        sha256=hashlib.sha256(payload).hexdigest(),
        transitions=transitions,
    )


def first_field_difference(left: Any, right: Any, path: str = "$") -> tuple[str, Any, Any] | None:
    if type(left) is not type(right):
        return path, left, right
    if isinstance(left, dict):
        for key in sorted(set(left) | set(right)):
            child_path = f"{path}.{key}"
            if key not in left:
                return child_path, "<missing>", right[key]
            if key not in right:
                return child_path, left[key], "<missing>"
            difference = first_field_difference(left[key], right[key], child_path)
            if difference:
                return difference
        return None
    if isinstance(left, list):
        for index, (left_value, right_value) in enumerate(zip(left, right)):
            difference = first_field_difference(left_value, right_value, f"{path}[{index}]")
            if difference:
                return difference
        if len(left) != len(right):
            return f"{path}.length", len(left), len(right)
        return None
    return None if left == right else (path, left, right)


def validate_trace(trace: Trace) -> list[str]:
    errors: list[str] = []
    if len(trace.frames) != EXPECTED_FRAMES:
        errors.append(f"{trace.label}: expected {EXPECTED_FRAMES} frames, got {len(trace.frames)}")
    if trace.transitions != EXPECTED_TRANSITIONS:
        errors.append(
            f"{trace.label}: transitions differ; expected {EXPECTED_TRANSITIONS}, got {trace.transitions}"
        )
    return errors


def compare(reference: Trace, candidate: Trace) -> dict[str, Any] | None:
    if reference.payload == candidate.payload:
        return None
    limit = min(len(reference.frames), len(candidate.frames))
    for index in range(limit):
        if reference.frames[index] != candidate.frames[index]:
            difference = first_field_difference(reference.frames[index], candidate.frames[index])
            field, reference_value, candidate_value = difference or ("$", "<byte drift>", "<byte drift>")
            return {
                "sequence": index,
                "field": field,
                "reference_value": reference_value,
                "candidate_value": candidate_value,
            }
    return {
        "sequence": limit,
        "field": "$.frame_count",
        "reference_value": len(reference.frames),
        "candidate_value": len(candidate.frames),
    }


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--reference", required=True, help="authoritative trace as LABEL=PATH")
    parser.add_argument("--candidate", action="append", default=[], help="candidate trace as LABEL=PATH; repeatable")
    parser.add_argument("--report", type=Path, help="write a deterministic JSON verification report")
    parser.add_argument("--skip-golden-hash", action="store_true", help="do not require the committed v0.1 SHA-256")
    args = parser.parse_args()

    try:
        reference = load_trace(args.reference)
        candidates = [load_trace(spec) for spec in args.candidate]
    except (OSError, ValueError) as error:
        print(f"FAIL {error}", file=sys.stderr)
        return 2

    errors = validate_trace(reference)
    if not args.skip_golden_hash and reference.sha256 != EXPECTED_SHA256:
        errors.append(
            f"{reference.label}: SHA-256 drift; expected {EXPECTED_SHA256}, got {reference.sha256}"
        )

    first_divergence: dict[str, Any] | None = None
    for candidate in candidates:
        errors.extend(validate_trace(candidate))
        divergence = compare(reference, candidate)
        if divergence:
            divergence = {"candidate_label": candidate.label, **divergence}
            first_divergence = first_divergence or divergence
            errors.append(
                f"{candidate.label}: first divergence at sequence {divergence['sequence']} "
                f"field {divergence['field']}: {divergence['reference_value']!r} != "
                f"{divergence['candidate_value']!r}"
            )

    report = {
        "schema": "world.verification/0.1",
        "status": "FAIL" if errors else "PASS",
        "frames": len(reference.frames),
        "sha256": reference.sha256,
        "transitions": reference.transitions,
        "reference": reference.label,
        "candidates": [candidate.label for candidate in candidates],
        "first_divergence": first_divergence,
    }

    print(f"reference {reference.label}: {len(reference.frames)} frames sha256={reference.sha256}")
    for candidate in candidates:
        print(f"candidate {candidate.label}: {len(candidate.frames)} frames sha256={candidate.sha256}")
    print("transitions " + " -> ".join(
        f"{item['behavior']}@{item['sequence']}" for item in reference.transitions
    ))
    if errors:
        for error in errors:
            print(f"FAIL {error}", file=sys.stderr)
    else:
        print("PASS traces are byte-identical and match the WorldEngine v0.1 golden contract")

    if args.report:
        args.report.parent.mkdir(parents=True, exist_ok=True)
        args.report.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8", newline="\n")
    return 1 if errors else 0


if __name__ == "__main__":
    raise SystemExit(main())
