/*******************************************************************************
* Header file for UI_page0.c
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
*
*******************************************************************************/

#ifndef __UI_PAGE0_H
#define __UI_PAGE0_H

#define	int8 signed char                // AKA int8_t
#define	uint8 unsigned char             // AKA uint8_t
#define	int16 signed int                // AKA int16_t
#define	uint16 unsigned int             // AKA uint16_t
#define	int32 signed long int           // AKA int32_t
#define	uint32 unsigned long int        // AKA uint32_t
#define	int64 signed long long int      // AKA int64_t
#define	uint64 unsigned long long int   // AKA uint64_t


//Defines to make code easier to read	
//#define LCD_W 240   // Portrate
//#define LCD_H 320   // Portrate

extern int16 rxtx_mode;    // Mode
extern int32 radio_freq;   // Radio frequency
extern int16 tx_offset;    // Transmit Offset
extern int16 mic_gain;     // Microphone gain

// Functions
void Page0_pointer1_update();   // Update pointer under mode freq and tx offset
void Page0_pointer2_update();   // Update pointer under mode freq and tx offset
void Refresh_page0();        // Refresh page 0
void Display_RXTX_Mode();    // Display the current RX/TX Mode
void Display_Frequency();    // Display the current Frequency
void Display_TX_Offset();    // Display the transmit offset
void Display_MicGain();      // Display Mic Gain
void Display_KeyerSpeed();   // Display Keyer Speed
void Display_FilterBW();     // Display BW Filter
void Display_RFGain();       // Display RF Gain
void Display_AFGain();       // Display AFGain
void Display_UTC_24HR();     // Display Time in 24hour format
void Change_Freq();          // Change osc and filters based on freq change
void Set_RFGain();           // Set RF Gain on Oscillator and Mixer Board
#endif