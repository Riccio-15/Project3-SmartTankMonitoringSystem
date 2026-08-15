#include "tasks/LCDTask.h"
#include "model/Context.h"
#include "config.h"
#include <Arduino.h>

LCDTask::LCDTask(LiquidCrystal_I2C *lcd) : lcd(lcd) {}

void LCDTask::tick() {
	String state = "";
	String opening = "OPENING: " + String(Context::getValveTargetOpen()) + "%";
	if (!Context::isNetworkOk())
		state = "UNCONNECTED";
	else {
		if (Context::isAutoMode())
			state = "AUTOMATIC";
		else if (Context::isManualMode())
			state = "MANUAL";
	}

	while (opening.length() < LCD_COLS)
		opening += ' ';
	while (state.length() < LCD_COLS)
		state += ' ';

	if (opening != lastOpeningDisplayed) {
		lcd->setCursor(0, 0);
		lcd->print(opening);
		lastOpeningDisplayed = opening;
	}

	if (state != lastStateDisplayed) {
		lcd->setCursor(0, 1);
		lcd->print(state);
		lastStateDisplayed = state;
	}
}
