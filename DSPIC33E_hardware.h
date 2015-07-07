/******************************************************************************
 * Header file for DSPIC33E_hardware.c
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 *****************************************************************************/

// Board specific definitions
#ifndef __DSPIC33E_HARDWARE_H
#define __DSPIC33E_HARDWARE_H


// all sort of CPU clock dependent constants
// define FCY, system cycle rate, in the 33F chip Fosc / 2
// #define FCY 29491200UL	// 29,4912 MHz
// #define FCY 38707200UL	// 38,7072 MHz


// define general I/O signals
// Use LAT for write only operations and PORT for read operations
#define	LCD_CS     LATGbits.LATG14    //LCD Slave Select on RG14(RP126)
#define	LCD_REST   LATGbits.LATG1     //LCD Reset on RG1(RP113)
#define	LCD_DC     LATGbits.LATG13    //LCD Data/Command on RG13(RP125)
#define	Debug_0    LATDbits.LATD0     //Debug_0 on RD0(RP64)
#define	Button_1  PORTGbits.RG15      //Push Button on RG15
#define	Button_2  PORTBbits.RB2       //Push Button on RB2

// SPI2CON1 Configuration
// 15-13    Not Used    = 000
// 12       DISSCK      = 0
// 11       DISSDO      = 0
// 10       MODE16      = 0
//  9       SMP         = 0
//  8       CKE         = 1         Not used in framed mode
//  7       SSEN        = 0
//  6       CKP         = 1
//  5       MSTEN       = 1         Master Mode Enable
//  4-2     SPRE        = 1 01      000=8 thru 111=1
//  1-0     PPRE        = 10        00=64, 01=16, 10=4, 11=1
//#define	SPI2CON1_INIT	0x0061
//#define	SPI2CON1_INIT	0x0161  //works with spi at 475 khz
#define	SPI2CON1_INIT	0x0176  //clk at 60Mhz/12 = 5Mhz

// SPI2CON2 Configuration
// 15       FRMEN       = 0         Frame Mode Enable
// 14       SPIFSD      = 0
// 13       FRMPOL      = 0
// 12-2     NOT USED    = 0 0000 0000 00
//  1       FRMDLY      = 0
//  0       SPIBEN      = 0
//#define	SPI2CON2_INIT	0x8001
#define	SPI2CON2_INIT	0x0000

// Declare functions available
void Init_SPI2(void);
void Init_P33EP512MU810_osc(void);
void Init_P33EP512MU810_pins(void);
void Init_QEI_1(void);
void Init_QEI_2(void);
void Init_PWM();
void Init_RTCC();

void Init_I2C2(void);                                   // Init I2C system
void I2C2_Byte_Write(unsigned char, unsigned char);     // write byte, (address, data)
//unsigned char I2C2_Byte_Read(unsigned char);		// read (address)
void I2C2_Byte_Read(unsigned char);		// read (address)
#endif
