#include <msp430.h>
#include "../Include/MSP430_shortcuts.h"

// Global variables:

// Main function:
void main(void) {

    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    InitializePorts();

    UCA0TXBUF = 0x55;

    LowPowerMode(3);

}

// ISRs:
