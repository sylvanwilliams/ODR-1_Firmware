/*******************************************************************************
* User Interface page 0
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
*
*
*
*******************************************************************************/

#include "UI_page0.h"
#include "main.h"
#include "p33exxxx.h" // processor specific definitions
#include "LCD_driver.h"
#include "UI_main.h"


//Arrays


//Variables


int16 rxtx_mode         = 0;          // Default to CW
int32 radio_freq        = 7100000;    // Default to 40 meters
int16 tx_offset         = 0;          // Transmit Offset

int16 mic_gain          = 10;
int16 key_speed         = 15;
int16 filter_bw         = 500;
int16 rf_gain           = 0;
int16 af_gain           = 15;

/*******************************************************************************
* Navigation pointer #1 is associated with the upper encoder and is used to
* highlight the item being pointed to. If the nav pointer has the focus, the
* pointer itself is moved from item to item. If the pointer does not have focus,
* the item being pointed to will have the focus.
*
*
*
*******************************************************************************/
void Page0_pointer1_update()
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
        if (page_pointer1 > 0x8009)        // if above highest possible count
        {
            page_pointer1 = 0x8009;        // Stop at highest count
        }
        else if (page_pointer1 < 0x8000)   // if below lowest possible count
        {
            page_pointer1 = 0x8000;        // Stop at lowest count
        }

        Display_RXTX_Mode();  // Update Mode display
        Display_Frequency();  // Update Frequency Display
        Display_TX_Offset();  // Update TX Offset Display
    }

    else if (encoder1_count) //item has focus and encoder moved
    {

        switch(page_pointer1) // Action based on item being pointed to
        {
            case 0x0000:  // Pointer position 0 Mode
                rxtx_mode += encoder1_count;
                if (rxtx_mode > 4)        // if above highest possible count
                {
                    rxtx_mode = 0;        // roll over
                }
                else if (rxtx_mode < 0)   // if below lowest possible count
                {
                    rxtx_mode = 3;        // roll over
                }
                Display_RXTX_Mode();      // Display new mode selected
            break;
            case 0x0001:  // Pointer position 1 freq digit 8
                radio_freq += (10000000 * encoder1_count);
                Display_Frequency();  // Update Frequency Display
            break;
            case 0x0002:  // Pointer position 2 freq digit 7
                radio_freq += (1000000 * encoder1_count);
                Display_Frequency();  // Update Frequency Display
            break;
            case 0x0003:  // Pointer position 3 freq digit 6
                radio_freq += (100000 * encoder1_count);
                Display_Frequency();  // Update Frequency Display
            break;
            case 0x0004:  // Pointer position 4 freq digit 5
                radio_freq += (10000 * encoder1_count);
                Display_Frequency();  // Update Frequency Display
            break;
            case 0x0005:  // Pointer position 5 freq digit 4
                radio_freq += (1000 * encoder1_count);
                Display_Frequency();  // Update Frequency Display
            break;
            case 0x0006:  // Pointer position 6 freq digit 3
                radio_freq += (100 * encoder1_count);
                Display_Frequency();  // Update Frequency Display
            break;
            case 0x0007:  // Pointer position 7 freq digit 2
                radio_freq += (10 * encoder1_count);
                Display_Frequency();  // Update Frequency Display
            break;
            case 0x0008:  // Pointer position 8 freq digit 1
                radio_freq += encoder1_count;
                Display_Frequency();  // Update Frequency Display
            break;
            case 0x0009:  // Pointer position 9 TX offset
                tx_offset += encoder1_count;
                if (tx_offset > 50)        // if above highest possible count
                {
                    tx_offset = 50;        // set to highest legal count
                }
                else if (tx_offset < -50)   // if below lowest possible count
                {
                    tx_offset = -50;        // set to lowest legal count
                }
                Display_TX_Offset();      // Display new TX offset
            break;
        }
        encoder1_count = 0;                    // clear encoder count after use
    }
}


