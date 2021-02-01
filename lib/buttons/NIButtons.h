#ifndef ARDUINO_PROJECTS_NIBUTTONS_H
#define ARDUINO_PROJECTS_NIBUTTONS_H

#include <vector>
#include <PMButton.h>
#include <Service.h>

class NIButton : public TimedService {
private:
    const int _gpio;
    PMButton _button;
    const uint16_t debounce;
    bool _pressed = false, _held = false, _held_long = false;

    void (*_on_short_press)() = nullptr;

    void (*_on_long_press)() = nullptr;

    void (*_on_llong_press)() = nullptr;

    void (*_on_press)() = nullptr;

    void (*_on_release)() = nullptr;

public:
    explicit NIButton(int gpio, uint16_t debounce = 20);

    bool begin() override;

    NIButton &on_short_press(void (*_on_pressed)());

    NIButton &long_press(long delay);

    NIButton &llong_press(long delay);

    NIButton &on_llong_press(void (*_on_pressed)());

    NIButton &on_long_press(void (*_on_pressed)());

    NIButton &on_press(void (*_on_press)());

    NIButton &on_release(void (*_on_release)());

    bool pressed();

    bool held();

    bool held_long();

    void cycle() override;
};

// 
class TMButton : public TimedService {
private:
    uint32_t _id;
    bool _pressed = false, _held = false, _held_long = false , _clicked = false;
    // the notifications are done in cycle(), indepent of the state
    bool _notify_pressed = false, _notify_held = false, _notify_held_long = false, _notify_clicked = false; 

    unsigned long _debounce = 10; // ms debounce period to prevent flickering when pressing or releasing the button
    unsigned long _dcGap = 200; // max ms between clicks for a double click event
    unsigned long _holdTime = 500; // ms hold period: how long to wait for press+hold event
    unsigned long _longHoldTime = 5000; // ms hold period: how long to wait for the long press+hold event
    // states
    bool  _holdEventPast, _longHoldEventPast ;
    //Timing variables 
    unsigned long _downTime, _upTime;
    
    void (*_on_short_press)() = nullptr;

    void (*_on_long_press)() = nullptr;

    void (*_on_llong_press)() = nullptr;

    void (*_on_press)() = nullptr;

    void (*_on_release)() = nullptr;

public:
    explicit TMButton(uint32_t id);
    bool begin();

    TMButton &on_short_press(void (*_on_pressed)());

    TMButton &long_press(long delay);

    TMButton &llong_press(long delay);

    TMButton &on_llong_press(void (*_on_pressed)());

    TMButton &on_long_press(void (*_on_pressed)());

    TMButton &on_press(void (*_on_press)());

    TMButton &on_release(void (*_on_release)());

    bool pressed();

    bool held();

    bool held_long();
    
    void cycle() override;

    // external cycle() will call the press() and unpress()
    void press();
    void unpress();
    uint32_t id() {return _id;}
};

#endif //ARDUINO_PROJECTS_NIBUTTONS_H
