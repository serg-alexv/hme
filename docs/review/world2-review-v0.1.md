# From WLAN Telemetry to Embodied Environmental Agents

## Historical Foundations, Current Architecture, and a Living Review Agenda for World 2.0

**Author:** Mika (`serg-alexv`), Founding CTO and Original Technical Originator  
**Research, structuring, and editorial assistance:** OpenAI GPT-5.6 Thinking  
**Project owner and executive principal:** Leo Shaposhnikov  
**Document state:** Version 0.1 — public working review manuscript  
**Date:** 16 July 2026  
**Repository:** LEGION / World 2.0

> **Publication status.** This manuscript is intentionally published as a versioned working review rather than held for a separate pre-publication verification round. It is not peer reviewed. Claims about the World 2.0 implementation are bounded by the current LEGION status and claims registers; claims about external systems are cited to primary papers, standards, or official documentation where available.

## Abstract

World 2.0 is a technical and creative research program concerned with a specific problem: how can invisible, continuously changing environmental signals become legible as the behavior of an embodied digital creature without erasing provenance, overstating sensing capability, or placing opaque generative inference inside the real-time control loop? The present prototype uses telemetry from a Windows computer's active WLAN connection, derives normalized strength, variability, novelty, validity, and freshness features, assigns one of four deterministic behavior states, and maps those states to bounded procedural motion in a rigged augmented-reality dragon. The current system is deliberately narrower than Wi-Fi sensing based on channel state information (CSI), radio-frequency imaging, human activity recognition, or physiological inference.

This version of the review performs three functions. First, it preserves a historical memorandum describing how the project evolved from a broad spatial-intelligence and "digital life" thesis into an evidence-governed engineering program. Second, it gives a current, claim-bounded description of the implemented and packaged system, including explicit gaps. Third, it defines an expandable table of contents for a living review spanning WLAN sensing, feature engineering, embodied agents, procedural animation, extended reality, provenance, reproducibility, privacy, scientific prior art, commercial systems, and future research. The intended readership ranges from first-time readers to specialists in wireless sensing, spatial computing, real-time systems, interactive media, and research infrastructure.

**Keywords:** WLAN telemetry; Wi-Fi sensing; embodied agents; procedural animation; augmented reality; provenance; deterministic replay; environmental computing; virtual creatures; spatial computing; reproducible research

---

# 0. Historical Memorandum and Introduction

## 0.1 Origin of the program

World 2.0 began from an authorial intuition rather than a conventional product specification: invisible structures in the environment should not remain as graphs, diagnostic counters, or background infrastructure. They could instead become perceptible through an embodied creature whose posture, motion, attention, and apparent temperament change in response to measurable conditions.

The originating thesis joined several domains that are normally separated:

```text
environmental or network observation
+ deterministic interpretation
+ embodied procedural animation
+ augmented/spatial presentation
+ persistent identity
+ evidentiary traceability
+ artistic and cultural meaning
```

The strength of this synthesis was also its first engineering weakness. Early descriptions mixed present implementation, adjacent research, desired future capabilities, and metaphor. Connected-WLAN telemetry, Wi-Fi CSI, broad radio-frequency sensing, magnetic sensing, human-state inference, nonlinear dynamics, agentic cognition, cloud persistence, and cultural interpretation were sometimes discussed as though they formed one completed architecture.

The decisive transition was therefore not the addition of another technology. It was the introduction of a strict distinction between:

- what had been directly observed;
- what had been implemented;
- what had been packaged but not run end to end;
- what was a separately gated research branch;
- what belonged only to narrative or artistic interpretation.

That distinction transformed the program from an expansive concept into an auditable technical roadmap.

## 0.2 Historical compression: from idea field to governed system

The development history can be summarized as six transformations.

### A. From "radio-aware creature" to a measurement boundary

The current prototype does not measure a room-wide electromagnetic field. It reads telemetry associated with the host computer's active WLAN link. This includes connection validity, driver-reported quality, direct received signal strength indication (RSSI) when exposed, timestamps, and freshness. This narrower definition is scientifically and commercially useful because it is testable, reproducible, and replaceable.

