#ifndef __MODALITY_TASK__
#define __MODALITY_TASK__

#include "devices/Button.h"
#include "kernel/Task.h"

class ModalityTask : public Task {
	
public:
	ModalityTask(Button* button);

	void tick();
	
private:
	Button* button;
};

#endif
