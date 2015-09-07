/******************************************************************************
 * DSPIC33E_hardware.c
 * This code contains routines to initialize  and configure
 * the dsPIC33EP512MU810 internal hardware and I/O pins.
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
 * Authors: Bill Peterson,
 * Changes:
 * 2014-Apr Develop code for dual encoders both with push switches
 *
 *****************************************************************************/

// #if defined(__dsPIC33E__)
// #include "p33exxxx.h"
// #elif defined(__PIC24E__)
// #include "p24exxxx.h"
// #endif
// #include "main.h"
#include "p33EP512MU810.h"
#include "DSPIC33E_hardware.h"
#include "si5351a.h"
/*******************************************************************************
* Initialize I2C #2
* 
* 
*******************************************************************************/
void Init_I2C2(void)	
{
    ODCFbits.ODCF4 = 1; // Open drain contol bits
    ODCFbits.ODCF5 = 1;

        I2C2CON = 0;			// stop & clear I2C unit
	I2C2STAT = 0;
	I2C2RCV	= 0;
	I2C2TRN	= 0;
	I2C2ADD	= 0;			// Master address
	I2C2MSK	= 0;

	I2C2BRG	= 511;      // set I2C clock rate 511=114kHz,135 = 400kHz

	I2C2CONbits.A10M = 0;	// 7-bit address
        I2C2CONbits.SCLREL = 1;
	I2C2CONbits.I2CEN = 1;	// enable I2C unit
}

/*******************************************************************************
* I2C #2 Write single byte
* 
* 
*******************************************************************************/
// Write one byte

void I2C2_Byte_Write	( unsigned char Write_Address, unsigned char Write_Data )
{
//	set_i2c_timeout();			// arm I2C timeout

	I2C2CONbits.SEN	= 1;			// start condition enable bit
	while (I2C2CON & 0b11111);
	
	I2C2TRN	= SI5351_BUS_BASE_ADDR;		// slave address
	while (I2C2STATbits.TRSTAT);		// wait while transmit status true
	
	I2C2TRN	= Write_Address;		// register address
	while (I2C2STATbits.TRSTAT);		// wait while transmit status true

	I2C2TRN	= Write_Data;			// data
	while (I2C2STATbits.TRSTAT);		// wait while transmit status true
	
	I2C2CONbits.PEN	= 1;			// stop condition enable bit
	while (I2C2CONbits.PEN);
}

/*******************************************************************************
* I2C #2 Read single byte
* 
* 
*******************************************************************************/	
// read one byte

void I2C2_Byte_Read ( unsigned char Read_Address )
{
	unsigned char	Read_Data;

//	set_i2c_timeout();					// arm I2C timeout
	
	I2C2CONbits.SEN	= 1;				// start condition
	while (I2C2CONbits.SEN);
	
	I2C2TRN	= SI5351_BUS_BASE_ADDR;			// slave address
	while (I2C2STATbits.TRSTAT);

	I2C2TRN	= Read_Address;				// register address
	while (I2C2STATbits.TRSTAT);

	I2C2CONbits.RSEN = 1;				// repeat start condition
	while (I2C2CONbits.RSEN);
	
	I2C2TRN	= SI5351_BUS_BASE_ADDR | 0x01;		// slave address again with Read bit
	while (I2C2STATbits.TRSTAT);

	while (I2C2STATbits.ACKSTAT);                   // wait for Ack

	I2C2CONbits.RCEN = 1;				// receive enable
	while (I2C2CONbits.RCEN);

	Read_Data = I2C2RCV;				// read data

	I2C2CONbits.ACKDT =	1;			// send Ack
	I2C2CONbits.ACKEN =	1;
	while (I2C2CONbits.ACKEN);

	I2C2CONbits.PEN	= 1;				// stop condition
	while (I2C2CONbits.PEN);

}

/*******************************************************************************
* Initialize SPI #1 Used for Audio Codec
*
*
*******************************************************************************/
void Init_SPI1()

{
    // setup the SPI1 peripheral
    SPI1STAT = 0x0;                     // disable the SPI module (just in case)
    SPI1CON1 = SPI1CON1_INIT;           // see header file for SPI1 configuration
    SPI1CON2 = SPI1CON2_INIT;           // see header file for SPI1 configuration
    SPI1STAT = 0x8000;                  // enable the SPI1 module
}

