#include <WiFi.h>
#include <PubSubClient.h>
#include "MqttService.h"
#define MSG_BUFFER_SIZE  50
#define KEEP_ALIVE_TIME 10 //seconds

MqttService::MqttService(const char* ssid, const char* password, const char* broker, int port, const char* topic):topic(topic), ssid(ssid), password(password), broker(broker), port(port)
{
	this->begin();
	WiFiClient espClient;
	this->mqttPubClient = new PubSubClient(espClient);
	this->mqttPubClient->setKeepAlive(KEEP_ALIVE_TIME);
}

void MqttService::connectWifi()
{
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

void MqttService::begin()
{
	this->connectWifi();
	this->mqttPubClient->setServer(this->broker, this->port);
}

//REV: considera l'invio dopo 10s di un messaggio in caso in cui il valore non fosse modificato per evitare che il broker dica che si è disconnesso
void MqttService::publish(int level)
{
	char msg[MSG_BUFFER_SIZE];
	snprintf (msg, MSG_BUFFER_SIZE, "level: #%ld", level);
	Serial.println(String("Publishing message: ") + msg);
	this->mqttPubClient->publish(this->topic, msg);  
}

bool MqttService::isConnected()
{
	return mqttPubClient->connected() && WiFi.status() != WL_CONNECTED;
}

void MqttService::reconnect()
{
	while (!this->mqttPubClient->connected()) {
		Serial.print("Attempting MQTT connection...");
		
		// Create a random client ID
		String clientId = String("tmp/DRM")+String(random(0xffff), HEX);

		// Attempt to connect
		if (this->mqttPubClient->connect(clientId.c_str())) {
			Serial.println("connected");
			this->mqttPubClient->subscribe(this->topic);
		} else {
			Serial.print("failed, rc=");
			Serial.print(this->mqttPubClient->state());
			Serial.println(" try again in 5 seconds");
			// Wait 5 seconds before retrying
			delay(5000);
		}
	}
}

void MqttService::loop()
{	
	if (WiFi.status() != WL_CONNECTED) {
		return;
	}
	if (!this->mqttPubClient->connected()) {
    	reconnect();
  	}
	this->mqttPubClient->loop();
}