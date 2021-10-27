/* Copyright 2021 imper0502
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

/* 初始化指示燈 */
#define TXLED D5
#define RXLED B0
#define LED_PIN_ON_STATE 0
#define LED_ON 0
#define LED_OFF 1
 
void keyboard_pre_init_user(void) {
    setPinOutput(TXLED);
    setPinOutput(RXLED);
    writePin(TXLED, LED_OFF);
    writePin(RXLED, LED_OFF);
}

// Defines names for use in layer keycodes and the keymap
enum layer_names {
    _BS,
    _FN
};

// Defines the keycodes used by our macros in process_record_user
enum custom_keycodes {
    QMKBEST = SAFE_RANGE,
    ALT_TAB
};
bool is_alt_tab_active = false; // ADD this near the begining of keymap.c
uint16_t alt_tab_timer = 0;     // we will be using them soon.

// Tap Dance declarations
enum {
    TD_WIN_FN,
    TD_DOT_FN,
    TD_IME_CAPSLOCK,
    TD_ESCAPE_SCREENSHOT,
    COPY_PASTE,
};

// Define a type containing as many tapdance states as you need
typedef enum {
    SINGLE_TAP,
    SINGLE_HOLD,
    DOUBLE_TAP,
    OTHERWISE
} td_state_t;

typedef struct {
    bool is_press_action;
    td_state_t state;
} td_tap_t;

// Create a global instance of the tapdance state type
static td_state_t td_state;

// Declare tap dance functions:
// Function to determine the current tapdance state
uint8_t current_dance(qk_tap_dance_state_t *state);

// `finished` and `reset` functions for each tapdance keycode
#define CPY_PST TD(COPY_PASTE)
void td_copy_paste_finished(qk_tap_dance_state_t *state, void *user_data);
void td_copy_paste_reset(qk_tap_dance_state_t *state, void *user_data);

// Tap Dance definitions
qk_tap_dance_action_t tap_dance_actions[] = {
    [TD_WIN_FN] = ACTION_TAP_DANCE_LAYER_TOGGLE(KC_LWIN, _FN),
    [TD_DOT_FN] = ACTION_TAP_DANCE_LAYER_TOGGLE(KC_DOT , _FN),
    [TD_IME_CAPSLOCK] = ACTION_TAP_DANCE_DOUBLE(G(KC_SPC) , KC_CAPS),
    [TD_ESCAPE_SCREENSHOT] = ACTION_TAP_DANCE_DOUBLE(KC_ESC, LSG(KC_S)),
    [COPY_PASTE] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_copy_paste_finished, td_copy_paste_reset),
};

// RT keys
#define SR_ESC  TD(TD_ESCAPE_SCREENSHOT)
#define TD_LWIN TD(TD_WIN_FN)
#define TD_DOT  TD(TD_DOT_FN)
#define SFT_ENT KC_SFTENT
#define ST_SPC  SFT_T(KC_SPC)
#define SR_IME  TD(TD_IME_CAPSLOCK)
// R1 overriding keys
#define SR_COMM KC_COMMA
#define SR_DOT  KC_DOT
#define SR_QUES KC_QUESTION
// R4 overriding keys
#define SR_PIPE KC_PIPE         // |^
#define SR_AT   KC_AT           // @`
#define SR_HASH KC_HASH         // #~
#define SR_ASTR KC_ASTERISK     //\*&
#define SR_PERC KC_PERCENT	    // %$
#define SR_LABK KC_LEFT_ANGLE_BRACKET   // <>
#define SR_LPRN KC_LEFT_PAREN           // ()
#define SR_LBRC KC_LBRACKET             // []
#define SR_LCBR KC_LEFT_CURLY_BRACE     // {}
#define SR_SLSH KC_SLASH                // /

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* Base */
    [_BS] = LAYOUT(
    ALT_TAB,SR_PIPE,SR_AT  ,SR_HASH,SR_ASTR,SR_PERC,                SR_LABK,SR_LPRN,SR_LBRC,SR_LCBR,SR_SLSH, RESET ,
    KC_TAB ,KC_Q   ,KC_W   ,KC_F   ,KC_P   ,KC_B   ,                KC_J   ,KC_L   ,KC_U   ,KC_Y   ,KC_MINS,KC_EQL ,
    KC_BSPC,KC_A   ,KC_R   ,KC_S   ,KC_T   ,KC_G   ,                KC_M   ,KC_N   ,KC_E   ,KC_I   ,KC_O   ,KC_QUOT,
    KC_LSFT,KC_Z   ,KC_X   ,KC_C   ,KC_D   ,KC_V   ,                KC_K   ,KC_H   ,SR_COMM,SR_DOT ,SR_QUES,KC_RSFT,
                            KC_LALT,TD_LWIN,KC_LCTL,KC_LSFT,SFT_ENT,ST_SPC ,TT(_FN),KC_RALT,
                            SR_ESC ,                CPY_PST,G(KC_V),                 SR_IME
    ),
    [_FN] = LAYOUT(
    KC_F1  ,KC_F2  ,KC_F3  ,KC_F4  ,KC_F5  ,KC_F6  ,                KC_F7  ,KC_F8  ,KC_F9  ,KC_F10 ,KC_F11 ,KC_F12 ,
    _______,KC_PSLS,KC_7   ,KC_8   ,KC_9   ,KC_PMNS,                KC_PGUP,KC_HOME,KC_UP  ,KC_END ,KC_INS ,KC_VOLU,
    _______,KC_PAST,KC_4   ,KC_5   ,KC_6   ,KC_PPLS,                KC_PGDN,KC_LEFT,KC_DOWN,KC_RGHT,KC_DEL ,KC_VOLD,
    _______,C(KC_Z),KC_1   ,KC_2   ,KC_3   ,KC_0   ,                KC_WHOM,KC_WBAK,KC_WREF,KC_WFWD,KC_WSCH,KC_MUTE,
                            KC_EQL ,TD_DOT ,KC_0   ,KC_PENT,KC_RSFT,KC_RCTL,TT(_FN),KC_RALT,
                            KC_CALC,                _______,_______,                KC_CAPS
    )
};