/*******************************************************************************
* Initialize SPI #2 Used for the LCD Display
* 
* 
*******************************************************************************/
void Init_SPI2()
//void Init_SPI2(int cke, int ckp)
{

    // setup the SPI2 peripheral
    SPI2STAT = 0x0;                     // disable the SPI module (just in case)
    IFS2bits.SPI2IF = 0;                //Clear interrupt flag
    SPI2STATbits.SISEL = 3;             //Select interrupt mode for SRRBF full
    IEC2bits.SPI2IE = 0;                //Enable SPI2 interrupt
    IPC8bits.SPI2IP = 2;                //Set priority one higher than main timer
    
    SPI2CON1 = SPI2CON1_INIT;           // see header file for SPI2 configuration
    SPI2CON2 = SPI2CON2_INIT;           // see header file for SPI2 configuration
    
    SPI2STATbits.SPIROV = 0;
    SPI2CON2bits.SPIBEN = 1;
    
    SPI2STATbits.SPIEN = 1;
// Another way to configure the SPI2CON1 register
//		SPI2CON1bits.CKP=0;         // Idle state for clock is a low level
//		SPI2CON1bits.CKE=1;         // Data out on Active to Idle Edge
//		SPI2CON1bits.MODE16=1       // 16-bit data transfer mode
//		SPI2CON1bits.MSTEN=1;       // Enable Master mode
//		SPI2CON1bits.SPRE=6;        // Set Secondary Pre-scalar for 2:1 ratio
//		SPI2CON1bits.PPRE =   2;    // Set Primary Pre-scalar for 4:1 ratio
// 		SPI2CON1bits.DISSDO = 0;    // Enable SDO output
//		SPI2CON1bits.DISSCK = 0;    // Enable SCK output

}

/*******************************************************************************
* Initialize SPI #3 Used for Ethernet
*
*
*******************************************************************************/
void Init_SPI3()

{
    // setup the SPI3 peripheral
    SPI3STAT = 0x0;                     // disable the SPI module (just in case)
    SPI3CON1 = SPI3CON1_INIT;           // see header file for SPI3 configuration
    SPI3CON2 = SPI3CON2_INIT;           // see header file for SPI3 configuration
    SPI3STAT = 0x8000;                  // enable the SPI3 module
}

/*******************************************************************************
* Initialize SPI #4 Used for EEPROM
*
*
*******************************************************************************/
void Init_SPI4()

{
    // setup the SPI4 peripheral
    SPI4STAT = 0x0;                     // disable the SPI module (just in case)
    SPI4CON1 = SPI4CON1_INIT;           // see header file for SPI4 configuration
    SPI4CON2 = SPI4CON2_INIT;           // see header file for SPI4 configuration
    SPI4STAT = 0x8000;                  // enable the SPI4 module
}


/*******************************************************************************
* Initialize DSPIC33E Oscillator
* 
* 
*******************************************************************************/
void Init_P33EP512MU810_osc()
{
// Configure Oscillator to operate the device at 120Mhz
   // Fosc= Fin*M/(N1*N2), Fcy=Fosc/2
   // Fosc= 12M*40/(2*2)=120Mhz for 8M input clock PLLFBD=58
   PLLFBD             = 38;         // PLL Feedback Divisor bits (M = PLLFBD + 2)
   
   CLKDIVbits.ROI     = 0;          // Recover on Interrupt, no effect on DOZEN
   CLKDIVbits.DOZE    = 0b000;      // Fcy divided by 1
   CLKDIVbits.DOZEN   = 0;          // DOZE disabled, peripheral clock ratio forced to 1:1
   CLKDIVbits.FRCDIV  = 0b000;      // Internal Fast RC divided by 1 (default)
   CLKDIVbits.PLLPOST = 0;          // N1=2
   CLKDIVbits.PLLPRE  = 0;          // N2=2

   OSCTUN             = 0;          // Tune FRC oscillator, if FRC is used

   REFOCONbits.ROON   = 0;          // Disable Reference Clock output before making changes
   REFOCONbits.ROSSLP = 0;          // Reference oscillator output is disabled in Sleep
   REFOCONbits.ROSEL  = 0;          // System clock used as the reference clock
   REFOCONbits.RODIV  = 0;          // Reference Oscillator Divider bits
   REFOCONbits.ROON   = 0;          // Enable Reference Clock if needed

   ACLKCON3bits.ENAPLL   = 0;       // Enable Auxiliary PLL (APLL) Disabled
   ACLKCON3bits.SELACLK  = 0;       // Select Auxiliary Clock Source
   ACLKCON3bits.AOSCMD   = 0;       // Auxiliary Oscillator Mode bits
   ACLKCON3bits.ASRCSEL  = 0;       // Select Reference Clock Source for APLL bit
   ACLKCON3bits.FRCSEL   = 0;       // Select FRC as Reference Clock Source for APLL bit
   ACLKCON3bits.APLLPOST = 0b000;   // Select PLL VCO Output Divider bits
   ACLKCON3bits.APLLPRE  = 0b000;   // PLL Phase Detector Input Divider bits

   ACLKDIV3bits.APLLDIV  = 0b000;   // PLL Feedback Divisor bits (PLL multiplier ratio)

   RCONbits.SWDTEN=0;               // Disable Watch Dog Timer if enabled in fuse bit

// Clock switching to incorporate PLL
   __builtin_write_OSCCONH(0x03);      // Clock Switch to Primary Osc with PLL (NOSC=0b011)
   //__builtin_write_OSCCONL(0x01);    // Start clock switching
   __builtin_write_OSCCONL(0x03);      // Start clock switching and turn on LP 32kHz oscillator
   while (OSCCONbits.COSC!= 0b011);    // Wait for Clock switch to occur
   while (OSCCONbits.LOCK!= 1);        // Wait for PLL to lock

}

