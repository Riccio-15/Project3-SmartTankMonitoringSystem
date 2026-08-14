#include "tasks/LCDTask.h"
#include <Arduino.h>
#include "model/Context.h"

LCDTask::LCDTask(LiquidCrystal_I2C* lcd) : lcd(lcd) {

}

void LCDTask::tick() {
    String state = "";
    String opening = "OPENING: " + String(Context::getValveTargetOpen()) + "%";
    if (!Context::isNetworkOk()) state = "UNCONNECTED";
    else {
        if (Context::isAutoMode()) state = "AUTOMATIC";
        else if(Context::isManualMode()) state = "MANUAL";
    }

    if (state == lastStateDisplayed && opening == lastOpeningDisplayed) {
        return; 
    } else {
        lcd->clear();
    }

    lcd->setCursor(0, 0);
    lcd->print(opening);
    lcd->setCursor(0, 1);
    lcd->print(state);
    lastStateDisplayed = state;
    lastOpeningDisplayed = opening;
}
