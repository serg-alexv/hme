# Lens Studio creature asset candidates

**Observed in:** Lens Studio Asset Library 5.22.1 on 2026-07-15
**Status:** availability candidates; rigging, clips, editability, and redistribution are not yet verified

Lens Studio's official [Asset Library](https://www.developers.snap.com/lens-studio/assets-pipeline/asset-library/asset-library-overview) supports importing reusable 3D assets, packages, components, and plugins. The following names and package identifiers were reported by the Lens Studio session:

| Candidate | Reported package | First inspection question | Suitability for today's proof |
|---|---|---|---|
| Dragon Head | `Dragon_Head.lspkg` | Are neck/head bones or separate transforms exposed? | Thematically strong, but cannot prove wing/tail movement if it is only a static head. |
| Mark the Monster | `Mark_the_Monster.lspkg` | Is there a skinned full-body rig and at least one animation clip? | Strong first full-character candidate. Snap's documentation confirms the prefab exists in the Asset Library, but not that it supplies the rig needed here. |
| Cartoon Kitty Pack | `Cartoon_Kitty_Resources.lspkg` | Which meshes, bones, clips, and editable materials are included? | Friendly pet form; inspect before integration. |
| Mamori | `Mamori.lspkg` | Is it a full animated character with editable joints? | Candidate if rig access is better than the dragon asset. |
| Octopuses with Physics | `Octopuses_with_Physics.lspkg` | Are tentacles joint-, physics-, or shader-driven, and can load amplitude be injected? | Useful secondary-motion reference, not the dragon proof. |
| 2D Dragon with Physics | Asset Library item | Does it expose controllable 2D physics parameters? | Fast fallback only; it does not satisfy the rigged 3D dragon target. |

## Admission test

Install one candidate non-destructively, then record:

```text
asset/version
-> prefab hierarchy
-> mesh and skin status
-> named bones/transforms
-> animation clips
-> editable material parameters
-> package/runtime cost
-> license/export boundary
```

The asset is admitted only if one measured load value can produce an unmistakable, bounded change in at least two independently readable channels, for example wing + tail or head + body.

Asset Library availability does not by itself grant permission to redistribute the package in this public repository. Confirm the asset's terms before committing package contents.
