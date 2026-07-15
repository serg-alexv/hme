# LEGION

**A governed repository for Mika’s agentic engineering swarm.**

LEGION is the operational control plane for a growing team of human and AI contributors working on the World 2.0 program: deterministic environmental telemetry, embodied creatures, reproducible evidence, spatial computing, network sensing, and future product branches.

> **Founder and original core author:** Mika / `serg-alexv`  
> **Repository role requested for this assistant:** standing organizer, maintainer, reviewer, and technical supervisor—subject to repository policy, evidence, and the founder’s authority.

## Prime directive

```text
observe reality
→ preserve provenance
→ derive features deterministically
→ embody the result visibly
→ freeze evidence
→ advance only through explicit gates
```

## Current verified nucleus

- Windows connected-WLAN telemetry acquisition
- deterministic feature derivation and replay
- explicit provenance classes: `SIMULATED`, `RECORDED_REPLAY`, `LIVE_WLAN`, `STALE_WLAN`
- rigged dragon asset and procedural state controller
- packaged Lens Studio live bridge pending end-to-end verification

## Repository map

```text
.github/                 contribution control, issue forms, pull-request rules
agents/                  swarm roles, contracts, handoffs, operating prompts
architecture/            canonical system design and interfaces
decisions/               architecture decision records
claims/                  allowed claims and evidence requirements
docs/                    roadmaps, runbooks, validation and commercial tracks
experiments/             immutable experiment directories and manifests
schemas/                 machine-readable data contracts
src/                     implementation modules as they are admitted
artifacts/                pointers/manifests only; large binaries stay in release/storage
```

## Status vocabulary

Every component must use exactly one status:

- `VERIFIED`
- `PACKAGED`
- `PENDING_RUNTIME_TEST`
- `NOT_IMPLEMENTED`
- `OPTIONAL_RESEARCH`
- `REJECTED_FOR_CURRENT_PHASE`
- `EVIDENCE_MISSING`

## Y/N operating mode

For material decisions, LEGION defaults to binary closure:

```text
Y = approved and executable under the recorded gate
N = rejected, deferred, or blocked with a written reason
```

Ambiguity is allowed during exploration, but no ambiguous item may enter the critical path.

## Immediate gate

```text
signal changes
→ state changes
→ dragon changes
→ complete evidence bundle is frozen
```

See `STATUS.md`, `GOVERNANCE.md`, `AGENTS.md`, and `docs/ROADMAP.md` before contributing.