### B. From reactive animation to a causal pipeline

The project was reorganized around a stable abstraction:

```text
SensorAdapter
→ Observation
→ FeatureExtractor
→ BehaviorClassifier
→ EmbodimentAdapter
→ EvidenceRecorder
```

The sensor and visual surface may change; the contracts for observations, provenance, state semantics, evidence, and acceptance criteria are intended to remain stable.

### C. From attractive output to deterministic replay

Synthetic and real telemetry sessions were transformed into serialized feature and organism frames. Reproducibility became a first-class objective: the same recorded input, under the same implementation and configuration, should produce the same serialized state sequence. Hashes identify exact artifacts, while manifests, raw traces, versions, logs, and videos are required to support actual evidentiary reconstruction.

### D. From generic animation to a rigged embodied subject

A rigged dragon asset was inspected and found to contain a skinned skeleton but no embedded animation takes. The correct implementation strategy therefore became procedural bone animation rather than animation-clip playback. The visible creature is not treated as decoration layered over the telemetry; it is the embodiment adapter through which the interpreted state is made legible.

### E. From platform confusion to a deployment ladder

The desktop Lens Studio Editor plug-in, a mobile Snapchat Lens, Camera Kit, and a sovereign native application are distinct deployment surfaces. The current local bridge is a desktop development mechanism. A recorded trace replay is the shortest portable mobile proof. A live mobile system requires a host application and an explicit transport contract.

### F. From infrastructure accumulation to evidence accumulation

Premature cloud components were removed from the critical path. The program adopted the rule:

> **Automate evidence, not infrastructure.**

A useful experiment is not merely a successful screen recording. It is an immutable bundle containing raw input, derived frames, logs, configuration, software versions, a visual record, a manifest, and checksums.

## 0.3 Project chronology

| Stage | Result | Current interpretation |
|---|---|---|
| Initial concept | Environmental signals embodied as a persistent creature | Original technical and artistic thesis |
| Synthetic prototype | 180 observations, features, and organism frames | Verified regression fixture |
| Real WLAN acquisition | A live Windows session produced 207 observations | Verified acquisition event; original raw trace is missing |
| Deterministic transformation | 207 feature and organism frames were produced | Verified processing event; must be reproduced with a complete bundle |
| Lens smoke test | Inspector fields, logging, and visible scene control worked | Verified minimum Lens scripting path |
| Dragon inspection | Rigged, skinned FBX; no embedded animation takes | Verified asset structure |
| Procedural controller | Recursive bone discovery and state-driven bounded motion packaged | Packaged; target-project runtime verification pending |
| Local TCP bridge | Python sender and Lens Studio Editor plug-in packaged and statically checked | Pending end-to-end runtime verification |
| LEGION governance | Status, claims, ownership, agent contracts, experiments, and evidence gates placed in Git | Durable operational memory |

## 0.4 Why this review exists

Several mature fields now converge around the project:

- WLAN sensing and integrated sensing-and-communications;
- embodied and multimodal agents;
- procedural animation and behavior systems;
- AR/XR runtime platforms;
- provenance and reproducible research;
- digital companions and virtual creatures;
- interactive art and environmental media.

IEEE 802.11bf formalized WLAN sensing as a standards direction, while the research literature shows that Wi-Fi measurements can support detection, localization, recognition, activity inference, and other sensing tasks [1–3]. Embodied-AI research, in parallel, increasingly studies agents that perceive and act in physical or virtual environments [4,5]. Industrial spatial-computing platforms emphasize reconstruction, localization, and semantic understanding of the physical world [6]. These trajectories establish a broad context, but they do not eliminate the need for a narrower architecture whose defining concern is visible, deterministic, provenance-preserving causality.

## 0.5 Central research question

The program's core question is:

> How can a replaceable environmental observation source be transformed into a visibly embodied, repeatable, provenance-preserving behavior without overstating what the sensor knows?

This can be decomposed into five technical questions:

