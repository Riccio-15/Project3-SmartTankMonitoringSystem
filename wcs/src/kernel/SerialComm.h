#ifndef __SERIAL_COMM__
#define __SERIAL_COMM__

#include <Arduino.h>

class SerialCommService
{
private:
    
    static void splitMessage();
    static void processMessage();

    SerialCommService();
    static SerialCommService instance;
    String inputBuffer;
    String messagePrefix;
    String messageContent;

public:
    static void poll();
    static void sendMode();
    static void sendOpening();

};

#endif