#include "SonarTask.h"
#include "config.h"
#include "model/Context.h"

SonarTask::SonarTask(ProximitySensor *sonar) : sonar(sonar) {}

void SonarTask::tick() {
    float distanceM = sonar->getDistance();

    if (distanceM < 0) {
        return;
    }

    int distanceMm = (int)(distanceM * 1000);
    int levelMm = TANK_HEIGHT_MM - distanceMm;

    if (levelMm < 0)
        levelMm = 0;
    if (levelMm > TANK_HEIGHT_MM)
        levelMm = TANK_HEIGHT_MM;

    Context::setLevel(levelMm);
    Context::setNewLevelToPublish(true);
}