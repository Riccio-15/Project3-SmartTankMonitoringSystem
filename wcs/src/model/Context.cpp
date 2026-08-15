#include "Context.h"

void Context::setMode(SystemMode mode) {
    getInstance().mode = mode;
}

bool Context::isAutoMode() {
    return getInstance().mode == MODE_AUTO;
}

bool Context::isManualMode() {
    return getInstance().mode == MODE_MANUAL;
}

void Context::setNetworkOk() {
    getInstance().networkOk = true;
}

void Context::setNetworkLost() {
    getInstance().networkOk = false;
}

bool Context::isNetworkOk() {
    return getInstance().networkOk;
}

void Context::setValveTargetOpen(int percent) {
    getInstance().valveTargetOpen = percent;
}

int Context::getValveTargetOpen() {
    return getInstance().valveTargetOpen;
}

void Context::setValveCurrentOpen(int percent) {
    getInstance().valveCurrentOpen = percent;
}

int Context::getValveCurrentOpen() {
    return getInstance().valveCurrentOpen;
}

bool Context::hasOpeningTargetJustChanged()
{
    return getInstance().openingTargetJustChanged;
}

bool Context::hasModeJustChanged()
{
    return getInstance().modeJustChanged;
}

void Context::setOpeningTargetJustChanged(bool changed)
{
    getInstance().openingTargetJustChanged = changed;
}

void Context::setModeJustChanged(bool changed)
{
    getInstance().modeJustChanged = changed;
}

Context& Context::getInstance() {
    static Context instance;
    return instance;
}

Context::Context()
    : mode(MODE_AUTO), networkOk(true), valveTargetOpen(0), valveCurrentOpen(0)
{
}