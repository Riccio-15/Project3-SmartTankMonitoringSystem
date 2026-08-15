#include "ServoMotorImpl.h"
#include <Arduino.h>

ServoMotorImpl::ServoMotorImpl(int pin) {
    this->pin = pin;
    _on = false;
}

void ServoMotorImpl::on() {
    // updated values: min is 544, max 2400 (see ServoTimer2 doc)
    motor.attach(pin); //, 544, 2400);
    _on = true;
}

bool ServoMotorImpl::isOn() { return _on; }

constexpr int MAX_ANGLE = 180;
// 750 -> 0, 2250 -> 180
// 750 + angle*(2250-750)/180
// updated values: min is 544, max 2400 (see ServoTimer2 doc)
// adapted values to MicroServo SG90
constexpr int min = 500;
constexpr int max = 2400;
constexpr float coeff = (max - min) / (float)MAX_ANGLE;

void ServoMotorImpl::setPosition(int angle) {
    if (angle > MAX_ANGLE) {
        angle = MAX_ANGLE;
    } else if (angle < 0) {
        angle = 0;
    }
    motor.write(min + angle * coeff);
}

int ServoMotorImpl::getPosition() {
    return (motor.read() - min) / coeff;
}

void ServoMotorImpl::off() {
    _on = false;
    motor.detach();
}