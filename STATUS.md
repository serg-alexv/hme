# LEGION Status Ledger

_Last updated: 2026-07-16_

## Critical path

```text
LIVE/REPLAY INPUT
→ feature values visibly change
→ behavior state visibly changes
→ dragon motion visibly changes
→ raw and derived evidence is frozen
```

## Component status

| Component | Status | Evidence / note | Next gate |
|---|---|---|---|
| Windows WLAN acquisition | `VERIFIED` | Real session produced 207 observations | Re-capture and preserve raw trace |
| Direct RSSI handling | `VERIFIED` | Direct value observed separately from quality estimate | Prefer direct RSSI when available |
| Synthetic pipeline | `VERIFIED` | 180 observation/feature/organism records | Keep as regression fixture |
| Real feature pipeline | `VERIFIED` | 207 derived feature and organism frames were produced | Reproduce with complete artifact bundle |
| Original real raw trace | `EVIDENCE_MISSING` | Known output hash cannot reconstruct bytes | Run a new preserved experiment |
| Primitive Lens controller | `VERIFIED` | Inspector, Logger, and visible object control worked | Retain as smoke test |
| Rigged dragon import | `VERIFIED` | Skinned skeleton present; no embedded animation takes | Preserve bone map and textures |
| Automatic procedural dragon controller | `PACKAGED` | Recursive bone discovery and state motion package created | Runtime verification in target project |
| Lens Studio local TCP plug-in | `PACKAGED` | Python and JavaScript static checks passed | Verify listener and field updates in Preview |
| Live WLAN-driven dragon | `NOT_IMPLEMENTED` | Complete runtime chain not yet demonstrated | Execute end-to-end test |
| Recorded mobile replay Lens | `NOT_IMPLEMENTED` | Deployment strategy defined | Build after desktop causal proof |
| Camera Kit live host | `OPTIONAL_RESEARCH` | Requires host app, access, transport contract | Activate after replay validation and external demand |
| Native iOS product | `OPTIONAL_RESEARCH` | Strategic sovereignty branch | Activate when sensor access outranks Snap distribution |
| Lorenz/RK4 bone modulation | `OPTIONAL_RESEARCH` | Reference design only | Activate after basic state motion is validated |
| ESP32 / Wi-Fi CSI | `OPTIONAL_RESEARCH` | Candidate scientific moat | Activate only with funding, partner, or pilot demand |
| ADK trace audit | `REJECTED_FOR_CURRENT_PHASE` | Not needed in fast deterministic loop | Reconsider for offline interpretation/audit |
| ARCADIA integration | `REJECTED_FOR_CURRENT_PHASE` | Not a reliable current dependency | Reconsider only after source and value audit |
| Pub/Sub / Worker Pools / Spanner | `REJECTED_FOR_CURRENT_PHASE` | No production load or global-state need | Activate only when explicit scale gates are met |

## Current blocking issue

**There is no preserved end-to-end experiment proving the complete causal chain inside Lens Studio.**

## Next experiment

Create `EXP-0001-live-bridge-proof` with:

- raw input stream;
- emitted live-state frames;
- Lens Studio console log;
- screen recording with telemetry overlay and dragon;
- exact versions/configuration;
- manifest and SHA-256 inventory;
- pass/fail report.

## Release condition for Milestone 0

All must be `Y`:

- [ ] TCP listener starts on `127.0.0.1:19331`.
- [ ] test frame updates the intended ScriptComponent fields.
- [ ] sequence and freshness update visibly.
- [ ] CALM, ATTUNE, STORM, and STALE/SENSOR_LOSS are observable.
- [ ] corresponding dragon motion is visibly different.
- [ ] stopping the sender causes stale behavior.
- [ ] complete artifact bundle is frozen and verified.
