#ifndef __POTENTIOMETERIMPL__
#define __POTENTIOMETERIMPL__

#include "Potentiometer.h"

class PotentiometerImpl : public Potentiometer
{

public:
    PotentiometerImpl(int pin);
    int readPercent();

private:
    int pin;
};

#endif