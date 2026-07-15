# Comparable Embodiment Systems

**Status:** verified research note
**Snapshot:** 2026-07-15
**Scope:** systems that convert external state into bounded, real-time visual or physical behavior

This is not a competitor list and it is not evidence for today's investor claim. It is a pattern library for WorldEngine implementations.

Today's proof remains one causal chain:

```text
Windows interface byte counters
-> measured throughput
-> normalized load
-> procedural joint amplitude
-> visible dragon movement
```

The systems below are comparable only where they solve part of the same engineering problem: ingest state, preserve timing, apply bounded transforms, render under runtime constraints, and expose enough state to debug failure.

## Verified matrix

| System | External-state boundary | Timing and state model | Embodiment mechanism | Pattern WorldEngine should reuse |
|---|---|---|---|---|
| [Snap Lens Studio](https://developers.snap.com/lens-studio/features/animation/animation-state-graph-editor) | scripts set typed animation parameters at runtime | state graph evaluates transitions from `Float`, `Int`, `Bool`, and `Trigger` parameters | clips, transitions, weights, additive/override layers, or direct scripted state changes | Treat measured load as an input parameter; keep measurement and animation state separate; expose current state and transitions for inspection. |
| [TikTok Effect House](https://effecthouse.tiktok.com/learn/guides/tutorials/apjs-api/animation-character-state-machine) | TypeScript component or visual graph updates runtime state | per-frame `onUpdate` logic or named animation clips selected by a state machine | transform mutation, rigged FBX animation, animator playback, fade-in | A second constrained mobile-AR precedent for state-to-transform and state-to-clip mappings. Its [performance rules](https://effecthouse.tiktok.com/learn/guides/getting-started/technical-guidelines/technical-optimization) make budgets explicit. |
| [Unity](https://docs.unity.cn/6000.1/Documentation/Manual/AnimationStateMachines.html) | scripts write Animator parameters | per-frame `Update` is distinct from [fixed updates](https://docs.unity.cn/6000.0/Documentation/Manual/fixed-updates.html); Animator state machines and blend trees consume parameters | animation clips, transitions, layers, blend trees, scripted properties | Keep the authoritative simulation clock distinct from render frames; use a continuous load parameter for blending and semantic states for discrete reactions. |
| [Unreal Engine](https://dev.epicgames.com/documentation/en-us/unreal-engine/state-machines-in-unreal-engine) | gameplay or replicated state feeds Animation Blueprint variables | tick-driven state, transition rules, state events, bounded transitions per frame | Animation Blueprints, state machines, root motion, skeletal pose | Separate essential authoritative state from cosmetic effects. Unreal's [networking overview](https://dev.epicgames.com/documentation/unreal-engine/networking-overview-for-unreal-engine?lang=en-US) makes that boundary explicit. |
| [Godot](https://docs.godotengine.org/en/stable/tutorials/animation/animation_tree.html) | script properties and expressions drive an `AnimationTree` | frame callbacks or fixed physics callbacks; state machine, transition, and 1D/2D blend nodes | property animation, skeletal clips, cross-fades, blend spaces | Small, readable reference for a portable adapter: input parameter -> blend position/state -> visible property or pose. |
| [ROS 2](https://design.ros2.org/articles/clock_and_time.html) | typed, timestamped messages from independent publishers | explicit system, steady, and replay/simulation time; logged data may be paused, slowed, accelerated, or replayed | downstream nodes consume state; `tf2` broadcasts [timestamped transforms](https://docs.ros.org/en/kilted/Tutorials/Intermediate/Tf2/Writing-A-Tf2-Broadcaster-Cpp.html) | Preserve timestamp, source, validity, and replay semantics. Never let the renderer silently replace missing or stale sensor data. |
| [Niantic Lightship](https://lightship.dev/docs/ardk/3.9/features/meshing/) | camera, depth, and tracking enter through AR Foundation interfaces | live spatial updates with a configurable mesh-update target rate | world mesh, colliders, anchors, and virtual objects interacting with estimated geometry | Camera-derived floor/mesh placement is a separate spatial adapter. It may locate the dragon; it does not manufacture network-load measurements. |

## What this comparison establishes

Across these systems, the reusable architecture is stable:

```text
timestamped external state
-> declared normalization
-> fixed or explicitly timed state update
-> bounded semantic state / continuous parameter
-> animation, transform, or material output
-> profiler, overlay, log, or replay evidence
```

WorldEngine's contribution is not inventing animation state machines, transforms, or sensor messages. Its claim must live in the explicit, reproducible transduction contract joining measurement provenance to embodied output.

## Dataset fields to collect

Each future platform entry must record:

| Field | Required question |
|---|---|
| Input surface | What can enter: script, HTTP, WebSocket, message bus, host API, file, or device sensor? |
| Provenance | Can source, timestamp, validity, and calibration be preserved? |
| Timing | Per-frame, fixed tick, callback, timestamped stream, or mixed? |
| Stale behavior | How is missing or delayed input represented? |
| Smoothing | Blend tree, interpolation, spring, filter, or none? |
| Embodiment | Bone, clip, transform, shader/material, audio, haptic, or spatial anchor? |
| Runtime budget | Package, mesh, joint, texture, CPU, GPU, memory, and frame-rate limits? |
| Inspection | Can current input, state, transition, and output be shown or logged? |
| Replay | Can a recorded input reproduce the same state/output sequence? |
| Privacy/security | What data leaves the device, and which platform policy governs it? |

## Exclusions from the verified matrix

- **Meta Spark AR:** useful historical prior art, but a retired platform is not a current execution target.
- **8th Wall:** relevant WebAR distribution precedent, but it is not needed to validate today's state-to-motion contract.
- **Generic “AI copilots”:** excluded because an assistant is not the real-time runtime, clock, sensor contract, or animation system.
- **Vendor claims without an official technical source:** not admitted.

## Decision for the current build

Use Lens Studio as the target embodiment runtime. Use the browser build as the inspectable investor proof. Borrow only the proven cross-system patterns:

1. timestamp and preserve the measured source;
2. normalize through a declared function;
3. keep state updates independent of render timing;
4. map load monotonically into a bounded pose envelope;
5. show the raw input and resulting pose together;
6. record enough information to replay and falsify the result.
