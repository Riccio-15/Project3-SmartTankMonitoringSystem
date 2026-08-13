#include "PotentiometerImpl.h"
#include <Arduino.h>

PotentiometerImpl::PotentiometerImpl(int pin) : pin(pin)
{
    pinMode(pin, INPUT);
}

int PotentiometerImpl::readPercent()
{
    int raw = analogRead(pin);

    long percent = map(raw, 0, 1023, 0, 100);
    if (percent < 0)
        percent = 0;
    if (percent > 100)
        percent = 100;

    return (int)percent;
}