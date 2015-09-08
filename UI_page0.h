/*******************************************************************************
* This work is licensed under the Creative Commons Attribution-ShareAlike
* 4.0 International License. To view a copy of this license, visit
* http://creativecommons.org/licenses/by-sa/4.0/ or send a letter to
* Creative Commons, PO Box 1866, Mountain View, CA 94042, USA.
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
void Read_RTC_Time();        // Get the current time from the RTC module
void Write_RTC_Hrs(int16 hours);     // Write hours into the RTC module
void Write_RTC_Min(int16 minutes);   // Write minutes into the RTC module
void Write_RTC_Sec(int16 seconds);   // Write seconds into the RTC module
#endif