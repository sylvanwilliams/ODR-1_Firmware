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

    if (encoder1_PBcount > 0x8000) //If button has been pressed and released
    {
        if (page_pointer1 & 0x8000) // If pointer already has the focus
        {
            page_pointer1 &= 0x7FFF; // Remove pointer focus and keep position
            encoder1_count = 0;          // Zero out the rotary encoder count
        }
        else
        {
            page_pointer1 |= 0x8000; // Give pointer focus and keep position
            encoder1_count = 0;          // Zero out the rotary encoder count
        }
        encoder1_PBcount = 0;            // Zero out the push button counter
    }

    else if ((page_pointer1 & 0x8000) && (encoder1_count)) //pointer has focus and encoder moved
    {
        page_pointer1 += encoder1_count;   // add the rotary encoder count to pointer position
        encoder1_count = 0;                    // clear encoder count after use
        if (page_pointer1 > 0x8017)        // if above highest possible count
        {
            page_pointer1 = 0x8017;        // Stop at highest count
        }
        else if (page_pointer1 < 0x8000)   // if below lowest possible count
        {
            page_pointer1 = 0x8000;        // Stop at lowest count
        }

        Display_P1_0();
        Display_P1_1();
        Display_P1_2();
        Display_P1_3();
        Display_P1_4();
        Display_P1_5();
        Display_P1_6();
        Display_P1_7();
        Display_P1_8();
        Display_P1_9();
        Display_P1_10();
        Display_P1_11();
    
        Display_P1_12();
        Display_P1_13();
        Display_P1_14();
        Display_P1_15();
        Display_P1_16();
        Display_P1_17();
        Display_P1_18();
        Display_P1_19();
        Display_P1_20();
        Display_P1_21();
        Display_P1_22();
        Display_P1_23();

    }

    else if (encoder1_count) //item has focus and encoder moved
    {

        switch(page_pointer1) // Action based on item being pointed to
        {
            case 0:  // Pointer position 0
                Display_P1_0();
            break;
            case 1:  // Pointer position 1
                Display_P1_1();
            break;
            case 2:  // Pointer position 2
                Display_P1_2();
            break;
            case 3:  // Pointer position 3
                Display_P1_3();
            break;
            case 4:  // Pointer position 4
                Display_P1_4();
            break;
            case 5:  // Pointer position 5
                Display_P1_5();
            break;
            case 6:  // Pointer position 6
                Display_P1_6();
            break;
            case 7:  // Pointer position 7
                Display_P1_7();
            break;
            case 8:  // Pointer position 8
                Display_P1_8();
            break;
            case 9:  // Pointer position 9
                Display_P1_9();
            break;
            case 10:  // Pointer position 10
                Display_P1_10();
            break;
            case 11:  // Pointer position 11
                Display_P1_11();
            break;
            case 12:  // Pointer position 12
                Display_P1_12();
            break;
            case 13:  // Pointer position 13
                Display_P1_13();
            break;
            case 14:  // Pointer position 14
                Display_P1_14();
            break;
            case 15:  // Pointer position 15
                Display_P1_15();
            break;
            case 16:  // Pointer position 16
                Display_P1_16();
            break;
            case 17:  // Pointer position 17
                Display_P1_17();
            break;
            case 18:  // Pointer position 18
                Display_P1_18();
            break;
            case 19:  // Pointer position 19
                Display_P1_19();
            break;
            case 20:  // Pointer position 20
                Display_P1_20();
            break;
            case 21:  // Pointer position 21
                Display_P1_21();
            break;
            case 22:  // Pointer position 22
                Display_P1_22();
            break;
            case 23:  // Pointer position 23
                Display_P1_23();
            break;
        }
        encoder1_count = 0;   // clear encoder count after use
    }
}