/*******************************************************************************
* Navigation pointer #2 is associated with the lower encoder and is used to
* highlight the item being pointed to. If the nav pointer has the focus, the
* pointer itself is moved from item to item. If the pointer does not have focus,
* the item being pointed to will have the focus.
*
*
*
*******************************************************************************/
void Page0_pointer2_update()
{

    if (encoder2_PBcount > 0x8000) //If button has been pressed and released
    {
        if (page_pointer2 & 0x8000) // If pointer already has the focus
        {
            page_pointer2 &= 0x7FFF; // Remove pointer focus and keep position
            encoder2_count = 0;          // Zero out the rotary encoder count
        }
        else
        {
            page_pointer2 |= 0x8000; // Give pointer focus and keep position
            encoder2_count = 0;          // Zero out the rotary encoder count
        }
        encoder2_PBcount = 0;            // Zero out the push button counter
    }

    else if ((page_pointer2 & 0x8000) && (encoder2_count)) //pointer has focus and encoder moved
    {
        page_pointer2 += encoder2_count;   // add the rotary encoder count to pointer position
        encoder2_count = 0;                    // clear encoder count after use
        if (page_pointer2 > 0x8005)        // if above highest possible count
        {
            page_pointer2 = 0x8005;        // Stop at highest count
        }
        else if (page_pointer2 < 0x8000)   // if below lowest possible count
        {
            page_pointer2 = 0x8000;        // Stop at lowest count
        }

        Display_MicGain();      // Display Mic Gain
        Display_KeyerSpeed();   // Display Keyer Speed
        Display_FilterBW();     // Display BW Filter
        Display_RFGain();       // Display RF Gain
        Display_AFGain();       // Display AFGain
    }

    else if (encoder2_count) //item has focus and encoder moved
    {

        switch(page_pointer2) // Action based on item being pointed to
        {
            case 0x0000:  // Pointer position 0 Main Menu

                Display_MicGain();      // temporarily do something for this case
            break;
            case 0x0001:  // Pointer position 1 Mic Gain
                mic_gain += encoder2_count;
                if (mic_gain > 20)        // if above highest possible count
                {
                    mic_gain = 20;        // stop at max
                }
                else if (mic_gain < 0)   // if below lowest possible count
                {
                    mic_gain = 0;        // stop at minimum
                }
                Display_MicGain();  // Update Mic Gain Display
            break;
            case 0x0002:  // Pointer position 2 keyer speed
                key_speed += encoder2_count;
                if (key_speed > 50)        // if above highest possible count
                {
                    key_speed = 50;        // stop at max
                }
                else if (key_speed < 5)   // if below lowest possible count
                {
                    key_speed = 5;        // stop at minimum
                }
                Display_KeyerSpeed();  // Update Keyer speed
            break;
            case 0x0003:  // Pointer position 3 BW Filter
                filter_bw += encoder2_count;
                if (filter_bw > 1000)        // if above highest possible count
                {
                    filter_bw = 1000;        // stop at max
                }
                else if (filter_bw < 100)   // if below lowest possible count
                {
                    filter_bw = 100;        // stop at minimum
                }
                Display_FilterBW();  // Update BW Filter Display
            break;
            case 0x0004:  // Pointer position 4 RF Gain
                rf_gain += encoder2_count;
                if (rf_gain > 30)        // if above highest possible count
                {
                    rf_gain = 30;        // stop at max
                }
                else if (rf_gain < -30)   // if below lowest possible count
                {
                    rf_gain = -30;        // stop at minimum
                }
                Display_RFGain();  // Update RF Gain Display
            break;
            case 0x0005:  // Pointer position 5 AF Gain
                af_gain += encoder2_count;
                if (af_gain > 50)        // if above highest possible count
                {
                    af_gain = 50;        // stop at max
                }
                else if (af_gain < 0)   // if below lowest possible count
                {
                    af_gain = 0;        // stop at minimum
                }
                Display_AFGain();  // Update AF Gain Display
            break;
        }
        encoder2_count = 0;                    // clear encoder count after use
    }
}


