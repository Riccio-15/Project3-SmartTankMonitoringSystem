#include "Task.h"

void Task::start(const char* name, uint32_t periodMs, uint32_t stackSize, UBaseType_t priority) {
    this->periodMs = periodMs;
    xTaskCreate(Task::run, name, stackSize, this, priority, nullptr);
}

void Task::run(void* param) {
    Task* self = static_cast<Task*>(param);
    const TickType_t delay = pdMS_TO_TICKS(self->periodMs);

    for (;;) {
        self->tick();
        vTaskDelay(delay);
    }
}