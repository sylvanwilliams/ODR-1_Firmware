/*******************************************************************************
* Driver Functions for Texas Instruments TLV320AIC3204 audio codec
*
* This work is licensed under the Creative Commons Attribution-ShareAlike
* 4.0 International License. To view a copy of this license, visit
* http://creativecommons.org/licenses/by-sa/4.0/ or send a letter to
* Creative Commons, PO Box 1866, Mountain View, CA 94042, USA.
*******************************************************************************/

#include "TLV320AIC3204.h"
#include "p33EP512MU810.h"
#include "DSPIC33E_hardware.h"
#include "LCD_driver.h" // only needed for the delay function
#include "UI_main.h" // Only needed to print chars to screen for debug

/*******************************************************************************
* Audio Codec SPI read function
* The codec is on SPI1 bus all by itself
* A 16 bit word is used to clock an 8-bit command followed by 8-bit data
* MSB is 7-bit register address plus low order bit set to 1 for read
* LSB is set to all zeros which will clock in the data on MISO
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
* MSB is 7-bit register address plus low order bit set to 0 for write
* LSB is the 8-bit data to store in the register
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
    Codec_Write_SPI (27, 0b01001101); // Audio Interface Set 1 WCLK & BCLK out, 16 bit DSP
    Codec_Write_SPI (28, 0b00000000); // Audio Interface Set 2 Data Offset = 0 BCLKs
    Codec_Write_SPI (29, 0b00000000); // Audio Interface Set 3 default
//    Codec_Write_SPI (29, 0b00010000); // Audio Interface Set 3 Send ADC to DAC
	
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

    Codec_Write_SPI (81, 0b11000000); // ADC Channel Setup, power-up ADCs
    Codec_Write_SPI (82, 0b00000000); // Unmute ADCs (muted at reset!)

    Codec_Write_SPI (0, 1);           // Select page 1
    Codec_Write_SPI (1,  0b00001000); // Disable connection of AVDD with DVDD
    Codec_Write_SPI (2,  0b00000001); // AVDD LDO Powered up 1.72V
    Codec_Write_SPI (3,  0b00001000); // Left DAC is Class AB and PTM_P1
    Codec_Write_SPI (4,  0b00001000); // Right DAC is Class AB and PTM_P1
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
    
//    Codec_Write_SPI (12, 0b00001000); // HPL Routing Selection LDAC+
//    Codec_Write_SPI (13, 0b00001000); // HPR Routing Selection RDAC+

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

/*******************************************************************************
* Codec_HP_Gain
*
* Sets gain of L & R Headphone Amplifiers
* Gain range is -6dB to +29dB in 1dB/bit increments (decimal -7 to 29)
* Anything <-6dB will cause output to be muted
* 
*******************************************************************************/
void Codec_HP_Gain (int8 HP_Gain)
{
    if (HP_Gain < -6) HP_Gain = 64;              // Mute output
    else if (HP_Gain < 0) HP_Gain &= 0b00111111; // Set gain negative
    else if (HP_Gain > 29) HP_Gain = 29;         // Set to 29dB maximum
	
    Codec_Write_SPI (0, 1);                      // Select page 1
    Codec_Write_SPI (16, HP_Gain);               // HPL Driver Gain Setting
    Codec_Write_SPI (17, HP_Gain);               // HPR Driver Gain Setting
}

/*******************************************************************************
* Codec_LO_Gain
*
* Sets gain of L & R Line Out Amplifiers used for transmit I & Q audio
* Gain range is -6dB to +29dB in 1dB/bit increments (decimal -7 to 29)
* Anything <-6dB will cause output to be muted
*
*******************************************************************************/
void Codec_LO_Gain (int8 LO_Gain)
{
    if (LO_Gain < -6) LO_Gain = 64;              // Mute output
    else if (LO_Gain < 0) LO_Gain &= 0b00111111; // Set gain negative
    else if (LO_Gain > 29) LO_Gain = 29;         // Set to 29dB maximum
	
    Codec_Write_SPI (0, 1);                      // Select page 1
    Codec_Write_SPI (18, LO_Gain);               // LOL Driver Gain Setting
    Codec_Write_SPI (19, LO_Gain);               // LOR Driver Gain Setting
}

