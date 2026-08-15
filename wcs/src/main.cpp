#include "config.h"
#include "kernel/Protocol.h"
#include "kernel/Scheduler.h"
#include "model/HWPlatform.h"
#include "tasks/LCDTask.h"
#include "tasks/ModalityTask.h"
#include "tasks/PotentiometerTask.h"
#include "tasks/SerialCommTask.h"
#include "tasks/ValveTask.h"
#include <Arduino.h>

HWPlatform *hw;
Scheduler *scheduler;

void setup() {
		Serial.begin(SERIAL_BAUD_RATE);

		hw = new HWPlatform();
		scheduler = new Scheduler();
		scheduler->init(50);

		Task *serialCommTask = new SerialCommTask();
		serialCommTask->init(50);
		scheduler->addTask(serialCommTask);

		Task *modalityTask = new ModalityTask(hw->getButton());
		modalityTask->init(50);
		scheduler->addTask(modalityTask);

		Task *potentiometerTask = new PotentiometerTask(hw->getPotentiometer());
		potentiometerTask->init(50);
		scheduler->addTask(potentiometerTask);

		Task *valveTask = new ValveTask(hw->getValveMotor());
		valveTask->init(50);
		scheduler->addTask(valveTask);

		Task *lcdTask = new LCDTask(hw->getLCD());
		lcdTask->init(200);
		scheduler->addTask(lcdTask);
}

void loop() { 
	
	scheduler->schedule();

}