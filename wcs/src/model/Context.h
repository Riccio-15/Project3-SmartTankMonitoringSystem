#ifndef __CONTEXT__
#define __CONTEXT__

enum SystemMode
{
    MODE_AUTO,
    MODE_MANUAL
};

class Context
{

public:
    static void setMode(SystemMode mode);
    static bool isAutoMode();
    static bool isManualMode();

    static void setNetworkOk();
    static void setNetworkLost();
    static bool isNetworkOk();

    //riguardante target che il servoMotore deve raggiungere
    //richiamato all'arrivop del comando open:.. oppure dalla task del Pot
    static void setValveTargetOpen(int percent);
    static int getValveTargetOpen();

    //riguardante servoMotore
    //REV: forse non necessario per come gestiamo il servo
    static void setValveCurrentOpen(int percent);
    static int getValveCurrentOpen();

    static Context &getInstance();

private:
    Context();

private:
    SystemMode mode;
    bool networkOk;
    int valveTargetOpen;
    int valveCurrentOpen;
};

#endif