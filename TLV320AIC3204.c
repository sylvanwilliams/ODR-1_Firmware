/*******************************************************************************
* Driver Code the Texas Instruments TLV320AIC3204 audio codec
*
*
* Authors: Bill Peterson,
* Changes:
* 2015-Aug Initial code writing
*
*******************************************************************************/
#include "TLV320AIC3204.h"
#include "p33EP512MU810.h"
#include "DSPIC33E_hardware.h"
#include "LCD_driver.h" // only needed for the delay function
#include "UI_main.h" // Only needed to print chars to screen for debug

/*******************************************************************************
* Audio Codec SPI read function
* The codec is on SPI1 bus all by itself
* A 16 bit word can be used here to clock an 8-bit command followed by 8-bit data
* First byte is 7-bit register address plus low order bit set to 1 for read
* Second byte is set to all zeros which will clock in the data on MISO
*******************************************************************************/
uint8 Codec_Read_SPI(uint8 address)
{
    uint16 temp;
    CODEC_CS = 0;                      // Lower the slave select line
    temp = SPI1BUF;                    // Dummy read to clear SPIRBF
    temp = ((address << 1)+1);         // Shift 7-bit address left 1 bit add 1
    temp = (temp << 8);                // Shift address to high order byte
    SPI1BUF = temp;                    // Send read command
    while (!SPI1STATbits.SPIRBF);      // Wait for receive buffer full
    CODEC_CS = 1;                      // Raise the slave select line
    return (uint8)(SPI1BUF & 0x00FF);  // Read 16bit number return low byte data
}


/*******************************************************************************
* Audio Codec SPI write function
* The codec is on SPI1 bus all by itself
* A 16 bit word can be used here to clock an 8-bit command followed by 8-bit data
* First byte is 7-bit register address plus low order bit set to 0 for write
* Second byte is the 8-bit data to store in the register
*******************************************************************************/
void Codec_Write_SPI(uint8 address, uint8 data)
{
    uint16 temp;
    CODEC_CS = 0;                     // Lower the slave select line
    temp = SPI1BUF;                   // Dummy read to clear SPIRBF
    temp = (address << 1);            // Shift 7-bit address left 1 bit
    temp = ((temp << 8)+ data);       // Shift address to high order byte, add data
    SPI1BUF = temp;                   // Send address and data
    while (!SPI1STATbits.SPIRBF);     // Wait for receive buffer full
    CODEC_CS = 1;                     // Raise the slave select line
}


