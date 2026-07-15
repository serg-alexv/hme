# Build Report

**Status:** current verified build record

**Verified:** 2026-07-15

**Pack:** frontier v0.1
**C compiler:** MSVC 19.44.35228 for x64
**C flags:** `/std:c11 /W4 /WX`
**C tests:** passed
**C authoritative dynamics/configuration:** integer/fixed-point only; no runtime floating-point conversion
**Golden frames:** 720
**Golden SHA-256:** `d965aa4a6397378a9385073e0b69657d9eb049cf577e3912e2c15c1af6be173d`
**C/Rust golden equality:** passed byte for byte
**Fixture clock:** exact 60 Hz timestamps, `t0 + floor(1000 × sequence / 60)`
**JSON Schema:** passed for all 720 `world.pose/0.1` frames
**JavaScript syntax:** passed with Node.js 24.14.0
**Rust:** 1.97.0; three tests passed
**Live adapter:** three Python regression tests passed; live frame passed `world.network-load/0.1` schema validation
**Lens runtime:** not executed in this environment

## Golden transitions

```text
sequence 0   → CALM
sequence 187 → ATTUNE
sequence 365 → STORM
sequence 540 → STALE
```
