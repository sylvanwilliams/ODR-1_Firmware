/***************************************************************************
* Description:
*
* This work is licensed under the Creative Commons Attribution-ShareAlike
* 4.0 International License. To view a copy of this license, visit
* http://creativecommons.org/licenses/by-sa/4.0/ or send a letter to
* Creative Commons, PO Box 1866, Mountain View, CA 94042, USA.
***************************************************************************/

#ifndef __ODR1_CONTROL_1_H
#define	__ODR1_CONTROL_1_H

#define	int8 signed char                // AKA int8_t
#define	uint8 unsigned char             // AKA uint8_t
#define	int16 signed int                // AKA int16_t
#define	uint16 unsigned int             // AKA uint16_t
#define	int32 signed long int           // AKA int32_t
#define	uint32 unsigned long int        // AKA uint32_t
#define	int64 signed long long int      // AKA int64_t
#define	uint64 unsigned long long int   // AKA uint64_t

void Init_Mixer_Board();    // initialize the mixer board and oscillator
void Set_bandpass_Filters(int32 freq);  // Set BP Filters on Osc & Mix Board
void Set_RFGain(int16 gain);            // Set RF gain on Osc & Mix Board

#endif

