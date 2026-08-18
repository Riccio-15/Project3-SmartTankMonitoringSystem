#include "ModalityTask.h"
#include "devices/Button.h"
#include "model/Context.h"

ModalityTask::ModalityTask(Button* button)
    : button(button), buttonJustPressed(false)
{

}

void ModalityTask::tick()
{
    if (button->isPressed() && !buttonJustPressed)
    {
        SystemMode newMode = Context::isAutoMode() ? MODE_MANUAL : MODE_AUTO;
        Context::setMode(newMode);
        Context::setModeJustChanged(true);
        buttonJustPressed = true;
    }
    if (!button->isPressed())
    {
        buttonJustPressed = false;
    }
}
