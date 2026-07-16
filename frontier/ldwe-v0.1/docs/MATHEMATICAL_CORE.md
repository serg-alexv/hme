# Mathematical Core

## 1. Determinism contract

```text
same input bytes
+ same configuration
+ same engine build
= same pose-frame bytes
```

The authoritative kernel avoids wall-clock integration, heap-dependent ordering, runtime random numbers, and floating-point trigonometry.

## 2. Fixed-point representation

Features use unsigned Q0.16:

\[
q(x)=\operatorname{round}(65535x), \quad x\in[0,1].
\]

Wave samples use signed Q1.30. Joint rotations are integer millidegrees. Root vertical displacement is integer microunits. Lens performs the final integer-to-float conversion only at the renderer boundary.

## 3. Phase accumulator

Each `WaveCell` owns a 32-bit phase:

\[
\phi_i[k+1] = \left(\phi_i[k] + \Delta_i(S_k,F_k)\right)\bmod 2^{32}.
\]

A committed quarter-wave lookup table produces deterministic sine values. The C and Rust implementations carry the same table.

## 4. Feature-conditioned frequency

\[
\Delta_i = \Delta_i^{(0)}\left[m_S + \alpha_i v_k + \beta_i n_k\right]
\]

The expression is implemented with integer multiplication and explicit shifts.

- `CALM`: low-frequency breathing and tail motion;
- `ATTUNE`: stronger orienting and wing preparation;
- `STORM`: higher-frequency wing/tail activity plus bounded tremor;
- `STALE`: low-energy droop and slow residual motion.

## 5. Feature-conditioned amplitude

\[
A_i[k] = A_{i,S}\left(\frac{1}{2}+\frac{s_k}{2}\right)\left(1+\gamma_i v_k+\eta_i n_k\right).
\]

All amplitudes are bounded before skeleton projection.

## 6. State membrane

The membrane is a deterministic hybrid system combining bounded continuous features, discrete state, and dwell counters. A candidate transition is accepted only when it persists for the configured dwell length. `STALE` is immediate when input is invalid or too old; recovery requires a valid dwell interval.

## 7. Skeleton projection

Let \(W[k]\in\mathbb{Z}^m\) be the wave vector and \(B\in\mathbb{Q}^{j\times m}\) the sparse binding matrix:

\[
\Theta[k] = \operatorname{clip}(b_{S_k}+BW[k],L^{-},L^{+}).
\]

The first implementation drives root roll/bob, neck pitch, head yaw, mirrored wing roll, and three phase-shifted tail yaw channels.

## 8. Intellectual basement

This engine does not claim to reproduce the full theories of these figures. It takes disciplined engineering lessons from them:

- **Fourier:** controlled oscillatory decomposition;
- **Schrödinger:** lawful state evolution rather than decorative randomness;
- **Gödel:** formal boundaries must remain explicit;
- **Prigogine:** temporality, fluctuation, and open-system behavior matter;
- **Dirac:** maximum operational force with minimum unnecessary machinery;
- **Fink:** identity must be confirmed before citation. If Mathias Fink is intended, the relevant lineage is wave control and time reversal in complex media.

Intellectual lineage is not proof of scientific novelty.
