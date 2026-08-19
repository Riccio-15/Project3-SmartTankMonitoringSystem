#include "ValveTask.h"
#include "model/Context.h"
#include <Arduino.h>

constexpr float ANGLE_TO_PERCENTAGE = 100.0f / 90.0f;
constexpr float COMPLETE_MOVEMENT_TIME = 10.0;
constexpr float MAX_ANGLE_MOTOR = 90.0;

ValveTask::ValveTask(ServoMotor* motor)
    : motor(motor), state(IDLE), openingAngle(0.0f)
{
    motor->on();
}

void ValveTask::tick()
{
    float delta = (Context::getValveTargetOpen() - Context::getValveCurrentOpen()) / 100.0f;
    switch (state) {
    case IDLE:
        if (fabs(delta) > 0.001f)
        {
            state = MOVING;
        }
        break;
    case MOVING:
        openingAngle += (delta / (float)COMPLETE_MOVEMENT_TIME) * MAX_ANGLE_MOTOR;
        motor->setPosition(openingAngle);
        Serial.println((delta / (float)COMPLETE_MOVEMENT_TIME) * MAX_ANGLE_MOTOR);
        Context::setValveCurrentOpen(openingAngle * ANGLE_TO_PERCENTAGE);
        if (fabs(delta) < 0.001f)
        {
            state = IDLE;
        }
        break;
    }
}
