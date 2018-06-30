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

void ConfigBuzzer(){
	SetPort(P8, OUT, 2);
}

void SendTrigger(void) {
	SetPort(P1, OUT, 2);
	DelayMicrosseconds(12);
	ClearPort(P1, OUT, 2);
}

void SaveMeasure(uint16_t distance){ // ASSERTIVA: counter adequado
	measures[counter] = distance;
	average += distance;
	counter++;
}

uint8_t AcceptableMeasures(){
	uint16_t i;
	average >>= 3;
	for(i=0; i<MEASURE_SIZE; i++){
		if(abs(measures[i]-average) > ACCEPTABLE_ABSOLUTE_ERROR)
			return 0;
	}
	return 1;
}

uint8_t AcceptableDistance(uint16_t distance){ // ASSERTIVA: average já foi dividido por 4
	if(abs(distance-average) > ACCEPTABLE_ABSOLUTE_ERROR)
			return 0;
	return 1;
}

void ConfigureIdle(){
	currentState = Idle;
	
	SetPort(P1, OUT, 0); // LED1 ON
	ClearPort(P4, OUT, 7); // LED2 OFF
	SetPort(P8, OUT, 2); // Buzzer OFF
	
	InterruptEnableS1();
	InterruptDisableS2();
	
	UARTM0SendString("IDLE\n");
	
	TA0CCTL0 &= ~CCIE;	
	TA0CCTL1 &= ~CCIE;
	 
}

void ConfigureArming(){
	currentState = Arming;
	
	SetPort(P1, OUT, 0); // LED1 ON
	SetPort(P4, OUT, 7); // LED2 ON
	SetPort(P8, OUT, 2); // Buzzer OFF
	
	InterruptDisableS1();
	InterruptDisableS2();
	
	// Send first trigger
	counter = 0;
	average = 0;
	
	UARTM0SendString("ARMING\n");
	
	// Interruption for LEDs and samples
	TA0CTL = TimerAConfiguration(ACLK, 1);
	TA0CCR0 = SAMPLE_PERIOD_ARMING; // 0.5s
	TA0CCR1 = SAMPLE_PERIOD_ARMING*0.75; // 3/4*0.5s
	TA0CCTL0 |= CCIE;
	TA0CCTL1 |= CCIE;
}

void ConfigureSet(){ // Assertiva TA1 configurado com SMCLK
	currentState = Set;
	
	ClearPort(P1, OUT, 0); // LED1 OFF
	SetPort(P4, OUT, 7); // LED2 ON
	SetPort(P8, OUT, 2); // Buzzer OFF
	
	InterruptDisableS1();
	InterruptEnableS2();
	
	UARTM0SendString("SET\n");
	
	TA0CCTL1 &= ~CCIE;
	
	// Interruption for samples
	TA0CTL = TimerAConfiguration(SMCLK, 1);
	TA0CCR0 = SAMPLE_PERIOD;
	TA0CCTL0 |= CCIE;
}

void ConfigureTriggered(){
	currentState = Triggered;
	
	ClearPort(P1, OUT, 0); // LED1 OFF
	SetPort(P4, OUT, 7); // LED2 ON
	
	InterruptDisableS1();
	InterruptEnableS2();
	
	TA0CCTL1 &= ~CCIE;
	
	// Interruption for LEDs and Buzzer
	TA0CTL = TimerAConfiguration(ACLK, 1);
	TA0CCR0 = 8191; // 1s
	TA0CCTL0 |= CCIE;
	
	UARTM0SendString("ALERTA!!!!\n");
	
}
