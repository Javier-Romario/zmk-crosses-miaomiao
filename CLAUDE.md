# CLAUDE.md — zmk-crosses-miaomiao

ZMK keyboard config for the Crosses (miaomiao variant) — dual PMW3610 trackballs, split wireless.

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