/*******************************************************************************
* Initialize Reference Clock output used for Codec M-Clock
* Using 12MHz main oscillator clock passed straight through
*
*******************************************************************************/
void Init_REFCLK()

{
    // Setup REFOCON Register
    REFOCONbits.RODIV  = 0b0000;    // Reference Oscillator Divider bits
    REFOCONbits.ROSEL  = 0b1;       // Reference Oscillator Source Select bit
    REFOCONbits.ROSSLP = 0b0;       // Reference Oscillator Run in Sleep bit
    REFOCONbits.ROON   = 0b1;       // Reference Oscillator Output Enable
}

/*******************************************************************************
* Initialize DSPIC33E I/O pins
* 
* 
*******************************************************************************/
void Init_P33EP512MU810_pins()
{
    // Set general I/O
    TRISA = 0x0081;    // PortA 0000 0000 1000 0001 (In=RA0)
    PORTA = 0x0000;

    TRISB = 0x0000;    // PortB 0000 0000 0000 0000
    PORTB = 0x0000;

    TRISC = 0x001E;    // PortC 0000 0000 0001 1110 (In=RC1-RC4)
    PORTC = 0x0000;

    TRISD = 0x1010;    // PortD 0001 0000 0001 0000 (In RD4,RD12)
    PORTD = 0x0000;

    TRISE = 0x0008;    // PortE 0000 0000 0000 1000 (In RE3, )
    PORTE = 0x0000;

    TRISF = 0x0001;    // PortF 0000 0000 0000 0001 (In RF0)
    PORTF = 0x0000;

    // Configure LCD Data / Command/data Pin
    TRISG = 0x0280;    // PortG 0000 0010 1000 0000 (In=RG7,RG9)
    PORTG = 0x0000;

    ANSELA = 0x0000;   //Configure analog inputs
    ANSELB = 0x0000;
    ANSELC = 0x0000;
    ANSELD = 0x0000;
    ANSELE = 0x0000;
    ANSELG = 0x0000;

    CNPUA = 0x0000;    // Port A Pull Up 0000 0000 1100 0000
    CNPDA = 0x0000;    // Port A Pull Down
    CNPUB = 0x0000;    // Port B Pull Up 0000 0000 0000 0000
    CNPDB = 0x0000;    // Port B Pull Down
    CNPUC = 0x0000;    // Port C Pull Up 0000 0000 0000 0000
    CNPDC = 0x0000;    // Port C Pull Down
    CNPUD = 0x0000;    // Port D Pull Up 0011 0000 0011 0000
    CNPDD = 0x0000;    // Port D Pull Down
    CNPUE = 0x0000;    // Port E Pull Up 0000 0000 0000 0010
    CNPDE = 0x0000;    // Port E Pull Down
    // CNPUF = 0x0018;    // Port F Pull Up 0000 0000 0001 1000 Pull up I2C2 SCL SDA for testing
    CNPUF = 0x0000;    // Port F Pull Up 0000 0000 0000 0000
    CNPDF = 0x0000;    // Port F Pull Down
    CNPUG = 0x0000;    // Port G Pull Up 0000 0000 0000 0000
    CNPDG = 0x0000;    // Port G Pull Down

    // Map SPI1 peripheral pins
    RPINR20bits.SDI1R = 0b1100000;    //SPI1 MISO1 to RP96 Table 11-1, 11-2
    RPOR7bits.RP97R   = 0b000101;     //SPI1 MOSI1 to RP97 Table 11-3
    RPOR12bits.RP112R = 0b000110;     //SPI1 SCK1 to RP112 Table 11-3
    RPOR13bits.RP113R = 0b000111;     //SPI1 SS1 to RP113 Table 11-3

    // Map SPI2 SS pin, other pins not remapable on dsPIC33EP512MU810
    RPOR6bits.RP87R   = 0b001010;     //LCD_CS to RE7(RP87) Table 11-3

    // Map SPI3 peripheral pins
    RPINR29bits.SDI3R = 0b1001100;    //SPI3 MISO3 to RPI76 Table 11-1, 11-2
    RPOR1bits.RP67R   = 0b000101;     //SPI3 MOSI3 to RP67 Table 11-3
    RPOR1bits.RP66R   = 0b000110;     //SPI3 SCK3 to RP66 Table 11-3
    RPOR0bits.RP65R   = 0b000111;     //SPI3 SS3 to RP65 Table 11-3

    // Map SPI4 peripheral pins
    RPINR31bits.SDI4R = 0b1000100;    //SPI4 MISO4 to RP68 Table 11-1, 11-2
    RPOR3bits.RP71R   = 0b000101;     //SPI4 MOSI4 to RP71 Table 11-3
    RPOR3bits.RP70R   = 0b000110;     //SPI4 SCK4 to RP70 Table 11-3
    RPOR2bits.RP69R   = 0b000111;     //SPI4 SS4 to RP69 Table 11-3

    // Map Audio Codec clock, interrupt and DCI module pins
    RPINR24bits.CSDIR = 0b0010111;    // DCI CSDI to RA7 RPI23 Table 11-1, 11-2
    RPOR4bits.RP80R   = 0b001011;     // DCI CSDO to RE0 RP80 Table 11-3
    RPOR15bits.RP126R = 0b001101;     // DCI COFS to RG14 RP126 Table 11-3
    RPOR14bits.RP125R = 0b001100;     // DCI CSCK to RG13 RP125 Table 11-3
    RPOR5bits.RP82R   = 0b110001;     // REFCLK to RE2 RP82 Table 11-3 used for MCLK
    RPINR0bits.INT1R  = 0b1010011;    // EXT INT1 to RE3 RPI83 Table 11-1, 11-2

    // Quadrature Encoder Interface #1 pin connections
    //RPINR14bits.QEA1R  = 0b1000100;  // Connect QEI1 QEA1 input to RP68 RD4 (Table 11-2)
    //RPINR14bits.QEB1R  = 0b1000101;  // Connect QEI1 QEB1 input to RP69 RD5 (Table 11-2)
    //RPINR15bits.HOME1R = 0b1001100;  // Connect QEI1 HOME1 input to RPI76 RD12 (Table 11-2)
    //RPINR15bits.INDX1R = 0b1001101;  // Connect QEI1 INDEX1 input to RPI77 RD13 (Table 11-2
    RPINR14bits.QEA1R  = 0b0110010;  // Connect QEI1 QEA1 input to RPI50 RC2 (Table 11-2)
    RPINR14bits.QEB1R  = 0b0110100;  // Connect QEI1 QEB1 input to RPI52 RC4 (Table 11-2)
    RPINR15bits.HOME1R = 0b0000000;  // Connect QEI1 HOME1 input to Vss (Table 11-2)
    RPINR15bits.INDX1R = 0b0110011;  // Connect QEI1 INDEX1 input to RPI51 RC3 (Table 11-2

    // Quadrature Encoder Interface #2 pin connections
    //RPINR16bits.QEA2R  = 0b0010111;  // Connect QEI2 QEA2 input to RPI23 RA7 (Table 11-2)
    //RPINR16bits.QEB2R  = 0b0010110;  // Connect QEI2 QEB2 input to RPI22 RA6 (Table 11-2)
    //RPINR17bits.HOME2R = 0b1111100;  // Connect QEI2 HOME2 input to RPI124 RG12 (Table 11-2)
    //RPINR17bits.INDX2R = 0b1010001;  // Connect QEI2 INDEX2 input to RPI81 RE1 (Table 11-2)
    RPINR16bits.QEA2R  = 0b0010000;  // Connect QEI2 QEA2 input to RPI16 RA0 (Table 11-2)
    RPINR16bits.QEB2R  = 0b0110001;  // Connect QEI2 QEB2 input to RPI49 RC1 (Table 11-2)
    RPINR17bits.HOME2R = 0b0000000;  // Connect QEI2 HOME2 input to Vss (Table 11-2)
    RPINR17bits.INDX2R = 0b1111001;  // Connect QEI2 INDEX2 input to RPI121 RG9 (Table 11-2)
    
}