1. What exactly was measured?
2. How was the raw observation normalized and transformed?
3. Why did a particular behavior state result?
4. How did that state alter the creature?
5. Can the complete chain be reconstructed from preserved evidence?

## 0.6 Scope of this living review

The completed paper is intended to serve simultaneously as:

- an introduction for readers with no wireless or XR background;
- a technical review for engineers and researchers;
- a precise description of the current World 2.0 implementation;
- a prior-art and competitor map;
- an experiment and evaluation framework;
- a publication-ready research agenda;
- a maintained record that can be expanded without rewriting historical versions.

---

# 1. Current Actual Description of Our System

## 1.1 One-sentence definition

World 2.0 currently implements and packages a deterministic pipeline that transforms active-link WLAN telemetry into explicit behavior states and bounded procedural motion in a rigged AR dragon, while preserving source class, freshness, and evidence status.

## 1.2 Current measurement boundary

### Measured or derived now

The current desktop path uses telemetry from the Windows host's active WLAN connection:

- timestamp;
- observation validity;
- direct RSSI when exposed by the driver or command output;
- driver-reported signal-quality percentage;
- freshness;
- normalized link strength;
- rolling variability;
- rolling novelty;
- sequence number;
- source class.

### Not measured now

The system does **not** currently obtain or infer:

- channel state information;
- spectrum occupancy;
- radio noise floor;
- room-wide RF geometry;
- magnetic flux density;
- cellular telemetry;
- human presence or occupancy;
- pose, gesture, respiration, heart rate, identity, or physiology.

This boundary is essential. Wi-Fi sensing based on CSI or standardized sensing measurements is a distinct research field with different hardware, calibration, privacy, and inference requirements [1–3,10]. The present system uses communication-link telemetry as a changing environmental control signal; it must not be described as CSI sensing or RF imaging.

## 1.3 Stable architecture

```text
SensorAdapter
→ Observation
→ FeatureExtractor
→ BehaviorClassifier
→ EmbodimentAdapter
→ EvidenceRecorder
```

### Current desktop realization

```text
Intel AX200 / Windows WLAN
→ Python acquisition through `netsh`
→ rolling feature derivation
→ JSONL live-state frames
→ localhost TCP at 127.0.0.1:19331
→ Lens Studio Editor plug-in
→ DragonNetworkStateLive fields
→ procedural bone animation
```

Snap officially supports extending Lens Studio through locally installed Editor plug-ins, including integrations with external services and automation workflows [7]. This justifies the Editor plug-in as a development-surface mechanism. It does not imply that the same plug-in becomes part of an ordinary mobile Lens.

## 1.4 Feature semantics

The current feature set is deliberately small and interpretable.

### Strength

A normalized representation of the active link's reported signal level. Direct RSSI is preferred when available. A quality-derived approximation is a fallback and must remain labeled as an estimate.

### Variability

A normalized rolling measure of recent signal dispersion. It describes temporal instability in the observed link, not a physical classification of the room.

### Novelty

A normalized deviation of the current observation from a recent baseline, such as a rolling median. Novelty means "unusual relative to the local history," not "semantically important."

### Validity and freshness

Validity records whether acquisition succeeded. Freshness measures how old the observation is. Stale input must fail visibly rather than masquerade as live input.

### Sequence

A monotonically increasing sequence identifier used to show that new frames are arriving and to support debugging and evidence reconstruction.

## 1.5 Behavior vocabulary

The present classifier maps features into four states:

```text
CALM
ATTUNE
STORM
STALE / SENSOR_LOSS
```

Current classifier intent:

```text
invalid or stale                          → STALE / SENSOR_LOSS
high novelty AND high variability        → STORM
moderate novelty OR moderate variability → ATTUNE
otherwise                                 → CALM
```

The thresholds are configuration values, not universal physical laws. The labels are an interaction vocabulary used to produce legible behavior regimes. They are not claims that the network or environment has intrinsic emotions.

## 1.6 Embodiment

