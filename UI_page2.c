/*******************************************************************************
* User Interface page 2
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
*
*
*
*
*******************************************************************************/

#include "UI_page2.h"
#include "main.h"
#include "p33exxxx.h" // processor specific definitions
#include "LCD_driver.h"
#include "UI_main.h"
#include "UI_page0.h"

//Arrays
// Array of pointers to adjustable parameter strings
char * parameter_name[24] =
{
    "000 Color Theme   ",
    "001 Backlight Lvl ",
    "002 Dft Radio Freq",
    "003 Dft Mic Gain  ",
    "004 Parameter Name",
    "005 Parameter Name",
    "006 Parameter Name",
    "007 Parameter Name",
    "008 Parameter Name",
    "009 Parameter Name",
    "010 Parameter Name",
    "011 Parameter Name",
    "012 Parameter Name",
    "013 Parameter Name",
    "014 Parameter Name",
    "015 Parameter Name",
    "016 Parameter Name",
    "017 Parameter Name",
    "018 Parameter Name",
    "019 Parameter Name",
    "020 Parameter Name",
    "021 Parameter Name",
    "022 Parameter Name",
    "023 Parameter Name",
};

// Array of pointers to color theme text strings
char * color_theme_text[3] =
{
    "0 Green      ",
    "1 Brown      ",
    "2 Blk & Wht  ",
};

//Variables
int16 value_00 = 0;
int16 value_01 = 50; // default to 50% backlight
int16 value_02 = 2;
int16 value_03 = 3;
int16 value_04 = 4;
int16 value_05 = 5;
int16 value_06 = 6;
int16 value_07 = 7;
int16 value_08 = 8;
int16 value_09 = 9;


/*******************************************************************************
* Navigation pointer #1 is associated with the upper encoder
*
*
*******************************************************************************/
void Page2_pointer1_update()
{

    if (encoder1_PBcount > 0x8000)       //If button has been pressed and released
    {
        if (page_pointer1 & 0x8000)      // If pointer already has the focus
        {
            page_pointer1 &= 0x7FFF;     // Remove pointer focus and keep position
            BACK_COLOR = field_color;
            Refresh_Parameter_Name();    // Display current parameter name
            BACK_COLOR = char_hglt_color;
            Refresh_Parameter_Value();    // Display current parameter value
            encoder1_count = 0;          // Zero out the rotary encoder count
        }
        else
        {
            Save_Page2_Item();            // Parameter update complete,save to EEPROM
            BACK_COLOR = char_hglt_color;
            Refresh_Parameter_Name();    // Display current parameter name
            BACK_COLOR = field_color;
            page_pointer1 |= 0x8000;      // Give pointer focus and keep position
            Refresh_Parameter_Value();    // Display current parameter value
            encoder1_count = 0;           // Zero out the rotary encoder count
        }
        encoder1_PBcount = 0;             // Zero out the push button counter
    }

    else if ((page_pointer1 & 0x8000) && (encoder1_count)) //pointer has focus and encoder moved
    {
        page_pointer1 += encoder1_count;   // add the rotary encoder count to pointer position
        encoder1_count = 0;                    // clear encoder count after use
        if (page_pointer1 > 0x8009)        // if above highest item count
        {
            page_pointer1 = 0x8009;        // Stop at highest item count
        }
        else if (page_pointer1 < 0x8000)   // if below lowest item count
        {
            page_pointer1 = 0x8000;        // Stop at lowest item count
        }
        BACK_COLOR = char_hglt_color;
        Refresh_Parameter_Name();    // Display current parameter name
        BACK_COLOR = field_color;
        Refresh_Parameter_Value();    // Display current parameter value
    }

    else if (encoder1_count) //item has focus and encoder moved
    {
        // Ignore encoder movement while the parameter value has focus
        encoder1_count = 0;   // clear encoder count
    }
}


