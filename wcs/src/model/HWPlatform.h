#ifndef __HW_PLATFORM__
#define __HW_PLATFORM__

#include "config.h"
#include "devices/Button.h"
#include "devices/Potentiometer.h"
#include "devices/ServoMotor.h"
#include "LiquidCrystal_I2C.h"

class HWPlatform {

public:
    HWPlatform();

    Button* getButton();
    Potentiometer* getPotentiometer();
    ServoMotor* getValveMotor();
    LiquidCrystal_I2C* getLCD();

private:
    Button* pButton;
    Potentiometer* pPotentiometer;
    ServoMotor* pValveMotor;
    LiquidCrystal_I2C* pLcd;
};

#endif