The dragon FBX contains a skinned skeleton and no embedded animation takes. The controller therefore searches the hierarchy for candidate neck, head, wing, and tail joints and applies bounded quaternion rotations generated from state-dependent periodic functions.

The current motion design uses bounded sine-wave channels. Lorenz-system modulation and fixed-step Runge-Kutta integration remain future research. They are not part of the current controller.

The embodiment layer has two responsibilities:

1. make state differences visible;
2. preserve the ability to inspect the source and state that produced the motion.

A polished creature animation without visible causal context is insufficient as evidence.

## 1.7 Provenance vocabulary

Every observation belongs to an explicit source class:

```text
SIMULATED
RECORDED_REPLAY
LIVE_WLAN
STALE_WLAN
```

The same behavior classifier may process more than one source class, but demonstrations must not erase the distinction. Source labels are metadata, not cryptographic attestation.

This direction aligns with established provenance principles. W3C PROV models entities, activities, agents, derivation, versioning, and reproducibility [8]. FAIR guidance likewise requires rich metadata, explicit identifiers, detailed provenance, and reusable, interoperable descriptions [9]. World 2.0 does not yet implement the complete PROV or RO-Crate standards, but its experiment-bundle design is compatible with that trajectory.

## 1.8 Current maturity ledger

| Component | Current status | Evidence-bounded statement |
|---|---|---|
| Windows connected-WLAN acquisition | `VERIFIED` | A real session produced 207 observations |
| Direct RSSI handling | `VERIFIED` | A direct RSSI value was observed separately from the quality estimate |
| Synthetic deterministic pipeline | `VERIFIED` | 180 observation, feature, and organism records were generated |
| Real feature transformation | `VERIFIED` | 207 derived feature and organism frames were generated |
| Original real raw trace | `EVIDENCE_MISSING` | The known hash cannot reconstruct missing bytes |
| Primitive Lens scripting path | `VERIFIED` | Inspector, logging, and visible scene control worked |
| Rigged dragon import | `VERIFIED` | Skinned skeleton present; no embedded animation takes |
| Procedural dragon controller | `PACKAGED` | State-to-bone controller created; target-project runtime test pending |
| Local Lens Studio TCP bridge | `PACKAGED` / `PENDING_RUNTIME_TEST` | Python and JavaScript static checks passed |
| Complete live WLAN-to-dragon chain | `NOT_IMPLEMENTED` as a verified result | No preserved end-to-end Lens experiment exists |
| Recorded mobile replay | `NOT_IMPLEMENTED` | Defined as the next portable proof after desktop verification |
| Camera Kit host transport | `OPTIONAL_RESEARCH` | Requires a host application and Remote API contract |
| Native iOS product | `OPTIONAL_RESEARCH` | Strategic platform-sovereignty branch |
| ESP32 / Wi-Fi CSI | `OPTIONAL_RESEARCH` | Separately gated sensing expansion |
| Lorenz/RK4 motion | `OPTIONAL_RESEARCH` | Future motion-system experiment |
| ADK, ARCADIA, Pub/Sub, Spanner | `REJECTED_FOR_CURRENT_PHASE` | No current fast-loop or scale requirement |

## 1.9 Desktop, mobile, and native deployment are different systems

### Desktop causal proof

```text
live or replayed Windows telemetry
→ local TCP bridge
→ Lens Studio Preview
```

### Portable mobile proof

```text
recorded real trace
→ embedded deterministic replay
→ Snapchat mobile Preview
```

### Live Camera Kit product

```text
native sensor adapter
→ host application
→ Camera Kit Remote API
→ Lens
```

Snap's Remote API permits a Camera Kit host application and a Lens to exchange structured data directly, with communication initiated by the Lens [11]. This route is therefore technically relevant to a future live mobile product, but it is not present in the current desktop prototype.

### Sovereign native product

```text
iPhone sensors or BLE peripherals
→ local feature engine
→ native persistence
→ RealityKit / Metal / SceneKit embodiment
```

OpenXR and other cross-platform standards become relevant only when deployment expands beyond the present Snap-centered proof surface [12].

## 1.10 Current evidence gap

