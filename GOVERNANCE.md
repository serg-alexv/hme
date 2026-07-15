# LEGION Governance

## 1. Authority

1. **Founder / original core author:** Mika (`serg-alexv`).
2. The founder owns the project thesis, ontology, product direction, acceptance criteria, release authority, and final Y/N decisions.
3. Contributors—human or agentic—earn authority through scoped responsibility, evidence, and review. No model, tool, sponsor, or platform silently acquires authorship or governance rights.

## 2. Standing technical-supervision charter

The assistant requested in the founding session is assigned the standing repository function:

- organizer of repository structure and work queues;
- maintainer of status, claims, decisions, and validation discipline;
- adversarial technical reviewer;
- supervisor of agent handoffs and evidence requirements;
- reviewer of pull requests when explicitly invoked through an available GitHub connection.

This charter is **persistent repository policy**, not a claim of autonomous continuous execution. It remains active until the founder changes it in a reviewed commit. The assistant cannot act while no session/tool authorization exists and cannot make the role technically immutable.

## 3. Y/N mode

Every critical-path proposal must end in one of two states:

- **Y — ADMIT:** the gate, owner, evidence requirement, rollback, and due condition are recorded.
- **N — BLOCK:** the reason is recorded as rejected, deferred, unsafe, unsupported, or premature.

`MAYBE` may exist in research notes but cannot authorize implementation, expenditure, public claims, or release.

## 4. Evidence hierarchy

From strongest to weakest:

1. reproducible artifact with raw inputs, versions, logs, checksums, and pass criteria;
2. runtime observation reproduced by an independent operator;
3. source-level inspection and automated tests;
4. documented design pending runtime verification;
5. hypothesis or future research direction;
6. narrative, analogy, or marketing language.

A weaker evidence class may not be described using the language of a stronger class.

## 5. Change classes

### Class A — reversible documentation

May be merged after one reviewer verifies internal consistency.

### Class B — reversible implementation

Requires tests or a validation plan and an explicit owner.

### Class C — architecture, schema, claims, security, money, or irreversible data change

Requires founder approval and written Y/N closure.

### Class D — public release, external commitments, ownership, licensing, or investor promises

Requires founder approval; legal or financial review may also be required.

## 6. Pull-request policy

Every PR must state:

- problem;
- scope;
- status before and after;
- evidence produced;
- risks;
- rollback;
- claim changes;
- Y/N decision requested.

No agent may merge its own material Class C or D change without independent review.

## 7. Agent accountability

Agents are treated as fallible contractors, not authorities. Every agent output must preserve:

- source attribution;
- assumptions;
- uncertainty;
- changed files;
- test results;
- unverified claims;
- next handoff.

Fabricated execution, fabricated citations, or concealed uncertainty is a blocking defect.

## 8. Repository continuity

The repository—not any individual chat—is the durable operational memory. Critical decisions must be committed to:

- `STATUS.md`
- `CLAIMS.md`
- `decisions/`
- `experiments/`
- `docs/ROADMAP.md`

Chat history alone is not accepted as project state.
