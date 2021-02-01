#include "NIButtons.h"

NIButton::NIButton(int gpio, uint16_t db) : _gpio(gpio), _button(gpio), debounce(db) {
    this->_button.debounce(debounce);
    this->_button.holdTime(500);
    this->_button.begin();
}

bool NIButton::begin() {
    this->_button.begin();
    return true;
}

NIButton &NIButton::on_short_press(void (*_on_pressed)()) {
    this->_on_short_press = _on_pressed;
    return *this;
}

NIButton &NIButton::long_press(long delay) {
    _button.holdTime(delay);
    return *this;
}

NIButton &NIButton::llong_press(long delay) {
    _button.longHoldTime(delay);
    return *this;
}

NIButton &NIButton::on_long_press(void (*_on_pressed)()) {
    this->_on_long_press = _on_pressed;
    return *this;
}

NIButton &NIButton::on_llong_press(void (*_on_pressed)()) {
    this->_on_llong_press = _on_pressed;
    return *this;
}

NIButton &NIButton::on_press(void (*_on_press)()) {
    this->_on_press = _on_press;
    return *this;
}

NIButton &NIButton::on_release(void (*_on_release)()) {
    this->_on_release = _on_release;
    return *this;
}

bool NIButton::pressed() {
    return _pressed;
}

bool NIButton::held() {
    return _held;
}

bool NIButton::held_long() {
    return _held_long;
}

void NIButton::cycle() {
    _button.checkSwitch();

    if (!_pressed && _button.pressed()) {
        elapsed = 0;
        _pressed = true;
        if (_on_press) {
            this->_on_press();
        }
        LOG("b | on:%d", _gpio);
    } else if (digitalRead((uint8_t) _gpio) && _pressed && elapsed > debounce) {
        if (_on_release) {
            this->_on_release();
        }
        LOG("b | off:%d", _gpio);
        _pressed = false;
        _held_long = false;
        _held = false;
        elapsed = 0;
    }

    if (_on_short_press && _button.clicked()) {
        this->_on_short_press();
        LOG("b | short:%d", _gpio);
    }
    if (_button.held()) {
        if (_on_long_press) {
            this->_on_long_press();
        }
        LOG("b | long:%d", _gpio);
        _held = true;
    }
    if (_button.heldLong()) {
        if (_on_llong_press) {
            this->_on_llong_press();
        }
        LOG("b | llong:%d", _gpio);
        _held_long = true;
    }
}





TMButton::TMButton(uint32_t id) : _id(id) {}

bool TMButton::begin() {
    _downTime = 0;
    _upTime = 0;    
    return (_id < 16);
}

TMButton &TMButton::on_short_press(void (*_on_pressed)()) {
    this->_on_short_press = _on_pressed;
    return *this;
}

TMButton &TMButton::long_press(long delay) {
    _holdTime = delay;
    return *this;
}

TMButton &TMButton::llong_press(long delay) {
    _longHoldTime = delay;
    return *this;
}

TMButton &TMButton::on_long_press(void (*_on_pressed)()) {
    this->_on_long_press = _on_pressed;
    return *this;
}

TMButton &TMButton::on_llong_press(void (*_on_pressed)()) {
    this->_on_llong_press = _on_pressed;
    return *this;
}

TMButton &TMButton::on_press(void (*_on_press)()) {
    this->_on_press = _on_press;
    return *this;
}

TMButton &TMButton::on_release(void (*_on_release)()) {
    this->_on_release = _on_release;
    return *this;
}

bool TMButton::pressed() {
    return _notify_pressed;
}

bool TMButton::held() {
    return _notify_held;
}

bool TMButton::held_long() {
    return _notify_held_long;
}

void TMButton::cycle() {

    if (!_notify_pressed && _pressed) {
        elapsed = 0;
        _notify_pressed = true;
        _notify_clicked = false;
        if (_on_press) {
            this->_on_press();
        }
        LOG("b | on:%d", _id);
    } else if (_notify_pressed && !_pressed) {
        if (_on_release) {
            this->_on_release();
        }
        LOG("b | off:%d", _id);
        _notify_held_long = false;
        _notify_held = false;
        _notify_pressed = false;
        elapsed = 0;
    }
    if (_clicked && ! _notify_clicked) {        
        _notify_clicked = true;
        if (_on_short_press) {
            this->_on_short_press();
        }
        LOG("b | click:%d", _id);
        elapsed = 0;

    }
   
    if (! _notify_held && _held) {
        if (_on_long_press) {
            this->_on_long_press();
        }
        LOG("b | long:%d", _id);
        _notify_held = true;
    }
    if (!_notify_held_long && _held_long) {
        if (_on_llong_press) {
            this->_on_llong_press();
        }
        LOG("b | llong:%d", _id);
        _notify_held_long = true;
    }
}

void TMButton::press() {
    // button pressed down
    _held = false;
    _held_long = false;
    if (!_pressed && ((millis() - _upTime) > _debounce)) {
        _pressed = true;
        _clicked = false;
        _downTime = millis();
        _holdEventPast = false;
        _longHoldEventPast = false;
    }
    if (_pressed &&  (millis() - _downTime) >= _holdTime) {
        if (not _holdEventPast) {
            _held = true;           
            _holdEventPast = true;
        }
        // Trigger "long" hold
        if ((millis() - _downTime) >= _longHoldTime) {
            if (not _longHoldEventPast)
                {
                    _held_long = true;
                    _longHoldEventPast = true;
                }
        }        
    }
}
void TMButton::unpress() {
    // released,
    if (_pressed && ((millis() - _downTime) > _debounce)) {        
        _pressed = false;
        _upTime = millis();             
    }
    
    if (!_clicked && ((millis() - _upTime) >= _dcGap)) {
        _clicked = true;
    }
}