/*******************************************************************************
* Refresh page 1 draws screen graphics and headers.
*
*******************************************************************************/
void Refresh_page1()
{
    LCD_Clear(field_color);        //Clear the screen with field color

    POINT_COLOR = border_color;
    LCD_DrawRectangle(2,2,317,237);                  // Screen Border
    LCD_Fill(3,3,316,18,heading_color);                 // Heading Fill
    LCD_DrawLine(159,19,159,236);                  // Screen Border
    POINT_COLOR = char_rev_color;
    BACK_COLOR = heading_color;
    LCD_8x16_String(68,3,"ADVANCED SETTINGS PAGE 1");   // Heading

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
    
    Display_P1_0();
    Display_P1_1();
    Display_P1_2();
    Display_P1_3();
    Display_P1_4();
    Display_P1_5();
    Display_P1_6();
    Display_P1_7();
    Display_P1_8();
    Display_P1_9();
    Display_P1_10();
    Display_P1_11();
    
    Display_P1_12();
    Display_P1_13();
    Display_P1_14();
    Display_P1_15();
    Display_P1_16();
    Display_P1_17();
    Display_P1_18();
    Display_P1_19();
    Display_P1_20();
    Display_P1_21();
    Display_P1_22();
    Display_P1_23();
}

/*******************************************************************************
* Display the item information
*
*******************************************************************************/
void Display_P1_0()
{
    if ((page_pointer1 & 0x00FF)== 0) // If AF Gain has navigation focus
    {
        BACK_COLOR = char_hglt_color;
    }
    POINT_COLOR = char_norm_color;
    LCD_8x16_String(86,21,"Value  0");     // Dummy data
    BACK_COLOR = field_color;             // Restore the background color
}

/*******************************************************************************
* Display the item information
*
*******************************************************************************/
void Display_P1_1()
{
    if ((page_pointer1 & 0x00FF)== 1) // If AF Gain has navigation focus
    {
        BACK_COLOR = char_hglt_color;
    }
    POINT_COLOR = char_norm_color;
    LCD_8x16_String(86,39,"Value  1");     // Dummy data
    BACK_COLOR = field_color;             // Restore the background color
}

/*******************************************************************************
* Display the item information
*
*******************************************************************************/
void Display_P1_2()
{
    if ((page_pointer1 & 0x00FF)== 2) // If AF Gain has navigation focus
    {
        BACK_COLOR = char_hglt_color;
    }
    POINT_COLOR = char_norm_color;
    LCD_8x16_String(86,57,"Value  2");     // Dummy data
    BACK_COLOR = field_color;             // Restore the background color
}

/*******************************************************************************
* Display the item information
*
*******************************************************************************/
void Display_P1_3()
{
    if ((page_pointer1 & 0x00FF)== 3) // If AF Gain has navigation focus
    {
        BACK_COLOR = char_hglt_color;
    }
    POINT_COLOR = char_norm_color;
    LCD_8x16_String(86,75,"Value  3");     // Dummy data
    BACK_COLOR = field_color;             // Restore the background color
}

/*******************************************************************************
* Display the item information
*
*******************************************************************************/
void Display_P1_4()
{
    if ((page_pointer1 & 0x00FF)== 4) // If AF Gain has navigation focus
    {
        BACK_COLOR = char_hglt_color;
    }
    POINT_COLOR = char_norm_color;
    LCD_8x16_String(86,93,"Value  4");     // Dummy data
    BACK_COLOR = field_color;             // Restore the background color
}

/*******************************************************************************
* Display the item information
*
*******************************************************************************/
void Display_P1_5()
{
    if ((page_pointer1 & 0x00FF)== 5) // If AF Gain has navigation focus
    {
        BACK_COLOR = char_hglt_color;
    }
    POINT_COLOR = char_norm_color;
    LCD_8x16_String(86,111,"Value  5");     // Dummy data
    BACK_COLOR = field_color;             // Restore the background color
}

