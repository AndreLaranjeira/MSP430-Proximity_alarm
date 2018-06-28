#include <msp430.h>
#include "../Include/MSP430_shortcuts.h"

int ConfigI2CMaster0(uint8_t ADDR, uint8_t clock, uint8_t BR0, uint8_t BR1) {
	
	UCB0CTL1 = UCSWRST;     // Put USCI-B0 in configuration mode.

	UCB0CTL0 = (UCMST | UCMODE_3 | UCSYNC);
	UCB0CTL1 |= (clock << 6);
	UCB0BR0 = BR0;
	UCB0BR1 = BR1;

	UCB0I2COA = ADDR;

	UCB0CTL1 &= ~UCSWRST;	// Bring USCI-B0 out of configuration mode.
	
	return 0;
	
}

int ConfigI2CMaster1(uint8_t ADDR, uint8_t clock, uint8_t BR0, uint8_t BR1) {
	
	UCB1CTL1 = UCSWRST;     // Put USCI-B0 in configuration mode.

	UCB1CTL0 = (UCMST | UCMODE_3 | UCSYNC);
	UCB1CTL1 |= (clock << 6);
	UCB1BR0 = BR0;
	UCB1BR1 = BR1;

	UCB1I2COA = ADDR;

	UCB1CTL1 &= ~UCSWRST;	// Bring USCI-B0 out of configuration mode.
	
	return 0;
	
}

int ConfigI2CB0Ports(void) {
	
	SetPort(P3, SEL, 0);
    SetPort(P3, DIR, 0);
    SetPort(P3, REN, 0);
    SetPort(P3, OUT, 0);

    SetPort(P3, SEL, 1);
    SetPort(P3, DIR, 1);
    SetPort(P3, REN, 1);
    SetPort(P3, OUT, 1);
	
	return 0;
	
}

int ConfigI2CB1Ports(void) {
	
    SetPort(P4, SEL, 1);
    SetPort(P4, DIR, 1);
    SetPort(P4, REN, 1);
    SetPort(P4, OUT, 1);

    SetPort(P4, SEL, 2);
    SetPort(P4, DIR, 2);
    SetPort(P4, REN, 2);
    SetPort(P4, OUT, 2);
	
	return 0;
	
}

int ConfigI2CSlave0(uint8_t ADDR) {
	
	UCB0CTL1 = UCSWRST;     // Put USCI-B0 in configuration mode.

	UCB0CTL0 = (UCMODE_3 | UCSYNC);
	UCB0CTL1 |= 0;

	UCB0I2COA = ADDR;

	UCB0CTL1 &= ~UCSWRST;	// Bring USCI-B0 out of configuration mode.
	
	return 0;
	
}

int ConfigI2CSlave1(uint8_t ADDR) {
	
	UCB1CTL1 = UCSWRST;     // Put USCI-B0 in configuration mode.

	UCB1CTL0 = (UCMODE_3 | UCSYNC);
	UCB1CTL1 |= 0;

	UCB1I2COA = ADDR;

	UCB1CTL1 &= ~UCSWRST;	// Bring USCI-B0 out of configuration mode.
	
	return 0;
	
}

