# Build Report

**Pack:** frontier v0.1  
**GCC:** 14.2.0  
**Clang:** 17.0.0  
**C flags:** `-std=c11 -O2 -Wall -Wextra -Wpedantic -Werror`  
**C tests:** passed under GCC and Clang  
**C authoritative dynamics/configuration:** integer/fixed-point only; no runtime floating-point conversion  
**Golden frames:** 720  
**Golden SHA-256:** `3d197890e7bfc93a990a458e1c64eaa956f400162d0b7d5f35fde13663992f4d`  
**Cross-compiler golden equality:** passed  
**JSON Schema:** passed for all 720 `legion.pose/0.1` frames  
**JavaScript syntax:** passed with Node.js 22.16.0  
**TypeScript syntax/transpile:** passed with TypeScript 5.8.3 and `--noCheck`  
**Rust:** source packaged; no `cargo` or `rustc` available in the build environment  
**Lens runtime:** not executed in this environment

## Golden transitions

```text
sequence 0   → CALM
sequence 187 → ATTUNE
sequence 365 → STORM
sequence 540 → STALE
```
