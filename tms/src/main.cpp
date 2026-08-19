#include "config.h"
#include "model/HWPlatform.h"
#include "mqtt/MqttService.h"
#include "tasks/SonarTask.h"
#include "tasks/MqttTask.h"
#include "tasks/LedTask.h"
#include <Arduino.h>

HWPlatform *hw;

SonarTask *sonarTask;
LedTask *ledTask;
MqttTask* mqttTask;
MqttService* mqttService;

void setup() {
    Serial.begin(115200);

    hw = new HWPlatform();

    sonarTask = new SonarTask(hw->getProximitySensor());
    sonarTask->start("SonarTask", SONAR_SAMPLE_PERIOD_MS);

    mqttService = new MqttService(WIFI_SSID, WIFI_PASSWORD, MQTT_BROKER, MQTT_PORT, MQTT_TOPIC_LEVEL);
    mqttTask = new MqttTask(mqttService);
    mqttTask->start("MqttTask", MQTT_TASK_PERIOD_MS);

    ledTask = new LedTask(hw->getGreenLight(), hw->getRedLight());
    ledTask->start("LedTask", LED_TASK_PERIOD_MS);
}

void loop() {
    vTaskDelay(pdMS_TO_TICKS(1000));
}
