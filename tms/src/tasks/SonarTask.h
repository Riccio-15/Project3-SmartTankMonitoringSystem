#ifndef __SONAR_TASK__
#define __SONAR_TASK__

#include "devices/ProximitySensor.h"
#include "kernel/Task.h"

class SonarTask : public Task {

  public:
    SonarTask(ProximitySensor *sonar);

    void tick();

  private:
    ProximitySensor *sonar;
};

#endif