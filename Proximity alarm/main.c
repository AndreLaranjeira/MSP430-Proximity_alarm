#include <msp430.h>
#include "../Include/MSP430_shortcuts.h"

// Global variables:
unsigned int S1_q = 0, S2_q = 0;

// Main function:
void main(void) {

    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    InitializePorts();
    ConfigS1();
    ConfigS2();
    ConfigLED1();
    ConfigLED2();

    while(1) {

        if((P2IN & BIT1) == 0) {    // Se S1 for pressionada.

            if(S1_q == 1) {         // Se o estado anterior de S1 for "não pressionado".

                // Colocar aqui o que acontece quando S1 é pressionado.
                // Pressionando S1, deve-se ativar o alarme.

                Debounce();         // Função de atraso para retirar debounces.
                S1_q = 0;           // Mude o estado anterior de S1.

            }

        }

        else {

            if(S1_q == 0) {

                S1_q = 1;       // Mude o estado anterior de S1.
                Debounce();         // Função de atraso para retirar debounces.

            }

        }

        if((P1IN & BIT1) == 0) {    // Se S2 for pressionada.

            if(S2_q == 1) {         // Se o estado anterior de S2 for "não pressionado".

                // Colocar aqui o que acontece quando S2 é pressionado.
                // Pressionando S2, deve-se desativar o alarme.

                Debounce();         // Função de atraso para retirar debounces.
                S2_q = 0;           // Mude o estado anterior de S2.

            }

        }

        else {

            if(S2_q == 0) {

                S2_q = 1;       // Mude o estado anterior de S2.
                Debounce();         // Função de atraso para retirar debounces.

            }

        }

    }

}

// ISRs:
