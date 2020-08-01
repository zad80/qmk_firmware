#include QMK_KEYBOARD_H
#include "version.h"
#include "config.h"



#define BASE 0 // default layer
#define SYMB 1 // symbols
#define MDIA 2 // media keys
debug_matrix   = true;
debug_keyboard = true;
debug_mouse    = true;
debug_enable   = true;
enum custom_keycodes {
    PLACEHOLDER = SAFE_RANGE, // can always be here
    EPRM,
    VRSN,
    RGB_SLD
};

//#define LOCAL_MATRIX_COLS 9
//#define MATRIX_COLS LOCAL_MATRIX_COLS*2
//#define MATRIX_ROWS 5
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/* Keymap 0: Basic layer
 *
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * |   =    |   1  |   2  |   3  |   4  |   5  | LEFT |           | RIGHT|   6  |   7  |   8  |   9  |   0  |   -    |
 * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
 * | Del    |   Q  |   W  |   E  |   R  |   T  |  L1  |           |  L1  |   Y  |   U  |   I  |   O  |   P  |   \    |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * | BkSp   |   A  |   S  |   D  |   F  |   G  |------|           |------|   H  |   J  |   K  |   L  |; / L2|' / Cmd |
 * |--------+------+------+------+------+------| Hyper|           | Meh  |------+------+------+------+------+--------|
 * | LShift |Z/Ctrl|   X  |   C  |   V  |   B  |      |           |      |   N  |   M  |   ,  |   .  |//Ctrl| RShift |
 * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   |Grv/L1|  '"  |AltShf| Left | Right|                                       |  Up  | Down |   [  |   ]  | ~L1  |
 *   `----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        | App  | LGui |       | Alt  |Ctrl/Esc|
 *                                 ,------|------|------|       |------+--------+------.
 *                                 |      |      | Home |       | PgUp |        |      |
 *                                 | Space|Backsp|------|       |------|  Tab   |Enter |
 *                                 |      |ace   | End  |       | PgDn |        |      |
 *                                 `--------------------'       `----------------------'
 */
