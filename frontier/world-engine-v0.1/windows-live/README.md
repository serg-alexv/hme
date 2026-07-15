# Windows product

Double-click:

```text
dist/World2NetworkDragon.exe
```

No Python, Rust, compiler, Lens Studio, Snapchat, or browser server is required.

## Verified behavior

```text
Windows byte-counter deltas
→ throughput
→ WorldStateFrame 0.1
→ Dragon | Equalizer | Field
```

The default Pet is an embedded render derived from the supplied rigged FBX. The application also provides a useful live upload/download monitor and exports its current frame to:

```text
%LOCALAPPDATA%\World2\world-state.json
```

Controls:

- `D`, `E`, `F`: select Pet.
- `M`: measured live counters / labelled replay.
- `↑`, `↓`: change declared normalization reference.
- `R`: reset live baseline.
- `T`: toggle always-on-top.

## Measurement boundary

```text
throughput_bps = 8 × (ΔRX_bytes + ΔTX_bytes) / Δtime_seconds
```

This is aggregate host network traffic from connected, operational, non-loopback interfaces. It is not RF spectrum, WLAN CSI, packet inspection, person detection, or room location.

## Release candidate

```text
bytes: 718336
sha256: 4f999368232fc318d7301ab229ce3eccf22e5b4e0ae380cb5dff5a92a9d9bccc
```

The hash must be regenerated if the executable is rebuilt.

![Verified live Windows product](../../../docs/images/windows-live-network-pets.png)
