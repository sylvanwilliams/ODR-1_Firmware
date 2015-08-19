/*******************************************************************************
* Header file for UI_main.c
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
*******************************************************************************/

#ifndef __UI_MAIN_H
#define __UI_MAIN_H

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
typedef enum eButtonState {NO_PRESS, PRESS} eButtonState;
typedef enum eButtonEvent {NONE, QUICK, EXTENDED} eButtonEvent;

typedef struct sEncoderData{
    int16 oldCount;
    int16 count;
    uint16 buttonCount;
    eButtonState buttonState;
    eButtonState oldButtonState;
}sEncoderData;

// variables available for external access
extern uint16 current_page;
extern int16 page_pointer1;          // pointer to item on page
extern int16 page_pointer2;          // pointer to item on page

extern int16 active_color_pallet;
extern uint16 char_hglt_color;    // Character Highlight Color
extern uint16 char_norm_color;    // Character Normal Color
extern uint16 char_rev_color;     // Character Reverse Color
extern uint16 field_color;        // Main field Color
extern uint16 heading_color;      // Heading Color
extern uint16 border_color;       // Outer Border Color

// Functions
void Encoder1_Update();           // update encoder 1 status
void Encoder2_Update();           // update encoder 1 status
void Color_pallet_update();       // update the color palate

int16 Encoder1Count(void);
void Encoder1CountZero(void);
eButtonState Encoder1ButtonEvent(void);

int16 Encoder2Count(void);
void Encoder2CountZero(void);
eButtonState Encoder2ButtonEvent(void);
#endif