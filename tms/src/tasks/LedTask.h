#ifndef __LED_TASK__
#define __LED_TASK__

#include "devices/Light.h"
#include "kernel/Task.h"

class LedTask : public Task {

  public:
    LedTask(Light *greenLed, Light *redLed);

    void tick();

  private:
    Light *greenLed;
    Light *redLed;
};

#endif