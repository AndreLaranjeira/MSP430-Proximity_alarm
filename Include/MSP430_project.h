 // MSP430 project functions

// Define guard:
#ifndef MSP430_PROJECT_H_
#define MSP430_PROJECT_H_

#include "../Include/MSP430_shortcuts.h"

#define PROX_SENSOR_CONSTANT 58
#define MEASURE_SIZE 4
#define ACCEPTABLE_RELATIVE_ERROR 5
#define ACCEPTABLE_ABSOLUTE_ERROR 116 // PROX_SENSOR_CONSTANT*2 = 2cm
#define SAMPLE_PERIOD 30 // In us

typedef enum {
	Idle,
	Arming,
	Set,
	Triggered
} State;

State GetState();
void ConfigProxSensor();
void SendTrigger();
void SaveMeasure(uint16_t);
uint8_t AcceptableMeasures();
uint8_t AcceptableDistance(uint16_t);
void ConfigureIdle();
void ConfigureArming();
void ConfigureSet();
void ConfigureTriggered();

#endif // MSP430_PROJECT_H_