/*******************************************************************************
* Display the item information
*
*******************************************************************************/
void Display_P1_6()
{
    if ((page_pointer1 & 0x00FF)== 6) // If AF Gain has navigation focus
    {
        BACK_COLOR = char_hglt_color;
    }
    POINT_COLOR = char_norm_color;
    LCD_8x16_String(86,129,"Value  6");     // Dummy data
    BACK_COLOR = field_color;             // Restore the background color
}

/*******************************************************************************
* Display the item information
*
*******************************************************************************/
void Display_P1_7()
{
    if ((page_pointer1 & 0x00FF)== 7) // If AF Gain has navigation focus
    {
        BACK_COLOR = char_hglt_color;
    }
    POINT_COLOR = char_norm_color;
    LCD_8x16_String(86,147,"Value  7");     // Dummy data
    BACK_COLOR = field_color;             // Restore the background color
}

/*******************************************************************************
* Display the item information
*
*******************************************************************************/
void Display_P1_8()
{
    if ((page_pointer1 & 0x00FF)== 8) // If AF Gain has navigation focus
    {
        BACK_COLOR = char_hglt_color;
    }
    POINT_COLOR = char_norm_color;
    LCD_8x16_String(86,165,"Value  8");     // Dummy data
    BACK_COLOR = field_color;             // Restore the background color
}

/*******************************************************************************
* Display the item information
*
*******************************************************************************/
void Display_P1_9()
{
    if ((page_pointer1 & 0x00FF)== 9) // If AF Gain has navigation focus
    {
        BACK_COLOR = char_hglt_color;
    }
    POINT_COLOR = char_norm_color;
    LCD_8x16_String(86,183,"Value  9");     // Dummy data
    BACK_COLOR = field_color;             // Restore the background color
}

/*******************************************************************************
* Display the item information
*
*******************************************************************************/
void Display_P1_10()
{
    if ((page_pointer1 & 0x00FF)== 10) // If AF Gain has navigation focus
    {
        BACK_COLOR = char_hglt_color;
    }
    POINT_COLOR = char_norm_color;
    LCD_8x16_String(86,201,"Value 10");     // Dummy data
    BACK_COLOR = field_color;             // Restore the background color
}

/*******************************************************************************
* Display the item information
*
*******************************************************************************/
void Display_P1_11()
{
    if ((page_pointer1 & 0x00FF)== 11) // If AF Gain has navigation focus
    {
        BACK_COLOR = char_hglt_color;
    }
    POINT_COLOR = char_norm_color;
    LCD_8x16_String(86,219,"Value 11");     // Dummy data
    BACK_COLOR = field_color;             // Restore the background color
}

/*******************************************************************************
* Display the item information
*
*******************************************************************************/
void Display_P1_12()
{
    if ((page_pointer1 & 0x00FF)== 12) // If AF Gain has navigation focus
    {
        BACK_COLOR = char_hglt_color;
    }
    POINT_COLOR = char_norm_color;
    LCD_8x16_String(244,21,"Value 12");     // Dummy data
    BACK_COLOR = field_color;             // Restore the background color
}

/*******************************************************************************
* Display the item information
*
*******************************************************************************/
void Display_P1_13()
{
    if ((page_pointer1 & 0x00FF)== 13) // If AF Gain has navigation focus
    {
        BACK_COLOR = char_hglt_color;
    }
    POINT_COLOR = char_norm_color;
    LCD_8x16_String(244,39,"Value 13");     // Dummy data
    BACK_COLOR = field_color;             // Restore the background color
}

/*******************************************************************************
* Display the item information
*
*******************************************************************************/
void Display_P1_14()
{
    if ((page_pointer1 & 0x00FF)== 14) // If AF Gain has navigation focus
    {
        BACK_COLOR = char_hglt_color;
    }
    POINT_COLOR = char_norm_color;
    LCD_8x16_String(244,57,"Value 14");     // Dummy data
    BACK_COLOR = field_color;             // Restore the background color
}

