# LEGION Governance

## 1. Project authority

1. **Project owner, executive principal, primary investor, and core organizational person:** Leo Shaposhnikov.
2. Leo holds final authority over ownership, financing, organizational commitments, public representation, investor promises, licensing, legal structure, and continuation or termination of the program.
3. **Founding CTO, original technical originator, and chief systems architect:** Mika (`serg-alexv`).
4. Mika owns the original technical thesis, system ontology, architecture proposals, implementation lineage, and technical acceptance criteria within the authority delegated by Leo.
5. Mika is not the default public face and does not unilaterally control project ownership, money, legal commitments, or organizational representation.
6. Contributors—human or agentic—earn authority through scoped responsibility, evidence, and review. No model, tool, sponsor, platform, repository host, or technical contributor silently acquires ownership or governance rights.

The repository is currently hosted under `serg-alexv`. GitHub hosting and CODEOWNERS enforcement are operational facts, not declarations of legal or executive ownership.

## 2. Authority matrix

| Domain | Final authority | Technical/operational authority |
|---|---|---|
| Ownership, financing, legal structure | Leo | advisory input only unless delegated |
| Public face, external representation | Leo | Mika only when explicitly delegated |
| Product continuation and strategic commitments | Leo | Mika proposes technical consequences |
| Core technical ontology and architecture | Leo approves material direction | Mika authors and maintains |
| Implementation and experiments | governed budget/scope from Leo | Mika and assigned engineering roles |
| Technical truth, evidence, and claims | no unsupported claim may be ordered into existence | truth-warden and technical reviewers may block |
| Releases | Leo for external/strategic releases | Mika for technical readiness within delegation |

Evidence rules constrain everyone. Ownership authority does not convert an unverified technical claim into a verified one, and technical authority does not create financial or executive authority.

## 3. Standing technical-supervision charter

The assistant requested in the founding session is assigned the standing repository function:

- organizer of repository structure and work queues;
- maintainer of status, claims, decisions, and validation discipline;
- adversarial technical reviewer;
- supervisor of agent handoffs and evidence requirements;
- reviewer of pull requests when explicitly invoked through an available GitHub connection.

This charter is persistent repository policy, not a claim of autonomous continuous execution. It remains active until the authorized project governance changes it in a reviewed commit. The assistant cannot act while no session or tool authorization exists and cannot make its role technically irreversible.

## 4. CTO operating constraints

LEGION does not encode personal insults, diagnoses, or self-denigrating labels as governance facts. Operational risk is handled through controls:

- Mika may not make unilateral Class D commitments.
- Budget, ownership, licensing, public representation, and investor promises require Leo’s approval.
- Material architecture or claim changes require recorded evidence and review.
- High-intensity ideation is separated from admission to the critical path.
- Reversible experiments are encouraged; irreversible actions require independent review.
- Any contributor may trigger a stop-and-review when evidence, safety, money, or ownership boundaries are unclear.

These controls preserve Mika’s technical originality and speed without making personal stability a hidden dependency of project governance.

## 5. Y/N mode

Every critical-path proposal must end in one of two states:

- **Y — ADMIT:** the gate, owner, evidence requirement, rollback, and due condition are recorded.
- **N — BLOCK:** the reason is recorded as rejected, deferred, unsafe, unsupported, or premature.

`MAYBE` may exist in research notes but cannot authorize implementation, expenditure, public claims, or release.

## 6. Evidence hierarchy

From strongest to weakest:

1. reproducible artifact with raw inputs, versions, logs, checksums, and pass criteria;
2. runtime observation reproduced by an independent operator;
3. source-level inspection and automated tests;
4. documented design pending runtime verification;
5. hypothesis or future research direction;
6. narrative, analogy, or marketing language.

A weaker evidence class may not be described using the language of a stronger class.

## 7. Change classes

### Class A — reversible documentation

May be merged after one reviewer verifies internal consistency.

### Class B — reversible implementation

Requires tests or a validation plan and an explicit owner.

### Class C — architecture, schema, claims, security, money, or irreversible data change

Requires the appropriate authority from the matrix above and written Y/N closure.

### Class D — public release, external commitments, ownership, licensing, legal structure, investor promises, or material expenditure

Requires Leo’s approval; legal or financial review may also be required. Mika’s technical assessment is required when the commitment depends on system capability or delivery feasibility.

## 8. External publishing rule

AI-assisted or agent-produced material may be developed and versioned inside the repository, but it may not be published to an external scholarly, archival, institutional, commercial, partner, grant, media, or public-distribution resource until Mika has directly revised or explicitly accepted the exact publication candidate.

External publication then requires the applicable authority under the matrix above. In particular, Leo retains approval over Class D consequences such as external representation, licensing, legal commitments, strategic releases, investor promises, and material expenditure.

The authoritative procedure is defined in [`docs/PUBLISHING.md`](docs/PUBLISHING.md). Approval attaches only to the exact reviewed version and destination; material changes reopen review.

## 9. Pull-request policy

Every PR must state:

- problem;
- scope;
- status before and after;
- evidence produced;
- risks;
- rollback;
- claim changes;
- authority required;
- Y/N decision requested.

No agent may merge its own material Class C or D change without independent review.

## 10. Agent accountability

Agents are treated as fallible contractors, not authorities. Every agent output must preserve:

- source attribution;
- assumptions;
- uncertainty;
- changed files;
- test results;
- unverified claims;
- next handoff.

Fabricated execution, fabricated citations, concealed uncertainty, silent ownership reassignment, or unauthorized external publication is a blocking defect.

## 11. Repository continuity

The repository—not any individual chat—is the durable operational memory. Critical decisions must be committed to:

- `OWNERSHIP.md`
- `STATUS.md`
- `CLAIMS.md`
- `docs/PUBLISHING.md`
- `decisions/`
- `experiments/`
- `docs/ROADMAP.md`

Chat history alone is not accepted as project state.