/*******************************************************************************
* Codec_MICPGA_Gain
*
* Sets gain of L & R Microphone Programmable Gain Amplifiers
* Gain range is 0dB to +47.5dB in 0.5dB/bit increments (decimal 0 to 95)
* Using differential inputs to the PGA provides an additional 6dB of gain.
*
*******************************************************************************/
void Codec_MICPGA_Gain (uint8 MICPGA_Gain)
{
    if (MICPGA_Gain > 95) MICPGA_Gain = 95; // Set to 47.5dB maximum
	
    Codec_Write_SPI (0, 1);                 // Select page 1
    Codec_Write_SPI (59, MICPGA_Gain);      // Left MICPGA Volume gain
    Codec_Write_SPI (60, MICPGA_Gain);      // Right MICPGA Volume gain
}

/*******************************************************************************
* Codec_DAC_Gain
*
* Sets digital gain of L & R DAC outputs
* Gain range is -63.5dB to +24dB in 0.5dB/bit increments (decimal -127 to +48)
* 
*
*******************************************************************************/
void Codec_DAC_Gain (int8 DAC_Gain)
{
    if (DAC_Gain < -127) DAC_Gain = -127;   // Set to -63.5dB min
    else if (DAC_Gain > 48) DAC_Gain = 48;  // Set to +24.0dB max
	
    Codec_Write_SPI (0, 0);                 // Select page 0
    Codec_Write_SPI (65, DAC_Gain);         // Left DAC Gain Setting
    Codec_Write_SPI (66, DAC_Gain);         // Right DAC Gain Setting
}

/*******************************************************************************
* Codec_ADC_Gain
*
* Sets digital gain of L & R ADC outputs
* Gain range is -12.0dB to +20dB in 0.5dB/bit increments (decimal -24 to +40)
* 
*
*******************************************************************************/
void Codec_ADC_Gain (int8 ADC_Gain)
{
    if (ADC_Gain < -24) ADC_Gain = 104;            // Set to -12.0dB min
    else if (ADC_Gain < 0) ADC_Gain &= 0b01111111; // Set gain negative
    else if (ADC_Gain > 40) ADC_Gain = 40;         // Set to +20.0dB max
	
    Codec_Write_SPI (0, 0);                        // Select page 0
    Codec_Write_SPI (83, ADC_Gain);                // Left ADC Gain Setting
    Codec_Write_SPI (84, ADC_Gain);                // Right ADC Gain Setting
}

/*******************************************************************************
* Codec_Mic_Bias
*
* Turn Codec Mic Bias on (1) or off (0)
* 
* 
*
*******************************************************************************/
void Codec_Mic_Bias (uint8 Mic_Bias)
{
    Codec_Write_SPI (0, 1);                // Select page 1
    if (Mic_Bias)
    {
        Codec_Write_SPI (51, 0b01100000);  // Turn on Mic Bias 2.5V
    }
    else
    {
        Codec_Write_SPI (51, 0b00000000);  // Turn off Mic Bias
    }
}

/*******************************************************************************
* Codec_Config_RX
*
* Configure Codec for Receive
* 
* 
*
*******************************************************************************/
void Codec_Config_RX (void)
{
    Codec_Write_SPI (0, 1);           // Select page 1
    Codec_Write_SPI (52, 0b00010000); // Left MICPGA Pos Input Routing IN2L+ RX_I+
    Codec_Write_SPI (54, 0b00010000); // Left MICPGA Neg Input Routing IN2R- RX_I-
    Codec_Write_SPI (55, 0b01000000); // Right MICPGA Pos Input Routing IN1R+ RX_Q+
    Codec_Write_SPI (57, 0b00010000); // Right MICPGA Neg Input Routing IN1L- RX_Q-
    Codec_Write_SPI (58, 0b00001100); // Floating Inputs connect to common mode
}