/*******************************************************************************
* Display the item information
*
*******************************************************************************/
void Display_P1_15()
{
    if ((page_pointer1 & 0x00FF)== 15) // If AF Gain has navigation focus
    {
        BACK_COLOR = char_hglt_color;
    }
    POINT_COLOR = char_norm_color;
    LCD_8x16_String(244,75,"Value 15");     // Dummy data
    BACK_COLOR = field_color;             // Restore the background color
}

/*******************************************************************************
* Display the item information
*
*******************************************************************************/
void Display_P1_16()
{
    if ((page_pointer1 & 0x00FF)== 16) // If AF Gain has navigation focus
    {
        BACK_COLOR = char_hglt_color;
    }
    POINT_COLOR = char_norm_color;
    LCD_8x16_String(244,93,"Value 16");     // Dummy data
    BACK_COLOR = field_color;             // Restore the background color
}

/*******************************************************************************
* Display the item information
*
*******************************************************************************/
void Display_P1_17()
{
    if ((page_pointer1 & 0x00FF)== 17) // If AF Gain has navigation focus
    {
        BACK_COLOR = char_hglt_color;
    }
    POINT_COLOR = char_norm_color;
    LCD_8x16_String(244,111,"Value 17");     // Dummy data
    BACK_COLOR = field_color;             // Restore the background color
}

/*******************************************************************************
* Display the item information
*
*******************************************************************************/
void Display_P1_18()
{
    if ((page_pointer1 & 0x00FF)== 18) // If AF Gain has navigation focus
    {
        BACK_COLOR = char_hglt_color;
    }
    POINT_COLOR = char_norm_color;
    LCD_8x16_String(244,129,"Value 18");     // Dummy data
    BACK_COLOR = field_color;             // Restore the background color
}

/*******************************************************************************
* Display the item information
*
*******************************************************************************/
void Display_P1_19()
{
    if ((page_pointer1 & 0x00FF)== 19) // If AF Gain has navigation focus
    {
        BACK_COLOR = char_hglt_color;
    }
    POINT_COLOR = char_norm_color;
    LCD_8x16_String(244,147,"Value 19");     // Dummy data
    BACK_COLOR = field_color;             // Restore the background color
}

/*******************************************************************************
* Display the item information
*
*******************************************************************************/
void Display_P1_20()
{
    if ((page_pointer1 & 0x00FF)== 20) // If AF Gain has navigation focus
    {
        BACK_COLOR = char_hglt_color;
    }
    POINT_COLOR = char_norm_color;
    LCD_8x16_String(244,165,"Value 20");     // Dummy data
    BACK_COLOR = field_color;             // Restore the background color
}

/*******************************************************************************
* Display the item information
*
*******************************************************************************/
void Display_P1_21()
{
    if ((page_pointer1 & 0x00FF)== 21) // If AF Gain has navigation focus
    {
        BACK_COLOR = char_hglt_color;
    }
    POINT_COLOR = char_norm_color;
    LCD_8x16_String(244,183,"Value 21");     // Dummy data
    BACK_COLOR = field_color;             // Restore the background color
}

/*******************************************************************************
* Display the item information
*
*******************************************************************************/
void Display_P1_22()
{
    if ((page_pointer1 & 0x00FF)== 22) // If AF Gain has navigation focus
    {
        BACK_COLOR = char_hglt_color;
    }
    POINT_COLOR = char_norm_color;
    LCD_8x16_String(244,201,"Value 22");     // Dummy data
    BACK_COLOR = field_color;             // Restore the background color
}

/*******************************************************************************
* Display the item information
*
*******************************************************************************/
void Display_P1_23()
{
    if ((page_pointer1 & 0x00FF)== 23) // If AF Gain has navigation focus
    {
        BACK_COLOR = char_hglt_color;
    }
    POINT_COLOR = char_norm_color;
    LCD_8x16_String(244,219,"Value 23");     // Dummy data
    BACK_COLOR = field_color;             // Restore the background color
}