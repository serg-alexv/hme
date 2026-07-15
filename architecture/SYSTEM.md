# Canonical System Architecture

## 1. Stable abstraction

```text
SensorAdapter
→ Observation
→ FeatureExtractor
→ BehaviorClassifier
→ EmbodimentAdapter
→ EvidenceRecorder
```

The sensor and embodiment are replaceable. Provenance, schemas, state semantics, evidence, and acceptance criteria are durable.

## 2. Current desktop path

```text
Intel AX200 / Windows WLAN
→ Python `netsh` acquisition
→ rolling feature derivation
→ JSONL live-state frames
→ localhost TCP 127.0.0.1:19331
→ Lens Studio Editor plug-in
→ DragonNetworkStateLive inputs
→ procedural bone animation
```

### Current acquisition fields

- timestamp
- connection validity
- direct RSSI when available
- driver quality percentage
- freshness

### Derived fields

- normalized strength
- rolling variability
- rolling novelty
- sequence
- source class

## 3. Behavior vocabulary

```text
CALM
ATTUNE
STORM
STALE / SENSOR_LOSS
```

Current classifier intent:

```text
invalid or stale                         → STALE / SENSOR_LOSS
high novelty AND high variability       → STORM
moderate novelty OR moderate variability→ ATTUNE
otherwise                                → CALM
```

Thresholds are configuration, not universal physical truths.

## 4. Provenance vocabulary

```text
SIMULATED
RECORDED_REPLAY
LIVE_WLAN
STALE_WLAN
```

Embodiment must not erase provenance. A visible or inspectable source label is required for demonstrations.

## 5. Deployment ladder

### Phase A — desktop causal proof

```text
live/replayed Windows telemetry
→ local bridge
→ Lens Studio Preview
```

### Phase B — portable proof

```text
recorded real trace
→ embedded Lens replay
→ Snapchat phone Preview
```

### Phase C — live mobile product

```text
native sensor adapter
→ Camera Kit host
→ Remote API
→ Lens
```

### Phase D — sovereign native product

```text
iPhone sensors / BLE peripherals
→ local feature engine
→ native persistence
→ RealityKit / Metal / SceneKit embodiment
```

## 6. Separately gated research

- ESP32 / Wi-Fi CSI sensing
- magnetometer adapter
- broadband RF adapter
- Lorenz/RK4 motion channels
- ADK offline interpretation/auditing
- global persistent world state

Each research branch must implement the common observation/feature contract rather than bypass it.

## 7. Failure model

The system must fail visibly and conservatively:

- acquisition failure → invalid observation;
- stale timestamp → `STALE_WLAN`;
- schema violation → reject frame;
- unknown source → reject frame;
- broken bridge → no false live label;
- missing raw evidence → `EVIDENCE_MISSING`;
- unsupported claim → block release.

## 8. Production evolution gates

Replace `netsh` with a native WLAN API only when sampling overhead, reliability, or deployment demands it.

Add cloud ingest only when a second external device/user requires remote upload.

Add Pub/Sub only when multiple producers/consumers, fan-out, backlog, or retry semantics are demonstrated.

Add globally consistent databases only when real multi-user transactional world state exists.
