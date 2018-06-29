#include <msp430.h>
#include "../Include/MSP430_shortcuts.h"
#include "../Include/MSP430_project.h"

static uint8_t good_read = 0;
static uint16_t distance = 0, start = 0, end = 0;

/* S1 */
#pragma vector=PORT2_VECTOR
__interrupt void P2_ISR(){
	switch(P2IV){
		case 0x04:
			ConfigureArming(); // This will disable interruptions on S1
		break;
		default: break;
	}
}

/* S2 */
#pragma vector=PORT1_VECTOR
__interrupt void P1_ISR(){
	switch(P1IV){
		case 0x04:			
			ConfigureIdle(); // This will disable interruptions for S2
		break;
		default: break;
	}
}

/* TA0 */
#pragma vector=TIMER0_A0_VECTOR
__interrupt void TA0_CCR0_ISR(){
	if(GetState() == Triggered){
		TogglePort(P1, OUT, 0);
		TogglePort(P4, OUT, 7);
		TogglePort(P8, OUT, 2);
  }
  else if(GetState() == Arming){
		SetPort(P1, OUT, 0);
		SetPort(P4, OUT, 7);  
  }
  TA0CCTL0 &= ~CCIFG;
}

#pragma vector=TIMER0_A1_VECTOR
__interrupt void TA0_CCRN_ISR(){
	switch(TA0IV){
		case 0x02:
			ClearPort(P1, OUT, 0);
			ClearPort(P4, OUT, 7);
			SendTrigger();
	}
}

/* TA1 */
#pragma vector = TIMER1_A1_VECTOR
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
					distance = (end-start);
					good_read = 0;
					
					// Arming
					if(GetState() == Arming){
						// Get samples
						if(GetMeasureCounter() < MEASURE_SIZE) {
							SaveMeasure(distance);
						}
						// Verify error
						else {
							if(AcceptableMeasures()) {
								ConfigureSet();
							}
							else {
								ConfigureIdle();
							}
						}
					}
					
					// Set
					else if(GetState() == Set){
						if(!AcceptableDistance(distance)) {
							ConfigureTriggered();
						}
					}
					
					
				}
			}
		
		break;
	}
}

/* TA2 */
#pragma vector=TIMER2_A0_VECTOR
__interrupt void TA2_CCR0_ISR(){
	SendTrigger();
	TA2CCTL0 &= ~CCIFG;
}