/*******************************************************************************
* Codec_Config_TX_Mic
*
* Configure Codec for Transmit using Mic input
* IN3L used for MIC input routed to Left MICPGA + input.
* 
*
*******************************************************************************/
void Codec_Config_TX_Mic (void)
{
    Codec_Write_SPI (0, 1);           // Select page 1
    Codec_Write_SPI (58, 0b00110100); // Floating Inputs connect to common mode
    Codec_Write_SPI (52, 0b00001100); // Left MICPGA Pos Input Routing IN3L+ 40k
    Codec_Write_SPI (54, 0b11000000); // Left MICPGA Neg Input Routing CM1L 40k
    Codec_Write_SPI (55, 0b01000000); // Right MICPGA Pos Input Routing IN1R+ RX_Q+
    Codec_Write_SPI (57, 0b00010000); // Right MICPGA Neg Input Routing IN1L- RX_Q-
	
    Codec_Write_SPI (14, 0b00001000); // LOL Routing Selection L DAC
    Codec_Write_SPI (15, 0b00001000); // LOR Routing Selection R DAC
	
}

/*******************************************************************************
* Codec_Config_PT1_RX
*
* Configure Codec for Analog Pass Through Receive
* I&Q are amplified and passed through to the Headphones
* This configuration is for external computer IQ mod/demod
* 
*
*******************************************************************************/
void Codec_Config_PT1_RX (void)
{
    Codec_Write_SPI (0, 0);           // Select page 0
    Codec_Write_SPI (29, 0b00000000); // Audio Interface Set 3 default

    Codec_Write_SPI (0, 1);           // Select page 1
    Codec_Write_SPI (52, 0b00010000); // Left MICPGA Pos Input Routing IN2L+ RX_I+
    Codec_Write_SPI (54, 0b00010000); // Left MICPGA Neg Input Routing IN2R- RX_I-
    Codec_Write_SPI (55, 0b01000000); // Right MICPGA Pos Input Routing IN1R+ RX_Q+
    Codec_Write_SPI (57, 0b00010000); // Right MICPGA Neg Input Routing IN1L- RX_Q-
    Codec_Write_SPI (58, 0b00001100); // Floating Input Configuration IN3L IN3R

    Codec_Write_SPI (12, 0b00000010); // HPL Routing Selection MAL
    Codec_Write_SPI (13, 0b00000010); // HPR Routing Selection MAR

    Codec_Write_SPI (59, 0b00001100); // Left MICPGA Volume 6dB to 53dB diff gain
    Codec_Write_SPI (60, 0b00001100); // Right MICPGA Volume 6dB to 53dB diff gain
  
    Codec_Write_SPI (24, 0b00000000); // Mixer Amp Left Volume 0dB to -30dB
    Codec_Write_SPI (25, 0b00000000); // Mixer Amp Right Volume 0dB to -30dB
}

/*******************************************************************************
* Codec_Config_PT1_TX
*
* Configure Codec for All Analog Pass Through Transmit
* using Stereo Mic Input for I & Q.
* This configuration is for external computer IQ mod/demod
* 
*
*******************************************************************************/
void Codec_Config_PT1_TX (void)
{
    Codec_Write_SPI (0, 1);           // Select page 1
    Codec_Write_SPI (58, 0b11110000); // Floating Inputs connect to common mode
    Codec_Write_SPI (52, 0b00000100); // Left MICPGA Pos Input Routing IN3L+ 10k
    Codec_Write_SPI (54, 0b01000000); // Left MICPGA Neg Input Routing CM1L 10k
    Codec_Write_SPI (55, 0b00000100); // Right MICPGA Pos Input Routing IN3R+ 10k
    Codec_Write_SPI (57, 0b01000000); // Right MICPGA Neg Input Routing CM1R 10k
	
    Codec_Write_SPI (14, 0b00000010); // LOL Routing Selection MAL
    Codec_Write_SPI (15, 0b00000010); // LOR Routing Selection MAR
	
    Codec_Write_SPI (24, 0b00000000); // Mixer Amp Left Volume 0dB
    Codec_Write_SPI (25, 0b00000000); // Mixer Amp Right Volume 0dB
	
}

