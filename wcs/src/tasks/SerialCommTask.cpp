#include "SerialCommTask.h"
#include "kernel/SerialComm.h"
#include "model/Context.h"

SerialCommTask::SerialCommTask(){}

void SerialCommTask::tick()
{
    SerialCommService::poll();
    sendCurrentState();
}

void SerialCommTask::sendCurrentState()
{
    if (Context::hasModeJustChanged()) {
        SerialCommService::sendMode();
    }
    if (Context::hasOpeningTargetJustChanged()) {
        SerialCommService::sendOpening();
    }
}