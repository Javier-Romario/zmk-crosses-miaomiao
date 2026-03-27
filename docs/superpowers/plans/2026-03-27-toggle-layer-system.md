# Toggle Layer System Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Replace hold-to-stay (`&mo`) layer combos with tap-to-toggle (`&tog`) combos, swap Space and Return on the base layer, and add a `&to BASE` exit key at the Q position on every non-base layer.

**Architecture:** Single file change — `config/crosses.keymap`. Three logical groups of edits: (1) base layer thumb swap, (2) combo behavioural + position changes, (3) Q-position exit on non-base layers. Each group is independently buildable and committable.

**Tech Stack:** ZMK firmware, Docker build via `./build-local.sh`

---

## File Map

| File | Change |
|---|---|
| `config/crosses.keymap` | All edits — base layer, combos, non-base layer Q positions |

No files are created. No other files are touched.

---

## Displacement Notes

Replacing Q (pos 0) with `&to BASE` on non-base layers removes keys that currently sit there:

| Layer | Key displaced |
|---|---|
| SYM | `{` (LBRC) — was at Q position |
| FUN | `F12` — was at Q position |
| MOUSE | `BT_CLR` — was at Q position |

NAV and NUM layers have `&trans` at Q position, so nothing is lost there. The displaced keys (above) are not duplicated elsewhere on their layers — if you need them after this change, add them to an empty `&trans` slot in a follow-up. This plan implements the spec as approved.

---

## Task 1: Swap Space and Return on the base layer

**Files:**
- Modify: `config/crosses.keymap:93`

The base layer thumb row (line 93) currently reads:

```
             &kp LCTRL   &kp LALT     &kp LGUI                      &kp SPACE    &kp RET      &kp BSPC
```

- [ ] **Step 1: Edit the base layer thumb row**

Change line 93 to swap `&kp SPACE` and `&kp RET`:

```c
             &kp LCTRL   &kp LALT     &kp LGUI                      &kp RET      &kp SPACE    &kp BSPC
```

- [ ] **Step 2: Build to verify no compilation errors**

```bash
./build-local.sh left
```

Expected: build completes, `firmware/nice_nano_v2-crosses_left.uf2` is updated. If you see a syntax error, re-check the line above matches exactly.

- [ ] **Step 3: Commit**

```bash
git add config/crosses.keymap
git commit -m "keymap: swap Space and Return on base layer thumb row"
```

---

## Task 2: Update combo bindings and reposition fun/mouse combos

**Files:**
- Modify: `config/crosses.keymap:46-72`

The combos block currently has six entries. Three need their binding changed (`&mo` → `&tog`). Two also need their positions swapped (fun and mouse have swapped physical positions in the new scheme).

Current state of combos block (lines 39–73):

```c
combo_hyper {
    timeout-ms = <50>;
    key-positions = <30 31>;
    bindings = <&kp LS(LC(LA(LGUI)))>;
};
combo_nav {
    timeout-ms = <50>;
    key-positions = <31 32>;
    bindings = <&mo NAV>;
};
combo_mouse {
    timeout-ms = <50>;
    key-positions = <34 35>;
    bindings = <&mo MOUSE>;
};

// Cross-hand (inner thumbs)
combo_num {
    timeout-ms = <50>;
    key-positions = <32 33>;
    bindings = <&mo NUM>;
};

// Right thumb pairs
combo_sym {
    timeout-ms = <50>;
    key-positions = <33 34>;
    bindings = <&mo SYM>;
};
combo_fun {
    timeout-ms = <50>;
    key-positions = <30 32>;
    bindings = <&mo FUN>;
};
```

- [ ] **Step 1: Replace the entire combos block**

Replace everything between `combos {` and its closing `};` with:

