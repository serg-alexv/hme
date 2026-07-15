# Canonical system architecture

## 1. Research object

World 2.0 studies a portable transformation:

```text
bounded observation
→ declared features
→ deterministic state or direct bounded mapping
→ embodied output
→ inspectable evidence
```

The observation, transformation, and output must remain distinguishable. A renderer may present the result; it may not enlarge what the sensor measured.

## 2. Verified live surface

```text
Windows aggregate interface byte counters (`netstat -e`)
→ 250 ms byte deltas
→ measured throughput
→ load = min(1, throughput / declared capacity)
→ direct bounded browser pose
→ visible counters, state, angles, history, and frame hash
```

Schema: `world.network-load/0.1` for the measurement and `world.network-load-pose/0.1` for the browser presentation frame.

This surface proves that a measured host-network-load value changes the browser dragon pose. It does not identify Wi-Fi radio conditions, packet meaning, a person, or a location.

## 3. Verified conformance surface

```text
shared synthetic 720-frame feature schedule
        ├─ C11 fixed-point kernel
        └─ Rust fixed-point kernel
              ↓
       byte-identical `world.pose/0.1`
```

This surface verifies state ordering, dwell, stale recovery, bounded motion waves, serialization, and cross-language equality. It is not the process currently producing the live browser frames.

## 4. Current integration gap

The live measurement adapter and the fixed-point WorldEngine kernel are separate verification surfaces. The next integration gate is:

```text
world.network-load/0.1
→ declared feature adapter
→ WorldEngine
→ world.pose/0.1
→ browser and Lens consumers
```

Completion requires one preserved run showing raw counters, adapter output, engine pose, rendered motion, stale behavior, versions, and hashes.

## 5. Lens surface

The repository contains a packaged Lens Studio consumer and editor bridge. It has not been verified end to end in this repository state.

Planned path:

```text
verified pose stream
→ local Lens Studio bridge
→ named rig joints
→ bounded transforms and material parameters
```

Camera use, when added, is a separate spatial-registration function: floor detection, pose, anchor, and placement. Network telemetry does not provide room coordinates.

## 6. Runtime boundaries

| Component | Owns | Does not own |
|---|---|---|
| measurement adapter | acquisition, timestamps, validity, provenance | creature meaning or pose |
| feature adapter | declared normalization and error handling | hidden inference |
| WorldEngine | state history, deterministic waves, pose, hash | measurement acquisition or rendering |
| browser/Lens/game adapter | application of an accepted pose or bounded state | rewriting provenance or measurement truth |
| evidence recorder | raw inputs, outputs, configuration, versions, hashes | retrospective fabrication of missing data |

## 7. Failure rules

- counter failure produces an invalid frame, not a plausible default;
- stale input produces a visible stale state;
- unknown schema or source is rejected;
- missing raw evidence is labeled `EVIDENCE_MISSING` or `HISTORICAL_REPORT`;
- unsupported sensing, platform, or life claims block publication.

## 8. Platform scope

Portability is a design target. Current verification covers a Windows measurement adapter and a browser renderer. Lens Studio, Camera Kit, native mobile, game engines, and physical actuators require separate adapters and separate runtime evidence.
