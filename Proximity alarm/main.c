#include <msp430.h>
#include "../Include/MSP430_shortcuts.h"

// Enumerations:
typedef enum{
	Idle,
	Arming,
	Set,
	Triggered
} State;

// Global variables:
unsigned int S1_q = 0, S2_q = 0;

// Main function:
void main(void) {

	State CurrentState = Idle;
	unsigned int i;

    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
	
    InitializePorts();
	
	// Configura��o dos bot�es.
	
    ConfigS1();
    ConfigS2();
	
	// Configura��o dos LEDs.
	
    ConfigLED1();
    ConfigLED2();
	
	// La�o de execu��o.

    while(1) {

		switch(CurrentState) {
			
			case Idle:
			
				// Nesse estado, o microcontrolador deve esperar o bot�o S1 ser
				// apertado para entrar no estado Arming.
			
				SetPort(P1, OUT, 0);	// Acende o LED1.
				ClearPort(P4, OUT, 7);	// Apaga o LED2.
			
				// La�o de execu��o do estado Idle.
			
				while(CurrentState == Idle) {
					
				}
			
				break;
				
			case Arming:
			
				// Nesse estado, o microcontrolador deve realizar 3 medidas de
				// dist�ncia e, caso elas estejam dentro de uma margem de erro,
				// entrar no estado Set. Caso contr�rio, deve-se entrar no
				// estado Idle.
				
				// La�o de execu��o do estado Arming.
			
				while(CurrentState == Arming) {
					
					for(i = 3; i > 0; --i) {
					
					}
					
				}
			
				break;
				
			case Set:
			
				// Nesse estado, o microcontrolador deve realizar medidas de
				// dist�ncia constantemente. Caso uma delas seja diferente do
				// valor esperado, deve-se entrar no estado Triggered. Caso o
				// bot�o S2 seja apertado, deve-se retornar ao estado Idle.
			
				ClearPort(P1, OUT, 0);	// Apaga o LED1.
				SetPort(P4, OUT, 7);	// Acende o LED2.
				
				// La�o de execu��o do estado Set.
			
				while(CurrentState == Set) {
					
				}
			
				break;
				
			case Triggered:
			
				// Nesse estado, o microcontrolador deve enviar uma notifica��o
				// de bluetooth para o dispositivo conectado, ligar o buzzer e
				// acender o LED1 e o LED2 alternadamente. Quando o bot�o S2
				// for apertado, deve-se ir para o estado Idle.
				
				// La�o de execu��o do estado Triggered.
			
				while(CurrentState == Triggered) {
					
				}
				
				break;
			
		}

    }

}

// ISRs:
