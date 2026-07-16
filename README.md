# World2 Network Dragon

A native Windows desktop creature whose breathing, wings, and state respond to the host machine's aggregate live network activity.

## Run

Download and launch `frontier/world-engine-v0.1/windows-live/dist/World2NetworkDragon.exe`. It is an x64 native executable with the C++ runtime statically linked; no Python, browser, server, or external asset is required.

## What is visible

- `LIVE COUNTERS` or explicitly labelled `REPLAY` provenance
- Dragon, Equalizer, and Field selectors
- live download, upload, total throughput, normalized load, state, and interface count
- continuous dragon breathing and articulated wings that intensify with measured network energy
- exported `%LOCALAPPDATA%\World2\world-state.json` with a SHA-256 payload hash

Controls: `M` live/replay, `D`/`E`/`F` embodiment, `↑`/`↓` reference capacity, `R` reset baseline, `T` always-on-top.

## Measurement boundary

The app samples Windows `GetIfTable2` counters every 250 ms for connected, operational, non-loopback interfaces. It measures aggregate host byte-counter deltas—not RF spectrum, Wi-Fi CSI, packet contents, people, or location.

## Build

Run `build_windows.bat` from an x64 Native Tools Command Prompt for Visual Studio 2022. The release build uses `/MT /O2 /W4 /WX`.

Verified release facts are recorded in `frontier/world-engine-v0.1/windows-live/RELEASE.md`.
