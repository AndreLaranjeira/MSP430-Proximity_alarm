// MSP430 shortcuts module - Header file.

// Define guard:
#ifndef MSP430_SHORTCUTS_H_
#define MSP430_SHORTCUTS_H_

// Includes:
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// Macros:
#define CaptureMode(mode) (CAP | (CM_##mode) | CCIS_0 | CCIE)
#define ClearPort(port, type, pin) ((port##type) &= ~(BIT##pin))
#define ComparePortEQ(port, type, pin, value) (((port##type) & (BIT##pin)) == value)
#define ComparePortNE(port, type, pin, value) (((port##type) & (BIT##pin)) != value)
#define FLLN(x) ((x)-1)
#define LowPowerMode(number) (__bis_SR_register(LPM##number##_bits))
#define ReadPort(port, type, pin) ((port##type) & (BIT##pin))  
#define SetPort(port, type, pin) ((port##type) |= (BIT##pin))
#define TogglePort(port, type, pin) ((port##type) ^= (BIT##pin))

#define TB0_CCR0_INT 59
#define TB0_CCRN_INT 58
#define USCI_A0_INT 56
#define TA0_CCR0_INT 53
#define TA0_CCRN_INT 52
#define TA1_CCR0_INT 49
#define TA1_CCRN_INT 48
#define TA2_CCR0_INT 44
#define TA2_CCRN_INT 43

// Functions:
int ConfigS1(void) {

    // Chave S1 (P2.1).

    ClearPort(P2, DIR, 1);     // Entrada = 0.
    SetPort(P2, REN, 1);      // REN = 1 para entradas.
    SetPort(P2, OUT, 1);     // IN = 1 if !S1.

    return 0;

}

int ConfigS2(void) {

    // Chave S2 (P1.1).

    ClearPort(P1, DIR, 1);     // Entrada = 0.
    SetPort(P1, REN, 1);      // REN = 1 para entradas.
    SetPort(P1, OUT, 1);     // IN = 1 if !S2.

    return 0;

}

int ConfigLED1(void) {

    // LED vermelho (P1.0).

    SetPort(P1, DIR, 0);      // Saída = 1.
    ClearPort(P1, OUT, 0);     // Output = 0.

    return 0;

}

int ConfigLED2(void) {

    // LED verde (P4.7).

    SetPort(P4, DIR, 7);      // Saída = 1.
    ClearPort(P4, OUT, 7);     // Output = 0.

    return 0;

}

int Debounce(void) {

    volatile int counter = 0xDFFF;

    while(counter != 0)
        counter--;

    return 0;

}

int InitializePorts(void) {

	// Removes warnings of uninitialized ports.
    // This will set all ports to inputs with a pull-down resistor.

    // Port initialization:

    PADIR = 0x0000;
	PAREN = 0xFFFF;
    PAOUT = 0x0000;

	PBDIR = 0x0000;
    PBREN = 0xFFFF;
    PBOUT = 0x0000;

	PCDIR = 0x0000;
    PCREN = 0xFFFF;
    PCOUT = 0x0000;

    PDDIR = 0x0000;	
    PDREN = 0xFFFF;
    PDOUT = 0x0000;

    return 0;

}

int InterruptDisableS1(void) {

    // Chave S1 (P2.1).

    ClearPort(P2, IE, 1);     // S1 não pode realizar interrupções.
    ClearPort(P2, IFG, 1);

    return 0;

}

int InterruptDisableS2(void) {

    // Chave S2 (P1.1).

    ClearPort(P1, IE, 1);     // S2 não pode realizar interrupções.
    ClearPort(P1, IFG, 1);

    return 0;

}

int InterruptEnableS1(void) {

    // Chave S1 (P2.1).

	ClearPort(P2, IFG, 1);	// Limpa a IFG de S1 para impedir uma interrupção imediata.
    SetPort(P2, IE, 1);     // S1 pode gerar interrupções.
    SetPort(P2, IES, 1);      // Interrupções ocorrem quando S1: 1 -> S1: 0.

    return 0;

}

int InterruptEnableS2(void) {

    // Chave S2 (P1.1).

	ClearPort(P1, IFG, 1);	// Limpa a IFG de S2 para impedir uma interrupção imediata.
    SetPort(P1, IE, 1);     // S2 pode gerar interrupções.
    SetPort(P1, IES, 1);      // Interrupções ocorrem quando S2: 1 -> S2: 0.

    return 0;

}

int WaitHalfSecond(void) {

    volatile uint32_t counter = 29250;

    while(counter != 0)
        counter--;

    return 0;

}

int Wait1Second(void) {
	
	WaitHalfSecond();
	WaitHalfSecond();

    return 0;
	
}

int Wait2Seconds(void) {
	
	Wait1Second();
	Wait1Second();
	
    return 0;
	
}

#endif // MSP430_SHORTCUTS_H_