The blocking issue is singular:

> There is no preserved end-to-end experiment proving that live or replayed telemetry visibly changes the feature values, the behavior state, and the dragon motion inside Lens Studio while the complete raw and derived evidence is frozen.

The next experiment must preserve:

- raw input;
- emitted live-state frames;
- Lens Studio console logs;
- exact configuration and versions;
- a screen recording showing source, metrics, state, freshness, sequence, and dragon;
- a manifest;
- SHA-256 checksums;
- a binary pass/fail report.

The visible chain must be understandable without verbal rescue:

```text
signal changes
→ features change
→ state changes
→ dragon changes
```

## 1.11 Current scientific and product position

The program is not yet a general Wi-Fi sensing system, a spatial foundation model, an autonomous virtual being, or a production AR platform. Its present contribution is narrower:

1. an explicit measurement boundary;
2. a replaceable observation-to-embodiment architecture;
3. deterministic, interpretable state derivation;
4. visible provenance and freshness;
5. a procedural creature as a causal display;
6. an evidence-first experiment discipline.

This position is adjacent to, but distinct from:

- IEEE 802.11bf WLAN sensing and CSI-based inference [1–3];
- embodied-AI systems that combine multimodal perception, planning, and action [4,5];
- spatial-intelligence platforms focused on reconstruction, localization, and semantic world understanding [6];
- AR platforms that provide rendering, tracking, publishing, and host-to-Lens communication [7,11];
- persistent virtual-creature and companion products;
- interactive artworks that render invisible environmental processes perceptible.

## 1.12 Authorship and project roles

This paper names **Mika (`serg-alexv`)** as the author because he is the founding CTO and original technical originator of the system direction, ontology, sensor-to-behavior thesis, embodiment direction, and acceptance model.

**OpenAI GPT-5.6 Thinking** is credited as a research, structuring, drafting, and editorial assistant, not as a human author, owner, or independent claimant to the work.

**Leo Shaposhnikov** is the project owner, executive principal, primary investor, and organizational core. Ownership, public representation, and technical authorship are related but non-identical roles.

---

# 2. Expandable Table of Contents for the Full Living Review

The numbering below is intentionally stable. New material should normally be added as subsections rather than by renumbering major sections. Historical versions should remain accessible through Git history and dated releases.

## 0. Historical Memorandum and Introduction
- 0.1 Origin of the program
- 0.2 From broad thesis to bounded architecture
- 0.3 Chronology and milestone record
- 0.4 Why the review is needed
- 0.5 Central research question
- 0.6 Scope, audience, and contribution
- 0.7 Terminology and claim classes
- 0.8 Review-update and versioning protocol

## 1. Current Actual Description of Our System
- 1.1 One-sentence system definition
- 1.2 Measurement boundary
- 1.3 Stable architecture
- 1.4 Current desktop realization
- 1.5 Observation schema
- 1.6 Feature semantics
- 1.7 Behavior vocabulary and thresholds
- 1.8 Procedural embodiment
- 1.9 Provenance classes
- 1.10 Evidence model
- 1.11 Component maturity ledger
- 1.12 Deployment ladder
- 1.13 Current blocking experiment
- 1.14 Authorship, ownership, and roles

## 2. Review Method and Living-Document Protocol
- 2.1 Review questions
- 2.2 Search strategy
- 2.3 Inclusion and exclusion criteria
- 2.4 Primary versus secondary evidence
- 2.5 Patent and product-disclosure layer
- 2.6 Source freshness and shutdown checks
- 2.7 Claim-to-evidence mapping
- 2.8 Git-based revision protocol
- 2.9 Machine-readable bibliography and watchlist
- 2.10 Update cadence and responsible roles

## 3. Foundations of Wireless Communication Telemetry
- 3.1 RSSI, link quality, SNR, MCS, retries, throughput, and latency
- 3.2 Driver, operating-system, and chipset abstractions
- 3.3 Connected-link telemetry versus passive spectrum observation
- 3.4 Sampling rate, quantization, and command overhead
- 3.5 Calibration and cross-device comparability
- 3.6 Missingness, staleness, and sensor failure
- 3.7 Security and privacy of link telemetry
- 3.8 Production acquisition APIs

