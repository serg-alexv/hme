# World2NetworkDragon.exe

Native x64 Windows network pet. It reads aggregate host network counters and maps the measured load to a breathing creature with articulated wings, plus Equalizer and Field views.

## Run

Double-click `dist/World2NetworkDragon.exe`. No Python, browser, server, or external asset is required.

Controls: `M` LIVE/REPLAY, `D` Dragon, `E` Equalizer, `F` Field, `↑`/`↓` reference capacity, `R` reset, `T` always-on-top.

The active frame is exported atomically to `%LOCALAPPDATA%\World2\world-state.json`. Its `integrity.payloadHash` is a BCrypt SHA-256 over the canonical frame body.

## Measurement boundary

Every 250 ms, `GetIfTable2` aggregates received/sent counters from connected, operational, non-loopback interfaces. This is host throughput—not RF sensing, packet inspection, person detection, or location estimation.

## Build

From an x64 Native Tools Command Prompt for Visual Studio 2022, run `build_windows.bat`. The binary uses `/MT /O2 /W4 /WX` and standard Windows system libraries.