/*******************************************************************************
* Navigation pointer #2 is associated with the lower encoder
*
*
*******************************************************************************/
void Page2_pointer2_update()
{

    if (encoder2_PBcount > 0x8000)       //If button has been pressed and released
    {
        if (page_pointer2 & 0x8000)      // If pointer already has the focus
        {
            page_pointer2 &= 0x7FFF;     // Remove pointer focus and keep position
            encoder2_count = 0;          // Zero out the rotary encoder count
        }
        else
        {
            page_pointer2 |= 0x8000;      // Give pointer focus and keep position
            encoder2_count = 0;           // Zero out the rotary encoder count
        }
        encoder2_PBcount = 0;             // Zero out the push button counter
    }

    else if ((page_pointer2 & 0x8000) && (encoder2_count)) //pointer has focus and encoder moved
    {
        page_pointer2 += encoder2_count;   // add the rotary encoder count to pointer position
        encoder2_count = 0;                // clear encoder count after use
        if (page_pointer2 > 0x8009)        // if above highest character position
        {
            page_pointer2 = 0x8009;        // Stop at highest character position
        }
        else if (page_pointer2 < 0x8000)   // if below lowest character position
        {
            page_pointer2 = 0x8000;        // Stop at lowest character position
        }
        BACK_COLOR = char_hglt_color;
        Refresh_Parameter_Value();    // Display current parameter value
    }

    else if (encoder2_count) // value has focus and encoder moved
    {
        Update_Parameter_Value();  // Update the parameter value
        encoder2_count = 0;   // clear encoder count after use
        BACK_COLOR = char_hglt_color;
        Refresh_Parameter_Value();    // Display current parameter value
    }
}


/*******************************************************************************
* Refresh page 2 draws screen graphics and headers.
*
*******************************************************************************/
void Refresh_page2()
{
    LCD_Clear(field_color);        //Clear the screen with field color

    POINT_COLOR = border_color;
    LCD_DrawRectangle(2, 2, 317, 237);                  // Screen Border
    LCD_Fill(3,3,316,18,heading_color);                 // Heading Fill
    POINT_COLOR = char_rev_color;
    BACK_COLOR = heading_color;
    LCD_8x16_String(50,3,"ADVANCED SETTINGS 2");   // Heading
    POINT_COLOR = char_norm_color;
    BACK_COLOR = char_hglt_color;
    Refresh_Parameter_Name();
    BACK_COLOR = field_color;
    Refresh_Parameter_Value();
}

/*******************************************************************************
* Refresh the parameter name
*
*******************************************************************************/
void Refresh_Parameter_Name()
{
    LCD_16x24_String(12,56,parameter_name[page_pointer1 & 0x0FFF]);    // Display current parameter name
}
/*******************************************************************************
* Refresh the item display
*
*******************************************************************************/
void Refresh_Parameter_Value()
{
    switch(page_pointer1 & 0x0FFF) // Action based on item being pointed to
    {
        case 0:  // Color pallet in use
            LCD_16x24_String(12,100,color_theme_text[active_color_pallet]);
        break;
        case 1:  // LCD Backlight Level (5-100%)
            LCD_16nz_Num(12,100,value_01,3);         // Backlight Percentage
            LCD_16x24_String(60,100," %        ");  // symbol plus line fill
        break;
        case 2:  // Pointer position 1
            Display_10_Numbers(radio_freq);            // Test data
        break;
        case 3:  // Pointer position 2
            LCD_16nz_Num(12,100,mic_gain,2);          // Display two numbers
            LCD_16x24_String(44,100," db        ");   // Display Mic Gain db
        break;
        case 4:  // Pointer position 4
            Display_10_Numbers(value_04);            // Test data
        break;
        case 5:  // Pointer position 5
            Display_10_Numbers(value_05);            // Test data
        break;
        case 6:  // Pointer position 6
            Display_10_Numbers(value_06);            // Test data
        break;
        case 7:  // Pointer position 7
            Display_10_Numbers(value_07);            // Test data
        break;
        case 8:  // Pointer position 8
            Display_10_Numbers(value_08);            // Test data
        break;
        case 9:  // Pointer position 9
            Display_10_Numbers(value_09);            // Test data
        break;
    }
}

