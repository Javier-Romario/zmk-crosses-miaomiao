# BLE Split Architecture — Crosses (miaomiao)

## Hardware

- **Board**: nice!nano v2 (nRF52840) on both halves
- **Trackballs**: dual PMW3610 (inorichi driver), one per half via SPI
- **Split**: wireless BLE, right is central/host, left is peripheral

## Role Diagram

```
Mac ←── BLE HID ──→ Right (central)  ←── BLE split GATT ──→  Left (peripheral)
```

- Right connects to Mac as a standard BLE HID device (keyboard + mouse)
- Right acts as central to left over ZMK's split GATT protocol
- Left has no direct Mac connection — all its key/trackball data goes through right
- Left's trackball (scroll) is forwarded to right via `CONFIG_ZMK_INPUT_SPLIT`

---

## Sleep Architecture

The core challenge: ZMK v0.3.0's central scan after System Off wake doesn't reliably reconnect to the peripheral. The architecture avoids this race entirely.

### Right: never sleeps

`CONFIG_ZMK_SLEEP=n` in `crosses_right.conf`. Right stays permanently powered. Its BLE central scan runs continuously and reconnects to left automatically after any left restart. This also keeps the right-Mac HID bond alive during Mac sleep/wake cycles.

**Why not `SLEEP=n` on both halves?** Left with `SLEEP=n` has no recovery cycle — its BLE stack accumulates stuck states with no way to clear them except a physical power cycle. Left needs periodic clean reboots.

### Left: insomnia-assisted sleep/wake cycle

Left uses `zmk-behavior-insomnia` (badjeff) to stay awake while connected and sleep-recover when the link drops:

1. **Connected**: insomnia pings ZMK's activity timer every 10 s → sleep timer never reaches 25 s → left stays awake indefinitely
2. **Link drops**: pings stop (insomnia checks `zmk_split_bt_peripheral_is_connected()`) → after 25 s of no pings, left enters System Off (nRF52840 deep sleep, ~1 µA)
3. **Recovery**: any key press wakes left from System Off via `wakeup-source` on `kscan0` → clean boot → fresh BLE advertising → right (always scanning) reconnects within ~5 s

This gives automatic recovery without user intervention: key press → reconnected.

---

## Config Files

### `crosses_left.conf` (peripheral)

| Config | Value | Reason |
|--------|-------|--------|
| `CONFIG_ZMK_SLEEP` | `y` | Enables System Off sleep/wake recovery cycle |
| `CONFIG_ZMK_IDLE_SLEEP_TIMEOUT` | `25000` | 25 s after last ping — 15 s margin over 10 s ping interval, survives ping jitter |
| `CONFIG_ZMK_INSOMNIA_PING_INTERVAL` | `10000` | Ping every 10 s; checks connection state frequently, detects zombie links sooner |
| `CONFIG_ZMK_INSOMNIA_PING_ON_START` | `y` | Auto-starts on boot, no key binding needed |
| `CONFIG_PMW3610_POLLING_RATE_125` | `y` | 125 Hz (left is scroll-only — 250 Hz unnecessary) |
| `CONFIG_ZMK_BLE_EXPERIMENTAL_FEATURES` | *(not set)* | Left uses split GATT only, not BLE HID — experimental HID features are unused and add BLE stack instability |

### `crosses_right.conf` (central)

