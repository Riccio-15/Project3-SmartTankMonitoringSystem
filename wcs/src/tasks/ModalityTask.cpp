#include "ModalityTask.h"
#include "devices/Button.h"
#include "model/Context.h"

ModalityTask::ModalityTask(Button* button)
    : button(button)
{

}

void ModalityTask::tick()
{
    if (button->isPressed())
    {
        SystemMode newMode = Context::isAutoMode() ? MODE_MANUAL : MODE_AUTO;
        Context::setMode(newMode);
        Context::setModeJustChanged(true);
    }
}
