#pragma once

#include <Arduino.h>
#include <Service.h>
#include <ErriezTM1637.h>
#define TM1637_MAX_BUTTON 16
#define BRIGHT_HIGH 8
class Display : public TimedService {
protected:
    TM1637 display;
    uint8_t brightness = BRIGHT_HIGH;
    uint8_t disp_buffer[4] = {0x00, 0x00, 0x00, 0x00};
    // proxy TM1637 buttons
    bool display_button_pressed[TM1637_MAX_BUTTON];
    bool dirty = false, blink = false, clear = false, disabled = false;

    const uint16_t timeout;
protected:
    virtual void set_brightness(uint8_t b);

public:
    Display(uint8_t _pin1, uint8_t _pin2, uint16_t _timeout);

    bool begin() override;

    virtual void set_blink(bool state);

    virtual void light_up();

    virtual void print(unsigned int position);

    virtual void print(const char *text);

    void cycle() override;
    void button_cycle();

    void disable(uint8_t cause) override;

    bool is_button_pressed(uint8_t i) { return (i < TM1637_MAX_BUTTON) ?  display_button_pressed[i]: false;}
   
};

class NoDisplay: public Display {
 protected:
    void set_brightness(uint8_t b) override;

 public:
    NoDisplay(uint8_t _pin1, uint8_t _pin2, uint16_t _timeout);
    bool begin() override;

    void set_blink(bool state) override;

    void light_up() override;

    void print(unsigned int position) override;

    void print(const char *text) override;

    void cycle() override;

    void button_cycle();

    void disable(uint8_t cause) override;
};
