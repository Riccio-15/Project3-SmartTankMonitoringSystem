#include "config.h"
#include "model/HWPlatform.h"
#include "tasks/SonarTask.h"
#include "tasks/LedTask.h"
#include <Arduino.h>

HWPlatform *hw;

SonarTask *sonarTask;
LedTask *ledTask;

void setup() {
    Serial.begin(115200);

    hw = new HWPlatform();

    sonarTask = new SonarTask(hw->getProximitySensor());
    sonarTask->start("SonarTask", SONAR_SAMPLE_PERIOD_MS);

   //TODO: task mqtt

    ledTask = new LedTask(hw->getGreenLight(), hw->getRedLight());
    ledTask->start("LedTask", LED_TASK_PERIOD_MS);
}

void loop() {
    vTaskDelay(pdMS_TO_TICKS(1000));
}
