# Symbol Layer Redesign Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Redesign the SYM layer to use both keyboard halves, move its combo trigger to a cross-hand key pair, and reduce left trackball scroll sensitivity.

**Architecture:** Three files change — the keymap gets a new combo position and a fully rewritten SYM layer binding block; the left conf raises the scroll tick threshold; the right overlay gains a Y-axis input scaler node inserted into the left trackball listener chain.

**Tech Stack:** ZMK firmware, Devicetree (.keymap, .overlay, .conf), Docker build via `./build-local.sh`

---

## File Map

| File | What changes |
|------|-------------|
| `config/crosses.keymap` | `combo_sym` key-positions `<33 34>` → `<31 34>`; `sym_layer` bindings fully replaced |
| `config/boards/shields/crosses/crosses_left.conf` | `CONFIG_PMW3610_SCROLL_TICK` 150 → 250 |
| `config/boards/shields/crosses/crosses_right.overlay` | New `left_tb_y_scaler` node; `left_trackball_listener` input-processors gains `<&left_tb_y_scaler 1 2>` |

---

## Task 1: Update SYM combo trigger

**Files:**
- Modify: `config/crosses.keymap` (combo_sym block, ~line 64)

- [ ] **Step 1: Change key-positions in combo_sym**

In `config/crosses.keymap`, find:

```c
        combo_sym {
            timeout-ms = <50>;
            key-positions = <33 34>;
            bindings = <&tog SYM>;
        };
```

Replace with:

```c
        combo_sym {
            timeout-ms = <50>;
            key-positions = <31 34>;
            bindings = <&tog SYM>;
        };
```

Position 31 is the left Opt thumb; position 34 is the right Space thumb. This makes SYM a cross-hand toggle — one thumb from each side — so neither hand is blocked while typing symbols.

- [ ] **Step 2: Commit**

```bash
git add config/crosses.keymap
git commit -m "keymap: move SYM combo to cross-hand (Opt+Space)"
```

---

## Task 2: Rewrite SYM layer bindings

**Files:**
- Modify: `config/crosses.keymap` (sym_layer block, ~line 143)

- [ ] **Step 1: Replace sym_layer bindings**

Find the entire `sym_layer` block and replace it:

```c
        sym_layer {
            display-name = "Symbols";
            // ┌──────────┬──────────┬──────────┬──────────┬──────────┐   ┌──────────┬──────────┬──────────┬──────────┬──────────┐
            // │  → Base  │    !     │    @     │    #     │    $     │   │    [     │    ]     │    -     │    =     │    \     │
            // ├──────────┼──────────┼──────────┼──────────┼──────────┤   ├──────────┼──────────┼──────────┼──────────┼──────────┤
            // │    %     │    ^     │    &     │    *     │    (     │   │    ;     │    '     │    ,     │    .     │    /     │
            // ├──────────┼──────────┼──────────┼──────────┼──────────┤   ├──────────┼──────────┼──────────┼──────────┼──────────┤
            // │  LShft   │    )     │    `     │          │          │   │  RShft   │          │          │          │          │
            // └──────────┴──────────┴──────────┼──────────┼──────────┼───┤   ├──────────┼──────────┼──────────┴──────────┴──────────┘
            //                                  │          │          │   │   │          │          │          │
            //                                  └──────────┴──────────┴───┘   └──────────┴──────────┴──────────┘
            //
            // Right side: unshifted key gives base symbol, Shift gives paired symbol:
            //   [ → {    ] → }    - → _    = → +    \ → |
            //   ; → :    ' → "    , → <    . → >    / → ?
            //   ` → ~  (left side)
            bindings = <
&to BASE   &kp EXCL   &kp AT     &kp HASH   &kp DLLR     &kp LBKT   &kp RBKT   &kp MINUS  &kp EQUAL  &kp BSLH
&kp PRCNT  &kp CARET  &kp AMPS   &kp ASTRK  &kp LPAR     &kp SEMI   &kp SQT    &kp COMMA  &kp DOT    &kp FSLH
&kp LSHFT  &kp RPAR   &kp GRAVE  &trans     &trans       &kp RSHFT  &trans     &trans     &trans     &trans
           &trans     &trans     &trans                  &trans     &trans     &trans
            >;
        };
```

**What each column achieves:**

Left side:
- Row 1: `→Base ! @ # $` — nav key + first four shifted-number-row symbols
- Row 2: `% ^ & * (` — remaining shifted-number-row symbols + open paren
- Row 3: `LShft ) `` ` — shift key, close paren, backtick (shift→`~`)
- Thumbs: all `&trans` — Return/Space/Bspc fall through to BASE

Right side:
- Row 1: `[ ] - = \` — unshifted; with shift: `{ } _ + |`
- Row 2: `; ' , . /` — unshifted; with shift: `: " < > ?`
- Row 3: `RShft` then blanks — shift key on the mirror position to LShft
- Thumbs: all `&trans`

- [ ] **Step 2: Build right side to verify keymap compiles**

```bash
./build-local.sh right
```

Expected output ends with:
```
✓ Built: firmware/nice_nano_v2-crosses_right.uf2
```

