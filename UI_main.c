/*******************************************************************************
* User Interface main captures user inputs from the rotary encoders and key
* inputs. The dual quadrature encoder interfaces are used to debounce the inputs
* in hardware.
*
* Color pallets for user display are also defined here.
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
* 2014-Apr Develop code for encoder # 1 navigation functions
*
*
*******************************************************************************/

#include "UI_main.h"
#include "p33exxxx.h" // processor specific definitions
#include "LCD_driver.h"
#include "UI_page0.h"
#include "UI_page1.h"
#include "UI_page2.h"
#include "DSPIC33E_hardware.h"

#define MENU_PRESS_DURATION 2
#define BUTTON_TICKS        ((unsigned int)(MENU_PRESS_DURATION/CYCLE_TIME)/2 | 0x8000)

//Arrays


//Variables
int16 encoder1_old      = 0;
int16 encoder1_count    = 0;
uint16 encoder1_PBcount = 0;
int16 encoder2_old      = 0;
int16 encoder2_count    = 0;
uint16 encoder2_PBcount = 0;
uint16 current_page = 0;
int16 page_pointer1    = 5;          // Default to kHz freq
int16 page_pointer2    = 5;          // Default to AF gain

int16 active_color_pallet = 0;
uint16 char_hglt_color; // Character Highlight Color
uint16 char_norm_color; // Character Normal Color
uint16 char_rev_color;  // Character Reverse Color
uint16 field_color;     // Main field Color
uint16 heading_color;   // Heading Color
uint16 border_color;    // Border Color


/*******************************************************************************
* Encoder 1 Status updates the encoder rotation count and push button status.
* The rotary encoder changes 4 states per detent so the last two bits are masked
* so that items increment once per detent.
* The encoder and PB counts need to be cleared after use by other functions.
* The push button count tells how long the button was pressed before released
* and traps the count on button release. The MSb indicates pressed and released.
*******************************************************************************/
void Encoder1_Update()
{
    int16 encoder1_new;

    encoder1_new = (POS1CNTL & 0xFFFC); // Read encoder1 position zero last two bits
    if (encoder1_new != encoder1_old)   // if position has changed
    {
        encoder1_count += ((encoder1_new - encoder1_old) >>2); // Update new count
        encoder1_old = encoder1_new;                           // Store last read
    }

    if (encoder1_PBcount < 0x8000)      // If button release not yet detected (MSB set)
    {
        if (!QEI1IOCbits.INDEX)         // if button is still being pressed
        {
            encoder1_PBcount ++;        // increment the PBcounter
        }
        else if (encoder1_PBcount)      // If button released and count is non-zero
        {
            encoder1_PBcount |= 0x8000; // Set MSB indicating button released and non-zero count trapped
            if (encoder1_PBcount > BUTTON_TICKS)  // If button was held for 2 seconds
            {
                if (current_page == 0)  // If we are on page 0
                {
                    current_page = 1;   // Switch to page 1
                    page_pointer1 = 0x8000;  // Default to item 0 and pointer has focus
                    Refresh_page1();    // display page 1
                }
                else
                {
                    current_page = 0;  // exit back to page 0
                    page_pointer1 = 5; // Default to kHz Frequency
                    page_pointer2 = 5; // Default to AF Gain
                    Refresh_page0();   // display page 0
                }
                encoder1_count = 0;
                encoder1_PBcount = 0;
            }
        }
    }
}


/*******************************************************************************
* Encoder 2 Status updates the encoder rotation count and push button status.
* The rotary encoder changes 4 states per detent so the last two bits are masked
* so that items increment once per detent.
* The encoder and PB counts need to be cleared after use by other functions.
* The push button count tells how long the button was pressed before released
* and traps the count on button release. The MSb indicates pressed and released.
*******************************************************************************/
void Encoder2_Update()
{
    int16 encoder2_new;

    encoder2_new = (POS2CNTL & 0xFFFC); // Read encoder1 position zero last two bits
    if (encoder2_new != encoder2_old)   // if position has changed
    {
        encoder2_count += ((encoder2_new - encoder2_old) >>2); // Update new count
        encoder2_old = encoder2_new;                           // Store last read
    }

    if (encoder2_PBcount < 0x8000)      // If button release not yet detected (MSB set)
    {
        if (!QEI2IOCbits.INDEX)         // if button is still being pressed
        {
            encoder2_PBcount ++;        // increment the PBcounter
        }
        else if (encoder2_PBcount)      // If button released and count is non-zero
        {
            encoder2_PBcount |= 0x8000; //Set MSB indicating button released and non-zero count trapped
            if (encoder2_PBcount > BUTTON_TICKS)  // If button was held for 2 seconds
            {
                if (current_page == 0)  // If we are on page 0
                {
                    current_page = 2;   // Switch to page 2
                    page_pointer1 = 0x8000;  // Default to item 0 and pointer has focus
                    Refresh_page2();    // display page 2
                }
                else
                {
                    current_page = 0;  // exit back to page 0
                    page_pointer1 = 5; // Default to kHz Frequency
                    page_pointer2 = 5; // Default to AF Gain
                    Refresh_page0();   // display page 0
                }
                encoder2_count = 0;
                encoder2_PBcount = 0;
            }
        }
    }
}

/*******************************************************************************
* Update the color pallet

*
*
*******************************************************************************/
void Color_pallet_update()
{
    switch(active_color_pallet) // Action based on color pallet selected
    {
        case 0:  // Color pallet 0 GREEN
            char_hglt_color = YELLOW;    // Character Highlight Color
            char_norm_color = BLACK;     // Character Normal Color
            char_rev_color  = WHITE;     // Character Reverse Color
            field_color     = 0x7FEF;    // Main field Color
            heading_color   = 0x02C0;    // Heading Color
            border_color    = BLACK;     // Inner Border Color
        break;
        case 1:  // Color pallet 1 BROWN
            char_hglt_color = YELLOW;   // Character Highlight Color
            char_norm_color = BLACK;    // Character Normal Color
            char_rev_color  = WHITE;    // Character Reverse Color
            field_color     = 0xFC67;   // Main field Color
            heading_color   = 0xB2C0;   // Fill color 1
            border_color    = BLACK;    // Border Color
        break;
        case 2:  // Color pallet 2 Black and White
            char_hglt_color = YELLOW;   // Character Highlight Color
            char_norm_color = BLACK;    // Character Normal Color
            char_rev_color  = WHITE;    // Character Reverse Color
            field_color     = WHITE;   // Main field Color
            heading_color   = BLACK;   // Fill color 1
            border_color    = BLACK;    // Border Color
        break;
    }
}

