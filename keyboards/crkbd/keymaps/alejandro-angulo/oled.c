#include "oled_driver.h"

#ifndef BLINK_TIMEOUT
#   define BLINK_TIMEOUT 3000
#endif

#define L_BASE 0
#define L_LOWER 2
#define L_RAISE 4
#define L_ADJUST 8

uint16_t blink_timeout;

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    return OLED_ROTATION_270;
}

void render_bootmagic_status(bool status) {
    /* Show Ctrl-Gui Swap options */
    static const char PROGMEM logo[][2][3] = {
        {{0x97, 0x98, 0}, {0xb7, 0xb8, 0}},
        {{0x95, 0x96, 0}, {0xb5, 0xb6, 0}},
    };
    if (status) {
        oled_write_ln_P(logo[0][0], false);
        oled_write_ln_P(logo[0][1], false);
    } else {
        oled_write_ln_P(logo[1][0], false);
        oled_write_ln_P(logo[1][1], false);
    }
}

void render_prompt(void) {
    uint16_t time = timer_read();
    bool blink = (time % 2000) < 500;

#if BLINK_TIMEOUT > 0
    // Stop blinking to allow the oled to turn off
    if (timer_expired(timer_read(), blink_timeout)) {
        blink = false;
    }
#endif

    switch (layer_state) {
        case L_LOWER:
            oled_write_ln_P(blink ? PSTR("> lo_") : PSTR("> lo "), false);
            break;
        case L_RAISE:
            oled_write_ln_P(blink ? PSTR("> hi_") : PSTR("> hi "), false);
            break;
        case L_ADJUST:
        case L_ADJUST|L_LOWER:
        case L_ADJUST|L_RAISE:
        case L_ADJUST|L_LOWER|L_RAISE:
            oled_write_ln_P(blink ? PSTR("> aj_") : PSTR("> aj "), false);
            break;
        default:
            oled_write_ln_P(blink ? PSTR("> _  ") : PSTR(">    "), false);
    }
};

void render_corne_logo(void) {
    static const char PROGMEM font_logo[16] = {0x80, 0x81, 0x82, 0x83, 0x84, 0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0};
    oled_write_P(font_logo, false);
};

void render_qmk_logo(void) {
    static const char PROGMEM font_qmk_logo[16] = {0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0};
    oled_write_P(font_qmk_logo, false);
};

void render_kb_split(void) {
    static const char PROGMEM font_kb_split[11] = {0xba, 0xbb, 0xbc, 0xbd, 0xbe, 0xda, 0xdb, 0xdc, 0xdd, 0xde, 0};
    oled_write_P(font_kb_split, false);
};

void render_layer(void) {
    static const char PROGMEM font_layer[4][6] = {
        {0x85, 0x86, 0x87, 0x88, 0x89, 0},
        {0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0},
        {0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0},
        {0x9a, 0x9b, 0x9c, 0x9d, 0x9e, 0},
    };
    uint8_t layer = 0;

    switch (layer_state) {
        case L_LOWER:
            layer = 1;
            break;
        case L_RAISE:
            layer = 2;
            break;
        case L_ADJUST:
        case L_ADJUST|L_LOWER:
        case L_ADJUST|L_RAISE:
        case L_ADJUST|L_LOWER|L_RAISE:
            layer = 3;
            break;
    }

    oled_write_P(font_layer[layer], false);
}

void render_mod_ctrl(void) {
    static const char PROGMEM font_ctrl[3] = {0x93, 0x94, 0};
    oled_write_P(font_ctrl, false);
};

void render_mod_alt(void) {
    static const char PROGMEM font_alt[3] = {0xb3, 0xb4, 0};
    oled_write_P(font_alt, false);
};

void render_mod_shift(void) {
    static const char PROGMEM font_shift[3] = {0xd3, 0xd4, 0};
    oled_write_P(font_shift, false);
};

void render_mod_gui(void) {
    static const char PROGMEM font_gui[3] = {0x95, 0x96, 0};
    oled_write_P(font_gui, false);
};

void render_mod_status(void) {
    uint8_t modifiers = get_mods();

#ifdef NO_ACTION_ONESHOT
    modifiers |= get_oneshot_mods();
#endif

    (modifiers & MOD_MASK_CTRL) ? render_mod_ctrl() : oled_write_P(PSTR("  "), false);
    oled_write_P(PSTR(" "), false);
    (modifiers & MOD_MASK_SHIFT) ? render_mod_shift() : oled_write_P(PSTR("  "), false);

    (modifiers & MOD_MASK_ALT) ? render_mod_alt() : oled_write_P(PSTR("  "), false);
    oled_write_P(PSTR(" "), false);
    (modifiers & MOD_MASK_GUI) ? render_mod_gui() : oled_write_P(PSTR("  "), false);
}

void oled_task_user(void) {
    if (is_master) {
        oled_write_ln("", false);
        oled_write_ln("", false);

        render_corne_logo();

        oled_write_ln("", false);
        oled_write_ln("", false);
        oled_write_ln("", false);

        render_qmk_logo();

        oled_write_ln("", false);
        oled_write_ln("", false);
        oled_write_ln("", false);

        render_prompt();
    } else {
        oled_write_ln("", false);
        oled_write_ln("", false);

        render_kb_split();

        oled_write_ln("", false);
        oled_write_ln("", false);
        oled_write_ln("", false);

        render_layer();

        oled_write_ln("", false);
        oled_write_ln("", false);
        oled_write_ln("", false);

        render_mod_status();
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
#if BLINK_TIMEOUT > 0
    if (record->event.pressed) {
      blink_timeout = timer_read() + BLINK_TIMEOUT;
    }
#endif

    return true;
}