/*******************************************************************************
* Initialize Audio Codec
*
*
*******************************************************************************/
void Init_Audio_Codec(void)
{
    // uint16 temp2 = 0; Debug variable

    // Reset Audio Codec
    CODEC_RST = 0;
    delayms(2);    // Delay for 2mS for reset of codec
    CODEC_RST = 1;
    delayms(2);    // Delay for 2mS before first write

    Codec_Write_SPI (0, 0);           // Select page 0
    Codec_Write_SPI (27, 0b01111101); // Audio Interface Set 1 WCLK & BCLK out, 32 bit DSP
    Codec_Write_SPI (28, 0b00000000); // Audio Interface Set 2 Data Offset = 0 BCLKs default
    Codec_Write_SPI (29, 0b00000000); // Audio Interface Set 3 default
	
	// PLL clock of 98.304 MHz derrived from a 12MHz MCLK input
    Codec_Write_SPI (4,  0b00000011); // Clock Setting Register 1, PLL Mux PLL_CLKIN = MCLK = 12MHZ 
    Codec_Write_SPI (6,  0b00001000); // Clock Setting Register 3, PLL J Values J=8
    Codec_Write_SPI (7,  0b00000111); // Clock Setting Register 4, PLL D Values (MSB) D=0.1920
    Codec_Write_SPI (8,  0b10000000); // Clock Setting Register 5, PLL D Values (LSB) D=0.1920
    Codec_Write_SPI (30, 0b10110000); // Clock Setting Register 12, BCLK N Divider
    Codec_Write_SPI (5,  0b10010001); // Clock Setting Register 2, PLL P and R Values P=1, R=1
	
	// DAC and ADC Clocks set to 8kHz fs
    Codec_Write_SPI (13, 0b00000011); // DAC OSR Setting Register 1, MSB In=6.144MHz, Out=8kHz 
    Codec_Write_SPI (14, 0b00000000); // DAC OSR Setting Register 2, LSB Out=8kHz
    Codec_Write_SPI (20, 0b10000000); // ADC Oversampling (AOSR) In=1.024MHz Out=8kHz
    Codec_Write_SPI (11, 0b10001000); // Clock Setting Register 6, NDAC In=98.304MHz, Out=12.288MHz
    Codec_Write_SPI (12, 0b10000010); // Clock Setting Register 7, MDAC In=12.288MHz, Out=6.144MHz
    Codec_Write_SPI (18, 0b10001000); // Clock Setting Register 8, NADC In=98.304MHz, Out=12.288MHz
    Codec_Write_SPI (19, 0b10001100); // Clock Setting Register 9, MADC In=12.288MHz, Out=1.024MHz

    Codec_Write_SPI (64, 0b00000010); // DAC Channel Setup Register 2
    Codec_Write_SPI (65, 0b00000000); // Left DAC Channel Digital Volume
    Codec_Write_SPI (63, 0b11010100); // DAC Channel Setup Register 1

    //Codec_Write_SPI (81, 0b00000010); // ADC Channel Setup, disable soft stepping

    Codec_Write_SPI (0, 1);           // Select page 1
    Codec_Write_SPI (1,  0b00001000); // Disable connection of AVDD with DVDD
    Codec_Write_SPI (2,  0b00000001); // AVDD LDO Powered up 1.72V
    Codec_Write_SPI (10, 0b00000000); // Common Mode Control CM 0.9V LDOIN 1.8-3.6V
    Codec_Write_SPI (9,  0b00110011); // Output Driver Power Control HPL HPR MAL MAR on
    delayms(1000);    // Delay for supply rail powerup

    Codec_Write_SPI (52, 0b00010000); // Left MICPGA Pos Input Routing IN2L+ RX_I+
    Codec_Write_SPI (54, 0b00010000); // Left MICPGA Neg Input Routing IN2R- RX_I-
    Codec_Write_SPI (55, 0b01000000); // Right MICPGA Pos Input Routing IN1R+ RX_Q+
    Codec_Write_SPI (57, 0b00010000); // Right MICPGA Neg Input Routing IN1L- RX_Q-
    Codec_Write_SPI (58, 0b00001100); // Floating Input Configuration IN3L IN3R
    Codec_Write_SPI (71, 0b00110010); // Analog Input Quick Charging 6.4ms

    Codec_Write_SPI (12, 0b00000010); // HPL Routing Selection MAL
    Codec_Write_SPI (13, 0b00000010); // HPR Routing Selection MAR

    //Codec_Write_SPI (12, 0b00000100); // HPL Routing Selection IN1L
    //Codec_Write_SPI (13, 0b00000100); // HPR Routing Selection IN1R

    Codec_Write_SPI (59, 0b00001100); // Left MICPGA Volume 6dB to 53dB diff gain
    Codec_Write_SPI (60, 0b00001100); // Right MICPGA Volume 6dB to 53dB diff gain

    Codec_Write_SPI (16, 0b00001111); // HPL Driver Gain Setting +15dB
    Codec_Write_SPI (17, 0b00001111); // HPR Driver Gain Setting +15dB

    Codec_Write_SPI (22, 0b01110101); // IN1L to HPL Volume 0dB to -72dB
    Codec_Write_SPI (23, 0b01110101); // IN1R to HPR Volume 0dB to -72dB
    
    Codec_Write_SPI (24, 0b00000000); // Mixer Amp Left Volume 0dB to -30dB
    Codec_Write_SPI (25, 0b00000000); // Mixer Amp Right Volume 0dB to -30dB

    //temp2 = Codec_Read_SPI(30); //Test read function

    //Debug screen printing
    //POINT_COLOR = char_norm_color;
    //BACK_COLOR = field_color;
    //LCD_16wz_Num(160,10,temp2,5);
    //LCD_16nz_Num(10,10,temp2,5);
}

