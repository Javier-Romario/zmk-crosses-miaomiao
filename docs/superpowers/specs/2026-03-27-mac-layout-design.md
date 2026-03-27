# Mac-Inspired Keymap Redesign

**Date:** 2026-03-27
**Keyboard:** Crosses (3x5+3, 36 keys, ZMK)
**Goal:** Reimagine the layout to feel like a Mac keyboard — modifiers on thumb cluster, layers via combos, clean home row.

---

## Summary of Changes from Current Layout

| Aspect | Before | After |
|---|---|---|
| Modifiers | Home row mods (hold A/S/D/F) | Dedicated thumb keys |
| Layer access | Hold-tap on thumbs (lt) | Two-key combos |
| Shift | Hold F / hold J | Hold Z / hold / |
| Esc | Tap left thumb | Hold Q |
| Hyper | Not present | Ctrl+Opt combo |

---

## Base Layer

```
┌─────┬─────┬─────┬─────┬─────┐   ┌─────┬─────┬─────┬─────┬─────┐
│  Q  │  W  │  E  │  R  │  T  │   │  Y  │  U  │  I  │  O  │  P  │
│ Esc*│     │     │     │     │   │     │     │     │     │     │
├─────┼─────┼─────┼─────┼─────┤   ├─────┼─────┼─────┼─────┼─────┤
│  A  │  S  │  D  │  F  │  G  │   │  H  │  J  │  K  │  L  │  '  │
│     │     │     │     │     │   │     │     │     │     │     │
├─────┼─────┼─────┼─────┼─────┤   ├─────┼─────┼─────┼─────┼─────┤
│  Z  │  X  │  C  │  V  │  B  │   │  N  │  M  │  ,  │  .  │  /  │
│  ⇧* │     │     │     │     │   │     │     │     │     │  ⇧* │
└─────┴─────┴─────┼─────┼─────┼───┤   ├─────┼─────┼─────┼─────┴─────┘
                  │Ctrl │ Opt │ ⌘ │   │ Spc │ Ret │  ⌫  │
                  └─────┴─────┴───┘   └─────┴─────┴─────┘
```

`*` = hold behaviour. All other keys are tap-only (no dual-role).

**Hold behaviours:**
- Hold `Q` → Esc
- Hold `Z` → Left Shift
- Hold `/` → Right Shift
- `Ctrl`, `Opt`, `⌘`, `Space`, `Return`, `⌫` are dedicated — tap and hold send the same key

---

## Thumb Combo Map

All layer access is via two-key combos on the thumb cluster. Combos are held to keep the layer active (momentary).

| Combo | Layer |
|---|---|
| `Ctrl` + `Opt` | **Hyper** (⌘⌃⌥⇧ — all four mods simultaneously) |
| `Opt` + `⌘` | **Navigation** |
| `⌘` + `Space` | **Numbers** (cross-hand inner thumbs) |
| `Space` + `Return` | **Symbols** |
| `Return` + `⌫` | **Function** |
| `Ctrl` + `⌘` | **Mouse + Media** |

---

## Navigation Layer (`Opt + ⌘`)

Right hand: arrows, home/end/page, delete cluster.
Left hand: clipboard shortcuts.

```
┌──────┬──────┬──────┬──────┬──────┐   ┌──────┬──────┬──────┬──────┬──────┐
│  ··  │  ··  │  ··  │  ··  │  ··  │   │ ⌘⇧Z  │  ⌘V  │  ⌘C  │  ⌘X  │  ⌘Z  │
├──────┼──────┼──────┼──────┼──────┤   ├──────┼──────┼──────┼──────┼──────┤
│  ··  │  ··  │  ··  │  ··  │  ··  │   │  ←   │  ↓   │  ↑   │  →   │CapsW │
├──────┼──────┼──────┼──────┼──────┤   ├──────┼──────┼──────┼──────┼──────┤
│  ··  │  ··  │  ··  │  ··  │  ··  │   │ Home │ PgDn │ PgUp │ End  │  ··  │
└──────┴──────┴──────┼──────┼──────┼───┤   ├──────┼──────┼──────┴──────┴──────┘
                     │ (hld)│ (hld)│ ⌘ │   │ Ret  │  ⌫   │ Del  │
                     └──────┴──────┴───┘   └──────┴──────┴──────┘
```

---

## Numbers Layer (`⌘ + Space`)

Right hand: numpad layout. Thumb row: `-`, `0`, `.` for arithmetic.

```
┌─────┬─────┬─────┬─────┬─────┐   ┌─────┬─────┬─────┬─────┬─────┐
│  ·· │  ·· │  ·· │  ·· │  ·· │   │  [  │  7  │  8  │  9  │  ]  │
├─────┼─────┼─────┼─────┼─────┤   ├─────┼─────┼─────┼─────┼─────┤
│  ·· │  ·· │  ·· │  ·· │  ·· │   │  ;  │  4  │  5  │  6  │  =  │
├─────┼─────┼─────┼─────┼─────┤   ├─────┼─────┼─────┼─────┼─────┤
│  ·· │  ·· │  ·· │  ·· │  ·· │   │  `  │  1  │  2  │  3  │  \  │
└─────┴─────┴─────┼─────┼─────┼───┤   ├─────┼─────┼─────┴─────┴─────┘
                  │  ⌘  │(hld)│   │   │  -  │  0  │  .  │
                  └─────┴─────┴───┘   └─────┴─────┴─────┘
