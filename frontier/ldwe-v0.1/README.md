# LEGION Deterministic Wave Engine — Frontier Pack v0.1

This pack is the clean engineering baseline for a network-conditioned rigged dragon.

```text
observation
→ bounded feature frame
→ deterministic fixed-point engine
→ integer pose frame
→ Lens Studio renderer
→ evidence record
```

## Authoritative components

- **C kernel:** verified reference implementation and golden output.
- **Rust kernel/monitor:** source complete; workstation build pending.
- **Lens Studio:** pose rendering and bone application only.
- **JSONL pose stream:** engine/renderer boundary.

## Project primitives

1. `CausalFrame`
2. `StateMembrane`
3. `WaveCell`
4. `BodyChannel`
5. `PoseFrame`
6. `EvidenceChain`

These are project primitives, not claims of discovering new universal mathematics.

## Verified

- GCC 14.2.0 and Clang 17 builds with warnings as errors.
- deterministic replay, stale-input, and joint-limit tests.
- identical 720-frame output under both compilers.
- golden SHA-256: `3d197890e7bfc93a990a458e1c64eaa956f400162d0b7d5f35fde13663992f4d`.

## Pending runtime gates

- C/Rust byte equivalence on the workstation.
- Lens Studio golden-trace replay.
- live Windows WLAN → Rust → Lens capture.
- comparative study against direct-threshold, Lens `Math.sin`, and Lorenz/RK4 baselines.

## Truth boundary

The current real sensor is the host computer's connected-WLAN telemetry. It is not CSI, room-wide RF imaging, occupancy sensing, physiological sensing, or a complete electromagnetic-field instrument.
