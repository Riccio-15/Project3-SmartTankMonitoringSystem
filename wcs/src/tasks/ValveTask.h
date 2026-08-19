#ifndef __VALVE_TASK__
#define __VALVE_TASK__

#include "devices/ServoMotor.h"
#include "kernel/Task.h"

class ValveTask : public Task
{
public:
    ValveTask(ServoMotor* motor);

    void tick();

private:
    ServoMotor* motor;
    enum ValveState { IDLE, MOVING } state;
    float openingAngle;
};

#endif
