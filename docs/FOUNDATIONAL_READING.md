# Foundational and Adversarial Reading

World 2.0 uses the following external documents as a practical constraint set for advanced development. They are foundational because they force questions about control, resilience, access, provenance, and oversight. They are adversarial because each is also vendor- or sponsor-produced material and must not be mistaken for independent validation of this project.

The PDFs are referenced by title and page. They are not copied into this repository.

## 1. IDC / Red Hat

**Title:** *Digital Sovereignty in Action: Building Resilient, Compliant, and Transparent Cloud Ecosystems*
**Form:** IDC InfoBrief sponsored by Red Hat
**Date:** August 2025
**Document identifier:** IDC #EUR153367225

### Relevant framework

Page 4 presents a six-layer digital-sovereignty stack:

1. data;
2. technical;
3. operational;
4. assurance;
5. supply chain;
6. geopolitical.

Page 5 separates data, technical, and operational sovereignty and argues for interoperability, portability, transferability, and avoidance of proprietary lock-in.

Pages 9-11 apply sovereignty requirements to AI workloads, including governance, control, hardware independence, auditability, explainability, and deployment jurisdiction.

Page 22 recommends local expertise, portability, open platforms, skills development, comprehensive data control, security, and sovereignty-compliant AI solutions.

### World 2.0 application

| Sovereignty layer | Repository requirement |
|---|---|
| Data | minimize collection; document what is measured and omitted; preserve provenance |
| Technical | portable schemas; C and Rust conformance; renderer independence |
| Operational | explicit runtime permissions; human-reviewed publication and configuration changes |
| Assurance | deterministic replay; hashes; first-divergence reporting; stale failure |
| Supply chain | visible tool/model/dependency boundaries; no irreplaceable cloud component in the fast loop |
| Geopolitical | no assumption that one account, vendor, jurisdiction, or device guarantees continuity |

The publication is sponsored content. Its market statistics and product examples require independent confirmation before external use.

## 2. Google Workspace

**Title:** *Assured Controls*
**Form:** Google Workspace product guide
**Length:** 37 pages

### Relevant framework

Page 4 frames the conflict between collaboration and control of storage, processing, and access.

Page 17 gives a five-step operating model:

1. assess the environment;
2. choose data regions;
3. refine access controls;
4. strengthen data protection;
5. maintain reporting and oversight.

Page 25 applies least privilege to sensitive information. Page 29 connects encryption, access control, and data-loss prevention. Page 33 makes reporting and oversight part of proving that controls continue to work.

### World 2.0 application

1. **Assess:** state the sensor, source, validity, freshness, and exclusions before interpreting a frame.
2. **Place deliberately:** choose local, hosted, or distributed execution according to the data and continuity risk.
3. **Restrict:** each runtime receives only the permissions and fields required for its task.
4. **Protect:** no secret, identifier, or packet payload belongs in the public pose contract.
5. **Prove continuously:** every important claim needs a trace, version, test, and reviewable failure mode.

This is a Google product guide. Its product claims are not evidence that World 2.0 is secure, compliant, sovereign, or production-ready.

## Combined developer rule

For every advanced active component, answer before deployment:

```text
What is measured?
What is not measured?
Who can access it?
Where does it execute?
Can it be moved or replaced?
What survives account, device, or vendor failure?
How is the result reproduced?
How is failure made visible?
Who reviews publication or irreversible action?
```

If these questions have no concrete answer, the component is experimental and must be labeled accordingly.
