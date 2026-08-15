#include "SerialComm.h"
#include "Protocol.h"
#include "model/Context.h"

SerialCommService SerialCommService::instance;

SerialCommService::SerialCommService()
{
    inputBuffer = "";
    inputBuffer.reserve(32);
}

void SerialCommService::splitMessage()
{
    String message = instance.inputBuffer;
    int indexLastCharPrefix = message.indexOf(":");
    instance.messagePrefix = message.substring(0, indexLastCharPrefix + 1);
    instance.messageContent = message.substring(indexLastCharPrefix + 1, message.length());
}

void SerialCommService::processMessage()
{
    instance.splitMessage();
    if (instance.messagePrefix.compareTo(MODE_PREFIX) == 0) {
        SystemMode mode = MODE_AUTO;
        if (instance.messageContent.compareTo(MSG_MODE_MANUAL) == 0) mode = MODE_MANUAL;
        Context::setMode(mode);
        Context::setModeJustChanged(true);
    }
    else if (instance.messagePrefix.compareTo(OPEN_PREFIX) == 0) {
        Context::setValveTargetOpen(instance.messageContent.toInt());
        Context::setOpeningTargetJustChanged(true);
    }
}

void SerialCommService::poll()
{
    while (Serial.available() > 0)
    {
        char c = (char)Serial.read();
        if (c == '\n')
        {

            processMessage();
            instance.inputBuffer = "";
        }
        else if (c != '\r')
        {
            instance.inputBuffer += c;
        }
    }
}

void SerialCommService::sendMode()
{
    Serial.print(MODE_PREFIX);
    if (Context::isNetworkOk()) {
        if (Context::isAutoMode()) Serial.print(MSG_MODE_AUTO);
        else if (Context::isManualMode()) Serial.print(MSG_MODE_MANUAL);
        Context::setModeJustChanged(false);
    }
}

void SerialCommService::sendOpening()
{
    Serial.print(OPEN_PREFIX);
    Serial.print(Context::getValveTargetOpen());
    Context::setOpeningTargetJustChanged(false);
}