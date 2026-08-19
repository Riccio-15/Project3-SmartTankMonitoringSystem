#include "HWPlatform.h"
#include "config.h"
#include "devices/Led.h"
#include "devices/Sonar.h"

HWPlatform::HWPlatform() {
    pSonar = new Sonar(SONAR_ECHO_PIN, SONAR_TRIG_PIN, SONAR_MAX_ECHO_TIME_US);
    pGreenLed = new Led(LED_GREEN_PIN);
    pRedLed = new Led(LED_RED_PIN);
}

ProximitySensor *HWPlatform::getSonar() { return this->pSonar; }

Light *HWPlatform::getGreenLed() { return this->pGreenLed; }

Light *HWPlatform::getRedLed() { return this->pRedLed; }