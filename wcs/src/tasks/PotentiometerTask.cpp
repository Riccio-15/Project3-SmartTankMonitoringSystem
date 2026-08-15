#include "PotentiometerTask.h"
#include "model/Context.h"
#include <Arduino.h>

// soglia minima di variazione (in percentuale)
// elimina il rumore analogico del potenziometro
constexpr int DEADBAND_PERCENT = 2;

PotentiometerTask::PotentiometerTask(Potentiometer *potentiometer)
    : potentiometer(potentiometer), lastValue(-1)
{
}

void PotentiometerTask::tick()
{

    if (!Context::isManualMode() || !Context::isNetworkOk())
    {
        return;
    }

    int value = potentiometer->readPercent();

    if (lastValue < 0 || abs(value - lastValue) >= DEADBAND_PERCENT)
    {
        Context::setValveTargetOpen(value);
        Context::setOpeningTargetJustChanged(true);
        lastValue = value;
    }
}