/*******************************************************************************
* Refresh page 0 draws the main radio screen graphics and headers.
* 
*******************************************************************************/
void Refresh_page0()
{
    LCD_Clear(field_color);        //Clear the screen with field color

    POINT_COLOR = border_color;
    LCD_DrawRectangle(2, 2, 317, 44);      // Bandscope Border

    POINT_COLOR = border_color;
    LCD_DrawRectangle(2, 47, 72, 92);     // Mode Border
    LCD_DrawRectangle(75, 47, 244, 92);   // Frequency Border
    LCD_DrawRectangle(247, 47, 317, 92);  // TX Offset Border
    LCD_Fill(3,48,71,63,heading_color);     // Mode Heading Fill
    LCD_Fill(76,48,243,63,heading_color);   // Frequency Heading Fill
    LCD_Fill(248,48,316,63,heading_color);  // TX Offset Heading Fill
    POINT_COLOR = char_rev_color;
    BACK_COLOR = heading_color;
    LCD_8x16_String(20,48,"Mode");      // Heading
    LCD_8x16_String(83,48,"MHz");       // Heading
    LCD_8x16_String(140,48,"kHz");      // Heading
    LCD_8x16_String(208,48,"Hz");       // Heading
    LCD_8x16_String(254,48,"TX OFST");  // Heading

    POINT_COLOR = border_color;
    LCD_DrawRectangle(2, 95, 105, 140);      // Signal Border
    LCD_DrawRectangle(108, 95, 211, 140);    // SWR Border
    LCD_DrawRectangle(214, 95, 317, 140);    // Battery Border
    LCD_Fill(3,96,104,111,heading_color);    // Signal Heading Fill
    LCD_Fill(109,96,210,111,heading_color);  // SWR Heading Fill
    LCD_Fill(215,96,316,111,heading_color);  // Battery Heading Fill
    POINT_COLOR = char_rev_color;
    BACK_COLOR = heading_color;
    LCD_8x16_String(24,96,"SIGNAL");       // Heading
    LCD_8x16_String(148,96,"SWR");         // Heading
    LCD_8x16_String(240,96,"BATTERY");     // Heading
    POINT_COLOR = char_norm_color;
    //BACK_COLOR = YELLOW;
    BACK_COLOR = field_color;
    LCD_16x24_String(10,114,"S9+20");      // Dummy Signal parameter
    LCD_16x24_String(112,114,"1.0:1");    // Dummy SWR parameter
    LCD_16x24_String(222,114,"13.8");     // Dummy Battery parameter

    POINT_COLOR = border_color;
    LCD_DrawRectangle(2, 143, 105, 188);    // Set up Border
    LCD_DrawRectangle(108, 143, 211, 188);  // Mic Gain Border
    LCD_DrawRectangle(214, 143, 317, 188);  // Keyer Border
    LCD_Fill(3,144,104,159,heading_color);    // Set-Up Heading Fill
    LCD_Fill(109,144,210,159,heading_color);  // Mic Gain Heading Fill
    LCD_Fill(215,144,316,159,heading_color);  // Keyer Heading Fill
    POINT_COLOR = char_rev_color;
    BACK_COLOR = heading_color;
    LCD_8x16_String(24,144,"UNUSED");       // Heading
    LCD_8x16_String(132,144,"MIC GAIN");    // Heading
    LCD_8x16_String(248,144,"KEYER");       // Heading
    POINT_COLOR = char_norm_color;
    BACK_COLOR = field_color;
    LCD_16x24_String(17,162,"free");      // Dummy Display unused parameter

    POINT_COLOR = border_color;
    LCD_DrawRectangle(2, 191, 105, 236);    // BW Filter Border
    LCD_DrawRectangle(108, 191, 211, 236);  // RF Gain Border
    LCD_DrawRectangle(214, 191, 317, 236);  // AF Gain Border
    LCD_Fill(3,192,104,207,heading_color);    // BW Filter Heading Fill
    LCD_Fill(109,192,210,207,heading_color);  // RF Gain Heading Fill
    LCD_Fill(215,192,316,207,heading_color);  // AF Gain Heading Fill
    POINT_COLOR = char_rev_color;
    BACK_COLOR = heading_color;
    LCD_8x16_String(20,192,"BW FILTER");   // Heading
    LCD_8x16_String(132,192,"RF GAIN");    // Heading
    LCD_8x16_String(244,192,"AF GAIN");    // Heading

    POINT_COLOR = char_norm_color;
    BACK_COLOR = field_color;
    
    Display_RXTX_Mode();    // Update Mode display
    Display_Frequency();    // Update Frequency Display
    Display_TX_Offset();    // Update TX Offset Display
    Display_MicGain();      // Display Mic Gain
    Display_KeyerSpeed();   // Display Keyer Speed
    Display_FilterBW();     // Display BW Filter
    Display_RFGain();       // Display RF Gain
    Display_AFGain();       // Display AFGain
}

