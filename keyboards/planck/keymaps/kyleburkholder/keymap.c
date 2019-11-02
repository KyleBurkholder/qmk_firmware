/* Copyright 2015-2017 Jack Humbert
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include QMK_KEYBOARD_H
#include "muse.h"

extern keymap_config_t keymap_config;

enum planck_layers {
  _QWERTY,
  _LOWER,
  _RAISE,
  _ADJUST,
  _FUNCTIONS
};

enum planck_keycodes {
  QWERTY = SAFE_RANGE
};

#define LOWER MO(_LOWER)
#define RAISE MO(_RAISE)

#define CTL_LBR LCTL_T(KC_LBRC)
#define CTL_RBR RCTL_T(KC_RBRC)

// Dashes (macOS)
#define KC_NDSH LALT(KC_MINS)
#define KC_MDSH S(LALT(KC_MINS))


// tapdance keycodes
enum td_keycodes {
  FN_LCB,
  FN_RCB
};

// define a type containing as many tapdance states as you need
typedef enum {
  SINGLE_TAP,
  SINGLE_HOLD,
  DOUBLE_SINGLE_TAP
} td_state_t;

// create a global instance of the tapdance state type
static td_state_t td_state;

// declare your tapdance functions:

// function to determine the current tapdance state
int cur_dance (qk_tap_dance_state_t *state);

// `finished` and `reset` functions for each tapdance keycode
void fnlcb_finished (qk_tap_dance_state_t *state, void *user_data);
void fnlcb_reset (qk_tap_dance_state_t *state, void *user_data);
void fnrcb_finished (qk_tap_dance_state_t *state, void *user_data);
void fnrcb_reset (qk_tap_dance_state_t *state, void *user_data);


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* Qwerty
 * ,-----------------------------------------------------------------------------------.
 * | Tab  |   Q  |   W  |   E  |   R  |   T  |   Y  |   U  |   I  |   O  |   P  | Bksp |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Esc  |   A  |   S  |   D  |   F  |   G  |   H  |   J  |   K  |   L  |   ;  |  "   |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |   (  |      |      |      |      |      |      |      |      |      |      |  )   |
 * | Shift|   Z  |   X  |   C  |   V  |   B  |   N  |   M  |   ,  |   .  |   /  |Shift |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |   {  |   [  |      |      |      |      |      |      |      |      |   ]  |  }   |
 * | Func | Ctrl | GUI  | Alt  |Lower |Space | Enter|Raise | Alt  | GUI  | Ctrl | Func |
 * `-----------------------------------------------------------------------------------'
 */
[_QWERTY] = LAYOUT_planck_grid(
    KC_TAB,     KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,     KC_BSPC,
    KC_ESC,     KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN,  KC_QUOT,
    KC_LSPO,    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,  KC_RSPC,
    TD(FN_LCB), CTL_LBR, KC_LALT, KC_LGUI, LOWER,  KC_SPC,  KC_ENT,  RAISE,   KC_RGUI, KC_RALT,  CTL_RBR,  TD(FN_RCB)
),

/* Lower
 * ,-----------------------------------------------------------------------------------.
 * |      |  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |  F7  |  F8  |  F9  | F10  |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Delet|   1  |   2  |   3  |   4  |   5  |   6  |   7  |   8  |   9  |   0  |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |   -  |   =  |   \  |   :  |   "  |ndash |mdash |   ,  |   .  |   /  |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |             |      |      |      |      |      |
 * `-----------------------------------------------------------------------------------'
 */
[_FUNCTIONS] = LAYOUT_planck_grid(
    _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  _______,
    _______, KC_F11,  KC_F12,  KC_F13,  KC_F14,  KC_F15,  KC_F16,  KC_F17,  KC_F18,  KC_F19,  KC_F20,  _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
),

/* Raise
 * ,-----------------------------------------------------------------------------------.
 * |   `  |   1  |   2  |   3  |   4  |   5  |   6  |   7  |   8  |   9  |   0  |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Delet|  !   |   @  |   #  |   $  |   %  |   ^  |   &  |   *  |   -  |   =  |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |   _  |   +  |   \  |   :  |   "  |ndash |mdash |   ,  |   .  |   /  |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |             |      |      |      |      |      |
 * `-----------------------------------------------------------------------------------'
 */
[_RAISE] = LAYOUT_planck_grid(
    KC_GRV,  KC_1,          KC_2,    KC_3,    KC_4,     KC_5,            KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    _______,
    KC_DEL,  KC_EXLM,       KC_AT,   KC_HASH, KC_DLR,   KC_PERC,         KC_CIRC, KC_AMPR, KC_ASTR, KC_MINS, KC_EQL,  _______,
    _______, KC_UNDERSCORE, KC_PLUS, KC_BSLS, KC_COLON, KC_DOUBLE_QUOTE, KC_NDSH, KC_MDSH, KC_COMM, KC_DOT,  KC_SLSH, _______,
    _______, _______,       _______, _______, _______,  _______,         _______, _______, _______, _______, _______, _______
),

/* Lower
 * ,-----------------------------------------------------------------------------------.
 * |  ~   |Prev  │Play  │Next  │Brig- │Sleep │Wake  │Brig+ | Mute | Vol- | Vol+ |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * │ Delet│Home  │ PgUp │ PgDn │  End │Launch|Switch|   ←  │   ↓  │   ↑  │   →  │      │
 * ├──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┤
 * │      | ⌘ ←  │ ⌘ ↑  │ ⌘ ↓  │ ⌘ →  │MoveL |MoveR | ⌥ ←  │ ⌥ ↓  │ ⌥ ↑  │ ⌥ →  │      │
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |             |      |      |      |      |      |
 * `-----------------------------------------------------------------------------------'
 */
