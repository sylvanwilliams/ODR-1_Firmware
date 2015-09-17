/*******************************************************************************
* User Interface page 0
*
* This work is licensed under the Creative Commons Attribution-ShareAlike
* 4.0 International License. To view a copy of this license, visit
* http://creativecommons.org/licenses/by-sa/4.0/ or send a letter to
* Creative Commons, PO Box 1866, Mountain View, CA 94042, USA.
*******************************************************************************/

#include "UI_page0.h"
#include "main.h"
#include "p33exxxx.h" // processor specific definitions
#include "LCD_driver.h"
#include "UI_main.h"
#include "DSPIC33E_hardware.h"
#include "si5351a.h"
#include "TLV320AIC3204.h"
#include "ODR1_Control_1.h"
#include "ODR1_Control_2.h"
#include "ODR1_Meters.h"

//Arrays


//Variables

int16 rxtx_mode         = 0;          // Default to CW
int16 tx_offset         = 0;          // Transmit Offset

int16 mic_gain          = 10;
int16 key_speed         = 15;
int16 filter_bw         = 500;
int16 rf_gain           = 2;
int16 af_gain           = 15;

int16 rtc_hrs           = 23;  // Real time clock hours 0-23
int16 rtc_min           = 59;  // Real time clock minutes 0-59
int16 rtc_sec           = 00;  // Real time clock seconds 0-59

