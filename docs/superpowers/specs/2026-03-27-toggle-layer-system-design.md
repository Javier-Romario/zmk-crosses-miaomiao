# Toggle Layer System

**Date:** 2026-03-27
**Keyboard:** Crosses (3×5+3, 36 keys, ZMK)
**Goal:** Replace hold-to-stay combo layers with tap-to-toggle layers, swap Space and Return, and add a single non-adjacent combo for the rare MOUSE layer — reducing chording and eliminating modifier clashes.

---

## Problem

The current system uses `&mo` (momentary) via thumb combos: you must hold the combo the entire time you're on a layer. This causes two problems:

1. **Modifier clashes** — holding e.g. Ctrl+Cmd to stay in MOUSE also registers those keys as held modifiers, firing unexpected shortcuts.
2. **Physical strain** — sustaining a two-key hold while typing on the layer is tiring and error-prone.

---

## Solution Overview

| Aspect | Before | After |
|---|---|---|
| Layer activation | Hold combo → layer active | Tap combo → layer toggles on |
| Layer exit | Release combo | Tap Q (any layer) or tap same combo again |
| Space / Return | Space left, Return right of center | **Swapped** — Return left, Space right of center |
| MOUSE access | Hold Return+Bspc (34+35) | Tap Ctrl+Cmd (30+32), non-adjacent |
| FUN access | Hold Ctrl+Cmd (30+32), non-adjacent | Tap Space+Bspc (34+35), adjacent |

---

## Base Layer

Alpha keys unchanged. Hold behaviours on Z, /, Q unchanged.

```
┌─────┬─────┬─────┬─────┬─────┐   ┌─────┬─────┬─────┬─────┬─────┐
│  Q  │  W  │  E  │  R  │  T  │   │  Y  │  U  │  I  │  O  │  P  │
│ Esc*│     │     │     │     │   │     │     │     │     │     │
├─────┼─────┼─────┼─────┼─────┤   ├─────┼─────┼─────┼─────┼─────┤
│  A  │  S  │  D  │  F  │  G  │   │  H  │  J  │  K  │  L  │  '  │
├─────┼─────┼─────┼─────┼─────┤   ├─────┼─────┼─────┼─────┼─────┤
│  Z  │  X  │  C  │  V  │  B  │   │  N  │  M  │  ,  │  .  │  /  │
│  ⇧* │     │     │     │     │   │     │     │     │     │  ⇧* │
└─────┴─────┴─────┼─────┼─────┼───┤   ├─────┼─────┼─────┴─────┴─────┘
                  │Ctrl │ Opt │ ⌘ │   │ Ret │ Spc │  ⌫  │
                  └─────┴─────┴───┘   └─────┴─────┴─────┘
```

`*` = hold behaviour. All other keys tap-only.

---

## Thumb Cluster Positions

```
 30      31      32  |  33      34      35
[Ctrl]  [Opt]  [Cmd] | [Ret]  [Spc]  [Bspc]
```

---

## Combo Scheme

All layer combos changed from `&mo` (momentary) to `&tog` (toggle). Hyper unchanged (not a layer).

```
[Ctrl]──[Opt]   [Opt]──[Cmd]   [Cmd]──[Ret]   [Ret]──[Spc]   [Spc]──[Bspc]
 Hyper            NAV             NUM             SYM             FUN

[Ctrl]────────[Cmd]   (non-adjacent — rare use only)
 MOUSE toggle
```

| Combo | Positions | Binding | Notes |
|---|---|---|---|
| Ctrl+Opt | 30+31 | `&kp LS(LC(LA(LGUI)))` | Hyper, unchanged |
| Opt+Cmd | 31+32 | `&tog NAV` | was `&mo NAV` |
| Cmd+Return | 32+33 | `&tog NUM` | was Cmd+Space |
| Return+Space | 33+34 | `&tog SYM` | same physical keys, was Space+Return |
| Space+Bspc | 34+35 | `&tog FUN` | was Return+Bspc (MOUSE) |
| Ctrl+Cmd | 30+32 | `&tog MOUSE` | was FUN; non-adjacent, acceptable for rare layer |

### Natural double-exit

Because `&tog` toggles, pressing the activating combo a second time turns the layer back off. Combined with the Q exit, there are always two ways out of any layer without reaching for a special key.

---

## Exit Mechanism

On every non-base layer, the **Q position** (top-left of left hand, position 0) is bound to `&to BASE`. This is a hard switch to base — it clears any active toggled layers and returns unconditionally to BASE.

- On BASE: Q position = `&mt ESC Q` (unchanged)
- On all other layers: Q position = `&to BASE`

---

## Layer Contents

All layer contents are unchanged from the current layout. The only modification to each non-base layer is the Q position binding.

Thumb keys on non-base layers remain `&trans` (transparent), so Cmd, Opt, Ctrl, Return, Space, and Bspc continue to function normally while a layer is active — e.g. Cmd+C works fine while in NAV.

---

## Changes Required in `crosses.keymap`

### 1. Base layer — swap Space and Return

```c
// pos 33: was &kp SPACE → now &kp RET
// pos 34: was &kp RET   → now &kp SPACE
```

### 2. Combos — update positions and change &mo to &tog

| Combo name | Old positions | New positions | Old binding | New binding |
|---|---|---|---|---|
| combo_nav | 31+32 | 31+32 | `&mo NAV` | `&tog NAV` |
| combo_num | 32+33 | 32+33 | `&mo NUM` | `&tog NUM` |
| combo_sym | 33+34 | 33+34 | `&mo SYM` | `&tog SYM` |
| combo_fun | 30+32 | 34+35 | `&mo FUN` | `&tog FUN` |
| combo_mouse | 34+35 | 30+32 | `&mo MOUSE` | `&tog MOUSE` |
| combo_hyper | 30+31 | 30+31 | unchanged | unchanged |

### 3. All non-base layers — Q position

In each of the 5 non-base layers, change position 0 from `&trans` to `&to BASE`.

---

## ZMK Behaviours

No new behaviours needed. All required behaviours (`&tog`, `&to`, `&mt`, `&kp`, `&combo`) are standard ZMK. Existing `&mt` and `&lt` config (tapping-term, quick-tap) is unchanged.

---

## What Does Not Change

- All alpha key positions
- All hold behaviours (Z=Shift, /=Shift, Q=Esc)
- All layer content (NAV, NUM, SYM, FUN, MOUSE key assignments)
- Combo timeout (50ms)
- Hyper combo
- `caps_word` behaviour
- Layer indices and `#define` constants
