#include "HWPlatform.h"
#include "config.h"
#include "devices/Led.h"
#include "devices/Sonar.h"

HWPlatform::HWPlatform() {
    pProximitySensor = new Sonar(SONAR_ECHO_PIN, SONAR_TRIG_PIN, SONAR_MAX_ECHO_TIME_US);
    pGreenLight = new Led(LED_GREEN_PIN);
    pRedLight = new Led(LED_RED_PIN);
}

ProximitySensor *HWPlatform::getProximitySensor() { return this->pProximitySensor; }

Light *HWPlatform::getGreenLight() { return this->pGreenLight; }

Light *HWPlatform::getRedLight() { return this->pRedLight; }
