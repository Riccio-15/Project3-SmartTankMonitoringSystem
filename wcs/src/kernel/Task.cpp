#include "Task.h"

Task::Task()
{
    active = false;
}

void Task::init(int period)
{
    myPeriod = period;
    periodic = true;
    active = true;
    timeElapsed = 0;
}

void Task::init()
{
    timeElapsed = 0;
    periodic = false;
    active = true;
    completed = false;
}

void Task::setActive(bool active)
{
    timeElapsed = 0;
    this->active = active;
}

bool Task::updateAndCheckTime(int basePeriod)
{
    timeElapsed += basePeriod;
    if (timeElapsed >= myPeriod)
    {
        timeElapsed = 0;
        return true;
    }
    else
    {
        return false;
    }
}

void Task::setCompleted()
{
    completed = true;
    active = false;
}

bool Task::isCompleted()
{
    return completed;
}

bool Task::isPeriodic()
{
    return periodic;
}

bool Task::isActive()
{
    return active;
}

int Task::getPeriod()
{
    return myPeriod;
}