# Deterministic Environmental Embodiment

## A Fixed-Point Wave Engine for Auditable Network-Conditioned Augmented-Reality Creatures

**Draft state:** frontier v0.1  
**Submission identity:** intentionally omitted from this public technical draft
**Engineering and editorial assistance:** tool-assisted; identities omitted
**Project:** World 2.0 / WORLD 2.0

## Abstract

Interactive creatures are commonly animated either by authored clips, runtime procedural functions, or opaque generative systems. When their behavior is said to reflect a physical environment, a second problem appears: the causal chain from measurement to visible action is difficult to audit and often mixes sensing, interpretation, animation, and metaphor. We present a deterministic environmental embodiment architecture in which connected-WLAN observations are transformed into bounded features, passed through a hysteretic state membrane, converted into a fixed-point wave field, projected onto a rigged skeleton, and preserved as integer pose frames. The reference kernel is implemented in C11, with a no-dependency Rust implementation and Windows monitor packaged for cross-language verification. Lens Studio is restricted to rendering pose frames and therefore does not own the scientific interpretation or authoritative dynamics. A supplied rigged dragon is used as the first embodiment. The current contribution is an auditable engine and validation protocol, not a claim of room-wide RF sensing, consciousness, digital life, or proven industrial impact.

## 1. Introduction

The technical problem is narrow: make an invisible and continuously changing environmental measurement visibly legible through one creature, while preserving the exact causal record.

The broader problem is ontological. “Signal,” “state,” “mood,” “motion,” and “meaning” are frequently collapsed into one another. This produces visually convincing systems whose claims cannot be reconstructed.

```text
CausalFrame
→ FeatureFrame
→ StateMembrane
→ WaveField
→ BodyChannel graph
→ PoseFrame
→ Renderer
→ EvidenceChain
```

The name **World 2.0** refers to the larger research thesis that computation can become an embodied participant in continuously changing physical conditions. The present paper evaluates only the engineering nucleus required to make that thesis testable.

## 2. Intellectual and mathematical lineage

The project begins from oscillatory decomposition, lawful state evolution, formal boundaries, irreversible time, and economical representation.

Fourier supplies the operational intuition that complex variation can be represented through controlled oscillatory components. Schrödinger exemplifies state evolution defined by explicit law rather than visual randomness. Gödel supplies a warning: formal systems possess boundaries and should not be treated as complete descriptions of the worlds they model. Prigogine foregrounds open systems, fluctuations, and temporality. Dirac is the central stylistic model: maximum operational force with minimum unnecessary machinery.

The intended “Fink” reference must be identified before publication. If it is Mathias Fink, time-reversal wave physics and control in complex media form the relevant connection. This draft deliberately leaves the citation unresolved rather than fabricating certainty.

This lineage motivates design constraints; it does not itself establish novelty.

## 3. Ontology surgery

### 3.1 Observation is not world state
A connected-WLAN interface yields a partial report about one link. It does not yield the complete electromagnetic condition of a room.

### 3.2 Feature is not physical essence
Strength, variability, and novelty are deterministic summaries under a stated window and normalization.

### 3.3 State is not emotion
`CALM`, `ATTUNE`, `STORM`, and `STALE` are regions of an operational state machine. They are labels for motion policy, not claims about sentience.

### 3.4 Pose is not evidence by itself
A visible motion proves only that a renderer moved. Causal validity requires the input stream, engine version, configuration, pose bytes, and run record.

## 4. Engine primitives

The engine defines six project primitives: `CausalFrame`, `StateMembrane`, `WaveCell`, `BodyChannel`, `PoseFrame`, and `EvidenceChain`.

The state membrane provides temporal hysteresis. Wave cells provide deterministic phase continuity. Body channels bind a small number of waves to many possible skeletons. Pose frames decouple dynamics from render technology.

## 5. Fixed-point dynamics

Feature values are represented in Q0.16. Each oscillator uses a 32-bit phase accumulator and a committed Q1.30 sine lookup table. State and feature values modulate phase increment and amplitude through integer arithmetic.

\[
\phi_i[k+1] = (\phi_i[k]+\Delta_i(S_k,F_k)) \bmod 2^{32}
\]

\[
W_i[k]=A_i(S_k,F_k)\sin(\phi_i[k]).
\]

\[
\theta_j[k] = \operatorname{clip}\left(b_j(S_k)+\sum_i B_{ji}W_i[k],-\ell_j,\ell_j\right).
\]

The output unit is millidegrees, avoiding platform-dependent trigonometric output inside the authoritative kernel.

## 6. Implementation

The C11 kernel has no third-party dependencies and performs no dynamic allocation. The Rust implementation mirrors the model and adds the Windows monitor and TCP transport. Lens Studio receives `world.pose/0.1` frames and applies the values to the dragon skeleton.

The supplied rig includes neck, head, wing, tail, limb, face, tongue, and muscle controls. The initial binding intentionally uses only a causal minimum: root, neck, head, mirrored wings, and three tail segments.

## 7. Current monitoring

The monitor reads host connected-WLAN telemetry. Direct RSSI and driver quality must remain distinguishable. Invalid observations are preserved. Feature extraction is bounded and versioned.

The monitor does not perform CSI sensing, spectrum analysis, room imaging, human pose inference, occupancy detection, or physiological measurement.

## 8. Validation

Validation is staged:

1. C determinism and limits;
2. C/Rust byte-equivalence;
3. Lens replay;
4. live WLAN causal capture;
5. comparison against procedural baselines;
6. transfer to a second skeleton.

The v0.1 package verifies the first stage and produces a 720-frame golden trace. Lens and live-Windows stages require execution in the user's installed environment.

## 9. Scientific questions

- Does a state membrane reduce visual flicker compared with direct thresholds?
- Does fixed-point replay improve cross-machine reproducibility?
- Which wave-to-joint matrices maximize causal legibility without caricature?
- Can one feature-to-wave policy transfer across different skeleton topologies?
- What minimum telemetry change remains perceptible in the embodiment?
- Can observers distinguish causal replay from unrelated procedural animation?

## 10. Limitations

The first sensor is narrow. The behavior labels are designed, not discovered. The skeleton matrix is manually authored. The current evidence does not establish biological plausibility, consciousness, generalized environmental understanding, or market impact.

## 11. Updated article skeleton

Future revisions should add primary-source references and quotation ledger, exact feature calibration, C/Rust equivalence, Lens runtime proof, a live run bundle, baseline experiments, second-skeleton transfer, privacy/security analysis, prior-art matrix, reproducibility appendix, and external pilot results.

## 12. Conclusion

The contribution is not another animated dragon. It is a separation of physical observation, deterministic interpretation, skeletal embodiment, and evidence into a portable engine contract. That separation makes an ambitious cultural thesis available to engineering criticism.
