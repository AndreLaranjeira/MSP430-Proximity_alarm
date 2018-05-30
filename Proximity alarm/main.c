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
	
	// Configuração dos botões.
	
    ConfigS1();
    ConfigS2();
	
	// Configuração dos LEDs.
	
    ConfigLED1();
    ConfigLED2();
	
	// Laço de execução.

    while(1) {

		switch(CurrentState) {
			
			case Idle:
			
				// Nesse estado, o microcontrolador deve esperar o botão S1 ser
				// apertado para entrar no estado Arming.
			
				SetPort(P1, OUT, 0);	// Acende o LED1.
				ClearPort(P4, OUT, 7);	// Apaga o LED2.
			
				// Laço de execução do estado Idle.
			
				while(CurrentState == Idle) {
					
				}
			
				break;
				
			case Arming:
			
				// Nesse estado, o microcontrolador deve realizar 3 medidas de
				// distância e, caso elas estejam dentro de uma margem de erro,
				// entrar no estado Set. Caso contrário, deve-se entrar no
				// estado Idle.
				
				// Laço de execução do estado Arming.
			
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
			
				ClearPort(P1, OUT, 0);	// Apaga o LED1.
				SetPort(P4, OUT, 7);	// Acende o LED2.
				
				// Laço de execução do estado Set.
			
				while(CurrentState == Set) {
					
				}
			
				break;
				
			case Triggered:
			
				// Nesse estado, o microcontrolador deve enviar uma notificação
				// de bluetooth para o dispositivo conectado, ligar o buzzer e
				// acender o LED1 e o LED2 alternadamente. Quando o botão S2
				// for apertado, deve-se ir para o estado Idle.
				
				// Laço de execução do estado Triggered.
			
				while(CurrentState == Triggered) {
					
				}
				
				break;
			
		}

    }

}

// ISRs:
