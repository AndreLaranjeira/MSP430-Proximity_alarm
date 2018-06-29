#include <msp430.h>
#include <math.h>
#include <stdint.h>
#include "../Include/MSP430_project.h"

static State currentState = Idle;
static uint16_t measures[MEASURE_SIZE], average, counter = 0;

State GetState(){
	return currentState;
}

uint16_t GetMeasureCounter(){
	return counter;
}

void ConfigProxSensor(){

	SetPort(P1, DIR, 2);
	ClearPort(P1, OUT, 2);

	ClearPort(P2, DIR, 0);
	SetPort(P2, SEL, 0);

	TA1CCTL1 = CaptureMode(3);
	TA1CTL = TimerAConfiguration(SMCLK, 2);
	
}

void SendTrigger(void) {
	SetPort(P1, OUT, 2);
	ClearPort(P1, OUT, 2);
}

void SaveMeasure(uint16_t distance){ // ASSERTIVA: counter adequado
	measures[counter] = distance;
	average += distance;
	counter++;
}

uint8_t AcceptableMeasures(){
	uint16_t i;
	average >>= 4;
	for(i=0; i<MEASURE_SIZE; i++){
		if(abs(measures[i]-average) > ACCEPTABLE_ABSOLUTE_ERROR)
			return 0;
	}
	return 1;
}

uint8_t AcceptableDistance(uint16_t distance){ // ASSERTIVA: average já foi dividido por 4
	uint16_t i;
	if(abs(distance-average) > ACCEPTABLE_ABSOLUTE_ERROR)
			return 0;
	return 1;
}

void ConfigureIdle(){
	currentState = Idle;
	
	SetPort(P1, OUT, 0); // LED1 ON
	ClearPort(P4, OUT, 7); // LED2 OFF
	SetPort(P2, IE, 1); // Enable interruption for S1
	ClearPort(P1, IE, 1); // No interruption for S2
	TA0CCTL0 &= ~CCIE;	
	TA1CCTL2 &= ~CCIE;
	
	LowPowerMode(4);
	
}

void ConfigureArming(){
	currentState = Arming;
	
	SetPort(P1, OUT, 0); // LED1 ON
	SetPort(P4, OUT, 7); // LED2 ON
	ClearPort(P2, IE, 1); // No interruption for S1
	ClearPort(P1, IE, 1); // No interruption for S2
	TA0CCTL0 &= ~CCIE;
	TA1CCTL2 &= ~CCIE;
	
	// Send first trigger
	counter = 0;
	average = 0;
	SendTrigger();
	
	LowPowerMode(1);
}

void ConfigureSet(){ // Assertiva TA1 configurado com SMCLK
	currentState = Set;
	
	ClearPort(P1, OUT, 0); // LED1 OFF
	SetPort(P4, OUT, 7); // LED2 ON
	ClearPort(P2, IE, 1); // No interruption for S1
	SetPort(P1, IE, 1); // Interruption for S2
	TA0CCTL0 &= ~CCIE;
	
	TA1CCR2 = SAMPLE_PERIOD;
	TA1CCTL2 = CCIE;
	
	LowPowerMode(1);
}

void ConfigureTriggered(){
	currentState = Triggered;
	
	ClearPort(P1, OUT, 0); // LED1 OFF
	SetPort(P4, OUT, 7); // LED2 ON
	ClearPort(P2, IE, 1); // No interruption for S1
	ClearPort(P1, IE, 1); // No interruption for S2
	
	TA0CTL = TimerAConfiguration(ACLK, 1);
	TA0CCR0 = 32768; // 1s
	TA0CCTL0 |= CCIE;
	
	LowPowerMode(3);
	
}
