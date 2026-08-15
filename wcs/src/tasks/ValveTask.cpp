#include "ValveTask.h"
#include "model/Context.h"
#include <Arduino.h>
#include <stdlib.h>

constexpr float ANGLE_TO_PERCENTAGE = 100.0f / 90.0f;
constexpr int COMPLETE_MOVEMENT_TIME = 2000;
constexpr int MAX_ANGLE_MOTOR = 90;

ValveTask::ValveTask(ServoMotor* motor)
    : motor(motor), startMovementTs(0), openingAngle(0)
{
    motor->on();
}

void ValveTask::tick()
{
    int delta = Context::getValveTargetOpen() - Context::getValveCurrentOpen();
    switch (state) {
    case IDLE:
        if (abs(delta) > 0)
        {
            state = MOVING;
            startMovementTs = millis();
        }
        break;
    case MOVING:
        long dt = millis() - startMovementTs;
        int sign = delta / abs(delta);
        openingAngle += (dt * sign / (float)COMPLETE_MOVEMENT_TIME) * MAX_ANGLE_MOTOR;
        motor->setPosition(openingAngle);
        Context::setValveCurrentOpen(openingAngle * ANGLE_TO_PERCENTAGE);
        if (abs(delta) == 0)
        {
            state = IDLE;
        }
        break;
    }
}