uint16 time_slice       = 0;   // A counter to schedule tasks

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
    int16 temp1; // temporary variable used for testing
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
                if (rxtx_mode > 6)        // if above highest possible count
                {
                    rxtx_mode = 0;        // roll over
                }
                else if (rxtx_mode < 0)   // if below lowest possible count
                {
                    rxtx_mode = 6;        // roll over
                }
                Display_RXTX_Mode();      // Display new mode selected
                Radio_Receive();          // Set Radio to Receive
            break;
            case 0x0001:  // Pointer position 1 freq digit 8
                radio_freq += (10000000 * encoderCount);
                Change_Freq();          // Update Osc Freq and filters
                Display_Frequency();    // Update Frequency Display
            break;
            case 0x0002:  // Pointer position 2 freq digit 7
                radio_freq += (1000000 * encoderCount);
                Change_Freq();          // Update Osc Freq and filters
                Display_Frequency();    // Update Frequency Display
            break;
            case 0x0003:  // Pointer position 3 freq digit 6
                radio_freq += (100000 * encoderCount);
                Change_Freq();          // Update Osc Freq and filters
                Display_Frequency();    // Update Frequency Display
            break;
            case 0x0004:  // Pointer position 4 freq digit 5
                radio_freq += (10000 * encoderCount);
                Change_Freq();          // Update Osc Freq and filters
                Display_Frequency();    // Update Frequency Display
            break;
            case 0x0005:  // Pointer position 5 freq digit 4
                radio_freq += (1000 * encoderCount);
                Change_Freq();          // Update Osc Freq and filters
                Display_Frequency();    // Update Frequency Display
            break;
            case 0x0006:  // Pointer position 6 freq digit 3
                radio_freq += (100 * encoderCount);
                Change_Freq();          // Update Osc Freq and filters
                Display_Frequency();    // Update Frequency Display
            break;
            case 0x0007:  // Pointer position 7 freq digit 2
                radio_freq += (10 * encoderCount);
                Change_Freq();          // Update Osc Freq and filters
                Display_Frequency();    // Update Frequency Display
            break;
            case 0x0008:  // Pointer position 8 freq digit 1
                radio_freq += encoderCount;
                Change_Freq();          // Update Osc Freq and filters
                Display_Frequency();    // Update Frequency Display
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
    else // If encoder does not need attention, do this
    {
         time_slice ++;        // Increment the time slice counter
         if (time_slice > 20)  // Higher values slow update rate
         {
             time_slice = 0;   // Roll over the time slice counter
         }
         switch(time_slice)    // Schedule one item to run
        {
            case 0:
                Display_Battery_Volts();  // Display Battery Voltage
            break;
            case 1:
                Display_OSCTemp(); //Temporarily show oscillator temperature
            break;
            case 2: //Temperature Compensate Oscillator Frequency
                Change_Freq();   // Temperature compensate Oscillator
            break;
            case 3: //Temporarily show frequency compensation
                temp1 = Freq_Error_Comp();  // Debug get frequency error
                LCD_16nz_Num(10,16,temp1,5); // Debug display error in Hz
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

    Read_RTC_Time(); // Get the current time from the RTC module

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
        if (page_pointer2 > 0x8007) // if above highest possible count
        {
            page_pointer2 = 0x8007; // Stop at highest count
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
        case 0x0000: // Pointer position 0 Time hours
            rtc_hrs += encoderCount;
            Write_RTC_Hrs(rtc_hrs);
            break;
        case 0x0001: // Pointer position 1 Time Minutes
            rtc_min += encoderCount;
            Write_RTC_Min(rtc_min);
            break;
        case 0x0002: // Pointer position 2 Time Seconds
            rtc_sec += encoderCount;
            Write_RTC_Sec(rtc_sec);
            break;
        case 0x0003: // Pointer position 1 Mic Gain
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
        case 0x0004: // Pointer position 2 keyer speed
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
        case 0x0005: // Pointer position 3 BW Filter
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
        case 0x0006: // Pointer position 4 RF Gain
            rf_gain += encoderCount;
            if (rf_gain > 3)        // if above highest possible count
            {
                rf_gain = 3;        // set to max count
            }
            else if (rf_gain < 0)   // if below lowest possible count
            {
                rf_gain = 0;        // set to lowest count
            }
            Set_RFGain(rf_gain);    // Set the RF gain on the Osc & Mix Board
            Display_RFGain(); // Update RF Gain Display
            break;
        case 0x0007: // Pointer position 5 AF Gain
            af_gain += encoderCount;
            if (af_gain > 29) // if above highest possible count
            {
                af_gain = 29; // stop at max
            }
            else if (af_gain < -6) // if below lowest possible count
            {
                af_gain = -6; // stop at minimum
            }
            Codec_HP_Gain ((int8)(af_gain)); // Change the Headphone amp gain
            Display_AFGain(); // Update AF Gain Display
            break;
        }
    }
    else  // Encoder does not need attention, do this
    {
        Display_UTC_24HR(); // Update time display
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
    // LCD_16x24_String(222,114,"13.8");     // Dummy Battery parameter

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
    Display_Battery_Volts(); // Display Battery Voltage
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
        case 3:  // Mode 3 AM
            LCD_16x24_String(10,66," AM");      // Display Mode AM
        break;
        case 4:  // Mode 4 Analog Pass Through
            LCD_16x24_String(10,66,"PT1");      // Display Mode PT
        break;
        case 5:  // Mode 5 ADC-DAC Pass Through
            LCD_16x24_String(10,66,"PT2");      // Display Mode PT
        break;
        case 6:  // Mode 6 ADC-DSP-DAC Pass Through
            LCD_16x24_String(10,66,"PT3");      // Display Mode PT
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
* Displays the Battery Voltage
*
*
*
*******************************************************************************/
void Display_Battery_Volts()
{
    int16 temp;

    POINT_COLOR = char_norm_color;
    BACK_COLOR = field_color;
    temp = Battery_Volts();             // Get the latest battery voltage
    LCD_16x24_Char(258,114,'.',0);      // Display period symbol, mode=0
    LCD_16wz_Num(274,114,temp,2);       // Display two decimal digits
    temp = (temp/100);                  // Divide number by 100
    LCD_16nz_Num(226,114,temp,2);       // Display two 10s digits
}

/*******************************************************************************
* Displays the current Mic Gain
*
*
*
*******************************************************************************/
void Display_MicGain()
{
    if ((page_pointer2 & 0x00FF)== 3) // If Mic Gain has navigation focus
    {
        BACK_COLOR = char_hglt_color;
    }
    POINT_COLOR = char_norm_color;
    LCD_16nz_Num(130,162,mic_gain,2);      // Display two numbers
    LCD_16x24_String(162,162,"dB");      // Display Mic Gain db
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
    if ((page_pointer2 & 0x00FF)== 4) // If Keyer Speed has navigation focus
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
    if ((page_pointer2 & 0x00FF)== 5) // If BW Filter has navigation focus
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
    if ((page_pointer2 & 0x00FF)== 6)     // If RF Gain has navigation focus
    {
        BACK_COLOR = char_hglt_color;
    }
    POINT_COLOR = char_norm_color;
    switch(rf_gain) // Action based on gain selected
    {
        case 0:  // Set -24dB RF Gain
            LCD_16x24_String(120,210,"-24dB");
        break;
        case 1:  // Set -6dB RF Gain
            LCD_16x24_String(120,210,"- 6dB");
        break;
        case 2:  // Set 0dB RF Gain
            LCD_16x24_String(120,210,"  0dB");
        break;
        case 3:  // Set +15dB RF Gain
            LCD_16x24_String(120,210,"+15dB");
        break;
        default: // Default 0dB RF Gain
            LCD_16x24_String(120,210,"  0dB");
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
    int16 temp;
    if ((page_pointer2 & 0x00FF)== 7) // If AF Gain has navigation focus
    {
        BACK_COLOR = char_hglt_color;
    }
    POINT_COLOR = char_norm_color;

    if (af_gain < 0)                          // If the af gain is negetive
    {
        temp = ((af_gain ^ 0xFFFF)+ 0x0001);  // Twos Complement to negate sign
        LCD_16x24_Char(226,210,'-',0);        // Display negetive symbol, mode=0
        LCD_16nz_Num(242,210,temp,2);         // Display two numbers
        LCD_16x24_String(274,210,"dB");       // Display AF Gain db
    }
    else
    {
        LCD_16x24_String(226,210,"+");        // Display AF Gain +
        LCD_16nz_Num(242,210,af_gain,2);      // Display two numbers
        LCD_16x24_String(274,210,"dB");       // Display AF Gain db
    }
    BACK_COLOR = field_color;             // Restore the background color
}

/*******************************************************************************
* Display Universal Time 24Hr format
* Hours Minutes and seconds
*******************************************************************************/
void Display_UTC_24HR()
{
    uint16 temp = (page_pointer2 & 0x00FF); // mask off the pointer count
    POINT_COLOR = char_norm_color;

    if (temp == 0)                       // If Hours has the focus
    {
        BACK_COLOR = char_hglt_color;    // Highlight hours
    }
    LCD_16wz_Num(12,162,rtc_hrs,2);      // Display hours   16x24 font
    BACK_COLOR = field_color;            // Restore the background color

    if (temp == 1)                       // If Minutes has the focus
    {
        BACK_COLOR = char_hglt_color;    // Highlight minutes
    }
    LCD_16wz_Num(44,162,rtc_min,2);      // Display minutes 16x24 font
    BACK_COLOR = field_color;            // Restore the background color

    if (temp == 2)                       // If Seconds has the focus
    {
        BACK_COLOR = char_hglt_color;    // Highlight seconds
    }
    LCD_16wz8_Num(80,170,rtc_sec,2);     // Display seconds 8x16 font
    BACK_COLOR = field_color;            // Restore the background color
}

/*******************************************************************************
* Read the Real Time Clock Module
* Store the result in Hours, Minutes, and Seconds variables
*******************************************************************************/
void Read_RTC_Time()
{
    uint16 temp;

    RCFGCALbits.RTCPTR = 0b01;               // Set RTCC Register Pointer to Days and Hours
    temp = RTCVAL;                           // Read Weekday and Hours
    rtc_hrs = (temp & 0x0030);               // mask off the HRTEN digit
    rtc_hrs = ((rtc_hrs >>4) * 10);          // Create decimal of 00, 10 or 20
    rtc_hrs = (rtc_hrs + (temp & 0x000F));   // Add in the HRONE value

    temp = RTCVAL;                           // Read minutes and seconds
    rtc_sec = (temp & 0x0070);               // mask off the SECTEN digit
    rtc_sec = ((rtc_sec >>4) * 10);          // Create decimal of 00,10...50
    rtc_sec = (rtc_sec + (temp & 0x000F));   // Add in the SECONE value

    temp = temp >>8;                         // Shift minutes into position
    rtc_min = (temp & 0x0070);               // mask off the MINTEN digit
    rtc_min = ((rtc_min >>4) * 10);          // Create decimal of 00,10...50
    rtc_min = (rtc_min + (temp & 0x000F));   // Add in the MINONE value

}

/*******************************************************************************
* Write the Real Time Clock Module Hours
*
*******************************************************************************/
void Write_RTC_Hrs(int16 hours)
{
    uint16 temp;

    if (hours > 23)      // if above highest possible count
    {
        hours = 0x23;    // set hours to max BCD value
    }
    else if (hours < 0)  // if below lowest possible count
    {
        hours = 0x00;    // set hours to min BCD value
    }
    else                 // Otherwise we have an in bound value
    {
        if (hours > 19)
        {
            hours = (hours -20);  // Get BCD 1s digit
            hours = hours + 0x20; // Add BCD 10s digit
        }
        else if (hours > 9)
        {
            hours = (hours -10);  // Get BCD 1s digit
            hours = hours + 0x10; // Add BCD 10s digit
        }
    }
    __builtin_write_RTCWEN();     // Enable access to the RTC registers
    RCFGCALbits.RTCPTR = 0b01;    // Set RTCC Register Pointer to Days and Hours
    temp = (RTCVAL & 0xFF00);     // Keep Weekday and clear Hours
    RCFGCALbits.RTCPTR = 0b01;    // Set RTCC Register Pointer to Days and Hours
    RTCVAL = temp | hours;        // Write new hours to RTCC module
    RCFGCALbits.RTCWREN = 0;      // Disable writes to the RTC control register
    Read_RTC_Time();              // Refresh the time values
}

/*******************************************************************************
* Write the Real Time Clock Module Minutes
*
*******************************************************************************/
void Write_RTC_Min(int16 minutes)
{
    uint16 temp;

    if (minutes > 59)      // if above highest possible count
    {
        minutes = 0x59;    // set hours to max BCD value
    }
    else if (minutes < 0)  // if below lowest possible count
    {
        minutes = 0x00;    // set hours to min BCD value
    }
    else                 // Otherwise we have an in bound value
    {
        if (minutes > 49)
        {
            minutes = (minutes -50);  // Get BCD 1s digit
            minutes = minutes + 0x50; // Add BCD 10s digit
        }
        else if (minutes > 39)
        {
            minutes = (minutes -40);  // Get BCD 1s digit
            minutes = minutes + 0x40; // Add BCD 10s digit
        }
        else if (minutes > 29)
        {
            minutes = (minutes -30);  // Get BCD 1s digit
            minutes = minutes + 0x30; // Add BCD 10s digit
        }
        else if (minutes > 19)
        {
            minutes = (minutes -20);  // Get BCD 1s digit
            minutes = minutes + 0x20; // Add BCD 10s digit
        }
        else if (minutes > 9)
        {
            minutes = (minutes -10);  // Get BCD 1s digit
            minutes = minutes + 0x10; // Add BCD 10s digit
        }
    }
    minutes = minutes <<8;        // Shift minutes in to high Byte
    __builtin_write_RTCWEN();     // Enable access to the RTC registers
    RCFGCALbits.RTCPTR = 0b00;    // Set RTCC Register Pointer to Min and Sec
    temp = (RTCVAL & 0x00FF);     // Keep Seconds and clear Minutes
    RCFGCALbits.RTCPTR = 0b00;    // Set RTCC Register Pointer to Min and Sec
    RTCVAL = temp | minutes;      // Write new minutes to RTCC module
    RCFGCALbits.RTCWREN = 0;      // Disable writes to the RTC control register
    Read_RTC_Time();              // Refresh the time values
}

/*******************************************************************************
* Write the Real Time Clock Module Minutes
*
*******************************************************************************/
void Write_RTC_Sec(int16 seconds)
{
    uint16 temp;

    if (seconds > 59)      // if above highest possible count
    {
        seconds = 0x59;    // set hours to max BCD value
    }
    else if (seconds < 0)  // if below lowest possible count
    {
        seconds = 0x00;    // set hours to min BCD value
    }
    else                 // Otherwise we have an in bound value
    {
        if (seconds > 49)
        {
            seconds = (seconds -50);  // Get BCD 1s digit
            seconds = seconds + 0x50; // Add BCD 10s digit
        }
        else if (seconds > 39)
        {
            seconds = (seconds -40);  // Get BCD 1s digit
            seconds = seconds + 0x40; // Add BCD 10s digit
        }
        else if (seconds > 29)
        {
            seconds = (seconds -30);  // Get BCD 1s digit
            seconds = seconds + 0x30; // Add BCD 10s digit
        }
        else if (seconds > 19)
        {
            seconds = (seconds -20);  // Get BCD 1s digit
            seconds = seconds + 0x20; // Add BCD 10s digit
        }
        else if (seconds > 9)
        {
            seconds = (seconds -10);  // Get BCD 1s digit
            seconds = seconds + 0x10; // Add BCD 10s digit
        }
    }
    __builtin_write_RTCWEN();     // Enable access to the RTC registers
    RCFGCALbits.RTCPTR = 0b00;    // Set RTCC Register Pointer to Min and Sec
    temp = (RTCVAL & 0xFF00);     // Keep Minutes and clear Seconds
    RCFGCALbits.RTCPTR = 0b00;    // Set RTCC Register Pointer to Min and Sec
    RTCVAL = temp | seconds;      // Write new minutes to RTCC module
    RCFGCALbits.RTCWREN = 0;      // Disable writes to the RTC control register
    Read_RTC_Time();              // Refresh the time values
}

/*******************************************************************************
* Displays the oscillator temperature
* Not used on this screen so this is a place holder for another screen
*
*
*******************************************************************************/
void Display_OSCTemp()
{
    int16 temp;
    POINT_COLOR = char_norm_color;
    BACK_COLOR = field_color;
    temp = Osc_Temperature();              // Get the latest temperature
    if (temp < 0)                          // If the af gain is negetive
    {
        temp = ((temp ^ 0xFFFF)+ 0x0001);  // Twos Complement to negate sign
        LCD_16x24_Char(256,16,'.',0);      // Display period symbol, mode=0
        LCD_16nz_Num(272,16,temp,1);       // Display decimal number
        LCD_16x24_Char(288,16,'C',0);      // Display C
        temp = (temp/10);                  // Divide number by 10
        LCD_16x24_Char(208,16,'-',0);      // Display negative symbol, mode=0
        LCD_16nz_Num(224,16,temp,2);       // Display two numbers
    }
    else
    {
        LCD_16x24_Char(256,16,'.',0);      // Display period symbol, mode=0
        LCD_16nz_Num(272,16,temp,1);       // Display decimal number
        LCD_16x24_Char(288,16,'C',0);      // Display C
        temp = (temp/10);                  // Divide number by 10
        LCD_16x24_Char(208,16,'+',0);      // Display positive symbol, mode=0
        LCD_16nz_Num(224,16,temp,2);       // Display two numbers
    }

}