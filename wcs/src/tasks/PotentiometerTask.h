#ifndef __POTENTIOMETER_TASK__
#define __POTENTIOMETER_TASK__

#include "kernel/Task.h"
#include "devices/Potentiometer.h"

class PotentiometerTask : public Task
{

public:
    PotentiometerTask(Potentiometer *potentiometer);

    void tick();

private:
    Potentiometer *potentiometer;
    int lastValue;
};

#endif