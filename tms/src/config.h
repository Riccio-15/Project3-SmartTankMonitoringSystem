#ifndef __CONFIG__
#define __CONFIG__

#include <Arduino.h>

// pin lato destro dell ESP32
constexpr int SONAR_TRIG_PIN = 23;
constexpr int SONAR_ECHO_PIN = 18;
constexpr int LED_GREEN_PIN  = 2;
constexpr int LED_RED_PIN    = 16;

constexpr const char* WIFI_SSID     = "LittleBarfly";
constexpr const char* WIFI_PASSWORD = "esiot2025";

constexpr const char* MQTT_BROKER      = "broker.mqtt-dashboard.com";
constexpr int          MQTT_PORT       = 1883;
constexpr const char*  MQTT_TOPIC_LEVEL = "tms/level";

constexpr int SONAR_SAMPLE_PERIOD_MS   = 1000;
constexpr long SONAR_MAX_ECHO_TIME_US  = 25000L; // timeout

// dalla superficie dell'acqua, il livello si ricava per differenza?
constexpr int TANK_HEIGHT_MM = 1000;

// periodo task
constexpr int LED_TASK_PERIOD_MS  = 200;
constexpr int MQTT_TASK_PERIOD_MS = 50;

#endif