```c
    combos {
        compatible = "zmk,combos";

        // Left thumb pairs
        combo_hyper {
            timeout-ms = <50>;
            key-positions = <30 31>;
            bindings = <&kp LS(LC(LA(LGUI)))>;
        };
        combo_nav {
            timeout-ms = <50>;
            key-positions = <31 32>;
            bindings = <&tog NAV>;
        };

        // Cross-hand (inner thumbs)
        combo_num {
            timeout-ms = <50>;
            key-positions = <32 33>;
            bindings = <&tog NUM>;
        };

        // Right thumb pairs
        combo_sym {
            timeout-ms = <50>;
            key-positions = <33 34>;
            bindings = <&tog SYM>;
        };
        combo_fun {
            timeout-ms = <50>;
            key-positions = <34 35>;
            bindings = <&tog FUN>;
        };

        // Non-adjacent — MOUSE/Media (rare use)
        combo_mouse {
            timeout-ms = <50>;
            key-positions = <30 32>;
            bindings = <&tog MOUSE>;
        };
    };
```

Key changes vs. current:
- `combo_nav`: `&mo NAV` → `&tog NAV`
- `combo_num`: `&mo NUM` → `&tog NUM`
- `combo_sym`: `&mo SYM` → `&tog SYM`
- `combo_fun`: positions `30 32` → `34 35`, `&mo FUN` → `&tog FUN`
- `combo_mouse`: positions `34 35` → `30 32`, `&mo MOUSE` → `&tog MOUSE`
- `combo_hyper`: unchanged

- [ ] **Step 2: Build to verify no compilation errors**

```bash
./build-local.sh left
```

Expected: clean build. If you see `unknown behavior &tog`, your ZMK version may be old — `&tog` is a built-in ZMK behavior and should be available in any recent ZMK build.

- [ ] **Step 3: Commit**

```bash
git add config/crosses.keymap
git commit -m "keymap: change layer combos from &mo (hold) to &tog (toggle), reposition fun/mouse combos"
```

---

## Task 3: Add &to BASE exit key at Q position on all non-base layers

**Files:**
- Modify: `config/crosses.keymap` — nav_layer, num_layer, sym_layer, fun_layer, mouse_layer

Position 0 (Q key, top-left of left hand) on every non-base layer becomes `&to BASE`. This is an unconditional switch back to the base layer, clearing all active toggles.

For NAV and NUM, position 0 is currently `&trans` — a clean replacement.
For SYM, FUN, and MOUSE, position 0 has a mapped key that will be displaced (see Displacement Notes above).

- [ ] **Step 1: Update nav_layer — change first binding from &trans to &to BASE**

Line 109, first token:

```c
// Before:
&trans      &trans      &trans      &trans      &trans            &kp LG(LS(Z))  &kp LG(V)    &kp LG(C)    &kp LG(X)    &kp LG(Z)

// After:
&to BASE    &trans      &trans      &trans      &trans            &kp LG(LS(Z))  &kp LG(V)    &kp LG(C)    &kp LG(X)    &kp LG(Z)
```

- [ ] **Step 2: Update num_layer — change first binding from &trans to &to BASE**

Line 128, first token:

```c
// Before:
&trans      &trans      &trans      &trans      &trans            &kp LBKT     &kp N7       &kp N8       &kp N9       &kp RBKT

// After:
&to BASE    &trans      &trans      &trans      &trans            &kp LBKT     &kp N7       &kp N8       &kp N9       &kp RBKT
```

- [ ] **Step 3: Update sym_layer — change first binding from &kp LBRC to &to BASE**

Line 147, first token. Note: this displaces `{` — see Displacement Notes.

```c
// Before:
&kp LBRC   &kp AMPS   &kp ASTRK  &kp LPAR   &kp RBRC         &trans        &trans        &trans        &trans        &trans

// After:
&to BASE   &kp AMPS   &kp ASTRK  &kp LPAR   &kp RBRC         &trans        &trans        &trans        &trans        &trans
```

- [ ] **Step 4: Update fun_layer — change first binding from &kp F12 to &to BASE**

Line 166, first token. Note: this displaces `F12` — see Displacement Notes.