[_LOWER] = LAYOUT_planck_grid(
    KC_TILD, KC_MPRV,       KC_MPLY,       KC_MNXT,     KC_BRID,       KC_SLEP,           KC_WAKE,        KC_BRIU,       KC_MUTE,       KC_VOLD,     KC_VOLU,       _______,
    KC_DEL,  KC_HOME,       KC_PGUP,       KC_PGDN,     KC_END,        LCTL(LSFT(KC_UP)), LCTL(KC_UP),    KC_LEFT,       KC_DOWN,       KC_UP,       KC_RGHT,       _______,
    _______, LGUI(KC_LEFT), LGUI(KC_UP), LGUI(KC_DOWN), LGUI(KC_RGHT),  LCTL(KC_LEFT),    LCTL(KC_RIGHT), LALT(KC_LEFT), LALT(KC_DOWN), LALT(KC_UP), LALT(KC_RGHT),   _______,
    _______, _______,       _______,     _______,       _______,       _______,           _______,        _______,       _______,       _______,     _______,       _______
),

/* Adjust (Lower + Raise)
 *
 * ,-----------------------------------------------------------------------------------.
 * |      | Reset|Debug |      |      |      |      |      |      |      |      |  Del |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |MUSmod|Aud on|Audoff|AGnorm|AGswap|Qwerty|      |      |      |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |Voice-|Voice+|Mus on|Musoff|MIDIon|MIDIof|TermOn|TermOf|      |      |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |             |      |      |      |      |      |
 * `-----------------------------------------------------------------------------------'
 */
[_ADJUST] = LAYOUT_planck_grid(
    _______, RESET,   DEBUG,   _______, _______, _______, _______, _______, _______,  _______, _______, KC_DEL ,
    _______, _______, MU_MOD,  AU_ON,   AU_OFF,  AG_NORM, AG_SWAP, QWERTY,  _______,  _______,  _______,  _______,
    _______, MUV_DE,  MUV_IN,  MU_ON,   MU_OFF,  MI_ON,   MI_OFF,  TERM_ON, TERM_OFF, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______,  _______, _______, _______
)

};

layer_state_t layer_state_set_user(layer_state_t state) {
  return update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST);
}

// determine the tapdance state to return
int cur_dance (qk_tap_dance_state_t *state) {
  if (state->count == 1) {
    if (state->interrupted || !state->pressed) { return SINGLE_TAP; }
    else { return SINGLE_HOLD; }
  }
  if (state->count == 2) { return DOUBLE_SINGLE_TAP; }
  else { return 3; } // any number higher than the maximum state value you return above
}

// handle the possible states for each tapdance keycode you define:

void fnlcb_finished (qk_tap_dance_state_t *state, void *user_data) {
  td_state = cur_dance(state);
  switch (td_state) {
    case SINGLE_TAP:
      register_code16(KC_LCBR);
      break;
    case SINGLE_HOLD:
      layer_on(_FUNCTIONS); // for a layer-tap key, use `layer_on(_MY_LAYER)` here
      break;
    case DOUBLE_SINGLE_TAP: // allow nesting of 2 parens `((` within tapping term
      tap_code16(KC_LCBR);
      register_code16(KC_LCBR);
  }
}

void fnlcb_reset (qk_tap_dance_state_t *state, void *user_data) {
  switch (td_state) {
    case SINGLE_TAP:
      unregister_code16(KC_LCBR);
      break;
    case SINGLE_HOLD:
      layer_off(_FUNCTIONS); // for a layer-tap key, use `layer_off(_MY_LAYER)` here
      break;
    case DOUBLE_SINGLE_TAP:
      unregister_code16(KC_LCBR);
  }
}

void fnrcb_finished (qk_tap_dance_state_t *state, void *user_data) {
  td_state = cur_dance(state);
  switch (td_state) {
    case SINGLE_TAP:
      register_code16(KC_RCBR);
      break;
    case SINGLE_HOLD:
      layer_on(_FUNCTIONS); // for a layer-tap key, use `layer_on(_MY_LAYER)` here
      break;
    case DOUBLE_SINGLE_TAP: // allow nesting of 2 parens `((` within tapping term
      tap_code16(KC_RCBR);
      register_code16(KC_RCBR);
  }
}

void fnrcb_reset (qk_tap_dance_state_t *state, void *user_data) {
  switch (td_state) {
    case SINGLE_TAP:
      unregister_code16(KC_RCBR);
      break;
    case SINGLE_HOLD:
      layer_off(_FUNCTIONS); // for a layer-tap key, use `layer_off(_MY_LAYER)` here
      break;
    case DOUBLE_SINGLE_TAP:
      unregister_code16(KC_RCBR);
  }
}

// define `ACTION_TAP_DANCE_FN_ADVANCED()` for each tapdance keycode, passing in `finished` and `reset` functions
qk_tap_dance_action_t tap_dance_actions[] = {
  [FN_LCB] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, fnlcb_finished, fnlcb_reset),
  [FN_RCB] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, fnrcb_finished, fnrcb_reset)
};
