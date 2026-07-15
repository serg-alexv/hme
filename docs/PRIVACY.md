# World 2.0 data minimization and privacy boundary

## Rule

Collect and preserve only the data required to verify the declared experiment or operate the declared adapter. Do not collect identity because it might become useful later.

## Current connected-WLAN adapter

The current Rust monitor invokes the Windows connected-WLAN interface command, parses the first signal-quality percentage, converts it to bounded Q0.16, and emits derived frames.

The emitted protocol requires:

- timestamp;
- sequence;
- source class;
- validity and freshness;
- bounded strength, variability, and novelty;
- behavior state;
- bounded integer pose;
- frame hash.

It does not require or emit:

- SSID;
- BSSID or access-point MAC address;
- IP address;
- packet payload;
- browsing or application traffic;
- account identity;
- device-owner identity;
- person identity;
- camera or microphone data.

The operating-system command may return fields that the monitor does not use. The monitor must not log or persist the full command output in normal operation.

## Demo data

The public browser demo uses the committed `SIMULATED` golden trace. It performs no sensor acquisition, uses no cookies, sends no telemetry, and requires no account.

## Evidence retention

An experiment manifest must name every retained data class, purpose, path, byte size, checksum, and deletion/retention decision. If a future adapter needs personal data, it requires a separate authority decision and privacy review before implementation.

## AI assistance

Do not send raw experiments to an external model by default. Prefer derived, minimized, or local evidence. If external processing is required, record the provider, exact data classes, purpose, retention terms, and approval.

AI output is not measurement evidence and cannot replace missing raw data.

## Review checklist

- [ ] Does each collected field have a named acceptance criterion?
- [ ] Is the source class visible?
- [ ] Are unnecessary identifiers absent from schemas, logs, fixtures, and screenshots?
- [ ] Does STALE prevent false live labeling?
- [ ] Is public demo data synthetic or explicitly approved for publication?
- [ ] Are retained files and external transfers recorded?
