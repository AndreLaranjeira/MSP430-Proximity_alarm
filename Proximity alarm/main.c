#include <msp430.h>
#include <math.h>
#include <stdint.h>
#include "../Include/MSP430_shortcuts.h"

// Macros:
#define PROX_SENSOR_CONSTANT 58

// Enumerations:
typedef enum{
	Idle,
	Arming,
	Set,
	Triggered
} State;

// Function headers:
void MeasureDistance(void);
uint16_t RelativeError(uint16_t, uint16_t);

// Global variables:
uint8_t S1_q = 0, S2_q = 0, good_read = 0;
uint16_t distance = 0, start = 0, end = 0;
uint8_t MEASURE_BUSY = 0;

// Main function:
void main(void) {

	// Local variable declarations:
	State CurrentState = Idle;
	uint16_t i, measures[3], average, relativeError;

		WDTCTL = WDTPW | WDTHOLD;	 // stop watchdog timer
	
		InitializePorts();	// Avoids energy loss to unitialized ports.
	
	/* Port usage:
		P1.0: LED1,
		P1.1: S2,
		P1.2: Proximity sensor trigger,
		P2.0 (TA1.1): Proximity sensor echo,
		P2.1: S1,
		P4.7: LED2	
	*/
	
	// Switch configuration:
	
		ConfigS1();
		ConfigS2();
	
	// LED configuration:
	
		ConfigLED1();
		ConfigLED2();
	
	// Proximity sensor configuration:
	
		SetPort(P1, DIR, 2);
		ClearPort(P1, OUT, 2);

		ClearPort(P2, DIR, 0);
		SetPort(P2, SEL, 0);

		TA1CCTL1 = CaptureMode(3);
		TA1CTL = TimerAConfiguration(SMCLK, 2);
	
		__enable_interrupt();
	
	// Execution loop:

		while(1) {

		switch(CurrentState) {
			
			case Idle:
			
				// Nesse estado, o microcontrolador deve esperar o botão S1 ser
				// apertado para entrar no estado Arming.
			
				SetPort(P1, OUT, 0);	// Liga o LED1.
				ClearPort(P4, OUT, 7);	// Desliga o LED2.
			
				if(ComparePortEQ(P2, IN, 1, 0)){
					Debounce();
					CurrentState = Arming;
					while(ComparePortEQ(P2, IN, 1, 0));
					Debounce();
				}
			
				break;
				
			case Arming:
			
				// Nesse estado, o microcontrolador deve realizar 3 medidas de
				// distância e, caso elas estejam dentro de uma margem de erro,
				// entrar no estado Set. Caso contrário, deve-se entrar no
				// estado Idle.
			
				for(i=0,average=0;i<3;i++) {
					MeasureDistance();
					while(MEASURE_BUSY);
					measures[i] = distance;
					average += distance;
				}
				
				average /= 3;
				
				// Verifica se algum dos erros percentuais com relação à média
				// é maior que 5%
				
				for(i=0;i<3;i++) {
					// Essa conta é muito complicada?
					if(RelativeError(average, measures[i]) > 5){
						CurrentState = Idle;
						break;
					}
				}
				
				if(CurrentState == Idle) break;
				
				CurrentState = Set;
				
				
				break;
				
			case Set:
			
				// Nesse estado, o microcontrolador deve realizar medidas de
				// distância constantemente. Caso uma delas seja diferente do
				// valor esperado, deve-se entrar no estado Triggered. Caso o
				// botão S2 seja apertado, deve-se retornar ao estado Idle.
			
				ClearPort(P1, OUT, 0);	// Desliga o LED1.
				SetPort(P4, OUT, 7);	// Liga o LED2.
				
				if(ComparePortEQ(P1, IN, 1, 0)){
					Debounce();
					CurrentState = Idle;
					while(ComparePortEQ(P1, IN, 1, 0));
					Debounce();
					break;
				}
			
				MeasureDistance();
				while(MEASURE_BUSY);
				
				if(RelativeError(average, distance) > 5) 
					CurrentState = Triggered;
			
				break;
				
			case Triggered:
			
				// Nesse estado, o microcontrolador deve enviar uma notificação
				// de bluetooth para o dispositivo conectado, ligar o buzzer e
				// acender o LED1 e o LED2 alternadamente. Quando o botão S2
				// for apertado, deve-se ir para o estado Idle.
				
				
				TA0CTL = TACLR | TASSEL__ACLK | MC_0;
				TA0CCR0 = 32768; // 1s
				TA0CCTL0 |= CCIE;				
				TA0CTL |= MC_2;
				
				// Enviar notificação e ligar o buzzer
				
				// Volta ao Idle
				
				while(ComparePortEQ(P1, IN, 1, 0)){
					Debounce();
					CurrentState = Idle;
					while(ComparePortEQ(P1, IN, 1, 0));
					Debounce();
				}
				
				TA0CCTL0 &= ~CCIE;
				
				break;
			
		}

		}

}

// Function implementations:
void MeasureDistance(void) {
	SetPort(P1, OUT, 2);
	// We could put a delay, but not necessary as we tested
	ClearPort(P1, OUT, 2);
	MEASURE_BUSY = 1;
}

uint16_t RelativeError(uint16_t average, uint16_t distance){
	return 100*abs(average-distance)/average;
}

// ISRs:
#pragma vector = TA1_CCRN_INT
__interrupt void TA1_CCRN_ISR() {

		switch(TA1IV) {

		// Proximity sensor echo interruption:
	
				case 0x02:

			// Measurement started.
		
						if((TA1CCTL1 & CCI) == CCI) {
								start = TA1CCR1;
								good_read = 1;

						}
			
			// Measurement ended.

						if((TA1CCTL1 & CCI) == 0) {
								if(good_read == 1) {
										end = TA1CCR1;
										distance = (end - start)/(PROX_SENSOR_CONSTANT);
										good_read = 0;
										MEASURE_BUSY = 0;
								}
						}

				break;
				
				

		}

}


#pragma vector=TIMER0_A0_VECTOR
__interrupt void TA0_CCR0_ISR(){
      TogglePort(P1, OUT, 0);
      TogglePort(P4, OUT, 7);
      TA0CCTL0 &= ~CCIFG;
}