/*******************************************************************************
* Initialize Quadrature Encoder Interface #1 Hardware
* This input is used by the frequency encoder and has high frequency debounce
* done with on-chip hardware.
*******************************************************************************/
void Init_QEI_1()
{
    QEI1CONbits.QEIEN   = 0;      // Module counters are disabled
    QEI1CONbits.QEISIDL = 0;      // Continue module operation in Idle mode
    QEI1CONbits.PIMOD   = 0;      // Index input event does not affect position counter
    QEI1CONbits.IMV     = 0;      // Index input event does not affect position counter
    QEI1CONbits.INTDIV  = 0b111;  // Timer Input Clock Prescale 1:256
    QEI1CONbits.CNTPOL  = 0;      // Counter direction is positive
    QEI1CONbits.GATEN   = 0;      // External gate does not affect position counter
    QEI1CONbits.CCM     = 0b00;   // Quadrature Encoder mode

    QEI1IOCbits.QCAPEN  = 0;      // HOMEx input does not trigger position capture event
    QEI1IOCbits.FLTREN  = 1;      // Input Pin Digital filter is enabled
    QEI1IOCbits.QFDIV   = 0b111;  // Digital Input Filter Clock Divide 1:256
    QEI1IOCbits.OUTFNC  = 0;      // CNTCMPx output pin disabled
    QEI1IOCbits.SWPAB   = 0;      // QEAx and QEBx are not swapped
    QEI1IOCbits.HOMPOL  = 0;      // HOMEx Input Polarity not inverted
    QEI1IOCbits.IDXPOL  = 0;      // INDXx Input Polarity not inverted
    QEI1IOCbits.QEBPOL  = 0;      // QEBx Input Polarity not inverted
    QEI1IOCbits.QEAPOL  = 0;      // QEAx Input Polarity not inverted
    
    QEI1CONbits.QEIEN   = 1;      // Module counters are enabled
}

