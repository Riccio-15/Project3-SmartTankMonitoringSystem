#ifndef __SCHEDULER__
#define __SCHEDULER__

#include "Task.h"

constexpr int MAX_TASKS = 50;

class Scheduler
{

public:
    void init(int basePeriod);
    virtual bool addTask(Task *task);
    virtual void schedule();

private:
    int basePeriod;
    int nTasks;
    Task *taskList[MAX_TASKS];
};

#endif