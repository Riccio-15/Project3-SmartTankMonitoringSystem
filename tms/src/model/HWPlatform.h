#ifndef __HW_PLATFORM__
#define __HW_PLATFORM__

#include "config.h"
#include "devices/ProximitySensor.h"
#include "devices/Light.h"

class HWPlatform {

public:
    HWPlatform();

    ProximitySensor* getSonar();
    Light* getGreenLed();
    Light* getRedLed();

private:
    ProximitySensor* pSonar;
    Light* pGreenLed;
    Light* pRedLed;
};

#endif