/*******************************************************************************
* Initialize Quadrature Encoder Interface #2 Hardware
* This input is used by the gain encoder and has high frequency debounce
* done with on-chip hardware.
*******************************************************************************/
void Init_QEI_2()
{
    QEI2CONbits.QEIEN   = 0;      // Module counters are disabled
    QEI2CONbits.QEISIDL = 0;      // Continue module operation in Idle mode
    QEI2CONbits.PIMOD   = 0;      // Index input event does not affect position counter
    QEI2CONbits.IMV     = 0;      // Index input event does not affect position counter
    QEI2CONbits.INTDIV  = 0b111;  // Timer Input Clock Prescale 1:256
    QEI2CONbits.CNTPOL  = 0;      // Counter direction is positive
    QEI2CONbits.GATEN   = 0;      // External gate does not affect position counter
    QEI2CONbits.CCM     = 0b00;   // Quadrature Encoder mode

    QEI2IOCbits.QCAPEN  = 0;      // HOMEx input does not trigger position capture event
    QEI2IOCbits.FLTREN  = 1;      // Input Pin Digital filter is enabled
    QEI2IOCbits.QFDIV   = 0b111;  // Digital Input Filter Clock Divide 1:256
    QEI2IOCbits.OUTFNC  = 0;      // CNTCMPx output pin disabled
    QEI2IOCbits.SWPAB   = 0;      // QEAx and QEBx are not swapped
    QEI2IOCbits.HOMPOL  = 0;      // HOMEx Input Polarity not inverted
    QEI2IOCbits.IDXPOL  = 0;      // INDXx Input Polarity not inverted
    QEI2IOCbits.QEBPOL  = 0;      // QEBx Input Polarity not inverted
    QEI2IOCbits.QEAPOL  = 0;      // QEAx Input Polarity not inverted

    QEI2CONbits.QEIEN   = 1;      // Module counters are enabled
}