| Config | Value | Reason |
|--------|-------|--------|
| `CONFIG_ZMK_SLEEP` | `n` | Never sleeps — preserves split connection |
| `CONFIG_BT_GAP_AUTO_UPDATE_CONN_PARAMS` | `y` | Right-only — allows Mac to negotiate HID connection params. Must NOT be in shared conf (would also apply to left's split connection and interfere) |
| `CONFIG_ZMK_SPLIT_BLE_PREF_TIMEOUT` | `400` | 4 s supervision timeout — right detects dead split link within 4 s |
| `CONFIG_ZMK_SPLIT_BLE_PREF_LATENCY` | `0` | No slave latency on split link — keeps input latency low |
| `CONFIG_BT_PERIPHERAL_PREF_MIN_INT` | `6` | 7.5 ms preferred HID interval to Mac for low-latency mouse |
| `CONFIG_BT_PERIPHERAL_PREF_MAX_INT` | `12` | 15 ms max |
| `CONFIG_ZMK_BLE_EXPERIMENTAL_FEATURES` | `y` | Required for BLE mouse/pointer HID profile to Mac |

### `crosses.conf` (shared, both halves)

| Config | Value | Reason |
|--------|-------|--------|
| `CONFIG_BT_CTLR_TX_PWR_PLUS_8` | `y` | Max TX power for reliable range |
| `CONFIG_ZMK_IDLE_SLEEP_TIMEOUT` | `900000` | 15 min default — overridden per-half |
| `CONFIG_ZMK_IDLE_TIMEOUT` | `60000` | 60 s idle (note: left sleeps at 25 s so idle never fires on left) |

---

## PMW3610 Trackball Notes

- **Driver limitation**: inorichi PMW3610 driver has no `pm_action` callback and no `GPIO_INT_WAKEUP`. The trackball IRQ **cannot wake either half from System Off** — only kscan (key matrix) can. `wakeup-source` is set on `kscan0` in `crosses.dtsi`.
- `zmk,gpio-key-wakeup-trigger` is for `CONFIG_ZMK_PM_SOFT_OFF` only — irrelevant here.
- **Left trackball**: scroll only. `INPUT_REL_X → INPUT_REL_HWHEEL`, `INPUT_REL_Y → INPUT_REL_WHEEL`. 125 Hz, CPI 3200 ÷ 8 = 400 effective.
- **Right trackball**: cursor + automouse layer (layer 1). 250 Hz, CPI 1600 ÷ 4 = 400 effective. Snipe at 200 ÷ 1.

---

## Troubleshooting

### Left stops responding

Recovery in order:

1. **Press any key on left** → should recover within ~5 s (wakes from System Off, right reconnects)
2. **Wait 30 s, try again** — if left is in a zombie-connected state (BLE layer thinks connected, RF is dead), the supervision timeout (4 s) plus sleep timeout (25 s) means it can take up to ~30 s for System Off to trigger
3. **Flip left power switch** → clean boot, right reconnects automatically

### Left stops responding after inactivity

During inactivity with a healthy link, insomnia pings keep left awake. If this is happening:
- The split link is likely dropping silently ("zombie connection") — BLE supervision timeout (4 s) should clear it, then left sleeps after 25 s, recovers on key press
- If a key press doesn't recover it within 30 s: left's BLE stack has hung while awake (never reached System Off). Power cycle required. A software watchdog (`CONFIG_WATCHDOG`) would fix this automatically but isn't currently implemented.

### After Mac sleep/wake

Right never sleeps so the split connection stays alive. The right-Mac HID bond should reconnect automatically when Mac wakes. If it doesn't respond: press any key to prompt reconnection; macOS occasionally needs this to trigger the BLE reconnect.

### Split left keys never register (trackball still works)

`CONFIG_ZMK_INPUT_SPLIT` and key matrix are independent. If trackball scroll works but keys don't: check `kscan0` wiring/`wakeup-source` in `crosses.dtsi`.

---

## Module Dependencies

| Module | Purpose | Notes |
|--------|---------|-------|
| `zmk-behavior-insomnia` (badjeff) | Keeps left awake while connected | Requires `zmk,behavior-insomnia` DT node in `crosses_left.overlay` to activate `CONFIG_ZMK_BEHAVIOR_INSOMNIA` |
| `inorichi/zmk-pmw3610-driver` | PMW3610 trackball | No sleep/wakeup support — trackball cannot wake from System Off |

---

## What Was Tried and Abandoned

| Approach | Problem |
|----------|---------|
| `CONFIG_ZMK_SLEEP=n` on both halves | Left accumulates stuck BLE states with no recovery cycle — needs physical reset |
| `CONFIG_ZMK_SLEEP=n` on left, `y` on right | Same stuck-state problem on left |
| `CONFIG_BT_GAP_AUTO_UPDATE_CONN_PARAMS=y` in shared conf | Applied to left's split connection, causing parameter renegotiation on the split link |
| `CONFIG_ZMK_BLE_EXPERIMENTAL_FEATURES=y` on left | Unnecessary (left is split GATT peripheral only) — added BLE stack instability |
| Insomnia ping 15 s / sleep 16 s | 1 s margin too tight — ping jitter caused accidental sleep |
