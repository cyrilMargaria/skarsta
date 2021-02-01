#include "Display.h"

static int8_t codes_numbers[] = {0x3f, 0x06, 0x5b, 0x4f, 0x66,
                                 0x6d, 0x7d, 0x07, 0x7f, 0x6f};
static char error_msg[5] = {'e', 'r', ' ', '\0', '\0'};

static int8_t get_code_n(uint8_t n) {
    return n > 9 ? 0x00 : codes_numbers[n];
}

static uint8_t get_code_c(char c) {
    if (c >= '0' && c <= '9') {
        return get_code_n((uint8_t) (c - '0'));
    }
    if (c >= 'A' && c <= 'Z') {
        c = static_cast<char>(c + 32);
    }
    switch (c) {
        case '-':
            return B01000000;
        case 's':
            return B01101101;
        case 'e':
            return B01111001;
        case 'r':
            return B01010000;
        case 't':
            return B01111000;
        default:
            return 0x00;
    }
}

Display::Display(uint8_t _pin1, uint8_t _pin2, uint16_t _timeout) :
        display(_pin1, _pin2), timeout(_timeout) {
    display.begin();
    display.setBrightness(brightness);
}

bool Display::begin() {
    display.clear();
    for (uint8_t i = 4; i > 0; i--) {
        display.writeData(i - 1, get_code_n(8));
        delay(200);
    }
    display.clear();
    return true;
}

void Display::set_blink(bool state) {
    if (disabled) return;
    if (blink != state) {
        LOG("d | blink=%s", state?"true":"false");
    }
    dirty = blink != state && !state;
    blink = state;
   
}

void Display::print(unsigned int position) {
    if (disabled) return;
    uint8_t buffer[4] = {
            get_code_n(position / 1000),
            get_code_n((position / 100) % 10),
            get_code_n((position % 100) / 10),
            get_code_n(position % 10)};
    for (uint8_t i = 0u; i < 4; i++) {
        dirty = dirty || disp_buffer[i] != buffer[i];
        disp_buffer[i] = buffer[i];
    }

    if (dirty) {
        LOG("d | print:%d", position);
    }
}

void Display::print(const char *text) {
    if (disabled) return;
    const uint8_t len = strlen(text);
    int8_t buffer[4] = {
            len > 0 ? get_code_c(text[0]) : 0x00,
            len > 1 ? get_code_c(text[1]) : 0x00,
            len > 2 ? get_code_c(text[2]) : 0x00,
            len > 3 ? get_code_c(text[3]) : 0x00};
    for (uint8_t i = 0u; i < 4; i++) {
        dirty = dirty || disp_buffer[i] != buffer[i];
        disp_buffer[i] = buffer[i];
    }

    if (dirty) {
        LOG("d | print:%s", text);
    }
}

void Display::set_brightness(uint8_t b) {
    if (brightness == b)
        return;
    brightness = b;
    LOG("d | brightness:%d", brightness);
    display.setBrightness(b);
}

void Display::light_up() {
    if (!this->blink)
        this->dirty = true;
}
void Display::button_cycle() {
    uint8_t keydata = display.getKeys();
    // at most one key can be detected
    // clear all buttons, set the one pressed
    uint32_t pre_change =0 ;
    for (size_t i=0; i< TM1637_MAX_BUTTON; i++ ) {
        if (display_button_pressed[i]) {
            pre_change |= 1<< i;
        }
        display_button_pressed[i] = false;
    }    
    if (keydata < TM1637_MAX_BUTTON) {
        if ((pre_change & (1<<keydata)) == 0) {
            LOG("B | P:%d", keydata);
        }
        LOG("B | s:%d", keydata);
        display_button_pressed[keydata] = true;
    }
    if ((pre_change != 0) && (0xFF ==keydata)) {
        LOG("B | ~P:%d", pre_change);
    }
}

void Display::cycle() {
  
    if (dirty) {
        this->set_brightness(BRIGHT_HIGH);
        display.writeData(0, disp_buffer, 4);
        dirty = false;
        elapsed = 0;
        LOG("d | redraw");
    } else if (blink && false) {
        if (elapsed >= 100) {
            if (!clear) {
                display.clear();
            } else {
                this->set_brightness(BRIGHT_HIGH);
                display.writeData(0, disp_buffer, 4);
            }
            clear = !clear;
            elapsed = 0;
        }
    } else if (!disabled && brightness != 0 && elapsed >= timeout) {
        this->set_brightness((uint8_t) (8 - ((elapsed - timeout) / 10000)));
        if (brightness != 0)
            display.writeData(0, disp_buffer, 4);
        else
            display.clear();
    }
}

void Display::disable(uint8_t cause) {
    error_msg[3] = (char) ('0' + (cause % 10));
    print(error_msg);
    disabled = true;
}



NoDisplay::NoDisplay(uint8_t _pin1, uint8_t _pin2, uint16_t _timeout) : Display(_pin1,  _pin2, _timeout) {
    
}

bool NoDisplay::begin() {  
    return true;
}

void NoDisplay::set_blink(bool state) {
   
}

void NoDisplay::print(unsigned int position) {
    uint8_t buffer[4] = {
            get_code_n(position / 1000),
            get_code_n((position / 100) % 10),
            get_code_n((position % 100) / 10),
            get_code_n(position % 10)};
    for (uint8_t i = 0u; i < 4; i++) {
        dirty = dirty || disp_buffer[i] != buffer[i];
        disp_buffer[i] = buffer[i];
    }

    if (dirty) {
        LOG("d | print:%d", position);
    }
}

void NoDisplay::print(const char *text) {
    const uint8_t len = strlen(text);
    uint8_t buffer[4] = {
            len > 0 ? get_code_c(text[0]) : 0x00,
            len > 1 ? get_code_c(text[1]) : 0x00,
            len > 2 ? get_code_c(text[2]) : 0x00,
            len > 3 ? get_code_c(text[3]) : 0x00};
    for (uint8_t i = 0u; i < 4; i++) {
        dirty = dirty || disp_buffer[i] != buffer[i];
        disp_buffer[i] = buffer[i];
    }

    if (dirty) {
        LOG("d | print:%s", text);
    }
}

void NoDisplay::set_brightness(uint8_t b) {
 
    LOG("d | brightness:%d", b);
}

void NoDisplay::light_up() {
    if (!this->blink)
        this->dirty = true;
}

void NoDisplay::cycle() {
    if (dirty) {       
        dirty = false;
        elapsed = 0;
        LOG("d | redraw");
    } else if (blink) {
        if (elapsed >= 100) {
            if (!clear) {
            }
            clear = !clear;
            elapsed = 0;
        }
    } 
}

void NoDisplay::disable(uint8_t cause) {
    error_msg[3] = (char) ('0' + (cause % 10));
}
