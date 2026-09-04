#ifndef __MQTT__
#define __MQTT__
#include <PubSubClient.h>
#include <WiFi.h>
#define MSG_BUFFER_SIZE 50

class MqttService {
  private:
    PubSubClient *mqttPubClient;
    const char *topic;
    const char *ssid;
    const char *password;
    const char *broker;
    int port;

    String clientId;

    void begin();
    void connectWifi();
    void reconnect();
    void buildStableClientId();

  public:
    MqttService(const char *ssid, const char *password, const char *broker,
                int port, const char *topic);
    void publish(int level);
    bool isConnected();
    void loop();
};

#endif