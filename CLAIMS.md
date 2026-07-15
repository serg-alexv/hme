# LEGION Claims Register

## Rule

Public language must never exceed the strongest preserved evidence.

| Claim | Status | Allowed wording | Forbidden escalation |
|---|---|---|---|
| Real Windows WLAN data was acquired | `VERIFIED` | “A real connected-WLAN session produced 207 observations.” | “The system measured the room’s complete RF field.” |
| Derived features were computed | `VERIFIED` | “Strength, variability, novelty, validity, and freshness features were derived.” | “The system detected people, physiology, or spatial pose.” |
| Deterministic replay exists | `VERIFIED` | “Recorded inputs can drive repeatable serialized outputs under the tested implementation and configuration.” | “The entire visual output is universally bit-identical across all devices.” |
| Source provenance is explicit | `VERIFIED` | “Inputs are labeled SIMULATED, RECORDED_REPLAY, LIVE_WLAN, or STALE_WLAN.” | “The labels cryptographically prove physical origin.” |
| Rigged dragon can be procedurally controlled | `PACKAGED` | “A controller was created to map states to bounded bone motion.” | “The final live dragon is already operational.” |
| Lens Studio live bridge works | `PENDING_RUNTIME_TEST` | “A local TCP bridge is packaged and statically validated.” | “The end-to-end live bridge is verified.” |
| Wi-Fi CSI / RuView is integrated | `NOT_IMPLEMENTED` | “CSI is a separately gated research direction.” | “The current system performs Wi-Fi DensePose or vital-sign sensing.” |
| Lorenz RK4 drives the dragon | `NOT_IMPLEMENTED` | “Lorenz/RK4 modulation is a future motion research path.” | “Current motion is chaos-driven.” |
| ADK orchestrates the system | `NOT_IMPLEMENTED` | “ADK may later audit or interpret traces outside the fast loop.” | “ADK currently controls the creature.” |
| ARCADIA is the cognitive engine | `NOT_IMPLEMENTED` | “ARCADIA was evaluated as a possible source of ideas.” | “ARCADIA is an installed runtime substrate.” |
| The project is autonomous digital life | `NARRATIVE_ONLY` | “The project explores embodied environmental responsiveness.” | “The creature is conscious, alive, or sentient.” |
| Original core authorship | `VERIFIED_BY_PROJECT_RECORD` | “Mika is the original core author and principal architect.” | Any wording that demotes the founder to a non-authorial ‘idea person.’ |

## Measurement boundary

Current input:

```text
host computer’s active WLAN connected-link telemetry
```

Current input is not:

```text
Wi-Fi CSI
spectrum occupancy
RF noise floor
cellular telemetry
magnetic flux density
room-wide electromagnetic mapping
human pose
occupancy
respiration
heart rate
```

## Required claim review

Any PR that changes public wording, metrics, status, architecture maturity, safety, scientific validity, ownership, or funding assertions must update this register or explicitly state `CLAIMS IMPACT: NONE` with justification.
