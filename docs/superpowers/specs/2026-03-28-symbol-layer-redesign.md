# Symbol Layer Redesign

**Date:** 2026-03-28
**Status:** Approved

## Problem

The current SYM layer is ergonomically imbalanced:
- Activated by a right-thumb combo (positions 33+34: Return+Space)
- All symbols placed on the left side only — right side is entirely blank
- Result: right hand holds the combo activation while left hand does all the typing

Additionally, the left trackball (scroll) is too sensitive.

## Design

### 1. SYM Combo Trigger — positions 31+34 (Opt + Space)

Change the SYM toggle from `33+34` (both right thumbs) to `31+34` (left Opt + right Space).

- Cross-hand combo: one thumb from each side, matching the intent of a "both sides" layer
- `&tog` behaviour preserved — tap to lock in, tap again to exit
- The freed `33+34` slot is left unassigned

```
Left thumbs:   [30:Ctrl]  [31:Opt]  [32:Cmd]
Right thumbs:             [33:Ret]  [34:Spc]  [35:Bspc]

Old SYM:  33+34  (both right thumbs)
New SYM:  31+34  (cross-hand)
```

### 2. SYM Layer — Left Side (Standalone symbols + shift)

Operators and shifted-number-row symbols that have no useful unshifted form on this layer.

```
┌──────────┬──────────┬──────────┬──────────┬──────────┐
│  → Base  │    !     │    @     │    #     │    $     │
├──────────┼──────────┼──────────┼──────────┼──────────┤
│    %     │    ^     │    &     │    *     │    (     │
├──────────┼──────────┼──────────┼──────────┼──────────┤
│  LShft   │    )     │    `     │          │          │
└──────────┴──────────┴──────────┼──────────┼──────────┤
                                 │          │          │          │
                                 └──────────┴──────────┘
```

- `!` `@` `#` `$` `%` `^` `&` `*` `(` `)` — standalone shifted symbols
- `` ` `` — condensed pair: unshifted gives `` ` ``, shifted gives `~`
- `LShft` on bottom-outer — explicit shift key, accessible without leaving the layer

### 3. SYM Layer — Right Side (Condensed pairs + shift)

All delimiter/punctuation symbols that have a meaningful unshifted+shifted pair. One key each; shift accesses the shifted variant.

```
┌──────────┬──────────┬──────────┬──────────┬──────────┐
│    [     │    ]     │    -     │    =     │    \     │
├──────────┼──────────┼──────────┼──────────┼──────────┤
│    ;     │    '     │    ,     │    .     │    /     │
├──────────┼──────────┼──────────┼──────────┼──────────┤
│  RShft   │          │          │          │          │
└──────────┴──────────┴──────────┼──────────┼──────────┤
                                 │  (trans) │  (trans) │  (trans)
                                 └──────────┴──────────┘
```

Shift pairs accessible via LShft or RShft:

| Key | Shifted |
|-----|---------|
| `[` | `{`     |
| `]` | `}`     |
| `-` | `_`     |
| `=` | `+`     |
| `\` | `\|`    |
| `;` | `:`     |
| `'` | `"`     |
| `,` | `<`     |
| `.` | `>`     |
| `/` | `?`     |
| `` ` `` (left) | `~` |

Right thumbs are `&trans` — Return, Space, and Bspc pass through to BASE and remain usable while in SYM.

**Coverage:** All 32 standard symbols accessible from one layer using 23 keys (vs. 17 symbols on 17 keys previously, with the right half wasted).

### 4. Left Trackball Sensitivity Reduction

The left trackball drives scroll via `zip_xy_to_scroll_mapper`. Two stacked levers:

**Lever 1 — `crosses_left.conf`:** Increase `CONFIG_PMW3610_SCROLL_TICK` from `150` to `250`.
- Controls how many raw sensor units must accumulate before one scroll tick fires.
- Higher value = fewer scroll events per physical movement.

**Lever 2 — `crosses_right.overlay`:** Add a `zmk,input-processor-scaler` for `INPUT_REL_Y` with ratio `1/2` in the `left_trackball_listener` input-processors chain, before `zip_xy_to_scroll_mapper`.
- Halves the Y movement value before it reaches the scroll accumulator.
- Stacks multiplicatively with the SCROLL_TICK change.

Both values are tunable post-build without structural changes.

## Files to Change

| File | Change |
|------|--------|
| `config/crosses.keymap` | New combo (31+34), updated SYM layer bindings |
| `config/boards/shields/crosses/crosses_left.conf` | `SCROLL_TICK` 150 → 250 |
| `config/boards/shields/crosses/crosses_right.overlay` | Add Y scaler node + insert into `left_trackball_listener` input-processors |
