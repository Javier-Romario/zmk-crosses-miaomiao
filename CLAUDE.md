# CLAUDE.md — zmk-crosses-miaomiao

ZMK keyboard config for the Crosses (miaomiao variant) — dual PMW3610 trackballs, split wireless.

## Split BLE Architecture

Right (central) never sleeps — `CONFIG_ZMK_SLEEP=n` in `crosses_right.conf`. Left (peripheral) uses `zmk-behavior-insomnia`: pings every 10 s while connected to prevent sleep, then enters System Off 25 s after the link drops. Key press wakes left from System Off via `wakeup-source` on `kscan0`; right (always scanning) reconnects within ~5 s.

Key files: `crosses_left.conf`, `crosses_right.conf`, `crosses.conf` (shared), `crosses.dtsi`, `crosses_left.overlay`.
Full design rationale, config table, and troubleshooting: `docs/ble-split-architecture.md`.

**Critical gotchas:**
- `CONFIG_BT_GAP_AUTO_UPDATE_CONN_PARAMS` must be in `crosses_right.conf` only — shared conf applies it to left's split connection and causes instability
- `CONFIG_ZMK_BLE_EXPERIMENTAL_FEATURES` must NOT be set on left — left is split GATT only, not BLE HID
- PMW3610 trackball IRQ cannot wake from System Off — only kscan can

## Rust CLI Tools

Prefer Rust CLI alternatives over standard Unix tools wherever applicable. They are installed and approved for use, including displaying output directly to the user.

| Task | Preferred | Fallback |
|------|-----------|----------|
| Find files | `fd` | `find` |
| Search file contents | `rg` (ripgrep) | `grep` |
| View files | `bat` | `cat` |
| List files | `eza` | `ls` |
| Git diffs | `delta` | `diff` |
| Disk usage | `dust` | `du` |
| Process list | `procs` | `ps` |
| In-place find/replace | `sd` | `sed` |
| Benchmarking | `hyperfine` | `time` |

Always try the Rust tool first; fall back to the standard tool only if the Rust alternative is unavailable or unsuitable for the task.
