# WORLD 2.0 External Publishing Rule

## Purpose

This policy governs publication or distribution of project material outside the controlled WORLD 2.0 repository workflow.

## Core rule

No human or agentic contributor may publish AI-assisted project material to an external resource without direct revision or acceptance by the repository technical steward.

External resources include, without limitation:

- journals and conference proceedings;
- preprint servers;
- DOI repositories such as Zenodo or OSF;
- institutional repositories;
- grant, accelerator, residency, museum, festival, or partner submissions;
- public websites, newsletters, media releases, and social platforms when they present technical or scholarly claims;
- GitHub Releases intended as public archival publications rather than ordinary repository development history.

## Required sequence

```text
agent or team draft
→ source and evidence check
→ repository technical-steward revision
→ claims-register consistency check
→ verified project-owner approval when Class D authority applies
→ external publication
→ repository record of the published version, destination, date, identifier, and checksum
```

## Authority

- The **repository technical steward** performs or explicitly accepts the final technical and editorial revision of AI-assisted publication material.
- Public releases require technical verification, an explicit artifact manifest, and confirmation that distribution rights are documented.
- Agents may research, draft, format, compare venues, prepare metadata, and stage submission packages. They may not independently perform the final external publication action unless the required human approvals have been recorded for that exact version and destination.

## Version identity

Approval applies only to the exact candidate reviewed. Any subsequent material change to claims, authorship, title, abstract, figures, conclusions, venue, license, or public metadata reopens review.

The publication record must preserve:

- repository commit or tag;
- final file checksum;
- author and contributor statement;
- destination and public identifier;
- publication date;
- license;
- recorded approvals;
- known limitations or post-publication corrections.

## Repository publication versus external publication

Ordinary commits to the WORLD 2.0 repository may be used for controlled working drafts and historical development. They do not constitute approval for external scholarly, archival, commercial, institutional, or media publication.

## Emergency correction

When a materially incorrect external claim is discovered, the team must:

1. record the defect;
2. notify the responsible authorities;
3. correct, retract, or annotate the external version where possible;
4. preserve the original and corrected records;
5. update `CLAIMS.md`, relevant status files, and the publication record.

## Standing team rule

This is a common rule for all present and future human and agentic team members. It remains active until changed through the applicable governance process.
