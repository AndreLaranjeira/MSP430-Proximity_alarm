#include <msp430.h>
#include "../Include/MSP430_shortcuts.h"

// Global variables:

// Main function:
void main(void) {

    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    InitializePorts();

    LowPowerMode(3);

}

// ISRs:
