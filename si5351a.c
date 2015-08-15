/******************************************************************************
   Author: Hans Summers, 2015
   Website: http://www.hanssummers.com\

   A very very simple Si5351a demonstration
   using the Si5351a module kit http://www.hanssummers.com/synth
   Please also refer to SiLabs AN619 which describes all the registers to use

   5-26-2015 N7IFC
   Modify to use DSPic I2C2 Write routine and to output complimentry
   output signals. Change crystal frequency to 25MHz in header file.

 *****************************************************************************/

#include <stdint.h>
//#include "i2c.h"
#include "si5351a.h"
#include "DSPIC33E_hardware.h"


//
// Set up specified PLL with mult, num and denom
// mult is 15..90
// num is 0..1,048,575 (0xFFFFF)
// denom is 0..1,048,575 (0xFFFFF)
//
void setupPLL(uint8_t pll, uint8_t mult, uint32_t num, uint32_t denom)
{
	uint32_t P1;	// PLL config register P1
	uint32_t P2;	// PLL config register P2
	uint32_t P3;	// PLL config register P3

	P1 = (uint32_t)(128 * ((float)num / (float)denom));
	P1 = (uint32_t)(128 * (uint32_t)(mult) + P1 - 512);
	P2 = (uint32_t)(128 * ((float)num / (float)denom));
	P2 = (uint32_t)(128 * num - denom * P2);
	P3 = denom;

	I2C2_Byte_Write(pll + 0, (P3 & 0x0000FF00) >> 8);
	I2C2_Byte_Write(pll + 1, (P3 & 0x000000FF));
	I2C2_Byte_Write(pll + 2, (P1 & 0x00030000) >> 16);
	I2C2_Byte_Write(pll + 3, (P1 & 0x0000FF00) >> 8);
	I2C2_Byte_Write(pll + 4, (P1 & 0x000000FF));
	I2C2_Byte_Write(pll + 5, ((P3 & 0x000F0000) >> 12) | ((P2 & 0x000F0000) >> 16));
	I2C2_Byte_Write(pll + 6, (P2 & 0x0000FF00) >> 8);
	I2C2_Byte_Write(pll + 7, (P2 & 0x000000FF));
}

//
// Set up MultiSynth with integer divider and R divider
// R divider is the bit value which is OR'ed onto the appropriate register, it is a #define in si5351a.h
//
void setupMultisynth(uint8_t synth, uint32_t divider, uint8_t rDiv)
{
	uint32_t P1;					// Synth config register P1
	uint32_t P2;					// Synth config register P2
	uint32_t P3;					// Synth config register P3

	P1 = 128 * divider - 512;
	P2 = 0;							// P2 = 0, P3 = 1 forces an integer value for the divider
	P3 = 1;

	I2C2_Byte_Write(synth + 0,   (P3 & 0x0000FF00) >> 8);
	I2C2_Byte_Write(synth + 1,   (P3 & 0x000000FF));
	I2C2_Byte_Write(synth + 2,   ((P1 & 0x00030000) >> 16) | rDiv);
	I2C2_Byte_Write(synth + 3,   (P1 & 0x0000FF00) >> 8);
	I2C2_Byte_Write(synth + 4,   (P1 & 0x000000FF));
	I2C2_Byte_Write(synth + 5,   ((P3 & 0x000F0000) >> 12) | ((P2 & 0x000F0000) >> 16));
	I2C2_Byte_Write(synth + 6,   (P2 & 0x0000FF00) >> 8);
	I2C2_Byte_Write(synth + 7,   (P2 & 0x000000FF));
}

//
// Switches off Si5351a output
// Example: si5351aOutputOff(SI_CLK0_CONTROL);
// will switch off output CLK0
//
void si5351aOutputOff(uint8_t clk)
{
	I2C2_Byte_Write(clk, 0x80);		// Refer to SiLabs AN619 to see bit values - 0x80 turns off the output stage
}