If the build fails, check for typo in binding names. All ZMK keycodes used here (`EXCL`, `AT`, `HASH`, `DLLR`, `PRCNT`, `CARET`, `AMPS`, `ASTRK`, `LPAR`, `RPAR`, `GRAVE`, `LSHFT`, `RSHFT`, `LBKT`, `RBKT`, `MINUS`, `EQUAL`, `BSLH`, `SEMI`, `SQT`, `COMMA`, `DOT`, `FSLH`) are defined in `<dt-bindings/zmk/keys.h>`.

- [ ] **Step 3: Commit**

```bash
git add config/crosses.keymap
git commit -m "keymap: redesign SYM layer — both sides, condensed shift pairs, explicit shift keys"
```

---

## Task 3: Increase left trackball scroll tick

**Files:**
- Modify: `config/boards/shields/crosses/crosses_left.conf` (line 11)

- [ ] **Step 1: Raise SCROLL_TICK**

In `crosses_left.conf`, find:

```
CONFIG_PMW3610_SCROLL_TICK=150
```

Replace with:

```
CONFIG_PMW3610_SCROLL_TICK=250
```

This setting controls how many raw sensor units must accumulate before one scroll tick fires. Raising from 150 to 250 means the trackball must move ~67% further per scroll event — noticeably less sensitive without being sluggish. Tune up or down in increments of 50 after testing.

- [ ] **Step 2: Commit**

```bash
git add config/boards/shields/crosses/crosses_left.conf
git commit -m "trackball: increase left scroll tick 150→250 to reduce sensitivity"
```

---

## Task 4: Add Y-axis scaler to left trackball listener

**Files:**
- Modify: `config/boards/shields/crosses/crosses_right.overlay`

- [ ] **Step 1: Add Y scaler node and update listener**

Replace the entire contents of `crosses_right.overlay` with:

```dts
#include "crosses.dtsi"
#include <input/processors.dtsi>

&default_transform {
    col-offset = <6>;
};

/ {
    // Suppress horizontal (X) movement so only vertical scroll fires.
    // Without this, REL_X gets mapped to REL_HWHEEL which terminals
    // interpret as left/right arrow keystrokes.
    left_tb_x_suppressor: left_tb_x_suppressor {
        compatible = "zmk,input-processor-scaler";
        #input-processor-cells = <2>;
        type = <INPUT_EV_REL>;
        codes = <INPUT_REL_X>;
    };

    // Scale Y movement to 1/2 before it reaches the scroll accumulator.
    // Stacks with SCROLL_TICK: together they significantly reduce scroll speed.
    left_tb_y_scaler: left_tb_y_scaler {
        compatible = "zmk,input-processor-scaler";
        #input-processor-cells = <2>;
        type = <INPUT_EV_REL>;
        codes = <INPUT_REL_Y>;
    };

    trackball_listener {
        compatible = "zmk,input-listener";
        device = <&trackball>;
    };

    left_trackball_listener {
        compatible = "zmk,input-listener";
        device = <&trackball_split>;
        input-processors = <&left_tb_x_suppressor 0 1>, <&left_tb_y_scaler 1 2>, <&zip_xy_to_scroll_mapper>;
    };
};
```

The `<&left_tb_y_scaler 1 2>` parameters are `<multiplier divisor>` — this halves each Y movement value before it reaches `zip_xy_to_scroll_mapper`. Combined with the SCROLL_TICK increase in Task 3, scroll requires roughly 3× more physical movement per tick. Tune the divisor (try `3` for even less sensitivity) in this file without touching SCROLL_TICK.

- [ ] **Step 2: Build both sides to verify all changes compile**

```bash
./build-local.sh all
```

Expected output:
```
✓ Built: firmware/nice_nano_v2-crosses_left.uf2
✓ Built: firmware/nice_nano_v2-crosses_right.uf2
✓ Built: firmware/nice_nano_v2-settings_reset.uf2
```

If the right side build fails on the overlay, verify `left_tb_y_scaler` node syntax matches `left_tb_x_suppressor` exactly (same `compatible`, `#input-processor-cells`, `type` — only `codes` differs).

- [ ] **Step 3: Commit**

```bash
git add config/boards/shields/crosses/crosses_right.overlay
git commit -m "trackball: add Y-axis scaler (1/2) to left scroll listener"
```

---

## Self-Review

**Spec coverage:**
- SYM combo 33+34 → 31+34 ✓ Task 1
- SYM left side (standalone symbols + LShft) ✓ Task 2
- SYM right side (condensed pairs + RShft) ✓ Task 2
- Right thumbs trans (Return/Space/Bspc pass through) ✓ Task 2
- SCROLL_TICK 150 → 250 ✓ Task 3
- Y-axis scaler 1/2 in left_trackball_listener ✓ Task 4
- Build verification after keymap changes ✓ Task 2 Step 2
- Build verification after all changes ✓ Task 4 Step 2

**Placeholder scan:** No TBDs, no vague steps, all code shown in full.

**Type consistency:** No shared types across tasks — each task is self-contained file edits.
