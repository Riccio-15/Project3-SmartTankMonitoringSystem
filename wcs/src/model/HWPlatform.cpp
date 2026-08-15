#include "HWPlatform.h"
#include "LiquidCrystal_I2C.h"
#include "config.h"
#include "devices/ButtonImpl.h"
#include "devices/PotentiometerImpl.h"
#include "devices/ServoMotorImpl.h"
#include <Arduino.h>

HWPlatform::HWPlatform() {
	pButton = new ButtonImpl(BT_PIN);
	pPotentiometer = new PotentiometerImpl(POT_PIN);
	pValveMotor = new ServoMotorImpl(SERVO_PIN);

	pLcd = new LiquidCrystal_I2C(0x27, 16, 2);
	pLcd->init();
	pLcd->backlight();
}

Button *HWPlatform::getButton() { 
	return this->pButton;
}

Potentiometer *HWPlatform::getPotentiometer() {
	return this->pPotentiometer;
}

ServoMotor *HWPlatform::getValveMotor() {
	return this->pValveMotor;
}

LiquidCrystal_I2C *HWPlatform::getLCD() { 
	return this->pLcd;
}