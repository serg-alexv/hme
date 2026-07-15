# Technical governance

This repository governs technical changes, evidence, and releases. It does not publish biographies, personal identities, private contacts, or ownership narratives.

## Change rules

- Claims require reproducible evidence or an explicit `NOT VERIFIED` label.
- Sensor provenance and measurement limits must remain visible.
- Consumers may render a `WorldStateFrame`; they may not relabel its source or recompute its authoritative state.
- Protocol changes require a schema-version change and compatibility note.
- Release artifacts require build output, tests, SHA-256, and a verified run record.
- Secrets, personal data, and unrelated incident history do not belong in the repository.

## Review roles

Roles are technical and non-personal: implementer, verifier, release operator, and documentation reviewer. A critical change may not be verified solely by the same execution that produced it.
