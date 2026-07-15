# WorldMath v0.1

**Status:** normative specification for WorldEngine v0.1.
**Scope:** deterministic transduction from a provenance-bearing feature frame to a semantic state and integer pose.
**Non-claim:** WorldMath is project mathematics, not a claim of a new universal theorem.

## 0. Minimal declaration

For discrete tick \(k\):

\[
|O_k\rangle \xrightarrow{\Phi_a} |F_k\rangle
\xrightarrow{\mathcal T_{\theta}} (|S_k\rangle,H_{k+1})
\xrightarrow{\mathcal E_{\beta}} |P_k\rangle.
\]

Where:

- \(|O_k\rangle\) is an observation plus its provenance envelope;
- \(\Phi_a\) is a versioned adapter profile;
- \(|F_k\rangle\) is a bounded feature vector;
- \(H_k\) is transition history;
- \(\mathcal T_{\theta}\) is the state operator under configuration \(\theta\);
- \(|S_k\rangle\) is one of four semantic states;
- \(\mathcal E_{\beta}\) is the embodiment operator under binding \(\beta\);
- \(|P_k\rangle\) is the integer pose and evidence frame.

The ket notation denotes typed ordered vectors. It does not import quantum superposition, measurement, Hilbert-space dynamics, or physical interpretation.

## 1. Axioms

### A1 — Observation is not meaning

An observation records what an adapter reports. Features summarize observations. States select policy. Poses embody policy. None of these layers may silently impersonate another.

### A2 — Provenance is part of state

Every authoritative frame carries source class, sequence, timestamp, validity, and freshness. A consumer may display these values but may not relabel them.

### A3 — Authority flows one way

```text
adapter → feature contract → state operator → embodiment operator → consumer
```

A renderer, game, UI, or AI assistant cannot write backward into authoritative history.

### A4 — History is explicit

Hysteresis and dwell are contained in \(H_k\). A stateless reimplementation is non-conforming even if it matches some isolated frames.

### A5 — Replay is a byte-level claim

Given identical feature-frame bytes, configuration, initial history, integer semantics, and serialization version, conforming implementations must emit identical pose-frame bytes.

## 2. Observation and feature boundary

The general observation envelope is:

\[
|O_k\rangle=(q_k,t_k,c_k,v_k,a_k,x_k),
\]

with sequence \(q\), timestamp \(t\), source class \(c\), validity \(v\), freshness \(a\), and adapter-specific measurement vector \(x\).

The adapter profile \(\Phi_a\) produces:

\[
|F_k\rangle=(q_k,t_k,c_k,v_k,a_k,s_k,r_k,n_k),
\]

where \(s\) is strength, \(r\) variability, and \(n\) novelty in unsigned Q0.16.

WorldEngine v0.1 begins at \(|F_k\rangle\). The committed golden schedule supplies feature frames directly. It therefore verifies \(\mathcal T\) and \(\mathcal E\), not a live sensor-specific \(\Phi_a\). Every live adapter requires a separate versioned specification and acceptance test.

## 3. Codes and numerical domains

### Source

| Code | Symbol |
|---:|---|
| 0 | `SIMULATED` |
| 1 | `RECORDED_REPLAY` |
| 2 | `LIVE_WLAN` |
| 3 | `STALE_WLAN` |

### Semantic state

| Code | Symbol |
|---:|---|
| 0 | `CALM` |
| 1 | `ATTUNE` |
| 2 | `STORM` |
| 3 | `STALE` |

### Fixed-point formats

\[
q_{16}(x)=\operatorname{round}(65535x),\quad x\in[0,1].
\]

- features: unsigned Q0.16;
- oscillator samples: signed Q1.30;
- phase: wrapping unsigned Q0.32 turn;
- joint angles: signed integer millidegrees;
- root displacement: signed integer microunits.

All narrowing, saturation, shifts, and wrap operations are part of the contract.

## 4. Transition operator \(\mathcal T\)

Let \(S\) be the current state and \(F=(s,r,n,a,v)\). Default configuration:

| Parameter | Q0.16 / ticks | Approximate real value |
|---|---:|---:|
| stale after | 1500 ms | 1.5 s |
| ATTUNE enter | 16384 | 0.2500 |
| ATTUNE exit | 10486 | 0.1600 |
| STORM variability enter | 36044 | 0.5500 |
| STORM novelty enter | 39321 | 0.6000 |
| STORM variability exit | 24903 | 0.3800 |
| STORM novelty exit | 26214 | 0.4000 |
| ATTUNE dwell | 8 ticks | 0.133 s at 60 Hz |
| STORM dwell | 6 ticks | 0.100 s at 60 Hz |
| CALM dwell | 24 ticks | 0.400 s at 60 Hz |
| stale recovery dwell | 20 ticks | 0.333 s at 60 Hz |

Candidate selection is ordered and normative:

