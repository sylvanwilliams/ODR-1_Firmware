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
#include "DSPIC33E_hardware.h"
#include "si5351a.h"

//Arrays


//Variables


int16 rxtx_mode         = 0;          // Default to CW
int32 radio_freq        = 10000000;   // Default to 10 meters
int32 radio_freq_min    = 500000;     // Minimum Radio Frequency MHz
int32 radio_freq_max    = 60000000;   // Maximum Radio Frequency MHz
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
    int16 encoderCount = Encoder1Count();
    static eButtonState oldButtonState = NO_PRESS;

    if ((NO_PRESS == oldButtonState) && (PRESS == Encoder1ButtonEvent())) //If button has been pressed and released
    {
        if (page_pointer1 & 0x8000) // If pointer already has the focus
        {
            page_pointer1 &= 0x7FFF; // Remove pointer focus and keep position
        }
        else
        {
            page_pointer1 |= 0x8000; // Give pointer focus and keep position
        }
    }

    else if ((page_pointer1 & 0x8000) && (encoderCount)) //pointer has focus and encoder moved
    {
        page_pointer1 += encoderCount;   // add the rotary encoder count to pointer position
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

    else if (encoderCount) //item has focus and encoder moved
    {

        switch(page_pointer1) // Action based on item being pointed to
        {
            case 0x0000:  // Pointer position 0 Mode
                rxtx_mode += encoderCount;
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
                radio_freq += (10000000 * encoderCount);
                Display_Frequency();  // Update Frequency Display
            break;
            case 0x0002:  // Pointer position 2 freq digit 7
                radio_freq += (1000000 * encoderCount);
                Display_Frequency();  // Update Frequency Display
            break;
            case 0x0003:  // Pointer position 3 freq digit 6
                radio_freq += (100000 * encoderCount);
                Display_Frequency();  // Update Frequency Display
            break;
            case 0x0004:  // Pointer position 4 freq digit 5
                radio_freq += (10000 * encoderCount);
                Display_Frequency();  // Update Frequency Display
            break;
            case 0x0005:  // Pointer position 5 freq digit 4
                radio_freq += (1000 * encoderCount);
                Display_Frequency();  // Update Frequency Display
            break;
            case 0x0006:  // Pointer position 6 freq digit 3
                radio_freq += (100 * encoderCount);
                Display_Frequency();  // Update Frequency Display
            break;
            case 0x0007:  // Pointer position 7 freq digit 2
                radio_freq += (10 * encoderCount);
                Display_Frequency();  // Update Frequency Display
            break;
            case 0x0008:  // Pointer position 8 freq digit 1
                radio_freq += encoderCount;
                Display_Frequency();  // Update Frequency Display
            break;
            case 0x0009:  // Pointer position 9 TX offset
                tx_offset += encoderCount;
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
 
    }
    
    oldButtonState = Encoder1ButtonEvent();
    Encoder1CountZero();
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
    int16 encoderCount = Encoder2Count();
    static eButtonState oldButtonState = NO_PRESS;

    if ((NO_PRESS == oldButtonState) && (PRESS == Encoder2ButtonEvent())) //If button has been pressed and released
    {
        if (page_pointer2 & 0x8000) // If pointer already has the focus
        {
            page_pointer2 &= 0x7FFF; // Remove pointer focus and keep position
        }
        else
        {
            page_pointer2 |= 0x8000; // Give pointer focus and keep position
        }
    }

    else if ((page_pointer2 & 0x8000) && (encoderCount)) //pointer has focus and encoder moved
    {
        page_pointer2 += encoderCount; // add the rotary encoder count to pointer position
        if (page_pointer2 > 0x8005) // if above highest possible count
        {
            page_pointer2 = 0x8005; // Stop at highest count
        }
        else if (page_pointer2 < 0x8000) // if below lowest possible count
        {
            page_pointer2 = 0x8000; // Stop at lowest count
        }

        Display_MicGain(); // Display Mic Gain
        Display_KeyerSpeed(); // Display Keyer Speed
        Display_FilterBW(); // Display BW Filter
        Display_RFGain(); // Display RF Gain
        Display_AFGain(); // Display AFGain
    }

    else if (encoderCount) //item has focus and encoder moved
    {

        switch (page_pointer2) // Action based on item being pointed to
        {
        case 0x0000: // Pointer position 0 Main Menu

            Display_MicGain(); // temporarily do something for this case
            break;
        case 0x0001: // Pointer position 1 Mic Gain
            mic_gain += encoderCount;
            if (mic_gain > 20) // if above highest possible count
            {
                mic_gain = 20; // stop at max
            }
            else if (mic_gain < 0) // if below lowest possible count
            {
                mic_gain = 0; // stop at minimum
            }
            Display_MicGain(); // Update Mic Gain Display
            break;
        case 0x0002: // Pointer position 2 keyer speed
            key_speed += encoderCount;
            if (key_speed > 50) // if above highest possible count
            {
                key_speed = 50; // stop at max
            }
            else if (key_speed < 5) // if below lowest possible count
            {
                key_speed = 5; // stop at minimum
            }
            Display_KeyerSpeed(); // Update Keyer speed
            break;
        case 0x0003: // Pointer position 3 BW Filter
            filter_bw += encoderCount;
            if (filter_bw > 1000) // if above highest possible count
            {
                filter_bw = 1000; // stop at max
            }
            else if (filter_bw < 100) // if below lowest possible count
            {
                filter_bw = 100; // stop at minimum
            }
            Display_FilterBW(); // Update BW Filter Display
            break;
        case 0x0004: // Pointer position 4 RF Gain
            rf_gain += encoderCount;
            if (rf_gain > 30) // if above highest possible count
            {
                rf_gain = 30; // stop at max
            }
            else if (rf_gain < -30) // if below lowest possible count
            {
                rf_gain = -30; // stop at minimum
            }
            Display_RFGain(); // Update RF Gain Display
            break;
        case 0x0005: // Pointer position 5 AF Gain
            af_gain += encoderCount;
            if (af_gain > 50) // if above highest possible count
            {
                af_gain = 50; // stop at max
            }
            else if (af_gain < 0) // if below lowest possible count
            {
                af_gain = 0; // stop at minimum
            }
            Display_AFGain(); // Update AF Gain Display
            break;
        }
    }

    oldButtonState = Encoder2ButtonEvent();
    Encoder2CountZero();
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
    LCD_8x16_String(24,144,"TIME UTC");       // Heading
    LCD_8x16_String(132,144,"MIC GAIN");    // Heading
    LCD_8x16_String(248,144,"KEYER");       // Heading
    POINT_COLOR = char_norm_color;
    BACK_COLOR = field_color;
    // LCD_16x24_String(17,162,"free");      // Dummy Display unused parameter

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
* Displays the current RX/TX Mode USB, LSB, CW...
*
*
*
*******************************************************************************/
void Display_RXTX_Mode()
{
    if ((page_pointer1 & 0x00FF)== 0) // If RXTX mode has navigation focus
    {
        BACK_COLOR = char_hglt_color;
    }
    POINT_COLOR = char_norm_color;
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
    BACK_COLOR = field_color; // Restore the background color
}

/*******************************************************************************
* Displays Frequency in large characters and inserts thousands separators
* Highlights the frequency decade to be adjusted
*******************************************************************************/
void Display_Frequency()
{
    uint8 t;          // Character position
    uint16 x = 79;    // Screen X position to display frequency
    uint16 y = 66;    // Screen Y position to display frequency
    uint8 enshow = 0;   // Leading zero suppression flag

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
}

/*******************************************************************************
* Displays the current transmit Offset
*
*
*
*******************************************************************************/
void Display_TX_Offset()
{
    int16 temp;

    if ((page_pointer1 & 0x00FF)== 9)           // If TX offset has navigation focus
    {
        BACK_COLOR = char_hglt_color;          // Turn on highlight
    }
    POINT_COLOR = char_norm_color;
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
    BACK_COLOR = field_color;                  // Restore the background color
}


/*******************************************************************************
* Displays the current Mic Gain
*
*
*
*******************************************************************************/
void Display_MicGain()
{
    if ((page_pointer2 & 0x00FF)== 1) // If Mic Gain has navigation focus
    {
        BACK_COLOR = char_hglt_color;
    }
    POINT_COLOR = char_norm_color;
    LCD_16nz_Num(130,162,mic_gain,2);      // Display two numbers
    LCD_16x24_String(162,162,"db");      // Display Mic Gain db
    BACK_COLOR = field_color;            // Restore the background color
}

/*******************************************************************************
* Displays the current Keyer Speed
*
*
*
*******************************************************************************/
void Display_KeyerSpeed()
{
    if ((page_pointer2 & 0x00FF)== 2) // If Keyer Speed has navigation focus
    {
        BACK_COLOR = char_hglt_color;
    }
    POINT_COLOR = char_norm_color;
    LCD_16nz_Num(226,162,key_speed,2);      // Display two numbers
    LCD_16x24_String(258,162,"wpm");        // Display Keyer Speed
    BACK_COLOR = field_color;               // Restore the background color
}

/*******************************************************************************
* Displays the current BW Filter
*
*
*
*******************************************************************************/
void Display_FilterBW()
{
    if ((page_pointer2 & 0x00FF)== 3) // If BW Filter has navigation focus
    {
        BACK_COLOR = char_hglt_color;
    }
    POINT_COLOR = char_norm_color;
    LCD_16nz_Num(14,210,filter_bw,3);      // Display three numbers
    LCD_16x24_String(62,210,"Hz");         // Display Filter BW
    BACK_COLOR = field_color;              // Restore the background color
}

/*******************************************************************************
* Displays the current RF Gain
*
*
*
*******************************************************************************/
void Display_RFGain()
{
    int16 temp;
    if ((page_pointer2 & 0x00FF)== 4)             // If RF Gain has navigation focus
    {
        BACK_COLOR = char_hglt_color;
    }
    POINT_COLOR = char_norm_color;
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
    BACK_COLOR = field_color;                   // Restore the background color
}

/*******************************************************************************
* Displays the current AF Gain
*
*
*
*******************************************************************************/
void Display_AFGain()
{
    if ((page_pointer2 & 0x00FF)== 5) // If AF Gain has navigation focus
    {
        BACK_COLOR = char_hglt_color;
    }
    POINT_COLOR = char_norm_color;
    LCD_16x24_String(240,210,"+");        // Display AF Gain +
    LCD_16nz_Num(256,210,af_gain,2);      // Display two numbers
    BACK_COLOR = field_color;             // Restore the background color
}


/*******************************************************************************
* Display Universal Time 24Hr format
* Hours and Minutes display
*******************************************************************************/
void Display_UTC_24HR()
{
    uint16 temp;

    RCFGCALbits.RTCPTR = 0b01;               // Set RTCC Register Pointer to Days and Hours
    temp = RTCVAL;                           // Grab data from RTC register
    LCD_16nz_Num(33,162,(temp & 0x000F),1);  // Display HRONE
    temp = temp >> 4;                        // Right shift 4 bits
    LCD_16nz_Num(17,162,(temp & 0x000F),1);  // Display HRTEN
    LCD_16x24_String(49,162,":");
    temp = RTCVAL >> 8;                      // Read minutes and seconds, shift to minutes
    LCD_16nz_Num(81,162,(temp & 0x000F),1);  // Display MINONE
    temp = temp >> 4;                        // Right shift 4 bits
    LCD_16nz_Num(65,162,(temp),1);           // Display MINTEN

}

/*******************************************************************************
* Change the Oscillator Frequency and set the bandpass filters based on frequency
* Then update the display frequency
*
*
*******************************************************************************/
void Change_Freq()
{
    // Don't let radio go out of frequency bounds
    if (radio_freq > radio_freq_max)
    {
        radio_freq = radio_freq_max;
    }
    if (radio_freq < radio_freq_min)
    {
        radio_freq = radio_freq_min;
    }

    // set si5351 freq to 2X desired freq
    si5351aSetFrequency(2 * radio_freq);

    if (radio_freq < 2690001)                                  // < 2.69MHz
    {
        BPF_S0  = 1;
        BPF_S1  = 1;
        BPF_CS0 = 0;
        BPF_CS1 = 1;
    }
    else if ((radio_freq > 2690000)&&(radio_freq < 4800001))   // 2.69-4.80MHz
    {
        BPF_S0  = 1;
        BPF_S1  = 1;
        BPF_CS0 = 1;
        BPF_CS1 = 0;
    }
    else if ((radio_freq > 4800000)&&(radio_freq < 7650001))   // 4.80-7.65MHz
    {
        BPF_S0  = 0;
        BPF_S1  = 1;
        BPF_CS0 = 0;
        BPF_CS1 = 1;
    }
    else if ((radio_freq > 7650000)&&(radio_freq < 11000001))  // 7.65-11.0MHz
    {
        BPF_S0  = 0;
        BPF_S1  = 1;
        BPF_CS0 = 1;
        BPF_CS1 = 0;
    }
    else if ((radio_freq > 11000000)&&(radio_freq < 16100001)) // 11.0-16.1MHz
    {
        BPF_S0  = 1;
        BPF_S1  = 0;
        BPF_CS0 = 0;
        BPF_CS1 = 1;
    }
    else if ((radio_freq > 16100000)&&(radio_freq < 22800001)) // 16.1-22.8MHz
    {
        BPF_S0  = 1;
        BPF_S1  = 0;
        BPF_CS0 = 1;
        BPF_CS1 = 0;
    }
    else if ((radio_freq > 22800000)&&(radio_freq < 31600001)) // 22.8-31.6MHz
    {
        BPF_S0  = 0;
        BPF_S1  = 0;
        BPF_CS0 = 0;
        BPF_CS1 = 1;
    }
    else                                                       // > 31.6Mhz
    {
        BPF_S0  = 0;
        BPF_S1  = 0;
        BPF_CS0 = 1;
        BPF_CS1 = 0;
    }

    Display_Frequency();  // Update Frequency Display
}