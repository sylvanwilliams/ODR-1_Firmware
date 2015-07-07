/*******************************************************************************
* User Interface page 1
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

#include "UI_page1.h"
#include "main.h"
#include "p33exxxx.h" // processor specific definitions
#include "LCD_driver.h"
#include "UI_main.h"

//Arrays


//Variables
int16 old_pointer1 = 0;   // Indicates menu item that lost pointer focus

/*******************************************************************************
* Navigation pointer #1 is associated with the upper encoder and is used to
* highlight the item being pointed to. If the nav pointer has the focus, the
* pointer itself is moved from item to item. If the pointer does not have focus,
* the item being pointed to will have the focus.
*
*
*
*******************************************************************************/
void Page1_pointer1_update()
{

    if (encoder1_PBcount > 0x8000)       //If button has been pressed and released
    {
        if (page_pointer1 & 0x8000)      // If pointer already has the focus
        {
            page_pointer1 &= 0x7FFF;     // Remove pointer focus and keep position
            encoder1_count = 0;          // Zero out the rotary encoder count
        }
        else
        {
            Save_Page1_Item();            // Menu item update complete,save to EEPROM
            page_pointer1 |= 0x8000;      // Give pointer focus and keep position
            encoder1_count = 0;           // Zero out the rotary encoder count
        }
        encoder1_PBcount = 0;             // Zero out the push button counter
    }

    else if ((page_pointer1 & 0x8000) && (encoder1_count)) //pointer has focus and encoder moved
    {
        old_pointer1 = page_pointer1;       // Save old pointer position less the flag
        page_pointer1 += encoder1_count;   // add the rotary encoder count to pointer position
        encoder1_count = 0;                    // clear encoder count after use
        if (page_pointer1 > 0x8017)        // if above highest item count
        {
            page_pointer1 = 0x8017;        // Stop at highest item count
        }
        else if (page_pointer1 < 0x8000)   // if below lowest item count
        {
            page_pointer1 = 0x8000;        // Stop at lowest item count
        }
        Refresh_Page1_Item(old_pointer1 & 0x0FFF);  // Refresh item previously pointed to
        BACK_COLOR = char_hglt_color;
        Refresh_Page1_Item(page_pointer1 & 0x0FFF); // Refresh item currently pointed to
        BACK_COLOR = field_color;
    }

    else if (encoder1_count) //item has focus and encoder moved
    {
        Update_Page1_Item();    // update the item value
        BACK_COLOR = char_hglt_color;
        Refresh_Page1_Item(page_pointer1); // Refresh item currently pointed to
        BACK_COLOR = field_color;
        encoder1_count = 0;   // clear encoder count after use
    }
}

/*******************************************************************************
* Refresh page 1 draws screen graphics and headers.
*
*******************************************************************************/
void Refresh_page1()
{
    int16 i = 0;  // for simple counter

    LCD_Clear(field_color);        //Clear the screen with field color

    POINT_COLOR = border_color;
    LCD_DrawRectangle(2,2,317,237);                  // Screen Border
    LCD_Fill(3,3,316,18,heading_color);                 // Heading Fill
    LCD_DrawLine(159,19,159,236);                  // Screen Border
    POINT_COLOR = char_rev_color;
    BACK_COLOR = heading_color;
    LCD_8x16_String(50,3,"ADVANCED SETTINGS 1");   // Heading

    BACK_COLOR = field_color;
    POINT_COLOR = char_norm_color;
    LCD_8x16_String(6,21,"Item # 0:");
    LCD_8x16_String(6,39,"Item # 1:");
    LCD_8x16_String(6,57,"Item # 2:");
    LCD_8x16_String(6,75,"Item # 3:");
    LCD_8x16_String(6,93,"Item # 4:");
    LCD_8x16_String(6,111,"Item # 5:");
    LCD_8x16_String(6,129,"Item # 6:");
    LCD_8x16_String(6,147,"Item # 7:");
    LCD_8x16_String(6,165,"Item # 8:");
    LCD_8x16_String(6,183,"Item # 9:");
    LCD_8x16_String(6,201,"Item #10:");
    LCD_8x16_String(6,219,"Item #11:");
    LCD_8x16_String(162,21,"Item #12:");
    LCD_8x16_String(162,39,"Item #13:");
    LCD_8x16_String(162,57,"Item #14:");
    LCD_8x16_String(162,75,"Item #15:");
    LCD_8x16_String(162,93,"Item #16:");
    LCD_8x16_String(162,111,"Item #17:");
    LCD_8x16_String(162,129,"Item #18:");
    LCD_8x16_String(162,147,"Item #19:");
    LCD_8x16_String(162,165,"Item #20:");
    LCD_8x16_String(162,183,"Item #21:");
    LCD_8x16_String(162,201,"Item #22:");
    LCD_8x16_String(162,219,"Item #23:");

    BACK_COLOR = char_hglt_color;
    Refresh_Page1_Item(0);  // Refresh and highlight the first item on the page
    BACK_COLOR = field_color;
    for(i=1;i<25;i++)                 // Loop to refresh all other items on the page
    {
        Refresh_Page1_Item(i);
    }
  }

