# Current verification status

## Verified on Windows — 2026-07-16

- `World2NetworkDragon.exe` builds with MSVC `/W4 /WX /MT`.
- It reads live `GetIfTable2` byte-counter deltas every 250 ms.
- A controlled live download reached `1,000,000 ppm` and changed state from `ATTUNE` to `STORM` without replay mode.
- The Pet selector switches between Warrior Dragon, Equalizer, and Field presentations.
- Every sample emits `%LOCALAPPDATA%\World2\world-state.json`.
- The emitted frame has a verified SHA-256 payload hash.
- Replay is visibly labelled and cannot be confused with live measurement.

## Release candidate

```text
frontier/world-engine-v0.1/windows-live/dist/World2NetworkDragon.exe
```

Size and SHA-256 are recorded in the Windows product README after the final build.

## Not verified

- Lens Studio runtime compilation with the supplied FBX.
- Live frame transport from the Windows producer into Lens Studio.
- macOS or BSD native builds.
- RF spectrum, WLAN CSI, room location, or camera placement.
