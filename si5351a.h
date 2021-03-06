/*************************************************************************
* This work is licensed under the Creative Commons Attribution-ShareAlike
* 4.0 International License. To view a copy of this license, visit
* http://creativecommons.org/licenses/by-sa/4.0/ or send a letter to
* Creative Commons, PO Box 1866, Mountain View, CA 94042, USA.
*************************************************************************/

#ifndef SI5351A_H
#define SI5351A_H

#include <stdint.h>

#define SI5351_BUS_BASE_ADDR	0xC0
#define SI_CLK0_CONTROL         16            // Register definitions
#define SI_CLK1_CONTROL         17
#define SI_CLK2_CONTROL         18
#define SI_SYNTH_PLL_A          26
#define SI_SYNTH_PLL_B          34
#define SI_SYNTH_MS_0		42
#define SI_SYNTH_MS_1		50
#define SI_SYNTH_MS_2		58
#define SI_PLL_RESET		177

#define SI_XTAL_LD_CAP		183
#define SI5351_SSC_PARAM0	149

#define SI_R_DIV_1		0b00000000    // R-division ratio definitions
#define SI_R_DIV_2		0b00010000
#define SI_R_DIV_4		0b00100000
#define SI_R_DIV_8		0b00110000
#define SI_R_DIV_16		0b01000000
#define SI_R_DIV_32		0b01010000
#define SI_R_DIV_64		0b01100000
#define SI_R_DIV_128		0b01110000

#define SI_CLK_SRC_PLL_A	0b00000000
#define SI_CLK_SRC_PLL_B	0b00100000

#define XTAL_FREQ	25000000             // Crystal frequency

void si5351aOutputOff(uint8_t clk);
void si5351aSetFrequency(uint32_t frequency);
void Init_si5351a(void);

#endif //SI5351A_H
