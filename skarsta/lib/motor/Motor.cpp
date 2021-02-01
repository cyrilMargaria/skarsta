#include <Motor.h>
#ifdef __EEPROM__
#include <EEPROM.h>
#endif

static Motor *motor = nullptr;

static uint32_t position_abs(uint32_t a, uint32_t b) {
    return a >= b ? a - b : b - a;
}

Motor::Motor(uint8_t _pin1, uint8_t _pin2, uint8_t stop_diff, uint8_t min_change, bool reverse, bool encoder_inline, uint8_t encoder_turn_count) :
        sensor((char) _pin1, (char) _pin2),
        sensor_pin_1(_pin1), sensor_pin_2(_pin2),
        pos_diff(stop_diff), min_change(min_change), reverse(reverse), encoder_inline(encoder_inline), encoder_turn_count(encoder_turn_count)  {
    motor = this;
    
}

bool Motor::begin() {
    LOG("m | begin()");

#ifdef __EEPROM__
    if (false) {
        EEPROM.get(ADDRESS_POSITION, position);    
        EEPROM.get(ADDRESS_END_STOP_0, end_stop[0]);
        EEPROM.get(ADDRESS_END_STOP_1, end_stop[1]);
        EEPROM.get(ADDRESS_MODE, mode);
    }
#endif
 
    LOG("m | pos:%d end_pos:%d-%d mode:%d", position, end_stop[0], end_stop[1], mode);
    LOG("m | em:%d m:%d c=%d", pos_diff, min_change, encoder_turn_count);

    this->position = position;
#ifdef __USENSOR__
    double sensor_value = sensor.measureDistanceCm();
    for (uint8_t i = 0; i < 5 && sensor_value < 0; i++) {
        delay(250);
        sensor_value = sensor.measureDistanceCm();
    }
    return sensor_value >= 0;
#else    
    // Encoder: the encoder lib is managing the interupts
    sensor.write(encoder_inline ? -position : position );    
    return true;
#endif
}

void Motor::off() {
    _off();
    LOG("m | off %d,%d", end_stop[0], end_stop[1]);
    state = OFF;
}

void Motor::dir_cw() {
    // CW: going up       
    if (disabled || ((CALIBRATED == mode ) && (position >= (end_stop[1]-pos_diff)))) {
        return;
    }
    LOG("m | cw");
    _dir_cw();
    state = CW;
}

void Motor::dir_ccw() {
    // CCW: going down 
    // Checks valid if we are in calibrated or sem-calibrated mode   
    if (disabled || ((UNCALIBRATED != mode ) && (position <= (end_stop[0]+pos_diff)))) {
        return;
    }
    LOG("m | ccw p=%d, end=%d df=%d", position, end_stop[0], pos_diff);
    _dir_ccw();
    state = CCW;
}
// unsigned position, if the encoder is near the down stop, it won't be accurate, should
// not be used for limit switch
uint32_t Motor::get_position() const {
    return position;
}

void Motor::reset_position() {
    if (disabled) return;
#ifndef __USENSOR__
    this->position = 0u;
    sensor.write(position);
#endif
    this->end_stop[0] = get_position();

#ifdef __EEPROM__
    updateEEPROM(ADDRESS_END_STOP_0, this->end_stop[0]);
#ifndef __USENSOR__
    updateEEPROM(ADDRESS_POSITION, position);
#endif
#endif
    LOG("m | rst, min_pos=%d", this->end_stop[0]);
}

void Motor::set_position(uint32_t pos) {
    if (mode != CALIBRATED || position_abs(pos, get_position()) < min_change) {
        return;
    }
    next_position = pos;
    if (next_position < get_position()) {
        this->dir_ccw();
    } else {
        this->dir_cw();
    }
    LOG("m | pos:%d", pos);
}

MotorState Motor::get_state() const {
    return this->state;
}

MotorMode Motor::get_mode() const {
    return this->mode;
}

void Motor::set_mode(MotorMode mode) {
    if (disabled) return;
    this->mode = mode;
#ifdef __EEPROM__
    updateEEPROM(ADDRESS_MODE, this->mode);
#endif
    LOG("m | mode:%d", mode);
}

void Motor::set_end_stop(unsigned int end_stop, unsigned int offset) {
    if (disabled) return;
    if (offset)
        this->position -= offset;
    this->end_stop[1] = end_stop - 2 * offset;
#ifdef __EEPROM__
    updateEEPROM(ADDRESS_END_STOP_1, this->end_stop[1]);
#endif
    LOG("m | end_pos:%d (position: %d)", end_stop, this->end_stop[1] );
}

void Motor::initPin(uint8_t pin, uint8_t val) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, val);
}


bool Motor::check_end_stops(const unsigned int end_stop_down,
                            const unsigned int end_stop_up) const {
    return (state == CCW && (position <= (end_stop_down + pos_diff))) ||
        (state == CW && (position >= (end_stop_up - pos_diff)));
}

void Motor::cycle() {
#ifdef __USENSOR__
    double measurement_raw;

    if ((elapsed > 125 || position == 0) &&
        (measurement_raw = sensor.measureDistanceCm()) >= 0) {
        const unsigned int cur_position = (unsigned int) measurement_raw * 10,
                position_diff = position_abs(position, cur_position);
        
        if ((get_state() == OFF && position_diff > 75) ||
            (get_state() != OFF && position_diff > 10)) {
            position_change += position_diff;
            position = cur_position;
        }

        elapsed = 0;
    }
#else
    int32_t measurement_raw = 0;
    if ((elapsed > 60 || position == 0) &&
        (measurement_raw = sensor.read())) {
        // 60 per turn , 45 mm for 10 turn
        // 0.075 mm per position
        // cur_position in
        uint32_t cur_position = encoder_inline ? -measurement_raw : measurement_raw;
        uint32_t position_diff = position_abs(position, cur_position);
        
        //LOG("m | s:%d r:%ld p:%ld np:%ld d:%ld %ld um", state, measurement_raw, position, cur_position, position_diff, position*75);

        if ((state == OFF && position_diff > (encoder_turn_count/2)) ||
            (state != OFF && position_diff > 5)) {
            position_change += position_diff;
            position = cur_position;            
        }
        elapsed = 0;
    }
    
#endif
   
    if (mode == UNCALIBRATED) {
        return;
    }

#if defined(__EEPROM__) && !defined(__USENSOR__)    
    updateEEPROM(ADDRESS_POSITION, position);
#endif
    // slow zone 
    if ( check_end_stops(end_stop[0]+encoder_turn_count*3, end_stop[1]-encoder_turn_count*3) ||
         (next_position >= 0 && check_end_stops((unsigned int) (next_position+encoder_turn_count),
                                               (unsigned int) (next_position-encoder_turn_count)))
         ) {
        speed_slow = true;
    } else {
        speed_slow = false;
    }
    
    if (check_end_stops(end_stop[0], end_stop[1]) ||
        (next_position >= 0 && check_end_stops((unsigned int) (next_position),
                                               (unsigned int) (next_position)))) {
        off();
        LOG("m | (L) s:%d p:%d np:%d", state, position, next_position);
        next_position = -1;
    }
};

void Motor::disable() {
    disabled = true;
    off();
}

uint32_t Motor::get_position_change() {
    uint32_t cur_change = position_change;
    position_change = 0;
    return cur_change;
}
