#include <msp430.h>
#include <math.h>
#include <stdint.h>
#include "../Include/MSP430_shortcuts.h"
#include "../Include/MSP430_project.h"

int main(void) {
	WDTCTL = WDTPW | WDTHOLD; // Stop watchdog timer.
	InitializePorts(); // Avoids energy loss to unitialized ports.
	
	/* Configuration */
	ConfigS1();
	ConfigS2();
	ConfigLED1();
	ConfigLED2();
	ConfigProxSensor();
	ConfigBuzzer();
	ConfigBT();
	
	__bic_SR_register(CPUOFF);
	
	/* Configure Idle state */
	ConfigureIdle();
	__enable_interrupt();
	
	//__bis_SR_register(LPM4_bits);
	
	
	return 0;
}


