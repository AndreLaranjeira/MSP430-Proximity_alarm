#include <msp430.h>
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

// Global variables:
uint8_t S1_q = 0, S2_q = 0, good_read = 0;
uint16_t distance = 0, start = 0, end = 0;

// Main function:
void main(void) {

	// Local variable declarations:
	State CurrentState = Idle;
	unsigned int i;

    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
	
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
			
				while(CurrentState == Idle) {
					
				}
			
				break;
				
			case Arming:
			
				// Nesse estado, o microcontrolador deve realizar 3 medidas de
				// distância e, caso elas estejam dentro de uma margem de erro,
				// entrar no estado Set. Caso contrário, deve-se entrar no
				// estado Idle.
			
				while(CurrentState == Arming) {
					
					for(i = 3; i > 0; --i) {
					
					}
					
				}
			
				break;
				
			case Set:
			
				// Nesse estado, o microcontrolador deve realizar medidas de
				// distância constantemente. Caso uma delas seja diferente do
				// valor esperado, deve-se entrar no estado Triggered. Caso o
				// botão S2 seja apertado, deve-se retornar ao estado Idle.
			
				ClearPort(P1, OUT, 0);	// Desliga o LED1.
				SetPort(P4, OUT, 7);	// Liga o LED2.
			
				while(CurrentState == Set) {
					
				}
			
				break;
				
			case Triggered:
			
				// Nesse estado, o microcontrolador deve enviar uma notificação
				// de bluetooth para o dispositivo conectado, ligar o buzzer e
				// acender o LED1 e o LED2 alternadamente. Quando o botão S2
				// for apertado, deve-se ir para o estado Idle.
				
				while(CurrentState == Triggered) {
					
				}
				
				break;
			
		}

    }

}

// Function implementations:
void MeasureDistance(void) {
	SetPort(P1, OUT, 2);
    ClearPort(P1, OUT, 2);
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
                }
            }

        break;

    }

}