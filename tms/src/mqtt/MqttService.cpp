#include "MqttService.h"
#include <PubSubClient.h>
#include <WiFi.h>
#define MSG_BUFFER_SIZE 50
#define KEEP_ALIVE_TIME 10 // seconds

WiFiClient espClient;

MqttService::MqttService(const char *ssid, const char *password,
                         const char *broker, int port, const char *topic)
    : topic(topic), ssid(ssid), password(password), broker(broker), port(port) {
    this->connectWifi();
    this->buildStableClientId();

    this->mqttPubClient = new PubSubClient(espClient);
    this->mqttPubClient->setServer(this->broker, this->port);
    this->mqttPubClient->setKeepAlive(KEEP_ALIVE_TIME);
}

void MqttService::buildStableClientId() {
    // deve restare identico ad ogni riconnessione
    this->clientId = "tms-" + WiFi.macAddress();
}

void MqttService::connectWifi() {
    Serial.println(String("Connecting to ") + ssid);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void MqttService::publish(int level) {
    char msg[MSG_BUFFER_SIZE];
    snprintf(msg, MSG_BUFFER_SIZE, "level: #%ld", level);
    Serial.println(String("Publishing message: ") + msg);
    this->mqttPubClient->publish(this->topic, msg);
}

bool MqttService::isConnected() { return mqttPubClient->connected(); }

void MqttService::reconnect() {
    while (!this->mqttPubClient->connected()) {
        Serial.print("Attempting MQTT connection...");

        bool connected = this->mqttPubClient->connect(
            this->clientId.c_str(), this->topic,
            1,    // QoS
            true, // retain: chi si iscrive dopo vede comunque l'ultimo stato
            "offline"); // messaggio in caso di disconnessione

        if (connected) {
            Serial.println("connected");
        } else {
            Serial.print("failed, rc=");
            Serial.print(this->mqttPubClient->state());
            Serial.println(" try again in 5 seconds");
            vTaskDelay(pdMS_TO_TICKS(5000));
        }
    }
}

void MqttService::loop() {
    if (WiFi.status() != WL_CONNECTED) {
        return;
    }
    if (!this->mqttPubClient->connected()) {
        reconnect();
    }
    this->mqttPubClient->loop();
}