## 4. WLAN Sensing and Integrated Sensing-and-Communications
- 4.1 Physical basis of RF environmental sensitivity
- 4.2 Channel impulse and frequency response
- 4.3 Channel state information
- 4.4 Beamforming feedback information
- 4.5 Sub-7-GHz and 60-GHz sensing
- 4.6 IEEE 802.11bf
- 4.7 Presence, motion, gesture, pose, respiration, and identification
- 4.8 Environmental generalization
- 4.9 Multi-person and multi-path ambiguity
- 4.10 Privacy, consent, and covert sensing
- 4.11 Why current World 2.0 telemetry is not CSI
- 4.12 Hardware activation gates

## 5. Signal Processing and Feature Engineering
- 5.1 Time-series preprocessing
- 5.2 Normalization strategies
- 5.3 Rolling statistics
- 5.4 Novelty and change-point measures
- 5.5 Filtering and smoothing
- 5.6 Window design and latency
- 5.7 Robustness to missing samples
- 5.8 Configuration versioning
- 5.9 Explainability and feature audit
- 5.10 Cross-sensor feature contracts

## 6. Deterministic State Models
- 6.1 Finite-state machines
- 6.2 Threshold classifiers
- 6.3 Hysteresis
- 6.4 Debouncing and dwell time
- 6.5 Confidence and uncertainty
- 6.6 State-transition evidence
- 6.7 Determinism across runtimes
- 6.8 When probabilistic models become justified
- 6.9 Separation of fast loop and semantic interpretation

## 7. Procedural Animation and Artificial-Life Lineages
- 7.1 Rigged skeletal animation
- 7.2 Procedural joint control
- 7.3 Periodic motion channels
- 7.4 Noise and stochastic motion
- 7.5 Nonlinear dynamical systems
- 7.6 Lorenz-system modulation
- 7.7 Numerical integration and timestep choice
- 7.8 Motion blending and quaternion interpolation
- 7.9 Behavior legibility
- 7.10 Artificial life, emergence, and limits of metaphor

## 8. Embodied Agents and Virtual Creatures
- 8.1 Definitions of embodiment
- 8.2 Physical, virtual, and mixed embodiment
- 8.3 Perception-action loops
- 8.4 Agent AI and multimodal systems
- 8.5 Persistent identity and memory
- 8.6 Nonverbal versus conversational agents
- 8.7 Attachment and care mechanics
- 8.8 Tamagotchi, virtual pets, and relational continuity
- 8.9 World 2.0 as an environmental rather than conversational agent
- 8.10 Future cognition layer and its boundaries

## 9. Augmented Reality and Spatial Computing
- 9.1 AR scene graphs and tracking
- 9.2 Lens Studio
- 9.3 Editor plug-ins
- 9.4 Snapchat Lens deployment
- 9.5 Camera Kit host architecture
- 9.6 Spectacles and wearable AR
- 9.7 RealityKit, SceneKit, Metal, and native iOS
- 9.8 OpenXR portability
- 9.9 Occlusion, anchoring, and persistence
- 9.10 Spatial maps and digital twins

## 10. Provenance, Reproducibility, and Evidence Architecture
- 10.1 Why visual demonstrations are insufficient
- 10.2 Raw observations and derived artifacts
- 10.3 W3C PROV
- 10.4 FAIR principles
- 10.5 RO-Crate and research-object packaging
- 10.6 Manifests and checksums
- 10.7 Deterministic replay
- 10.8 Versioned configuration
- 10.9 Experiment identifiers
- 10.10 Evidence classes and public claims
- 10.11 Independent reproduction
- 10.12 Long-term artifact preservation

