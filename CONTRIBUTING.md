# Contributing to LEGION

## Before opening work

Read:

1. `README.md`
2. `GOVERNANCE.md`
3. `AGENTS.md`
4. `STATUS.md`
5. `CLAIMS.md`
6. `architecture/SYSTEM.md`

## Work must begin with an issue

The issue must define:

- observable objective;
- exact scope;
- owner/agent role;
- inputs;
- acceptance criteria;
- evidence to preserve;
- security and claims impact;
- Y/N decision requested.

## Branch naming

```text
feat/LEGION-0000-short-name
fix/LEGION-0000-short-name
docs/LEGION-0000-short-name
exp/LEGION-0000-short-name
```

## Commit rules

Use imperative messages with a scope:

```text
feat(sensor): preserve direct RSSI and adapter metadata
fix(lens): reject stale sequence updates
docs(claims): narrow Wi-Fi sensing language
exp(bridge): freeze EXP-0001 evidence manifest
```

## Pull-request requirements

A PR must include:

- linked issue;
- before/after status;
- changed files;
- tests and commands run;
- logs or artifact links;
- claims impact;
- security impact;
- rollback procedure;
- residual uncertainty;
- requested Y/N decision.

## Evidence requirements

Implementation claims require at least one of:

- automated test output;
- preserved runtime log;
- screen recording;
- raw and derived trace bundle;
- checksum inventory;
- independent reproduction.

Screenshots without source inputs and versions are supporting material, not complete evidence.

## Large artifacts

Do not commit large videos, raw captures, FBX packages, or generated archives directly unless Git LFS and retention policy are intentionally configured. Commit manifests, checksums, provenance, and stable storage/release references.

## Claims safety

Update `CLAIMS.md` when a change affects:

- what is measured;
- how features are derived;
- what is verified;
- scientific or safety wording;
- architecture maturity;
- authorship, ownership, or partnership language.

## Agent-generated contributions

Agent PRs must disclose:

- agent/model/tool identity when known;
- prompt/task contract;
- source material;
- files changed;
- tests actually executed;
- areas not independently verified.

An agent may not approve its own material architecture, schema, security, money, or public-claims change.

## Merge standard

```text
correct
+ scoped
+ reviewed
+ evidenced
+ reversible
+ status-consistent
```

Elegant but unverified work remains a draft.
