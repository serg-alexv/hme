# Release evidence

- Artifact: `dist/World2NetworkDragon.exe`
- Architecture: x64 Windows
- Size: 718,336 bytes
- SHA-256: `B2107825FF8D582FB0F23A9F099C943F2CF036BDF0F125FD0C589E57B8D194D7`
- Build: Visual Studio 2022, C++17, `/MT /O2 /W4 /WX`
- Runtime: process remained alive during the verification interval
- Telemetry: `LIVE_NETWORK_COUNTERS`, source `windows-getiftable2`, valid live frame
- Export: sequence advanced from 9 to 21 during a three-second observation
- Integrity: exported payload SHA-256 independently recomputed and matched
- UI: Dragon, Equalizer, and Field selectors; explicit LIVE/REPLAY provenance
- Dragon: continuous breathing, articulated wing pivots, energy-responsive flap rate and travel

The executable is not Authenticode-signed. Windows reputation warnings may appear until a signed release is established.