/*******************************************************************************
* Refresh the item display
*
*******************************************************************************/
void Refresh_Page1_Item(int16 t)
{
    switch(t) // Action based on item being pointed to
    {
        case 0:  // Pointer position 0
            LCD_8x16_String(86,21,"Value  0");     // Dummy data
        break;
        case 1:  // Pointer position 1
            LCD_8x16_String(86,39,"Value  1");     // Dummy data
        break;
        case 2:  // Pointer position 2
            LCD_8x16_String(86,57,"Value  2");     // Dummy data;
        break;
        case 3:  // Pointer position 3
            LCD_8x16_String(86,75,"Value  3");     // Dummy data
        break;
        case 4:  // Pointer position 4
            LCD_8x16_String(86,93,"Value  4");     // Dummy data
        break;
        case 5:  // Pointer position 5
            LCD_8x16_String(86,111,"Value  5");     // Dummy data
        break;
        case 6:  // Pointer position 6
            LCD_8x16_String(86,129,"Value  6");     // Dummy data;
        break;
        case 7:  // Pointer position 7
            LCD_8x16_String(86,147,"Value  7");     // Dummy data
        break;
        case 8:  // Pointer position 8
            LCD_8x16_String(86,165,"Value  8");     // Dummy data
        break;
        case 9:  // Pointer position 9
            LCD_8x16_String(86,183,"Value  9");     // Dummy data
        break;
        case 10:  // Pointer position 10
            LCD_8x16_String(86,201,"Value 10");     // Dummy data
        break;
        case 11:  // Pointer position 11
            LCD_8x16_String(86,219,"Value 11");     // Dummy data
        break;
        case 12:  // Pointer position 12
            LCD_8x16_String(244,21,"Value 12");     // Dummy data
        break;
        case 13:  // Pointer position 13
            LCD_8x16_String(244,39,"Value 13");     // Dummy data
        break;
        case 14:  // Pointer position 14
            LCD_8x16_String(244,57,"Value 14");     // Dummy data
        break;
        case 15:  // Pointer position 15
            LCD_8x16_String(244,75,"Value 15");     // Dummy data
        break;
        case 16:  // Pointer position 16
            LCD_8x16_String(244,93,"Value 16");     // Dummy data
        break;
        case 17:  // Pointer position 17
            LCD_8x16_String(244,111,"Value 17");     // Dummy data
        break;
        case 18:  // Pointer position 18
            LCD_8x16_String(244,129,"Value 18");     // Dummy data
        break;
        case 19:  // Pointer position 19
            LCD_8x16_String(244,147,"Value 19");     // Dummy data
        break;
        case 20:  // Pointer position 20
            LCD_8x16_String(244,165,"Value 20");     // Dummy data
        break;
        case 21:  // Pointer position 21
            LCD_8x16_String(244,183,"Value 21");     // Dummy data
        break;
        case 22:  // Pointer position 22
            LCD_8x16_String(244,201,"Value 22");     // Dummy data
        break;
        case 23:  // Pointer position 23
            LCD_8x16_String(244,219,"Value 23");     // Dummy data
        break;
    }
}

/*******************************************************************************
* Update the item value
*
*******************************************************************************/
void Update_Page1_Item()
{
    switch(page_pointer1) // Update action based on item being pointed to
    {
        case 0:  // Pointer position 0
            LCD_8x16_String(86,21,"Value  0");     // Dummy data
        break;
        case 1:  // Pointer position 1
            LCD_8x16_String(86,39,"Value  1");     // Dummy data
        break;
        case 2:  // Pointer position 2
            LCD_8x16_String(86,57,"Value  2");     // Dummy data;
        break;
        case 3:  // Pointer position 3
            LCD_8x16_String(86,75,"Value  3");     // Dummy data
        break;
        case 4:  // Pointer position 4
            LCD_8x16_String(86,93,"Value  4");     // Dummy data
        break;
        case 5:  // Pointer position 5
            LCD_8x16_String(86,111,"Value  5");     // Dummy data
        break;
        case 6:  // Pointer position 6
            LCD_8x16_String(86,129,"Value  6");     // Dummy data;
        break;
        case 7:  // Pointer position 7
            LCD_8x16_String(86,147,"Value  7");     // Dummy data
        break;
        case 8:  // Pointer position 8
            LCD_8x16_String(86,165,"Value  8");     // Dummy data
        break;
        case 9:  // Pointer position 9
            LCD_8x16_String(86,183,"Value  9");     // Dummy data
        break;
        case 10:  // Pointer position 10
            LCD_8x16_String(86,201,"Value 10");     // Dummy data
        break;
        case 11:  // Pointer position 11
            LCD_8x16_String(86,219,"Value 11");     // Dummy data
        break;
        case 12:  // Pointer position 12
            LCD_8x16_String(244,21,"Value 12");     // Dummy data
        break;
        case 13:  // Pointer position 13
            LCD_8x16_String(244,39,"Value 13");     // Dummy data
        break;
        case 14:  // Pointer position 14
            LCD_8x16_String(244,57,"Value 14");     // Dummy data
        break;
        case 15:  // Pointer position 15
            LCD_8x16_String(244,75,"Value 15");     // Dummy data
        break;
        case 16:  // Pointer position 16
            LCD_8x16_String(244,93,"Value 16");     // Dummy data
        break;
        case 17:  // Pointer position 17
            LCD_8x16_String(244,111,"Value 17");     // Dummy data
        break;
        case 18:  // Pointer position 18
            LCD_8x16_String(244,129,"Value 18");     // Dummy data
        break;
        case 19:  // Pointer position 19
            LCD_8x16_String(244,147,"Value 19");     // Dummy data
        break;
        case 20:  // Pointer position 20
            LCD_8x16_String(244,165,"Value 20");     // Dummy data
        break;
        case 21:  // Pointer position 21
            LCD_8x16_String(244,183,"Value 21");     // Dummy data
        break;
        case 22:  // Pointer position 22
            LCD_8x16_String(244,201,"Value 22");     // Dummy data
        break;
        case 23:  // Pointer position 23
            LCD_8x16_String(244,219,"Value 23");     // Dummy data
        break;
    }
}

/*******************************************************************************
* Save the item value to EEPROM
*
*******************************************************************************/
void Save_Page1_Item()
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
