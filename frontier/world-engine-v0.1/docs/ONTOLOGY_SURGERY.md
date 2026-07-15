# Ontology Surgery

## 1. The category error being removed

Earlier prototypes allowed these objects to blur:

```text
physical condition
sensor report
derived statistic
behavioral interpretation
procedural motion
rendered appearance
narrative meaning
scientific claim
```

They are not interchangeable.

A dragon can look agitated without the WLAN being physically "angry." A feature can be novel without the environment being objectively anomalous. An animation can be deterministic without reproducing the complete physical world.

## 2. Canonical layers

### L0 — World condition
The external process exists independently of this software. It is only partially observable.

### L1 — CausalFrame

\[
C_k = (\mathrm{sequence}, t_k, \mathrm{source}, \mathrm{validity}, x_k)
\]

The frame says what was observed, where it came from, and whether it is usable.

### L2 — FeatureFrame

\[
F_k = (s_k, v_k, n_k, a_k)
\]

- \(s_k\): normalized link strength;
- \(v_k\): rolling variability;
- \(n_k\): novelty relative to recent history;
- \(a_k\): age/freshness.

Features are interpretations of observations, not direct physical essences.

### L3 — StateMembrane

\[
S_k \in \{\mathrm{CALM},\mathrm{ATTUNE},\mathrm{STORM},\mathrm{STALE}\}
\]

It uses hysteresis and dwell time. A boundary must persist before the state changes, preventing single-sample flicker.

### L4 — WaveField
A bank of deterministic oscillators transforms state and features into bounded motion drives.

### L5 — BodyChannel graph
A sparse matrix binds wave drives to named skeleton axes.

### L6 — PoseFrame
The engine emits integers: root displacement, joint angles, state, sequence, freshness, and frame hash.

### L7 — Rendered embodiment
Lens Studio converts integer pose values to local transforms. Lens does not infer physical truth or reclassify behavior.

### L8 — Evidence
Raw observation, config, engine version, pose bytes, logs, video, and hashes are preserved together.

### L9 — Narrative
Terms such as "mood," "attention," "organism," or "Chaos" are interpretive language. They may be artistically useful but cannot silently replace L0–L8.

## 3. Project primitives

### CausalFrame
The smallest provenance-bearing input.

### StateMembrane
A temporal boundary operator, not a personality model.

### WaveCell

\[
W_i[k] = A_i(S_k,F_k)\,\sin(\phi_i[k])
\]

### BodyChannel

\[
\theta_j[k] = \operatorname{clip}\left(b_j(S_k) + \sum_i B_{ji}W_i[k],-\ell_j,\ell_j\right)
\]

### PoseFrame
The complete renderer contract at one tick.

### EvidenceChain
A deterministic sequence of pose-frame bytes plus a run-level cryptographic hash.

## 4. What the ontology refuses

- sensor telemetry is not the world itself;
- classification is not consciousness;
- deterministic waves are not proof of biological life;
- visible response is not proof of causal validity unless the input and transformation are preserved;
- a project-specific primitive is not automatically a novel scientific theorem.