bool is_typing_timer_active = false;
int16_t typing_timer = 0;
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (IS_LAYER_ON(_FN)) {
        if (!is_typing_timer_active) is_typing_timer_active = true;
        typing_timer = timer_read();
    }
    switch (keycode) {
        case QMKBEST:
            if (record->event.pressed) {
                // when keycode QMKBEST is pressed
                tap_code16(C(KC_C));
            } else {
                // when keycode QMKBEST is released
                tap_code16(C(KC_V));
            }
            break;
        case ALT_TAB:
            if (record->event.pressed) {
                if (!is_alt_tab_active) {
                    is_alt_tab_active = true;
                    register_code(KC_LALT);
                }
                alt_tab_timer = timer_read();
                register_code(KC_TAB);
            } else {
                unregister_code(KC_TAB);
            }
            break;
    }
    return true;
}

void matrix_scan_user(void) { // The very important timer.
    writePin(TXLED, IS_LAYER_OFF(_FN));
    // writePin(RXLED, IS_LAYER_OFF(_FN));

    if (is_alt_tab_active) {
        if (timer_elapsed(alt_tab_timer) > 1000) {
            unregister_code(KC_LALT);
            is_alt_tab_active = false;
        }
    }
    
    if (is_typing_timer_active && IS_LAYER_ON(_FN)) {
        if (timer_elapsed(typing_timer) > 30000) {
            layer_off(_FN);
            is_typing_timer_active = false;
        }
    }
}

// Key Overrides
const key_override_t backspace_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_BSPACE, KC_DELETE);
const key_override_t comma_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_COMMA, KC_SCOLON);
const key_override_t dot_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_DOT, KC_COLON);
const key_override_t question_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_QUESTION, KC_EXCLAIM);
const key_override_t pipe_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_PIPE, KC_CIRCUMFLEX);
const key_override_t at_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_AT, KC_GRAVE);
const key_override_t hash_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_HASH, KC_TILDE);
const key_override_t asterisk_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_ASTERISK, KC_AMPERSAND);
const key_override_t percent_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_PERCENT, KC_DOLLAR);
const key_override_t left_angle_bracket_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_LEFT_ANGLE_BRACKET, KC_RIGHT_ANGLE_BRACKET);
const key_override_t left_paren_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_LEFT_PAREN, KC_RIGHT_PAREN);
const key_override_t left_bracket_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_LBRACKET, KC_RBRACKET);
const key_override_t left_curly_bracket_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_LEFT_CURLY_BRACE, KC_RIGHT_CURLY_BRACE);
const key_override_t slash_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_SLASH, KC_BSLASH);

// This globally defines all key overrides to be used
const key_override_t **key_overrides = (const key_override_t *[]){
    &backspace_key_override,
    &comma_key_override,
    &dot_key_override,
    &question_key_override,
    &pipe_key_override,
    &at_key_override,
    &hash_key_override,
    &asterisk_key_override,
    &percent_key_override,
    &left_angle_bracket_key_override,
    &left_paren_key_override,
    &left_bracket_key_override,
    &left_curly_bracket_key_override,
    &slash_key_override,
    NULL // Null terminate the array of overrides!
};

// Declare tap dance functions:
// Function to determine the current tapdance state
uint8_t current_dance(qk_tap_dance_state_t *state);

// `finished` and `reset` functions for each tapdance keycode
#define CPY_PST TD(COPY_PASTE)
void td_copy_paste_finished(qk_tap_dance_state_t *state, void *user_data);
void td_copy_paste_reset(qk_tap_dance_state_t *state, void *user_data);

// Tap Dance Functions Defination
// Determine the tapdance state to return
uint8_t current_dance(qk_tap_dance_state_t *state) {
    switch (state->count) {
        case 1:
            if (!state->interrupted && !state->pressed) return SINGLE_TAP;
            else if (state->pressed) return SINGLE_HOLD;
        case 2:
            if (!state->pressed) return DOUBLE_TAP;
        default:
            return OTHERWISE;
    }
}

void td_copy_paste_finished(qk_tap_dance_state_t *state, void *user_data) {
    td_state = current_dance(state);
    switch (td_state) {
        case SINGLE_TAP:  register_code16(C(KC_V)); break;
        case SINGLE_HOLD: tap_code16(C(KC_C));      break;
        default: return;
    }
}
void td_copy_paste_reset(qk_tap_dance_state_t *state, void *user_data) {
    switch (td_state) {
        case SINGLE_TAP:  unregister_code16(C(KC_V)); break;
        case SINGLE_HOLD: tap_code16(C(KC_V));        break;
        default: return;
    }
}
