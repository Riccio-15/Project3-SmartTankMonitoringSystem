#include "LedTask.h"
#include "model/Context.h"

LedTask::LedTask(Light *greenLed, Light *redLed)
    : greenLed(greenLed), redLed(redLed) {}

void LedTask::tick() {
    if (Context::isNetworkOk()) {
        greenLed->switchOn();
        redLed->switchOff();
    } else {
        greenLed->switchOff();
        redLed->switchOn();
    }
}