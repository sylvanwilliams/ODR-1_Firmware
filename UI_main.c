/*******************************************************************************
* User Interface main captures user inputs from the rotary encoders and key
* inputs. The dual quadrature encoder interfaces are used to debounce the inputs
* in hardware.
*
* Color pallets for user display are also defined here.
*
* This work is licensed under the Creative Commons Attribution-ShareAlike
* 4.0 International License. To view a copy of this license, visit
* http://creativecommons.org/licenses/by-sa/4.0/ or send a letter to
* Creative Commons, PO Box 1866, Mountain View, CA 94042, USA.
*******************************************************************************/

#include "UI_main.h"
#include "p33exxxx.h" // processor specific definitions
#include "LCD_driver.h"
#include "UI_page0.h"
#include "UI_page1.h"
#include "UI_page2.h"
#include "DSPIC33E_hardware.h"
#include "ODR1_Control_1.h"

#define MENU_PRESS_DURATION 2
#define BUTTON_TICKS        ((unsigned int)(MENU_PRESS_DURATION/TIMER1_CYCLE_TIME))

//Arrays


//Variables
uint16 current_page = 0;
int16 page_pointer1    = 5;       // Default to kHz freq
int16 page_pointer2    = 7;       // Default to AF gain

int16 active_color_pallet = 0;
uint16 char_hglt_color;           // Character Highlight Color
uint16 char_norm_color;           // Character Normal Color
uint16 char_rev_color;            // Character Reverse Color
uint16 field_color;               // Main field Color
uint16 heading_color;             // Heading Color
uint16 border_color;              // Border Color

sEncoderData encoder1Data;
sEncoderData encoder2Data;

/*******************************************************************************
 * Encoder 1 Status updates the encoder rotation count and push button status.
 * The 16 detent rotary encoder changes 4 states per detent
 * The 12 detent rotary encoder changes 8 states per detent
 * The last 2 or 3 bits are masked (ignored) before testing for a position change.
 * The final count is /4 or /8 so that items increment once per detent.
 * The encoder and PB counts need to be cleared after use by other functions.
 * The push button count tells how long the button has been pressed.
 * IMPORTANT! The encoderDatax.count member is cumulative per ISR. You MUST
 * clear it after servicing the UI is finished
 *******************************************************************************/
void Encoder1_Update()
{
    int16 encoder1_new;
    
    encoder1Data.oldButtonState = encoder1Data.buttonState;
    
    // encoder1_new = (POS1CNTL & 0xFFFC); // 16-detent read encoder1 position mask last two bits
    encoder1_new = (POS1CNTL & 0xFFF8); // 12-detent read encoder1 position mask last three bits
    if (encoder1_new != encoder1Data.oldCount) // if position has changed
    {
        // encoder1Data.count += ((encoder1_new - encoder1Data.oldCount) >> 2); // 16-detent update new count
        encoder1Data.count += ((encoder1_new - encoder1Data.oldCount) >> 3); // 12-detent update new count
        encoder1Data.oldCount = encoder1_new; // Store last read
    }

    if(!QEI1IOCbits.INDEX) //Don't care about state, just count
    {
        encoder1Data.buttonCount++;
    }
    else    //Button not depressed so reset count
    {
        encoder1Data.buttonCount = 0;
        encoder1Data.buttonState = NONE;
    }

    if (encoder1Data.buttonCount) // If button is being pressed
    {
        if (encoder1Data.buttonCount > BUTTON_TICKS) // If button was held for defined duration
        {
            encoder1Data.buttonState = EXTENDED;
            if (current_page == 0) // If we are on page 0
            {
                current_page = 1; // Switch to page 1
                page_pointer1 = 0x8000; // Default to item 0 and pointer has focus
                Refresh_page1(); // display page 1
            }
            else
            {
                current_page = 0; // exit back to page 0
                page_pointer1 = 5; // Default to kHz Frequency
                page_pointer2 = 7; // Default to AF Gain
                Refresh_page0(); // display page 0
            }
            encoder1Data.buttonCount = 0;
        }
        else // Button has a short duration press
        {
            encoder1Data.buttonState = QUICK;
        }
    }

}

int16 Encoder1Count(void)
{
    return encoder1Data.count;
}

void Encoder1CountZero(void)
{
    encoder1Data.count = 0;
}

eButtonState Encoder1ButtonEvent(void)
{
    return encoder1Data.buttonState;
}
/*******************************************************************************
 * Encoder 2 Status updates the encoder rotation count and push button status.
 * The 16 detent rotary encoder changes 4 states per detent
 * The 12 detent rotary encoder changes 8 states per detent
 * The last 2 or 3 bits are masked (ignored) before testing for a position change.
 * The final count is /4 or /8 so that items increment once per detent.
 * The encoder and PB counts need to be cleared after use by other functions.
 * The push button count tells how long the button has been pressed.
 * IMPORTANT! The encoderDatax.count member is cumulative per ISR. You MUST
 * clear it after servicing the UI is finished
 *******************************************************************************/
void Encoder2_Update()
{
    int16 encoder2_new;

    encoder2Data.oldButtonState = encoder2Data.buttonState;

    //encoder2_new = (POS2CNTL & 0xFFFC); // 16-detent read encoder1 position mask last two bits
    encoder2_new = (POS2CNTL & 0xFFF8); // 12-detent read encoder1 position mask last three bits
    if (encoder2_new != encoder2Data.oldCount) // if position has changed
    {
        //encoder2Data.count += ((encoder2_new - encoder2Data.oldCount) >> 2); // 16-detent update new count
        encoder2Data.count += ((encoder2_new - encoder2Data.oldCount) >> 3); // 12-detent update new count
        encoder2Data.oldCount = encoder2_new; // Store last read
    }

    if (!QEI2IOCbits.INDEX) //Don't care about state, just count
    {
        encoder2Data.buttonCount++;
    }
    else //Button not depressed so reset count
    {
        encoder2Data.buttonCount = 0;
        encoder2Data.buttonState = NONE;
    }

    if (encoder2Data.buttonCount)
    {
        if (encoder2Data.buttonCount > BUTTON_TICKS) // If button was held for defined duration
        {
            encoder2Data.buttonState = EXTENDED;
            if (current_page == 0) // If we are on page 0
            {
                current_page = 2; // Switch to page 1
                page_pointer1 = 0x8000; // Default to item 0 and pointer has focus
                Refresh_page2(); // display page 1
            }
            else
            {
                current_page = 0; // exit back to page 0
                page_pointer1 = 5; // Default to kHz Frequency
                page_pointer2 = 7; // Default to AF Gain
                Refresh_page0(); // display page 0
            }
            encoder2Data.buttonCount = 0;
        }
        else
        {
            encoder2Data.buttonState = QUICK;
        }
    }
}

int16 Encoder2Count(void)
{
    return encoder2Data.count;
}

void Encoder2CountZero(void)
{
    encoder2Data.count = 0;
}

eButtonState Encoder2ButtonEvent(void)
{
    return encoder2Data.buttonState;
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

/******************************************************************************
 * Function:       void Scan_Key_Inputs(void)
 *
 * PreCondition:   None
 *
 * Input:          None
 *
 * Output:         None
 *
 * Side Effects:   None
 *
 * Overview:       Read the active low keyer inputs and react according to mode
 *****************************************************************************/
void Scan_Key_Inputs(void)
{
    if (!KEY_DOT)
    {
        Radio_Transmit();
    }
    else if (!KEY_DSH)
    {
        Radio_Transmit();
    }
    else Radio_Receive();
}