```c
// Before:
&kp F12    &kp F7     &kp F8     &kp F9     &kp PSCRN        &trans        &trans        &trans        &trans        &trans

// After:
&to BASE   &kp F7     &kp F8     &kp F9     &kp PSCRN        &trans        &trans        &trans        &trans        &trans
```

- [ ] **Step 5: Update mouse_layer — change first binding from &bt BT_CLR to &to BASE**

Line 185, first token. Note: this displaces `BT_CLR` — see Displacement Notes.

```c
// Before:
&bt BT_CLR       &trans           &kp C_BRI_DN     &kp C_BRI_UP     &trans           &bt BT_SEL 0  &bt BT_SEL 1  &bt BT_SEL 2  &bt BT_SEL 3  &bt BT_SEL 4

// After:
&to BASE         &trans           &kp C_BRI_DN     &kp C_BRI_UP     &trans           &bt BT_SEL 0  &bt BT_SEL 1  &bt BT_SEL 2  &bt BT_SEL 3  &bt BT_SEL 4
```

- [ ] **Step 6: Build both sides to verify**

```bash
./build-local.sh all
```

Expected: all three firmware files built successfully under `firmware/`. If you see `BASE undeclared`, ensure `#define BASE 0` is present at the top of the file (it is — line 7).

- [ ] **Step 7: Commit**

```bash
git add config/crosses.keymap
git commit -m "keymap: add &to BASE exit key at Q position on all non-base layers"
```

---

## Task 4: Update layer diagram comments (optional but recommended)

**Files:**
- Modify: `config/crosses.keymap` — ASCII diagram comments in each layer

The inline ASCII diagrams above each layer's `bindings` block are documentation only — they don't affect firmware. Update them to reflect:

- Base layer: swap Space/Return labels in thumb row comment
- Non-base layers: Q position comment should read `→ Base` instead of its old label

This is low-stakes — incorrect comments don't break anything. Do this task if you care about keeping the source readable.

- [ ] **Step 1: Update base layer thumb row comment (line 87)**

```c
// Before:
//                                  │   Ctrl   │   Opt    │ ⌘ │   │  Space   │  Return  │   Bspc   │

// After:
//                                  │   Ctrl   │   Opt    │ ⌘ │   │  Return  │  Space   │   Bspc   │
```

- [ ] **Step 2: Update Q position comment on each non-base layer**

For each non-base layer, the top-left cell of the ASCII diagram should read `→ Base` (or `→BASE`). Example for nav_layer (line 100):

```c
// Before:
// │          │          │          │          │          │   │  ⌘⇧Z     │   ⌘V     │   ⌘C     │   ⌘X     │   ⌘Z     │

// After:
// │  → Base  │          │          │          │          │   │  ⌘⇧Z     │   ⌘V     │   ⌘C     │   ⌘X     │   ⌘Z     │
```

Apply the same `→ Base` label to the Q position comment cell in sym_layer, fun_layer, mouse_layer, and num_layer.

- [ ] **Step 3: Commit**

```bash
git add config/crosses.keymap
git commit -m "keymap: update ASCII diagram comments to reflect toggle layer changes"
```

---

## Task 5: Regenerate keymap-drawer (optional)

**Files:**
- `keymap-drawer/crosses.yaml` (regenerated)
- `keymap-drawer/crosses.svg` (regenerated)

If you have `keymap-drawer` installed, regenerate the visual layout diagram:

- [ ] **Step 1: Parse keymap to YAML**

```bash
keymap -c keymap_drawer.config.yaml parse -z config/crosses.keymap > keymap-drawer/crosses.yaml
```

- [ ] **Step 2: Draw SVG from YAML**

```bash
keymap -c keymap_drawer.config.yaml draw keymap-drawer/crosses.yaml > keymap-drawer/crosses.svg
```

- [ ] **Step 3: Commit regenerated diagrams**

```bash
git add keymap-drawer/crosses.yaml keymap-drawer/crosses.svg
git commit -m "draw: regenerate keymap diagram for toggle layer system"
```