/*******************************************************************************
* Refresh the item display
*
*******************************************************************************/
void Update_Parameter_Value()
{
    switch(page_pointer1) // Action based on item being pointed to
    {
        case 0:  // Change screen color theme
            active_color_pallet += encoder2_count;    // Change color pallet
            if (active_color_pallet > 2)              // if above highest possible count
            {
                active_color_pallet = 2;              // stop at max value
            }
            else if (active_color_pallet < 0)         // if below lowest possible count
            {
                active_color_pallet = 0;              // stop at minimum value
            }
            Color_pallet_update();                    // Update color pallet
            Refresh_page2();                          // Refresh with new color theme
            BACK_COLOR = field_color;
            Refresh_Parameter_Name();                 // Write over name without highlight

        break;
        case 1:  // Change LCD Backlight level
            value_01 += encoder2_count;         // New Backlight value
            if (value_01 > 100)                 // if above highest legal value
            {
                value_01 = 100;                 // stop at max value
            }
            else if (value_01 < 5)
            {
                value_01 = 5;                   // stop at min value
            }
            PDC3 = ((100 - value_01) * (PTPER / 100 ));  // Set new PWM duty cycle
        break;
        case 2:  // Default Radio Frequency
            switch(page_pointer2)
            {
                default:  // default to 21 MHz
                    radio_freq = 21000000;
                break;
                case 2:  // Pointer position 2 freq digit 8
                    radio_freq += (10000000 * encoder2_count);
                break;
                case 3:  // Pointer position 3 freq digit 7
                    radio_freq += (1000000 * encoder2_count);
                break;
                case 4:  // Pointer position 4 freq digit 6
                    radio_freq += (100000 * encoder2_count);
                break;
                case 5:  // Pointer position 5 freq digit 5
                    radio_freq += (10000 * encoder2_count);
                break;
                case 6:  // Pointer position 6 freq digit 4
                    radio_freq += (1000 * encoder2_count);
                break;
                case 7:  // Pointer position 7 freq digit 3
                    radio_freq += (100 * encoder2_count);
                break;
                case 8:  // Pointer position 8 freq digit 2
                    radio_freq += (10 * encoder2_count);
                break;
                case 9:  // Pointer position 9 freq digit 1
                    radio_freq += encoder2_count;
                break;

                if (radio_freq > 55000000) // High freq boundary
                {
                    radio_freq = 55000000;
                }
                if (radio_freq < 400000)  // Low freq boundary
                {
                    radio_freq = 400000;
                }
            }
        break;
        case 3:  // Default Mic Gain
            mic_gain += encoder2_count;
            if (mic_gain > 20)        // if above highest possible count
            {
                mic_gain = 20;        // stop at max
            }
            else if (mic_gain < 0)    // if below lowest possible count
            {
                mic_gain = 0;         // stop at minimum
            }
        break;
        case 4:  // Pointer position 4
            value_04 += encoder2_count;              // Dummy count variable
        break;
        case 5:  // Pointer position 5
            value_05 += encoder2_count;              // Dummy count variable
        break;
        case 6:  // Pointer position 6
            value_06 += encoder2_count;              // Dummy count variable
        break;
        case 7:  // Pointer position 7
            value_07 += encoder2_count;              // Dummy count variable
        break;
        case 8:  // Pointer position 8
            value_08 += encoder2_count;              // Dummy count variable
        break;
        case 9:  // Pointer position 9
            value_09 += encoder2_count;              // Dummy count variable
        break;
    }
}

/*******************************************************************************
* Save the item value to EEPROM
*
*******************************************************************************/
void Save_Page2_Item()
{
    BACK_COLOR = heading_color;
    POINT_COLOR = YELLOW;
    LCD_8x16_String(220,3,"SAVING");   // Heading
    delayms(500);                     // delay to slow the loop down for testing
    LCD_8x16_String(220,3,"      ");   // Heading
    delayms(500);                     // delay to slow the loop down for testing
    LCD_8x16_String(220,3,"SAVING");   // Heading
    delayms(500);                     // delay to slow the loop down for testing
    LCD_8x16_String(220,3,"      ");   // Heading
    BACK_COLOR = field_color;
    POINT_COLOR = char_norm_color;
}

/*******************************************************************************
* Display a 10 digit number and highlight the character position
* set by encoder 2.
*******************************************************************************/
void Display_10_Numbers(int32 num)
{
    uint8 t;          // Character position
    uint16 x = 12;    // Screen X position to display frequency
    uint16 y = 100;    // Screen Y position to display frequency

    BACK_COLOR = field_color;               // Restore the background color
    NumToCharArray(num); // Convert number to character array
    // Display array characters on screen
    for(t=0;t<10;t++)   // Display 10 total characters
    {
        if (!(page_pointer1 & 0x8000))          // Check if value has focus
        {
            if (t == (page_pointer2 & 0x00FF))  // Check if character position needs highlighted
            {
                BACK_COLOR = char_hglt_color;   // Turn on the highlight color
            }
        }

        LCD_16x24_Char(x,y,num32_char[t],0);    // display number character
        BACK_COLOR = field_color;               // Restore the background color
        x = x+16;                               // Increment character position
        if ((t==0)|(t==3)|(t==6))               // thoushands separator positions
        {
           LCD_16x24_Char(x,y,'.',0);           // display decimal character
           x = x+16;                            // increment character position
        }
    }
}
