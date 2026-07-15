# World 2.0 demo mechanics

## Evidence rule

A demo interaction is admitted only when the source class, state transition, consumer mapping, and failure behavior are visible. Spectacle does not substitute for evidence.

## Implemented low-risk interactions

### 1. Verified trace scrubber

The viewer can select any of 720 byte-verified frames and inspect:

- input source;
- strength, variability, and novelty;
- behavior;
- integer pose;
- freshness and sequence;
- frame and trace hashes;
- the plain-language reason for the state.

This uses `SIMULATED` evidence and makes no live-sensing claim.

### 2. Replay

The same committed trace drives the browser pose in sequence. C and Rust must first produce byte-identical JSONL with the recorded SHA-256.

### 3. Sender stop

Selecting sender stop jumps to the declared STALE segment. A later live bridge test must independently prove that a real disconnected sender causes the same failure state.

### 4. Clickable runtime contracts

The viewer can select sensor, kernel, UI, game, renderer, and AI nodes to see their permitted and forbidden tasks.

## Pending controlled experiment: physical attenuation

A foil pouch, movement, hotspot, or other nearby object may change connected-link telemetry, but no object is guaranteed to produce a particular feature value or state transition. Deliberate interference must also respect local radio regulations and venue/network policy.

This interaction remains `PENDING_RUNTIME_TEST` until a preserved experiment records:

- the exact action and timing;
- raw/minimized link telemetry;
- configuration and hardware;
- observed features and state;
- failed as well as successful trials;
- latency and repeatability;
- a no-action control;
- the complete checksum manifest.

Do not use wording such as “instant microscopic fluctuation,” “localized RF noise measurement,” or “the action forces STORM” without that evidence.

## Example game mappings

| Input state | Bounded mechanic | Evidence behavior |
|---|---|---|
| ATTUNE | reveal a path | retain source and sequence |
| STORM | charge a hazard | label as a game mapping |
| STALE | pause scoring | no reward from missing data |

These mechanics make the abstraction playable. They do not enlarge the measurement claim.
