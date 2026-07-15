# World 2.0 program scope

**Status:** research direction. This document does not establish implementation maturity.

## Question

How can a bounded observation of a physical or digital system become inspectable behavior in an object, shader, creature, sound, interface, or actuator without hiding provenance or overstating the measurement?

## Design target

```text
observation
→ declared transformation
→ bounded behavior
→ platform adapter
→ evidence
```

The platform adapter may target a browser, Lens Studio, a game engine, a native application, or hardware. Cross-platform use is not automatic: every adapter requires its own timing, security, privacy, and runtime verification.

## Current evidence

The current measured demonstration is narrower than the program:

```text
Windows aggregate interface byte counters
→ measured throughput
→ normalized load against a declared capacity
→ direct bounded browser-dragon pose
```

The repository separately verifies a fixed-point C/Rust kernel using a synthetic 720-frame feature schedule. The live measurement adapter does not yet drive that kernel, and the Lens Studio path is packaged but not verified end to end.

## Non-claims

The current work does not establish:

- room-wide RF sensing, Wi-Fi CSI, spectrum analysis, or radio localization;
- human activity, identity, physiology, emotion, or intent;
- biological life, consciousness, sentience, or autonomous agency;
- deployment over every platform or application;
- a universal mathematical theory of interaction with reality.

## Expansion rule

A new sensor, platform, or output is admitted only with:

1. a precise input boundary;
2. an explicit transformation;
3. validity and stale behavior;
4. privacy and retention rules;
5. a reproducible runtime test;
6. evidence that distinguishes the observation from the interpretation and output.
