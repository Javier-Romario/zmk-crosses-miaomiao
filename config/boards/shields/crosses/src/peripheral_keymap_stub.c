/*
 * zmk_keymap_highest_layer_active is only compiled for the central side
 * (see ZMK app/CMakeLists.txt: "if NOT CONFIG_ZMK_SPLIT OR ROLE_CENTRAL").
 *
 * The PMW3610 driver calls it unconditionally in get_input_mode_for_current_layer().
 * On the peripheral, scroll-layers and snipe-layers are not set in DTS so the
 * driver always returns MOVE regardless of layer — returning 0 here is correct.
 */
int zmk_keymap_highest_layer_active(void) { return 0; }
