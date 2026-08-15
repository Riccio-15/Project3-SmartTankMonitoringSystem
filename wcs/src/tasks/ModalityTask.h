#ifndef __MODALITY_TASK__
#define __MODALITY_TASK__

#include "devices/Button.h"

class ModalityTask
{
public:
    ModalityTask(Button* button);

    void tick();
    
private:
    Button* button;
};

#endif
