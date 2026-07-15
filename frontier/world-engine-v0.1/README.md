# WorldEngine — Frontier Pack v0.1

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
- **Rust kernel/monitor:** build- and test-verified mirror implementation.
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

- MSVC 19.44 C11 build with `/W4 /WX`.
- Rust 1.97 build and tests.
- deterministic replay, stale-input, stale-recovery, state-transition, input-dependency, and joint-limit tests.
- byte-identical 720-frame output from C and Rust.
- exact `60 Hz` fixture timestamps: `t0 + floor(1000 × sequence / 60)`.
- golden SHA-256: `d965aa4a6397378a9385073e0b69657d9eb049cf577e3912e2c15c1af6be173d`.

## Live network-load proof

Today's bounded investor proof is independent of the pending Lens gate:

```powershell
python tools/live_network_load.py --capacity-mbps 20
```

Open `http://127.0.0.1:8787/?live=1`. The browser maps measured aggregate Windows interface throughput to a monotonic procedural dragon posture envelope and preserves recent measured frames at `/api/trace`.

## Pending runtime gates

- Lens Studio golden-trace replay.
- live Windows WLAN → Rust → Lens capture.
- comparative study against direct-threshold, Lens `Math.sin`, and Lorenz/RK4 baselines.

## Truth boundary

The current real sensor is the host computer's connected-WLAN telemetry. It is not CSI, room-wide RF imaging, occupancy sensing, physiological sensing, or a complete electromagnetic-field instrument.
