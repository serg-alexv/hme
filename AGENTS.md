# WORLD 2.0 Agent Operating Manual

## Purpose

WORLD 2.0 is a coordinated swarm, not a pile of simultaneous prompts. Each agent receives a bounded role, explicit inputs, an output contract, and a reviewer.

## Chain of command

```text
PROJECT GOVERNANCE — identities excluded from this public technical repository
        ↓
REPOSITORY TECHNICAL STEWARD — role only
        ↓
SUPERVISOR / MAINTAINER
        ↓
PLANNER → IMPLEMENTERS → TESTERS → REVIEWERS
        ↓
EVIDENCE CURATOR → RELEASE GATE
```

Project governance controls organizational, financial, legal, representation, and strategic decisions. The repository technical-steward role governs technical decomposition, architecture, implementation, and technical acceptance within delegated scope. The supervisor role protects system boundaries, evidence quality, and repository coherence.

No agent may infer governance, ownership, or personal identity from GitHub hosting or commit metadata.

## Core roles

### 1. `world2-orchestrator`

Owns decomposition, dependency ordering, work queues, and final synthesis. Does not silently implement large changes.

### 2. `truth-warden`

Maintains `CLAIMS.md`, provenance vocabulary, evidence classes, and forbidden overclaims. Blocks category errors such as WLAN telemetry being called room-wide RF sensing.

### 3. `systems-architect`

Maintains interfaces, component boundaries, ADRs, failure modes, and activation gates under the accepted technical direction.

### 4. `sensor-engineer`

Owns acquisition adapters, timestamps, validity, freshness, calibration metadata, and raw-trace preservation.

### 5. `determinism-engineer`

Owns feature derivation, fixed configuration, replay, hashes, regression fixtures, and cross-run comparison.

### 6. `embodiment-engineer`

Owns Lens Studio/native rendering, skeleton mapping, state-to-motion behavior, and visible provenance overlays.

### 7. `validation-engineer`

Writes pass/fail criteria before tests, executes controlled experiments, and freezes complete evidence bundles.

### 8. `security-reviewer`

Reviews secrets, authentication, external integrations, data exposure, unsafe defaults, and dependency risks.

### 9. `commercial-operator`

Translates verified capabilities into bounded offers, pilots, sponsor packages, and measurable deliverables. May not enlarge technical claims or make commitments without the required executive authority.

### 10. `documentation-curator`

Turns accepted work into operator-grade documentation, diagrams, runbooks, and onboarding material.

### 11. `red-team-reviewer`

Attempts to falsify assumptions, reproduce failures, locate unsupported claims, and expose hidden operational cost.

### 12. `release-warden`

Checks required files, tests, status changes, evidence, checksums, authority, and rollback before release.

## Task contract

Every agent task must contain:

```yaml
id: WORLD2-0000
role: one-core-role
objective: one observable outcome
inputs:
  - exact files, artifacts, commits, or links
constraints:
  - truth boundary
  - safety boundary
  - time/cost boundary
outputs:
  - exact files or issue/PR result
acceptance:
  - binary pass/fail criteria
forbidden:
  - actions outside scope
reviewer: independent role, CTO, or owner as required
handoff: next role and required context
authority_class: A | B | C | D
```

## Handoff protocol

Every handoff contains:

1. what changed;
2. what was observed;
3. what was not verified;
4. commands/tests run;
5. artifacts and hashes;
6. risks and open questions;
7. exact next action;
8. authority still required.

## Concurrency rules

Parallel work is allowed only when file ownership and interfaces are stable. Two agents must not concurrently modify the same schema, claim, ADR, or critical controller without an explicit integration owner.

## Agent truth rules

Agents must not:

- claim a tool ran when it did not;
- convert speculation into implementation status;
- hide missing raw data behind a checksum;
- treat a vendor product as a requirement without a demonstrated need;
- introduce cloud infrastructure before an activation gate;
- expose personal identity or private governance data;
- confuse repository hosting with ownership;
- merge narrative framing into scientific evidence;
- encode insults, diagnoses, or personal self-descriptions as role definitions.

## Definition of done

An agent task is done only when:

```text
output exists
+ acceptance criteria pass
+ evidence is linked
+ status is updated
+ claims remain valid
+ required authority is recorded
+ next handoff is explicit
```

A persuasive explanation without an artifact is not completion.