```

---

## Symbols Layer (`Space + Return`)

Left hand: all symbols. Mirrors the Numbers layer approach (left = symbols, right = numbers).

```
┌─────┬─────┬─────┬─────┬─────┐   ┌─────┬─────┬─────┬─────┬─────┐
│  {  │  &  │  *  │  (  │  }  │   │  ·· │  ·· │  ·· │  ·· │  ·· │
├─────┼─────┼─────┼─────┼─────┤   ├─────┼─────┼─────┼─────┼─────┤
│  :  │  $  │  %  │  ^  │  +  │   │  ·· │  ·· │  ·· │  ·· │  ·· │
├─────┼─────┼─────┼─────┼─────┤   ├─────┼─────┼─────┼─────┼─────┤
│  ~  │  !  │  @  │  #  │  |  │   │  ·· │  ·· │  ·· │  ·· │  ·· │
└─────┴─────┴─────┼─────┼─────┼───┤   ├─────┼─────┼─────┴─────┴─────┘
                  │  (  │  )  │  _│   │(hld)│(hld)│  ·· │
                  └─────┴─────┴───┘   └─────┴─────┴─────┘
```

---

## Function Layer (`Return + ⌫`)

Left hand: F-keys in numpad layout.

```
┌─────┬─────┬─────┬─────┬──────┐   ┌─────┬─────┬─────┬─────┬─────┐
│ F12 │  F7 │  F8 │  F9 │PrtSc │   │  ·· │  ·· │  ·· │  ·· │  ·· │
├─────┼─────┼─────┼─────┼──────┤   ├─────┼─────┼─────┼─────┼─────┤
│ F11 │  F4 │  F5 │  F6 │ScLk  │   │  ·· │  ·· │  ·· │  ·· │  ·· │
├─────┼─────┼─────┼─────┼──────┤   ├─────┼─────┼─────┼─────┼─────┤
│ F10 │  F1 │  F2 │  F3 │Pause │   │  ·· │  ·· │  ·· │  ·· │  ·· │
└─────┴─────┴─────┼─────┼──────┼───┤   ├─────┼─────┼─────┴─────┴─────┘
                  │Menu │ Spc  │ ⌘ │   │  ·· │(hld)│(hld)│
                  └─────┴──────┴───┘   └─────┴─────┴─────┘
```

---

## Mouse + Media Layer (`Ctrl + ⌘`)

Left hand: media controls + RGB + Bluetooth management.
Right hand home row: mouse buttons on H/J/K.

```
┌──────┬──────┬──────┬──────┬──────┐   ┌──────┬──────┬──────┬──────┬──────┐
│BT CLR│  ··  │BRI ↓ │BRI ↑ │  ··  │   │ BT1  │ BT2  │ BT3  │ BT4  │ BT5  │
├──────┼──────┼──────┼──────┼──────┤   ├──────┼──────┼──────┼──────┼──────┤
│ PREV │VOL ↓ │VOL ↑ │ NEXT │  ··  │   │ 🖱L  │ 🖱M  │ 🖱R  │  ··  │  ··  │
├──────┼──────┼──────┼──────┼──────┤   ├──────┼──────┼──────┼──────┼──────┤
│RGB EF│HUE + │SAT + │SAT - │RGB ↑ │   │  ··  │  ··  │  ··  │  ··  │  ··  │
└──────┴──────┴──────┼──────┼──────┼───┤   ├──────┼──────┼──────┴──────┴──────┘
                     │ Mute │  ⏯  │ ⏹ │   │  ··  │  ··  │  ··  │
                     └──────┴──────┴───┘   └──────┴──────┴──────┘
```

Mouse clicks: `H` = left click, `J` = middle click, `K` = right click.

---

## Implementation Notes

### ZMK Behaviours Needed

- `&mt` (mod-tap) for hold-Q=Esc, hold-Z=Shift, hold-/=Shift
- `&combo` for all 6 thumb combos (layer + Hyper)
- `&mo` (momentary layer) triggered by combos — combos output a keycode that activates a layer, or use `&sl`/`&mo` via combo binding
- Hyper = `&kp LS(LC(LA(LGUI)))` — all four mods simultaneously
- Existing `caps_word` behaviour carries over unchanged
- Existing `&lt` and `&mt` config (tapping-term, quick-tap) should be reviewed since hold-tap timing matters more now that Z and / are shift keys

### Combo Timing

Combos need a short `timeout-ms` (e.g. 30–50ms) to avoid accidental triggers during fast typing. Since these are all thumb keys, simultaneous press is natural and timing can be tight.

Note: `Ctrl + ⌘` (Mouse+Media) are non-adjacent keys (leftmost and rightmost of left thumb cluster). ZMK combos support non-adjacent keys fine, but the physical roll distance is larger — if it feels awkward in practice, swap this combo to `Ctrl + Space` (cross-hand) or move Mouse+Media to a different combo pair.

### Layer Count

6 layers total: Base (0), Nav (1), Num (2), Sym (3), Fun (4), Mouse+Media (5).
Existing layer indices shift — `#define` constants need updating.
