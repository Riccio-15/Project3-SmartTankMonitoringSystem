#include "MqttTask.h"
#include "model/Context.h"

MqttTask::MqttTask(MqttService* mqttService):mqttService(mqttService){}

void MqttTask::tick()
{
	this->mqttService->loop();
	Context::setNetworkOk(this->mqttService->isConnected());
	if (Context::isNetworkOk() && Context::hasNewLevelToPublish()) {
		this->mqttService->publish(Context::getLevel());
		Context::setNewLevelToPublish(false);
	}
}