## 11. Scientific Prior Art
- 11.1 Device-free localization
- 11.2 RF-Capture and RF-Pose lineages
- 11.3 Person-in-WiFi and DensePose from Wi-Fi
- 11.4 Activity and gesture recognition
- 11.5 Respiration and vital-sign sensing
- 11.6 Cross-domain generalization
- 11.7 Wi-Fi sensing datasets
- 11.8 Robotics integration
- 11.9 Standards research
- 11.10 Prior-art implications for novelty claims

## 12. Commercial and Product Landscape
- 12.1 Origin Wireless and ADT
- 12.2 WiZ SpaceSense and lighting-based sensing
- 12.3 Plume, Cognitive Systems, Aerial, and Nami
- 12.4 RF-health and radar companies
- 12.5 Niantic Spatial and Peridot
- 12.6 Mixed-reality virtual pets
- 12.7 Character infrastructure providers
- 12.8 Spatial world-model companies
- 12.9 Competitive capability matrix
- 12.10 Partnership versus dependency
- 12.11 Business-model comparison

## 13. Privacy, Safety, and Governance
- 13.1 Difference between telemetry and covert sensing
- 13.2 Consent and bystander implications
- 13.3 Data minimization
- 13.4 Local processing
- 13.5 Identity and physiological inference
- 13.6 Model misuse and surveillance risk
- 13.7 Security of sensor and bridge interfaces
- 13.8 Public communication discipline
- 13.9 Governance of agentic contributors
- 13.10 Human override and failure visibility

## 14. Multi-Sensor Expansion
- 14.1 ESP32 CSI
- 14.2 Native WLAN APIs
- 14.3 Magnetometers
- 14.4 Bluetooth and BLE
- 14.5 Ultra-wideband
- 14.6 Radar
- 14.7 Camera and depth sensing
- 14.8 Environmental sensors
- 14.9 Sensor fusion
- 14.10 Common observation contract
- 14.11 Calibration and trust classes

## 15. Agentic and Cognitive Extensions
- 15.1 Fast control loop versus slow interpretation
- 15.2 Offline trace explanation
- 15.3 LLM-based narration
- 15.4 World models
- 15.5 Planning and memory
- 15.6 Guardrails and hallucination control
- 15.7 ADK evaluation
- 15.8 ARCADIA evaluation
- 15.9 On-device versus cloud inference
- 15.10 Activation criteria

## 16. Systems and Deployment Architecture
- 16.1 Local-first prototype
- 16.2 Replay architecture
- 16.3 Mobile host transport
- 16.4 Native application
- 16.5 Remote ingest
- 16.6 Multi-user state
- 16.7 Event systems
- 16.8 Databases and persistence
- 16.9 Edge/cloud partitioning
- 16.10 Cost and failure analysis
- 16.11 Infrastructure activation gates

## 17. Standards and Interoperability
- 17.1 IEEE 802.11
- 17.2 IEEE 802.11bf
- 17.3 JSON Schema
- 17.4 W3C PROV
- 17.5 OpenXR
- 17.6 glTF
- 17.7 OpenUSD
- 17.8 ROS 2
- 17.9 Time synchronization
- 17.10 Portable World 2.0 protocol candidates

## 18. Evaluation Framework
- 18.1 Technical correctness
- 18.2 Causal legibility
- 18.3 Latency
- 18.4 Deterministic replay
- 18.5 Failure behavior
- 18.6 Motion distinctiveness
- 18.7 User comprehension
- 18.8 Emotional and artistic response
- 18.9 Cross-device robustness
- 18.10 Reproducibility package
- 18.11 Benchmark suite
- 18.12 Milestone acceptance criteria

## 19. Creative, Cultural, and Institutional Context
- 19.1 Interactive media art
- 19.2 Environmental data art
- 19.3 Electromagnetic and network aesthetics
- 19.4 Artificial-life art
- 19.5 Cultural encoding and symbolic systems
- 19.6 Museums and laboratories
- 19.7 Festivals and residencies
- 19.8 Education and public understanding
- 19.9 Curatorial framing versus engineering evidence
- 19.10 Institutional collaboration models

