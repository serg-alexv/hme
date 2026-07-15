# WORLD 2.0 Experiments

Experiments are immutable evidence units. Do not rewrite a completed experiment to improve the story; create a new experiment and compare them.

## Directory convention

```text
experiments/
  EXP-0001-live-bridge-proof/
    README.md
    manifest.json
    raw/
    derived/
    logs/
    media/
    checksums.sha256
    result.md
```

Large raw files and videos may be stored in a release, object store, or Git LFS. Their stable references, byte sizes, provenance, and hashes must remain in the experiment directory.

## Required manifest fields

- experiment ID and title;
- operator;
- UTC start/end;
- objective and hypothesis;
- hardware and software versions;
- source class;
- input and output schemas;
- configuration and thresholds;
- exact commands;
- artifact locations, sizes, and SHA-256 values;
- record counts;
- pass/fail criteria;
- observed result;
- residual uncertainty.

## Non-negotiable rules

1. A hash is not a backup.
2. A screenshot is not a raw trace.
3. A polished video is not sufficient without provenance and configuration.
4. Missing data is recorded as `EVIDENCE_MISSING`, never reconstructed by narrative.
5. Failed experiments remain valuable and must not be deleted.

## First experiment

`EXP-0001-live-bridge-proof` must demonstrate:

```text
input changes
→ features change
→ state changes
→ dragon changes
→ sender stops
→ stale behavior appears
```
