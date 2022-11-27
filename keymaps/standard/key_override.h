/* Key Override */
#pragma once

const key_override_t arterisk_override = ko_make_basic(MOD_MASK_SHIFT, KC_ASTERISK, KC_CIRCUMFLEX); // *^
const key_override_t slash_override =    ko_make_basic(MOD_MASK_SHIFT, KC_SLASH, KC_PERCENT);       // /%
const key_override_t left_paren_override = ko_make_basic(MOD_MASK_SHIFT, KC_LEFT_PAREN, KC_RIGHT_PAREN);
const key_override_t left_bracket_override = ko_make_basic(MOD_MASK_SHIFT, KC_LBRACKET, KC_RBRACKET);
const key_override_t left_curly_bracket_override = ko_make_basic(MOD_MASK_SHIFT, KC_LEFT_CURLY_BRACE, KC_RIGHT_CURLY_BRACE);
const key_override_t left_angle_bracket_override = ko_make_basic(MOD_MASK_SHIFT, KC_LEFT_ANGLE_BRACKET, KC_RIGHT_ANGLE_BRACKET);
const key_override_t question_override = ko_make_basic(MOD_MASK_SHIFT, KC_QUESTION, KC_EXCLAIM);  // ?!
const key_override_t comma_override = ko_make_basic(MOD_MASK_SHIFT, KC_COMMA, KC_SCOLON);         // ,;
const key_override_t dot_override = ko_make_basic(MOD_MASK_SHIFT, KC_DOT, KC_COLON);              // .:
const key_override_t delete_override = ko_make_basic(MOD_MASK_SHIFT, KC_BACKSPACE, KC_DELETE);

const key_override_t **key_overrides = (const key_override_t *[]) {
    &arterisk_override,
    &slash_override,
    &left_paren_override,
    &left_bracket_override,
    &left_curly_bracket_override,
    &left_angle_bracket_override,
    &question_override,
    &comma_override,
    &dot_override,
    &delete_override,
    NULL // Null terminate the array of overrides!
};