1. if `valid == false` or `freshness_ms > 1500`, candidate is `STALE`;
2. else if current state is `STALE`, candidate is `CALM`;
3. else if current state is `STORM`, remain `STORM` while \(r\ge24903\land n\ge26214\), otherwise candidate is `ATTUNE`;
4. else if \(r\ge36044\land n\ge39321\), candidate is `STORM`;
5. else if current state is `ATTUNE`, candidate is `CALM` only when \(r\le10486\land n\le10486\), otherwise remain `ATTUNE`;
6. else candidate is `ATTUNE` when \(r\ge16384\lor n\ge16384\), otherwise `CALM`.

The candidate counter is set to one when the candidate changes and saturating-incremented when it persists. The state changes only after the candidate reaches its required dwell. `STALE` requires one tick. Recovery from `STALE` always passes through `CALM` and requires 20 valid ticks.

The six-frame STORM persistence rule is therefore real, but it is not a standalone `0.50/0.50` threshold. Thresholds and hysteresis are configuration data and must not be simplified differently in another runtime.

## 5. Fixed simulation clock

The authoritative clock is the integer tick:

\[
\tau_k=\frac{k}{60}\text{ seconds}.
\]

One accepted input frame advances the engine exactly one tick. Renderer frame time, network arrival jitter, and wall-clock delta do not alter the authoritative phase step. `timestamp_ms` is provenance metadata and is hashed; it is not an integration timestep.

Golden fixtures serialize timestamps as:

\[
t_k=t_0+\left\lfloor\frac{1000k}{60}\right\rfloor.
\]

## 6. Wave field

For wave cell \(i\):

\[
\phi_i[k+1]=(\phi_i[k]+\Delta_i[S_k,F_k])\bmod 2^{32}.
\]

The implementation computes \(\Delta_i\) from a committed base phase step, state-frequency multiplier, variability, and novelty using integer products and explicit shifts.

The bounded wave sample is:

\[
W_i[k]=\left(\operatorname{sin}_{Q1.30}(\phi_i[k])A_i[S_k,F_k]\right)\gg30.
\]

`sin` is a committed 257-entry quarter-wave table with deterministic interpolation. Runtime floating-point trigonometry is forbidden in the authority kernel.

## 7. Embodiment operator \(\mathcal E\)

Let \(W_k\in\mathbb Z^7\) be the wave vector, \(B\in Q0.15^{9\times7}\) the sparse binding matrix, \(b(S)\) the state offset, and \(\ell\) the joint limits:

\[
\Theta_k=\operatorname{clip}(b(S_k)+BW_k,-\ell,+\ell).
\]

WorldEngine v0.1 emits root vertical displacement plus nine joint channels:

```text
root roll
neck 1 pitch
neck 2 pitch
head yaw
left wing roll
right wing roll
tail 1 yaw
tail 2 yaw
tail 3 yaw
```

Consumer adapters may reinterpret the same state for a UI, game, audio engine, or agent visualization, but a consumer-specific effect is not part of the authoritative pose unless it is added through a new versioned binding.

## 8. Pose and hash

\(|P_k\rangle\) contains:

```text
sequence, tick, timestamp, freshness,
strength, variability, novelty,
validity, source, state,
root displacement, joint vector, frame hash
```

The frame hash is 64-bit FNV-1a over the fields in the exact order defined by the C and Rust implementations, using little-endian byte iteration for each integer. It is a deterministic integrity marker, not a signature and not proof of physical origin.

The JSON Lines boundary is `world.pose/0.1`. Canonical key order, decimal formatting, lowercase hexadecimal hash formatting, and the final newline are part of byte-level trace conformance.

## 9. Runtime allocation

| Runtime | Normative responsibility |
|---|---|
| sensor adapter | implement and version \(\Phi_a\); preserve provenance and errors |
| C WorldEngine | reference \(\mathcal T\), \(\mathcal E\), pose hash, serialization |
| Rust WorldEngine | byte-identical mirror and monitoring host |
| Lens Studio | resolve named bones and apply integer pose values only |
| browser/UI/game | inspect or map authoritative frames without rewriting them |
| AI assistant | explain traces, locate drift, or propose a reviewable new configuration outside the fast loop |

## 10. Conformance

A runtime conforms to WorldMath v0.1 only if it passes:

1. state-code and source-code tests;
2. every ordered transition branch, including recovery from stale under high features;
3. dwell and hysteresis boundary tests;
4. integer overflow, wrap, rounding, and joint-limit tests;
5. deterministic repeated execution;
6. the shared 720-frame fixture;
7. byte equality against the C reference trace;
8. expected transitions and full-trace SHA-256;
9. `world.pose/0.1` schema validation;
10. first-divergence reporting when equality fails.

No renderer screenshot, animation quality judgment, or AI explanation substitutes for these tests.

## 11. Intellectual discipline

WorldMath takes engineering lessons from Fourier decomposition, explicit state evolution, formal limits, nonequilibrium time, wave control, and Dirac's economy of representation. Grigori Perelman is an ambition standard for depth and consequence, not evidence that this project has achieved comparable mathematics.

Historical lineage may motivate work. Only definitions, implementations, tests, preserved evidence, and independent reproduction establish the technical result.