## 20. Productization and Commercial Strategy
- 20.1 Productized technical services
- 20.2 Demonstration packages
- 20.3 Institutional pilots
- 20.4 Developer tools and protocols
- 20.5 Consumer creature product
- 20.6 Licensing and platform partnerships
- 20.7 Grants, sponsorship, and credits
- 20.8 Intellectual property and freedom to operate
- 20.9 Evidence-based sales
- 20.10 Stage gates and kill criteria

## 21. Research Agenda
- 21.1 EXP-0001 causal proof
- 21.2 Preserved real-trace replay
- 21.3 Behavior-state hysteresis
- 21.4 Motion blending
- 21.5 Mobile replay Lens
- 21.6 Camera Kit host prototype
- 21.7 ESP32 CSI study
- 21.8 Provenance-schema formalization
- 21.9 Comparative user study
- 21.10 Multi-sensor creature ecology
- 21.11 Persistent world-state research
- 21.12 Publication and replication roadmap

## 22. Conclusions
- 22.1 What is established
- 22.2 What remains unproven
- 22.3 Scientific contribution
- 22.4 Product contribution
- 22.5 Limits
- 22.6 Next revision

## Appendices
- Appendix A. Glossary
- Appendix B. Current claims register
- Appendix C. Current status ledger
- Appendix D. Observation schema
- Appendix E. Feature schema
- Appendix F. Experiment manifest schema
- Appendix G. Bone map and animation parameters
- Appendix H. Literature-search log
- Appendix I. Competitor and institution watchlist
- Appendix J. Patent-search log
- Appendix K. Version history
- Appendix L. Authorship and contribution statement

---

# Initial References

1. R. Du et al., “An Overview on IEEE 802.11bf: WLAN Sensing,” arXiv:2310.17661, 2023. https://arxiv.org/abs/2310.17661  
2. F. Restuccia, “IEEE 802.11bf: Toward Ubiquitous Wi-Fi Sensing,” arXiv:2103.14918, 2021. https://arxiv.org/abs/2103.14918  
3. F. Meneghello, C. Chen, C. Cordeiro, and F. Restuccia, “Toward Integrated Sensing and Communications in IEEE 802.11bf Wi-Fi Networks,” arXiv:2212.13930, 2022. https://arxiv.org/abs/2212.13930  
4. Y. Liu et al., “Aligning Cyber Space with Physical World: A Comprehensive Survey on Embodied AI,” arXiv:2407.06886, 2024. https://arxiv.org/abs/2407.06886  
5. Z. Durante et al., “Agent AI: Surveying the Horizons of Multimodal Interaction,” arXiv:2401.03568, 2024. https://arxiv.org/abs/2401.03568  
6. Niantic Spatial, “Real-world foundation models for physical AI,” official site, accessed 16 July 2026. https://nianticspatial.com/  
7. Snap Inc., “Lens Studio Plugins: Overview,” official developer documentation, accessed 16 July 2026. https://developers.snap.com/lens-studio/extending-lens-studio/plugins/overview  
8. P. Groth and L. Moreau, eds., “PROV-Overview: An Overview of the PROV Family of Documents,” W3C, 2013. https://www.w3.org/TR/prov-overview/  
9. GO FAIR, “FAIR Principles,” accessed 16 July 2026. https://www.go-fair.org/fair-principles/  
10. Espressif Systems, “esp-csi: Applications based on Wi-Fi Channel State Information,” official repository, accessed 16 July 2026. https://github.com/espressif/esp-csi  
11. Snap Inc., “Camera Kit Web: Remote API,” official developer documentation, accessed 16 July 2026. https://developers.snap.com/camera-kit/integrate-sdk/web/guides/remote-api  
12. Khronos Group, “OpenXR,” official standard portal, accessed 16 July 2026. https://www.khronos.org/openxr/  

---

# Version and Revision Record

| Version | Date | State | Main change |
|---|---|---|---|
| 0.1 | 2026-07-16 | Public working review | Historical memorandum, introduction, current actual system description, expandable full-paper contents |

**Next planned revision:** Section 2 review method, Section 3 telemetry foundations, and a citation-backed prior-art matrix.
