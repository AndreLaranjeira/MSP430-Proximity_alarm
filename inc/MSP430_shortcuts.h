// MSP430 shortcuts module - Header file.

// Define guard:
#ifndef MSP430_SHORTCUTS_H_
#define MSP430_SHORTCUTS_H_

// Includes:
#include <msp430.h>
#include <stdlib.h>
#include <stdint.h>

// Macros:
#define I2C_READ 0x00
#define I2C_WRITE UCTR
#define LCD_BL_ON 0x08
#define LCD_BL_OFF 0x00
#define LCD_DATA 0x01
#define LCD_EN 0x04
#define LCD_INST 0x00
#define LCD_SPACE 0xFE
#define LCD_SQUARE 0xFF
#define LCD_WAIT0 80
#define LCD_WAIT1 45
#define LCD_WAIT2 50
#define LCD_WAIT3 1700
#define SM_CLK 2

// Macro functions:
#define CaptureMode(mode) (CAP | (CM_##mode) | CCIS_0 | CCIE)
#define ClearPort(port, type, pin) ((port##type) &= ~(BIT##pin))
#define ComparePortEQ(port, type, pin, value) (((port##type) & (BIT##pin)) == value)
#define ComparePortNE(port, type, pin, value) (((port##type) & (BIT##pin)) != value)
#define FLLN(x) ((x)-1)
#define LowPowerMode(number) (__bis_SR_register(LPM##number##_bits))
#define ReadPort(port, type, pin) ((port##type) & (BIT##pin))  
#define SetPort(port, type, pin) ((port##type) |= (BIT##pin))
#define TimerAConfiguration(clock, mode) (TACLR | (TASSEL__##clock) | (MC_##mode))
#define TogglePort(port, type, pin) ((port##type) ^= (BIT##pin))

// Interrupt vector addresses:
#define TB0_CCR0_INT 59
#define TB0_CCRN_INT 58
#define USCI_A0_INT 56
#define USCI_B0_INT 55
#define ADC12_A_INT 54
#define TA0_CCR0_INT 53
#define TA0_CCRN_INT 52
#define TA1_CCR0_INT 49
#define TA1_CCRN_INT 48
#define USCI_A1_INT 46
#define USCI_B1_INT 45
#define TA2_CCR0_INT 44
#define TA2_CCRN_INT 43

// Function headers:
int ConfigI2CMaster0(uint8_t, uint8_t, uint8_t, uint8_t);
int ConfigI2CMaster1(uint8_t, uint8_t, uint8_t, uint8_t);
int ConfigI2CB0Ports(void);
int ConfigI2CB1Ports(void);
int ConfigI2CSlave0(uint8_t);
int ConfigI2CSlave1(uint8_t);
int ConfigLCDM0(uint8_t, uint8_t);
int ConfigLED1(void);
int ConfigLED2(void);
int ConfigS1(void);
int ConfigS2(void);
int ConfigUARTModule0(uint8_t, uint16_t, uint8_t, uint8_t);
int Debounce(void);
int DelayMicrosseconds(uint16_t);
int Delay40Microsseconds(uint16_t);
int DelaySeconds(uint8_t);
int InitializePorts(void);
int InterruptDisableS1(void);
int InterruptDisableS2(void);
int InterruptEnableS1(void);
int InterruptEnableS2(void);
uint8_t I2CM0ReceiveData(void);
int I2CM0RestartTransmission(uint8_t, uint8_t);
int I2CM0SendData(uint8_t, uint16_t);
int I2CM0StartTransmission(uint8_t);
int I2CM0StopTransmission(void);
int I2CM0TransmitDataSet(uint8_t, uint8_t*, uint16_t, uint16_t);
int LCDM0BacklightOFF(void);
int LCDM0BacklightON(void);
int LCDM0CreateCharacter(uint8_t, uint8_t*);
int LCDM0SendByte(uint8_t, uint8_t, uint8_t, uint16_t);
int LCDM0SendNibble(uint8_t, uint8_t, uint8_t);
int LCDM0Start4BitMode(uint16_t);
int LCDM0UpdatePositions(uint8_t, uint8_t, uint8_t, uint8_t);
int UARTM0SendString(char*, uint16_t);

#endif // MSP430_SHORTCUTS_H_