/*******************************************************************************
* Codec_Config_PT2_RX
*
* Configure Codec for Digital Pass Through Receive
* I&Q are sampled by the ADC and Passed to the DAC then to the Headphones
* This configuration keeps audio data within the CODEC
*
*
*******************************************************************************/
void Codec_Config_PT2_RX (void)
{
    Codec_Write_SPI (0, 0);           // Select page 0
    Codec_Write_SPI (29, 0b00010000); // Audio Interface Set 3 Send ADC to DAC

    Codec_Write_SPI (0, 1);           // Select page 1
    Codec_Write_SPI (52, 0b00010000); // Left MICPGA Pos Input Routing IN2L+ RX_I+
    Codec_Write_SPI (54, 0b00010000); // Left MICPGA Neg Input Routing IN2R- RX_I-
    Codec_Write_SPI (55, 0b01000000); // Right MICPGA Pos Input Routing IN1R+ RX_Q+
    Codec_Write_SPI (57, 0b00010000); // Right MICPGA Neg Input Routing IN1L- RX_Q-
    Codec_Write_SPI (58, 0b00001100); // Floating Input Configuration IN3L IN3R

    Codec_Write_SPI (12, 0b00001000); // HPL Routing Selection LDAC+
    Codec_Write_SPI (13, 0b00001000); // HPR Routing Selection RDAC+

    Codec_Write_SPI (59, 0b00001100); // Left MICPGA Volume 6dB to 53dB diff gain
    Codec_Write_SPI (60, 0b00001100); // Right MICPGA Volume 6dB to 53dB diff gain

}

/*******************************************************************************
* Codec_Config_PT3_RX
*
* Configure Codec for Digital Pass Through Receive
* I&Q are sampled by the ADC, passed to DSP then DAC then to the Headphones
* This configuration allows the dsPIC to see the audio data stream
*
*
*******************************************************************************/
void Codec_Config_PT3_RX (void)
{
    Codec_Write_SPI (0, 0);           // Select page 0
    Codec_Write_SPI (29, 0b00000000); // Audio Interface Set 3 default

    Codec_Write_SPI (0, 1);           // Select page 1
    Codec_Write_SPI (52, 0b00010000); // Left MICPGA Pos Input Routing IN2L+ RX_I+
    Codec_Write_SPI (54, 0b00010000); // Left MICPGA Neg Input Routing IN2R- RX_I-
    Codec_Write_SPI (55, 0b01000000); // Right MICPGA Pos Input Routing IN1R+ RX_Q+
    Codec_Write_SPI (57, 0b00010000); // Right MICPGA Neg Input Routing IN1L- RX_Q-
    Codec_Write_SPI (58, 0b00001100); // Floating Input Configuration IN3L IN3R

    Codec_Write_SPI (12, 0b00001000); // HPL Routing Selection LDAC+
    Codec_Write_SPI (13, 0b00001000); // HPR Routing Selection RDAC+

    Codec_Write_SPI (59, 0b00001100); // Left MICPGA Volume 6dB to 53dB diff gain
    Codec_Write_SPI (60, 0b00001100); // Right MICPGA Volume 6dB to 53dB diff gain

}

