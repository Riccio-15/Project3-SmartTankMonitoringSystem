#ifndef __TASK__
#define __TASK__

#include <Arduino.h>

class Task {

public:
    virtual void tick() = 0;

    // avvia il task come thread FreeRTOS che chiama tick() ogni periodMs
    void start(const char* name, uint32_t periodMs, uint32_t stackSize = 2048, UBaseType_t priority = 1);

private:
    static void run(void* param);

private:
    uint32_t periodMs;
};

#endif