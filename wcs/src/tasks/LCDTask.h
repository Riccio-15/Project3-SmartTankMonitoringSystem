#ifndef __LCD_TASK__
#define __LCD_TASK__

#include "LiquidCrystal_I2C.h"
#include "kernel/Task.h"

class LCDTask: public Task {

public:
    LCDTask(LiquidCrystal_I2C* lcd);

    void tick();

private:
    LiquidCrystal_I2C* lcd;
    String lastStateDisplayed;
    String lastOpeningDisplayed;
};


#endif