// If it accepts an argument (i.e, is a function), it doesn't need KC_.
// Otherwise, it needs KC_*
[BASE] = LAYOUT_ergodox(
  // left hand
  KC_EQL,          KC_1,        KC_2,          KC_3,    KC_4,    KC_5,    KC_LEFT,//7
  KC_DEL,          KC_Q,        KC_W,          KC_E,    KC_R,    KC_T,    TG(SYMB),//7
  KC_BSPC,         KC_A,        KC_S,          KC_D,    KC_F,    KC_G,  ALT_T(KC_NO), //7
  KC_LSFT,         CTL_T(KC_Z), KC_X,          KC_C,    KC_V,    KC_B,  ALT_T(KC_APP),//7
  LT(SYMB,KC_GRV), KC_QUOT,     LALT(KC_LSFT), KC_LEFT, KC_RGHT,     KC_RGHT,  KC_LGUI,//7
KC_HOME,    KC_SPC, KC_BSPC,//3
  // right hand
  KC_RGHT,      KC_6,    KC_7,    KC_8,    KC_9,   KC_0,                KC_MINS,//7
  TG(SYMB),     KC_Y,    KC_U,    KC_I,    KC_O,   KC_P,                KC_BSLS,//7
  KC_MINS,      KC_H,    KC_J,    KC_K,    KC_L,   LT(MDIA, KC_SCLN),   KC_QUOT,
  KC_RALT,      KC_N,    KC_M,    KC_COMM, KC_DOT, CTL_T(KC_SLSH),      KC_RSFT,
  KC_PGUP,   CTL_T(KC_ESC), KC_UP, KC_DOWN,KC_LBRC, KC_RBRC, TT(SYMB),
  KC_PGDN, KC_TAB, KC_ENTER
),
// /* Keymap 1: Symbol Layer
//  *
//  * ,---------------------------------------------------.           ,--------------------------------------------------.
//  * |Version  |  F1  |  F2  |  F3  |  F4  |  F5  |      |           |      |  F6  |  F7  |  F8  |  F9  |  F10 |   F11  |
//  * |---------+------+------+------+------+------+------|           |------+------+------+------+------+------+--------|
//  * |         |   !  |   @  |   {  |   }  |   |  |      |           |      |   Up |   7  |   8  |   9  |   *  |   F12  |
//  * |---------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
//  * |         |   #  |   $  |   (  |   )  |   `  |------|           |------| Down |   4  |   5  |   6  |   +  |        |
//  * |---------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
//  * |         |   %  |   ^  |   [  |   ]  |   ~  |      |           |      |   &  |   1  |   2  |   3  |   \  |        |
//  * `---------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
//  *   | EPRM  |      |      |      |      |                                       |      |    . |   0  |   =  |      |
//  *   `-----------------------------------'                                       `----------------------------------'
//  *                                        ,-------------.       ,-------------.
//  *                                        |Animat|      |       |Toggle|Solid |
//  *                                 ,------|------|------|       |------+------+------.
//  *                                 |Bright|Bright|      |       |      |Hue-  |Hue+  |
//  *                                 |ness- |ness+ |------|       |------|      |      |
//  *                                 |      |      |      |       |      |      |      |
//  *                                 `--------------------'       `--------------------'
//  */
// // SYMBOLS
// [SYMB] = LAYOUT_ergodox(
//     // left hand
//       VRSN,            KC_F1,       KC_F2,        KC_F3,    KC_F4,  KC_F5,  KC_TRNS,//7
//       KC_TRNS,         KC_EXLM,     KC_AT,        KC_LCBR,  KC_RCBR,KC_PIPE,KC_TRNS,//7
//       KC_TRNS,         KC_HASH,     KC_DLR,       KC_LPRN,  KC_RPRN,KC_GRV, ALT_T(KC_NO),                        RGB_MOD,KC_TRNS,//9
//       KC_TRNS,         KC_PERC,     KC_CIRC,      KC_LBRC,  KC_RBRC,KC_TILD,                                            KC_TRNS,//7
//       EPRM,            KC_TRNS,     KC_TRNS,      KC_TRNS,  KC_TRNS,                                             RGB_VAD,RGB_VAI,KC_TRNS,//8
// // right hand
//     KC_TRNS, KC_F6,   KC_F7,  KC_F8,   KC_F9,   KC_F10,  KC_F11,
//     KC_TRNS, KC_UP,   KC_7,   KC_8,    KC_9,    KC_ASTR, KC_F12,
//     KC_DOWN, KC_4,   KC_5,    KC_6,    KC_PLUS, KC_TRNS, KC_TRNS,                                                RGB_TOG, RGB_SLD,
//     KC_TRNS, KC_AMPR, KC_1,   KC_2,    KC_3,    KC_BSLS,                                                                KC_TRNS,
//     KC_TRNS,KC_DOT,  KC_0,    KC_EQL,  KC_TRNS,                                                                  KC_TRNS, RGB_HUD, RGB_HUI
// ),
// /* Keymap 2: Media and mouse keys
//  *
//  * ,--------------------------------------------------.           ,--------------------------------------------------.
//  * |        |      |      |      |      |      |      |           |      |      |      |      |      |      |        |
//  * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
//  * |        |      |      | MsUp |      |      |      |           |      |      |      |      |      |      |        |
//  * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
//  * |        |      |MsLeft|MsDown|MsRght|      |------|           |------|      |      |      |      |      |  Play  |
//  * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
//  * |        |      |      |      |      |      |      |           |      |      |      | Prev | Next |      |        |
//  * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
//  *   |      |      |      | Lclk | Rclk |                                       |VolUp |VolDn | Mute |      |      |
//  *   `----------------------------------'                                       `----------------------------------'
//  *                                        ,-------------.       ,-------------.
//  *                                        |      |      |       |      |      |
//  *                                 ,------|------|------|       |------+------+------.
//  *                                 |      |      |      |       |      |      |Brwser|
//  *                                 |      |      |------|       |------|      |Back  |
//  *                                 |      |      |      |       |      |      |      |
//  *                                 `--------------------'       `--------------------'
//  */
// /* MEDIA AND MOUSE
// for now is equal to symbol*/
// [MDIA] = LAYOUT_ergodox(
//     // left hand
//       VRSN,            KC_F1,       KC_F2,        KC_F3,    KC_F4,  KC_F5,  KC_TRNS,//7
//       KC_TRNS,         KC_EXLM,     KC_AT,        KC_LCBR,  KC_RCBR,KC_PIPE,KC_TRNS,//7
//       KC_TRNS,         KC_HASH,     KC_DLR,       KC_LPRN,  KC_RPRN,KC_GRV, ALT_T(KC_NO),                        RGB_MOD,KC_TRNS,//9
//       KC_TRNS,         KC_PERC,     KC_CIRC,      KC_LBRC,  KC_RBRC,KC_TILD,                                            KC_TRNS,//7
//       EPRM,            KC_TRNS,     KC_TRNS,      KC_TRNS,  KC_TRNS,                                             RGB_VAD,RGB_VAI,KC_TRNS,//8
// // right hand
//     KC_TRNS, KC_F6,   KC_F7,  KC_F8,   KC_F9,   KC_F10,  KC_F11,
//     KC_TRNS, KC_UP,   KC_7,   KC_8,    KC_9,    KC_ASTR, KC_F12,
//     KC_DOWN, KC_4,   KC_5,    KC_6,    KC_PLUS, KC_TRNS, KC_TRNS,                                                RGB_TOG, RGB_SLD,
//     KC_TRNS, KC_AMPR, KC_1,   KC_2,    KC_3,    KC_BSLS,                                                                KC_TRNS,
//     KC_TRNS,KC_DOT,  KC_0,    KC_EQL,  KC_TRNS,                                                                  KC_TRNS, RGB_HUD, RGB_HUI
// ),
// /*
// [MDIA] = LAYOUT_ergozad(
//     KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
//     KC_TRNS, KC_TRNS, KC_TRNS, KC_MS_U, KC_TRNS, KC_TRNS, KC_TRNS,
//     KC_TRNS, KC_TRNS, KC_MS_L, KC_MS_D, KC_MS_R, KC_TRNS,
//     KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
//     KC_TRNS, KC_TRNS, KC_TRNS, KC_BTN1, KC_BTN2,
//     KC_TRNS, KC_TRNS,
//     KC_TRNS,
//     KC_TRNS, KC_TRNS, KC_TRNS,
// // right hand
//     KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
//     KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
//     KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_MPLY,
//     KC_TRNS,  KC_TRNS, KC_TRNS, KC_MPRV, KC_MNXT, KC_TRNS, KC_TRNS,
//     KC_VOLU, KC_VOLD, KC_MUTE, KC_TRNS, KC_TRNS,
//     KC_TRNS, KC_TRNS,
//     KC_TRNS,
//     KC_TRNS, KC_TRNS, KC_WBAK
// ),
// */
};

void oled_task_user(void) {
    // Host Keyboard Layer Status

}

