#ifndef __TASK__
#define __TASK__

class Task
{

public:
    Task();

    /* periodic */
    virtual void init(int period);

    /* aperiodic */
    virtual void init();

    virtual void tick() = 0;
    virtual void setActive(bool active);

    bool updateAndCheckTime(int basePeriod);
    void setCompleted();
    bool isCompleted();
    bool isPeriodic();
    bool isActive();
    int getPeriod();

private:
    int myPeriod;
    int timeElapsed;
    bool active;
    bool periodic;
    bool completed;
};

#endif