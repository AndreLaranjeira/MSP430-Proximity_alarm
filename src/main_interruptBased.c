#include <msp430.h>
#include <math.h>
#include <stdint.h>
#include "../Include/MSP430_shortcuts.h"
#include "../Include/MSP430_project.h"

void main(void) {
	WDTCTL = WDTPW | WDTHOLD; // Stop watchdog timer.
	InitializePorts(); // Avoids energy loss to unitialized ports.
	
	/* Configuration */
	ConfigS1();
	ConfigS2();
	ConfigLED1();
	ConfigLED2();
	ConfigUART();
	ConfigProxSensor();
	
	/* Configure Idle state */
	ConfigureIdle();
	
}
