#include "MotorBridge.h"

bool MotorBridge::begin() {
    if (!Motor::begin())
        return false;

    initPin(r_enable);
    initPin(l_enable);
    initPin(r_pwm);
    initPin(l_pwm);
    _off();

    return true;
}

void MotorBridge::_off() {
    digitalWrite(r_enable, LOW);
    digitalWrite(l_enable, LOW);
    this->speed = 0;
    analogWrite(r_pwm, 0);
    analogWrite(l_pwm, 0);
}

void MotorBridge::enable() {
    digitalWrite(r_enable, HIGH);
    digitalWrite(l_enable, HIGH);
}

void MotorBridge::_dir_cw() {
    setSpeed(CW, MIN_SPEED);
    this->enable();
}

void MotorBridge::_dir_ccw() {
    setSpeed(CCW, MIN_SPEED);
    this->enable();
}

void MotorBridge::setSpeed(MotorState state, uint8_t speed) {
    if (state == OFF) {
        return;
    }
    this->speed = speed;
    LOG("m | speed:%d", speed);

    const bool dir = state == (reverse ? CW : CCW);
    analogWrite(l_pwm, dir ? 0 : speed);
    analogWrite(r_pwm, dir ? speed : 0);
}

void MotorBridge::cycle() {
    if (accel_elapsed >= SPEED_STEP_DURATION && speed <= MAX_SPEED && speed >= MIN_SPEED) {
        // decelerate at 3 crank turn
        uint8_t new_speed =  min(speed + 5 % (MAX_SPEED + 1), MAX_SPEED);
        if (speed_slow) {
            new_speed =  max((speed - 10 % (MAX_SPEED + 1)), MIN_SPEED);
            LOG("m | slow");                        
        }
        setSpeed(get_state(), new_speed);
        accel_elapsed = 0;
    }    
    Motor::cycle();
}
