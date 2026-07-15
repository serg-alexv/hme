# Validation Plan

## V0 — Kernel determinism

Pass when C builds with warnings as errors, identical runs produce identical pose structs, the golden JSONL hash matches, all joint limits hold, and invalid input enters `STALE`.

## V1 — Cross-language equivalence

```text
same synthetic input
→ C pose JSONL
→ Rust pose JSONL
→ byte-for-byte equality
```

Any mismatch blocks the Rust monitor from becoming authoritative.

## V2 — Lens replay

Pass when the plug-in listens on `127.0.0.1:19331`, all 720 golden frames are accepted monotonically, CALM/ATTUNE/STORM/STALE are visible, frame hash prefixes match, and every driven bone resolves.

## V3 — Live WLAN

Pass when one uninterrupted recording preserves raw telemetry, bounded features, Lens preview/logger, movement-induced changes, sender-stop `STALE`, and the complete checksum manifest.

## V4 — Scientific value

Compare:

- direct state animation;
- unconstrained Lens `Math.sin`;
- Lorenz/RK4;
- WorldEngine fixed-point wave field;
- transition flicker;
- causal legibility;
- response latency;
- replay equality;
- CPU/memory cost;
- transfer to a second skeleton.

## V5 — Industry claim

No industry-shifting claim is admitted before independent reproduction, an external pilot, archival publication, measurable baseline improvement, or adoption by another engineering team.
