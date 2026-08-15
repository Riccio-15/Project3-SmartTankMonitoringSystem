#ifndef __SERIAL_COMM_TASK__
#define __SERIAL_COMM_TASK__

#include "kernel/Task.h"

class SerialCommTask : public Task
{

public:
    SerialCommTask();

    void tick();

private:
    void sendCurrentState();
};

#endif