int ConfigLCDM0(uint8_t ADDR, uint8_t BL) {
	
	I2CM0StartTransmission(ADDR);

    LCDM0Start4BitMode(LCD_WAIT0);

    LCDM0SendByte(0x28, BL, LCD_INST, LCD_WAIT1);	// Utilize both display lines;
    LCDM0SendByte(0x08, BL, LCD_INST, LCD_WAIT1);	// Shut display off;
    LCDM0SendByte(0x01, BL, LCD_INST, LCD_WAIT3);	// Clear LCD;
    LCDM0SendByte(0x06, BL, LCD_INST, LCD_WAIT1);	// Puts display in entry mode with cursor moving to the right. No display shifiting.
    LCDM0SendByte(0x0C, BL, LCD_INST, LCD_WAIT1);	// Turns display on with cursor blinking.

    I2CM0StopTransmission();
	
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

int Debounce(void) {

    volatile int counter = 0xDFFF;

    while(counter != 0)
        counter--;

    return 0;

}

int DelayMicrosseconds(uint16_t number) {
	
	TA2CTL = TimerAConfiguration(SMCLK, 2);
	
	while(TA2R <= number);
	
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

int I2CM0SendData(uint8_t data, uint16_t delay) {
	
	while((UCB0IFG & UCTXIFG) == 0);
    UCB0TXBUF = data;
    while((UCB0CTL1 & UCTXSTT) != 0);
	
	if((UCB0IFG & UCNACKIFG) != 0)
		return -1;
		
	DelayMicrosseconds(delay);
	
	return 0;
	
}

int I2CM0StartTransmission(uint8_t ADDR) {
	
	UCB0I2CSA = ADDR;

    while((UCB0STAT & UCBBUSY) != 0);

    UCB0CTL1 |= (UCTR | UCTXSTT);
	
	return 0;
	
}

int I2CM0StopTransmission(void) {
	
    UCB0CTL1 |= UCTXSTP;
    while((UCB0CTL1 & UCTXSTP) != 0);
	
	return 0;
	
}

int I2CM0TransmitDataSet(uint8_t ADDR, uint8_t *data_set,
						 uint16_t length, uint16_t data_delay) {
	
	I2CM0StartTransmission(ADDR);

	while(length != 0) {
		
		if(I2CM0SendData(*data_set, data_delay) == -1)
			return length;
		
		else {
			data_set++;
			length--;
		}
		
	}
	
	I2CM0StopTransmission();
	
	return 0;
	
}

int LCDM0BacklightOFF(void) {
	
	LCDM0SendByte(0x0C, LCD_BL_OFF, LCD_INST, LCD_WAIT3);
	return 0;
	
}

int LCDM0BacklightON(void) {
	
	LCDM0SendByte(0x0C, LCD_BL_ON, LCD_INST, LCD_WAIT3);
	return 0;
	
}

int LCDM0CreateCharacter(uint8_t char_ADDR, uint8_t *design) {
	
	unsigned int i;
	
	uint8_t CGRAM_ADDR;
	uint8_t CGRAM_prefix = (char_ADDR << 3);
	uint8_t line;
	
	if(char_ADDR > 8)
		return -1;
	
	for(i = 0; i < 8; i++) {
	
		line = *design;									// Get one line of the new character.
		design++;										// Increment design to point to next line;
		
		CGRAM_ADDR = (CGRAM_prefix | i); 				// Configure address of the line.
		
		LCDM0SendByte((0x40 | CGRAM_ADDR), 0, LCD_INST, LCD_WAIT1);	// Point cursor to address of the line.
		LCDM0SendByte(line, 0, LCD_DATA, LCD_WAIT2);					// Write line to address.
		
	}
	
	return 0;
	
}

int LCDM0SendByte(uint8_t byte, uint8_t BL, uint8_t command, uint16_t delay) {
	
	LCDM0SendNibble((byte >> 4), BL, command);
	DelayMicrosseconds(LCD_WAIT0);
	LCDM0SendNibble((byte & 0x0F), BL, command);
	
	DelayMicrosseconds(delay);
	
	return 0;
	
}

int LCDM0SendNibble(uint8_t nibble, uint8_t BL, uint8_t command) {
	
	I2CM0SendData(((nibble << 4) | BL | command), LCD_WAIT0);
	I2CM0SendData(((nibble << 4) | BL | command | LCD_EN), LCD_WAIT0);
	I2CM0SendData(((nibble << 4) | BL | command), LCD_WAIT0);
	
	return 0;
	
}

int LCDM0Start4BitMode(uint16_t delay) {
	
	unsigned int i;
	uint8_t sequenceFor4BitMode[12] = {0x30, 0x34, 0x30, 0x30, 0x34, 0x30,
									   0x30, 0x34, 0x30, 0x20, 0x24, 0x20};
	
	for(i = 0; i < 12; i++)
        I2CM0SendData(sequenceFor4BitMode[i], delay);
	
	return 0;
	
}

int LCDM0UpdatePositions(uint8_t left, uint8_t right, uint8_t down, uint8_t up) {
	
	uint8_t i;
	
	// First line:
	
	LCDM0SendByte(0x80, LCD_BL_ON, LCD_INST, LCD_WAIT1); 	// Put cursor on address 0x00.
	
	for(i = (7 - left); i != 0; i--)
		LCDM0SendByte(LCD_SPACE, LCD_BL_ON, LCD_DATA, LCD_WAIT2);	// Write the spaces of the left bar.
	
	for(i = left; i != 0; i--)
		LCDM0SendByte(LCD_SQUARE, LCD_BL_ON, LCD_DATA, LCD_WAIT2);	// Write the squares of the left bar.
	
	LCDM0SendByte(0x89, LCD_BL_ON, LCD_INST, LCD_WAIT1); 	// Put cursor on address 0x09.
	
	for(i = right; i != 0; i--)
		LCDM0SendByte(LCD_SQUARE, LCD_BL_ON, LCD_DATA, LCD_WAIT2);	// Write the squares of the right bar.
	
	for(i = (7 - right); i != 0; i--)
		LCDM0SendByte(LCD_SPACE, LCD_BL_ON, LCD_DATA, LCD_WAIT2);	// Write the spaces of the right bar.
	
	// Second line:
	
	LCDM0SendByte(0xC0, LCD_BL_ON, LCD_INST, LCD_WAIT1); 	// Put cursor on address 0x40.
	
	for(i = (7 - down); i != 0; i--)
		LCDM0SendByte(LCD_SPACE, LCD_BL_ON, LCD_DATA, LCD_WAIT2);	// Write the spaces of the down bar.
	
	for(i = down; i != 0; i--)
		LCDM0SendByte(LCD_SQUARE, LCD_BL_ON, LCD_DATA, LCD_WAIT2);	// Write the squares of the down bar.
	
	LCDM0SendByte(0xC9, LCD_BL_ON, LCD_INST, LCD_WAIT1); 	// Put cursor on address 0x49.
	
	for(i = up; i != 0; i--)
		LCDM0SendByte(LCD_SQUARE, LCD_BL_ON, LCD_DATA, LCD_WAIT2);	// Write the squares of the up bar.
	
	for(i = (7 - up); i != 0; i--)
		LCDM0SendByte(LCD_SPACE, LCD_BL_ON, LCD_DATA, LCD_WAIT2);	// Write the spaces of the up bar.
	
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

void ConfigUART(){ // 9600 Baud Rate

	P3DIR |= BIT3;  // P3.3 TX
	P3DIR &= ~BIT4; // P3.4 RX
  P3SEL |= (BIT3 | BIT4);
  
  UCA0CTL1 |= UCSWRST; // Reset UCA0 module
  
	UCA0CTL0 = 0;
	UCA0CTL1 |= UCSSEL__ACLK;

	UCA0BRW = 3;

	UCA0MCTL = UCBRF_0 | UCBRS_3;

	UCA0CTL1 &= ~UCSWRST;

	UCA0IE = UCRXIE;
  
}

void UARTSendString(char *data){
	while(*data){
		UCA0TXBUF = *(data++);
    while(UCA0STAT & UCBUSY);		
	}
}