/*******************************************************************************
* Refresh the item display
*
*******************************************************************************/
void Refresh_Page0_Item(int16 t)
{
    uint8 t;          // Character position
    uint16 x = 79;    // Screen X position to display frequency
    uint16 y = 66;    // Screen Y position to display frequency
    uint8 enshow=0;   // Leading zero suppression flag
    int16 temp;

    switch(t) // Action based on item being pointed to
    {
        case 0:  // Item 0
            switch(rxtx_mode) // Action based on mode selected
            {
                case 0:  // Mode 0 CW
                    LCD_16x24_String(10,66," CW");      // Display Mode CW
                break;
                case 1:  // Mode 1 USB
                    LCD_16x24_String(10,66,"USB");      // Display Mode USB
                break;
                case 2:  // Mode 2 LSB
                    LCD_16x24_String(10,66,"LSB");      // Display Mode LSB
                break;
                case 3:  // Mode 2 LSB
                    LCD_16x24_String(10,66," AM");      // Display Mode AM
                break;
            }
        break;
        case 1 ... 8:  // Display Frequency
            NumToCharArray(radio_freq); // Convert frequency to character array

            // Display array characters on screen suppressing leading zeros and inserting separators
            for(t=(10-8);t<10;t++)   // Display least significant 8 of the 10 total charactors
            {
                if (t == ((page_pointer1 & 0x00FF) + 1)) // Check if this character position needs highlighted
                {
                    BACK_COLOR = char_hglt_color; // Turn on the highlight color
                }

                if((enshow==0) && (num32_char[t]=='0'))     // test for leading zeros
                {
                    LCD_16x24_Char(x,y,' ',0);              // display space instead of zero
                    BACK_COLOR = field_color;               // Restore the background color
                    x = x+16;                               // increment character position
                    if ((t==0)|(t==3)|(t==6))               // thoushands separator positions
                    {
                       LCD_16x24_Char(x,y,' ',0);           // display space instead of separator
                       x = x+16;                            // increment character position
                    }
                }
                else
                {
                    enshow = 1;                             // leading zero flag off
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
        break;
        case 9:  // Displays the current transmit Offset
            if (tx_offset < 0)                         // If the offset is negetive
            {
                temp = ((tx_offset ^ 0xFFFF)+ 0x0001); // Twos Complement to negate sign
                LCD_16x24_Char(252,66,'-',0);          // Display negetive symbol, mode=0
                LCD_16nz_Num(268,66,temp,2);           // Display two numbers
                LCD_16x24_Char(300,66,'k',0);          // Display k , mode=0
                //LCD_16x24_String(248,69,"+10k");
            }
            else
            {
                LCD_16x24_Char(252,66,'+',0);          // Display plus symbol, mode=0
                LCD_16nz_Num(268,66,tx_offset,2);      // Display two numbers
                LCD_16x24_Char(300,66,'k',0);          // Display k , mode=0
            }
        break;
        case 10:  // Displays the current Mic Gain
            LCD_16nz_Num(130,162,mic_gain,2);      // Display two numbers
            LCD_16x24_String(162,162,"db");      // Display Mic Gain db
        break;
        case 11:  // Displays the current Keyer Speed
            LCD_16nz_Num(226,162,key_speed,2);      // Display two numbers
            LCD_16x24_String(258,162,"wpm");        // Display Keyer Speed
        break;
        case 12:  // Displays the current BW Filter
            LCD_16nz_Num(14,210,filter_bw,3);      // Display three numbers
            LCD_16x24_String(62,210,"Hz");         // Display Filter BW
        break;
        case 13:  // Displays the current RF Gain
            if (rf_gain < 0)                            // If the rf gain is negetive
            {
                temp = ((rf_gain ^ 0xFFFF)+ 0x0001);    // Twos Complement to negate sign
                LCD_16x24_Char(125,210,'-',0);          // Display negetive symbol, mode=0
                LCD_16nz_Num(141,210,temp,2);           // Display two numbers
                LCD_16x24_String(173,210,"db");         // Display RF Gain db
            }
            else
            {
                LCD_16x24_Char(125,210,'+',0);          // Display plus symbol, mode=0
                LCD_16nz_Num(141,210,rf_gain,2);        // Display two numbers
                LCD_16x24_String(173,210,"db");         // Display RF Gain db
            }
        break;
        case 14:  // Displays the current AF Gain
            LCD_16x24_String(240,210,"+");        // Display AF Gain +
            LCD_16nz_Num(256,210,af_gain,2);      // Display two numbers
        break;

    }
}




