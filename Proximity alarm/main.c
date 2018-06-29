#include <msp430.h>
#include <math.h>
#include <stdint.h>
#include "../Include/MSP430_shortcuts.h"

// Macros:
#define ERROR_VALUE 100

// Enumerations:
typedef enum{
	Idle,
	Arming,
	Set,
	Triggered
} State;

// Function headers:
uint8_t AcceptableDistance(uint16_t, uint16_t);
void MeasureDistance(void);

// Global variables:
uint8_t good_read = 0, MEASURE_BUSY = 0, ARMING_FAILED = 0;
uint16_t distance = 0, start = 0, end = 0;

// Main function:
void main(void) {

	// Local variable declarations:
	State CurrentState = Idle;
	uint16_t i, measures[4], average;

	WDTCTL = WDTPW | WDTHOLD;	 // stop watchdog timer
	
	InitializePorts();	// Avoids energy loss to unitialized ports.
	
	/* Port usage:
		P1.0: LED 1
		P1.1: Switch 2
		P1.2: Proximity sensor trigger
		P2.0 (TA1.1): Proximity sensor echo
		P2.1: Switch 1
		P4.7: LED 2
		P7.0: Buzzer
	*/
	
	// Switch configuration:
	
		ConfigS1();
		ConfigS2();
	
	// LED configuration:
	
		ConfigLED1();
		ConfigLED2();

	// Buzzer configuration:

	    SetPort(P7, DIR, 0);
	    SetPort(P7, OUT, 0);

    // Configure Timer A0 to control LEDs and buzzer in Triggered state.

        TA0CTL = TimerAConfiguration(ACLK, 0);  // Timer is on hold.
        TA0CCR0 = 8191; // f = 4 Hz.
        TA0CCTL0 |= CCIE;
	
	// Proximity sensor configuration:
	
		SetPort(P1, DIR, 2);
		ClearPort(P1, OUT, 2);

		ClearPort(P2, DIR, 0);
		SetPort(P2, SEL, 0);

		TA1CCTL1 = CaptureMode(3);
		TA1CTL = TimerAConfiguration(SMCLK, 2);
	
	// Enable interruptions:

		__enable_interrupt();
	
	// Execution loop:

		while(1) {

		switch(CurrentState) {
			
			case Idle:
			
				// Nesse estado, o microcontrolador deve esperar o botão S1 ser
				// apertado para entrar no estado Arming.
			
				SetPort(P1, OUT, 0);	// Liga o LED1.
				ClearPort(P4, OUT, 7);	// Desliga o LED2.
				SetPort(P7, OUT, 0);  // Desliga o buzzer.

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
			
			    average = 0;
			    ARMING_FAILED = 0;

				for(i=0; i<4; i++) {

	                SetPort(P1, OUT, 0);    // Liga o LED1.
	                SetPort(P4, OUT, 7);  // Liga o LED2.

					MeasureDistance();
					while(MEASURE_BUSY);
					measures[i] = distance;
					average += distance;

					Delay40Microsseconds(18750);    // Wait 0.75 seconds.

	                ClearPort(P1, OUT, 0);    // Desliga o LED1.
	                ClearPort(P4, OUT, 7);  // Desliga o LED2.

					Delay40Microsseconds(6250);     // Wait 0.25 seconds.

				}
				
				average >>= 2;
				
				// Verifica se alguma das medições está fora da margem de erro.
				
				for(i=0; i<4; i++) {

					if(!AcceptableDistance(average, measures[i])){
					    ARMING_FAILED = 1;
						break;
					}

				}
				
				if(ARMING_FAILED == 0)
				    CurrentState = Set;
				
				else
				    CurrentState = Idle;

				break;
				
			case Set:
			
				// Nesse estado, o microcontrolador deve realizar medidas de
				// distância constantemente. Caso uma delas seja diferente do
				// valor esperado, deve-se entrar no estado Triggered. Caso o
				// botão S2 seja apertado, deve-se retornar ao estado Idle.
			
				ClearPort(P1, OUT, 0);	// Desliga o LED1.
				SetPort(P4, OUT, 7);	// Liga o LED2.

				DelaySeconds(1);

				while(CurrentState == Set) {

				    if(ComparePortEQ(P1, IN, 1, 0)){
				        Debounce();
				        CurrentState = Idle;
				        while(ComparePortEQ(P1, IN, 1, 0));
				        Debounce();
				    }

				    else {
			
				        MeasureDistance();
				        while(MEASURE_BUSY);
				
				        DelayMicrosseconds(1000);

				        if(!AcceptableDistance(average, distance))
				            CurrentState = Triggered;
			
				    }

				}

				break;
				
			case Triggered:
			
				// Nesse estado, o microcontrolador deve enviar uma notificação
				// de bluetooth para o dispositivo conectado, ligar o buzzer e
				// acender o LED1 e o LED2 alternadamente. Quando o botão S2
				// for apertado, deve-se ir para o estado Idle.
				
			    // Enable timer responsible for the LEDs and buzzer.

                TA0CTL |= MC__UP;
                TA0CCTL0 |= CCIE;
				
				// Enviar notificação de bluetooth.
				
				// Espera o botão ser apertado.
				
				while(ComparePortNE(P1, IN, 1, 0));

				// Retorna para o estado original.

				Debounce();
				CurrentState = Idle;
				while(ComparePortEQ(P1, IN, 1, 0));
				Debounce();
				
				// Disable timer responsible for the LEDs and buzzer.

				TA0CTL &= ~MC__UP;
				TA0CCTL0 &= ~CCIE;
				
				break;
			
		    }

		}

}

// Function implementations:
void MeasureDistance(void) {
	SetPort(P1, OUT, 2);
	DelayMicrosseconds(12);
	ClearPort(P1, OUT, 2);
	MEASURE_BUSY = 1;
}

uint8_t AcceptableDistance(uint16_t average, uint16_t distance) {

    if(abs(average - distance) < ERROR_VALUE)
        return 1;

    return 0;

}

// ISRs:
#pragma vector = TA0_CCR0_INT
__interrupt void TA0_CCR0_ISR() {
      TogglePort(P1, OUT, 0);   // LED 1.
      TogglePort(P4, OUT, 7);   // LED 2.
      TogglePort(P7, OUT, 0);   // Buzzer.
      TA0CCTL0 &= ~CCIFG;       // Limpa a flag de interrupções.
}

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
										distance = (end - start);
										good_read = 0;
										MEASURE_BUSY = 0;
								}
						}

				break;
				
				

		}

}

/* TODO list
 *
 * Add Bluetooth integration.
 * Refactor code.
 * Write comprehensive tutorial for module.
 *
 */
