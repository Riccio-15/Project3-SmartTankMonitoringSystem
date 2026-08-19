#ifndef __HW_PLATFORM__
#define __HW_PLATFORM__

#include "config.h"
#include "devices/ProximitySensor.h"
#include "devices/Light.h"

class HWPlatform {

public:
    HWPlatform();

    ProximitySensor* getProximitySensor();
    Light* getGreenLight();
    Light* getRedLight();

private:
    ProximitySensor* pProximitySensor;
    Light* pGreenLight;
    Light* pRedLight;
};

#endif