/*******************************************************************************
* Initialize Pulse Width Modulators
* Period set to ~50 kHz which is above 48kHz sound card sample rate
* Currently only one of the 6 PWMs in use
* PWM1 - LCD Backlight control
*******************************************************************************/
void Init_PWM()
{
    // PTPER = Fosc /(desired period * PTCON2)
    // PTPER = 120MHz /(50kHz * 2) = 1200
    PTCON2 = 0x0001;                  // Prescaler 1=/2, 2=/4, 3=/8, 4=/16, 5=/32
    PTPER = 1200;                     // 50kHz PWM Period on Primary Time Base
    PHASE1 = 0;                       // PWM1 phase shift
    PHASE2 = 120;                     // PWM2 phase shift
    PHASE3 = 240;                     // PWM3 phase shift
    // PDCn = (desired duty cycle * PTPER)
    // PDC3 = 15000;                     // Default screen brightness to 50% Duty Cycle
    // PDC1 = PDC2 = 15000;              // Set to 50% duty, currently not used
    PDC3 = (PTPER / 2);               // Default screen brightness to 50% Duty Cycle
    PDC1 = PDC2 = (PTPER / 2);        // Set to 50% duty, currently not used
    DTR1 = DTR2 = DTR3 = 5;           // Set Dead Time Values
    ALTDTR1 = ALTDTR2 = ALTDTR3 = 5;  // Set Alternate Dead Time Values
    IOCON3 = 0xA000;                  // PWM3H pin 3 used for LCD backlight output
    IOCON1 = IOCON2 = 0x0000;         // PWM1 and PWM2 outputs Disabled
    //Set Primary Time Base, Edge-Aligned Mode and Independent Duty Cycles
    PWMCON1 = PWMCON2 = PWMCON3 = 0x0000;
    FCLCON1 = FCLCON2 = FCLCON3 = 0x0003; // Fault inputs disabled
    PTCON = 0x8000;                       // Enable PWM Module
}


/*******************************************************************************
* Initialize Real Time Clock and Calendar
* Prerequisites:
*     OSCCON register bit 2 is set to enable 32kHz secondary oscillator.
*
*
*******************************************************************************/
void Init_RTCC()
{
    __builtin_write_RTCWEN(); // Enable access to the RTC registers
    RCFGCALbits.RTCEN = 0;    // Temporarily disable RTC during changes
    ALCFGRPTbits.ALRMEN = 0;  // Disable the alarms
    RCFGCALbits.RTCOE = 1;    // Enable the RTCC output pin
    PADCFG1bits.RTSECSEL = 1; // Output the 1Hz signal on the RTCC pin
    //Default the date/time to 5/3/2014, 29:59
    RCFGCALbits.RTCPTR = 3;   // Start with the year and work down
    RTCVAL = 0x14;            // Year
    RTCVAL = 0x0503;          // month and day
    RTCVAL = 0x0123;          // WDAY<10:8> HRTEN<5:4> HRONE<3:0>
    RTCVAL = 0x5900;          // MINTEN<14:12> MINONE<11:8> SECTEN<6:4> SECONE<3:0>
    RCFGCALbits.RTCEN = 1;    // Enable the RTC
    RCFGCALbits.RTCWREN = 0;  // Stop future writes to the RTC control register
}

/******************************************************************************
 * Function:       void Init_Timer1( void )
 *
 * PreCondition:   None
 *
 * Input:          None
 *
 * Output:         None
 *
 * Side Effects:   None
 *
 * Overview:       Initialize Timer1 for Period Interrupts
 *                 At 120MHz, Fcy = 60MHz or 16.666nS
 *                 With prescaler set to /64 one timer tick = 1.066uS
 *                 Configure for 12mS cycle --> PR1 = 11250
 *****************************************************************************/
void Init_Timer1( void )
{
    T1CON = 0;          // Timer reset
    T1CONbits.TCKPS = TIMER_PRESCALER;// Set prescaler to /64
    IFS0bits.T1IF = 0;  // Reset Timer1 interrupt flag
    IPC0bits.T1IP = 1;  // Timer1 Interrupt priority level=1
    IEC0bits.T1IE = 1;  // Enable Timer1 interrupt
    TMR1 = 0x0000;
    PR1 = TIMER1_PERIOD;       // Timer1 period register - see macro
    T1CONbits.TON = 1;  // Enable Timer1 and start the counter
}