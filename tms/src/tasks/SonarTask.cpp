#include "SonarTask.h"
#include "config.h"
#include "model/Context.h"

SonarTask::SonarTask(ProximitySensor *sonar) : sonar(sonar) {}

void SonarTask::tick() {
	float distanceM = sonar->getDistance();
	Serial.println(distanceM * 1000.0f);

	if (distanceM < 0) {
		return;
	}

	float distanceMm = distanceM * 1000.0f;
	float levelMm = TANK_HEIGHT_MM - distanceMm;

	if (levelMm < 0)
		levelMm = 0;
	if (levelMm > TANK_HEIGHT_MM)
		levelMm = TANK_HEIGHT_MM;

	Context::setLevel(levelMm);
	Context::setNewLevelToPublish(true);
}