// 
// Set CLK0 output ON and to the specified frequency
// Frequency is in the range 1MHz to 150MHz
// Example: si5351aSetFrequency(10000000);
// will set output CLK0 to 10MHz
//
// This example sets up PLL A
// and MultiSynth 0
// and produces the output on CLK0
//
void si5351aSetFrequency(uint32_t frequency)
{
	uint32_t pllFreq;
	uint32_t xtalFreq = XTAL_FREQ;
	uint32_t l;
	float f;
	uint8_t mult;
	uint32_t num;
	uint32_t denom;
	uint32_t divider;

	divider = 900000000 / frequency; // Calculate the division ratio. 900,000,000 is the maximum internal
					// PLL frequency: 900MHz
	if (divider % 2) divider--;     // Ensure an even integer division ratio

	pllFreq = divider * frequency;  // Calculate the pllFrequency: the divider * desired output frequency

	mult = pllFreq / xtalFreq;       // Determine the multiplier to get to the required pllFrequency
	l = pllFreq % xtalFreq;         // It has three parts:
	f = l;				// mult is an integer that must be in the range 15..90
	f *= 1048575;			// num and denom are the fractional parts, the numerator and denominator
	f /= xtalFreq;			// each is 20 bits (range 0..1048575)
	num = f;			// the actual multiplier is  mult + num / denom
	denom = 1048575;		// For simplicity we set the denominator to the maximum 1048575

	// Set up PLL A with the calculated multiplication ratio
	setupPLL(SI_SYNTH_PLL_A, mult, num, denom);
	// Set up MultiSynth divider 0, with the calculated divider. 
	// The final R division stage can divide by a power of two, from 1..128. 
	// reprented by constants SI_R_DIV1 to SI_R_DIV128 (see si5351a.h header file)
	// If you want to output frequencies below 1MHz, you have to use the 
	// final R division stage
	setupMultisynth(SI_SYNTH_MS_0, divider, SI_R_DIV_1);
        setupMultisynth(SI_SYNTH_MS_2, divider, SI_R_DIV_1);    // Set Multisynth 2 the same
        // set crystal load capacitance to 8pf
      //I2C2_Byte_Write(SI_XTAL_LD_CAP, 0b10010010);
        // Turn OFF spread spectrum
      //I2C2_Byte_Write(SI5351_SSC_PARAM0, 0b00000000);

	// Reset the PLL. This causes a glitch in the output. For small changes to 
	// the parameters, you don't need to reset the PLL, and there is no glitch
      //I2C2_Byte_Write(SI_PLL_RESET, 0xA0);
	// Finally switch on the CLK0 output (0x4F)
	// and set the MultiSynth0 input to be PLL A
      //I2C2_Byte_Write(SI_CLK0_CONTROL, 0b01001100 | SI_CLK_SRC_PLL_A);
        // Turn on CLK2 and invert output
      //I2C2_Byte_Write(SI_CLK2_CONTROL, 0b01011100 | SI_CLK_SRC_PLL_A);
}

/******************************************************************************
 * Function:       Init_si5351a
 *
 * PreCondition:   None
 *
 * Input:          None
 *
 * Output:         None
 *
 * Side Effects:   None
 *
 * Overview:       Initialize the si5351a registers that only need set once.
 *****************************************************************************/

void Init_si5351a(void)
{
    si5351aSetFrequency(10000000); // Set frequency to 10 MHz
    // set crystal load capacitance to 8pf
    I2C2_Byte_Write(SI_XTAL_LD_CAP, 0b10010010);
    // Turn OFF spread spectrum
    I2C2_Byte_Write(SI5351_SSC_PARAM0, 0b00000000);
    // Reset the PLL. This causes a glitch in the output. For small changes to
    // the parameters, you don't need to reset the PLL, and there is no glitch
    I2C2_Byte_Write(SI_PLL_RESET, 0xA0);
    // Finally switch on the CLK0 output (0x4F)
    // and set the MultiSynth0 input to be PLL A
    I2C2_Byte_Write(SI_CLK0_CONTROL, 0b01001100 | SI_CLK_SRC_PLL_A);
    // Turn on CLK2 and invert output
    I2C2_Byte_Write(SI_CLK2_CONTROL, 0b01011100 | SI_CLK_SRC_PLL_A);
}