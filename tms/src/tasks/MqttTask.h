#ifndef __MQTT_TASK__
#define __MQTT_TASK__

#include "kernel/Task.h"
#include "mqtt/MqttService.h"

class MqttTask : public Task
{
private:
	MqttService* mqttService;
public:
	MqttTask(MqttService* mqttService);
	void tick();
};

#endif