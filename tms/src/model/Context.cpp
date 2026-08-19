#include "Context.h"
#include <Arduino.h>

// Con task FreeRTOS reali, Context e' acceduto da thread concorrenti
// serve il mutex
static SemaphoreHandle_t getMutex() {
    static SemaphoreHandle_t mutex = xSemaphoreCreateMutex();
    return mutex;
}

void Context::setLevel(int levelMm) {
    xSemaphoreTake(getMutex(), portMAX_DELAY);
    getInstance().level = levelMm;
    xSemaphoreGive(getMutex());
}

int Context::getLevel() {
    xSemaphoreTake(getMutex(), portMAX_DELAY);
    int value = getInstance().level;
    xSemaphoreGive(getMutex());
    return value;
}

void Context::setNetworkOk(bool ok) {
    xSemaphoreTake(getMutex(), portMAX_DELAY);
    getInstance().networkOk = ok;
    xSemaphoreGive(getMutex());
}

bool Context::isNetworkOk() {
    xSemaphoreTake(getMutex(), portMAX_DELAY);
    bool value = getInstance().networkOk;
    xSemaphoreGive(getMutex());
    return value;
}

bool Context::hasNewLevelToPublish() {
    xSemaphoreTake(getMutex(), portMAX_DELAY);
    bool value = getInstance().newLevelToPublish;
    xSemaphoreGive(getMutex());
    return value;
}

void Context::setNewLevelToPublish(bool value) {
    xSemaphoreTake(getMutex(), portMAX_DELAY);
    getInstance().newLevelToPublish = value;
    xSemaphoreGive(getMutex());
}

Context &Context::getInstance() {
    static Context instance;
    return instance;
}

Context::Context() : level(0), networkOk(false), newLevelToPublish(false) {}