/*******************************************************************************
* Codec_Beep_Freq
*
* Set Codec Beep Frequency (uint8 value of 1-10 x 100 Hz)
* Used for general purpose audio beeps using codec builtin hardware.
* The sine and cosine values below are based on an 8kHz DAC sample rate.
*
*******************************************************************************/
void Codec_Beep_Freq (uint8 Beep_Freq)
{
    Codec_Write_SPI (0, 0);             // Select page 0
	
    switch(Beep_Freq) // Switch on 1 of 10 frequencies (100-1000 Hz)
    {
        case 1:  // 100Hz
            Codec_Write_SPI (76, 0x0A); // SINE MSB
            Codec_Write_SPI (77, 0x0B); // SINE LSB
            Codec_Write_SPI (78, 0x7F); // COSINE MSB
            Codec_Write_SPI (79, 0x9B); // COSINE LSB
        break;
        case 2:  // 200Hz
            Codec_Write_SPI (76, 0x14); // SINE MSB
            Codec_Write_SPI (77, 0x06); // SINE LSB
            Codec_Write_SPI (78, 0x7E); // COSINE MSB
            Codec_Write_SPI (79, 0x6D); // COSINE LSB
        break;
        case 3:  // 300Hz
            Codec_Write_SPI (76, 0x1D); // SINE MSB
            Codec_Write_SPI (77, 0xE2); // SINE LSB
            Codec_Write_SPI (78, 0x7C); // COSINE MSB
            Codec_Write_SPI (79, 0x77); // COSINE LSB
        break;
        case 4:  // 400Hz
            Codec_Write_SPI (76, 0x27); // SINE MSB
            Codec_Write_SPI (77, 0x8E); // SINE LSB
            Codec_Write_SPI (78, 0x79); // COSINE MSB
            Codec_Write_SPI (79, 0xBC); // COSINE LSB
        break;
        case 5:  // 500Hz
            Codec_Write_SPI (76, 0x30); // SINE MSB
            Codec_Write_SPI (77, 0xFC); // SINE LSB
            Codec_Write_SPI (78, 0x76); // COSINE MSB
            Codec_Write_SPI (79, 0x42); // COSINE LSB
        break;
        case 6:  // 600Hz
            Codec_Write_SPI (76, 0x3A); // SINE MSB
            Codec_Write_SPI (77, 0x1C); // SINE LSB
            Codec_Write_SPI (78, 0x72); // COSINE MSB
            Codec_Write_SPI (79, 0x0D); // COSINE LSB
        break;
        case 7:  // 700Hz
            Codec_Write_SPI (76, 0x42); // SINE MSB
            Codec_Write_SPI (77, 0xE1); // SINE LSB
            Codec_Write_SPI (78, 0x6D); // COSINE MSB
            Codec_Write_SPI (79, 0x23); // COSINE LSB
        break;
        case 8:  // 800Hz
            Codec_Write_SPI (76, 0x4B); // SINE MSB
            Codec_Write_SPI (77, 0x3D); // SINE LSB
            Codec_Write_SPI (78, 0x67); // COSINE MSB
            Codec_Write_SPI (79, 0x8E); // COSINE LSB
        break;
        case 9:  // 900Hz
            Codec_Write_SPI (76, 0x53); // SINE MSB
            Codec_Write_SPI (77, 0x21); // SINE LSB
            Codec_Write_SPI (78, 0x61); // COSINE MSB
            Codec_Write_SPI (79, 0x55); // COSINE LSB
        break;
        default: // 1000Hz or Frequency out of bounds
            Codec_Write_SPI (76, 0x5A); // SINE MSB
            Codec_Write_SPI (77, 0x82); // SINE LSB
            Codec_Write_SPI (78, 0x5A); // COSINE MSB
            Codec_Write_SPI (79, 0x82); // COSINE LSB
        break;
    }
    
}

/*******************************************************************************
* Codec_Beep_Vol
*
* Set Codec Beep Volume
* Used for CW side tone
* Volume range -63dB to 0dB (decimal 0 to 63 unsigned)
*
*******************************************************************************/
void Codec_Beep_Vol (uint8 Beep_Vol)
{
    if (Beep_Vol > 63) Beep_Vol = 63;  // Set to -63dB min
    Beep_Vol |= 0b01000000;            // Set Right Chan as vol control
    Codec_Write_SPI (0, 0);            // Select page 0
    Codec_Write_SPI (72, Beep_Vol);    // Set Beep Vol
}

/*******************************************************************************
* Codec_Beep
*
* Set Codec Beep Duration and enable beep
* Duration in ms up to 8000ms
* Time is based on a 8kHz DAC sample rate
*
*******************************************************************************/
void Codec_Beep (uint16 Beep_Length)
{
    uint8 Beep_Len0;
    uint8 Beep_Len1;
	
    if (Beep_Length < 1) Beep_Length = 1;            // Set to 1ms min
    else if (Beep_Length > 8000) Beep_Length = 8000; // Set to 8000ms max
	
    Beep_Length = (Beep_Length << 3);          // Multiply by 8
    Beep_Len0 = (uint8)(Beep_Length & 0x00FF);  // Length LSB
    Beep_Len1 = (uint8)(Beep_Length >> 8);      // Length MSB
	
    Codec_Write_SPI (0, 0);                  // Select page 0
    Codec_Write_SPI (73, 0x00);              // Set Beep Length 23:16
    Codec_Write_SPI (74, Beep_Len1);         // Set Beep Length 15:8
    Codec_Write_SPI (75, Beep_Len0);         // Set Beep Length 7:0
    Codec_Write_SPI (71, 0x80);              // Enable Beep Generator
}