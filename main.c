/******************************************************************************
 * Encoder_Test using the dsPIC33EP512MU810 and 320x240 ILI9341 display
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
 *****************************************************************************/

#include "p33EP512MU810.h"
#include "DSPIC33E_hardware.h"
#include "main.h"
#include "devconfig.h"
#include "LCD_driver.h"
#include "LCD_fonts.h"
#include "UI_main.h"
#include "UI_page0.h"
#include "UI_page1.h"
#include "UI_page2.h"
#include "si5351a.h"
#include "TLV320AIC3204.h"

void Service_Interface(void);
void Init_Mixer_Board();

int main(void)
{

    // Initialize processor and its peripherals
    Init_P33EP512MU810_osc();
    Init_P33EP512MU810_pins();
    Init_QEI_1();     // Initialize quadrature encoder interface #1
    Init_QEI_2();     // Initialize quadrature encoder interface #2
    Init_SPI1();      // Initialize SPI 1
    Init_SPI2();      // Initialize SPI 2
    Init_SPI3();      // Initialize SPI 3
    Init_SPI4();      // Initialize SPI 4
    Init_PWM();       // Initialize PWM Module
    Init_RTCC();      // Initialize the real time clock
    Init_I2C2();      // Initialize I2C2
    Init_REFCLK();    // Initialize Reference Clock output used as codec MCLK

    // Initialize other board hardware
    Lcd_Init();		     // tft initialization
    Init_Mixer_Board();      // Initialize the Mixer board

    AUX_OUT = 0;             // Rear Auxiliary Output
    PTT_OUT = 0;             // Rear PTT Output

    // Display and fill main screen
    Color_pallet_update();  // Set up the color pallet
    Refresh_page0();        // Refresh main screen
    
    Init_Audio_Codec();      // Initialize the audio Codec

    //Configure Timer1 for interrupt operation
    Init_Timer1();

    while(1)  // main, loop forever
    {   
        Service_Interface();
    }

}
/******************************************************************************/
/******************************************************************************
 * Function:       void __attribute__((interrupt,no_auto_psv)) _T1Interrupt( void )
 *
 * PreCondition:   None
 *
 * Input:          None
 *
 * Output:         None
 *
 * Side Effects:   None
 *
 * Overview:       ISR ROUTINE FOR THE TIMER1 INTERRUPT
 *****************************************************************************/
void __attribute__((interrupt, no_auto_psv)) _T1Interrupt(void)
{
    IFS0bits.T1IF = 0;
    T1CONbits.TON = 0;

    PERIODIC = ~PERIODIC;
    Encoder1_Update(); // Get the latest encoder status
    Encoder2_Update(); // Get the latest encoder status

    TMR1 = 0;
    T1CONbits.TON = 1;

    /* reset Timer 1 interrupt flag */
}

/******************************************************************************
 * Function:       void Service_Interface(void)
 *
 * PreCondition:   None
 *
 * Input:          None
 *
 * Output:         None
 *
 * Side Effects:   None
 *
 * Overview:       Main UI thread to scan encoders, button presses, etc
 *****************************************************************************/
void Service_Interface(void)
{
    
    if (current_page == 0)
    {
        Page0_pointer1_update(); // Update upper main screen data
        Page0_pointer2_update(); // Update lower main screen data
        // Display_UTC_24HR(); // Update time display
    }
    else if (current_page == 1)
    {
        Page1_pointer1_update(); // Update upper main screen data
    }
    else if (current_page == 2)
    {
        Page2_pointer1_update(); // Update screen data
        if (!(page_pointer1 & 0x8000)) // Check for item focus
        {
            Page2_pointer2_update(); // Focus on item data
        }
    }

}

/******************************************************************************
 * Function:       Init_Mixer_Board
 *
 * PreCondition:   None
 *
 * Input:          None
 *
 * Output:         None
 *
 * Side Effects:   None
 *
 * Overview:       Initialize the Oscillator and Mixer Board
 *****************************************************************************/
void Init_Mixer_Board()
{
    BPF_S0  = 0;    // Band Pass Filter Select 0
    BPF_S1  = 0;    // Band Pass Filter Select 1
    BPF_CS0 = 0;    // Band Pass Filter Chip Select 0
    BPF_CS1 = 0;    // Band Pass Filter Chip Select 1
    ATT_S0  = 0;    // RF Attenuator Select 0
    ATT_S1  = 0;    // RF Attenuator Select 1
    TX_RX   = 0;    // RX/TX Control line, 0=Receive
    AUX_OUT = 0;    // Rear Auxiliary Output
    PTT_OUT = 0;    // Rear PTT Output

    EN_12V  = 1;    // 12V Power Enable
    delayms(250);   // Delay for 250mS for supply rails

    // Default to 6M Bandpass filter
    BPF_S0  = 0;
    BPF_S1  = 1;
    BPF_CS0 = 1;
    BPF_CS1 = 0;

    // Default osillator frequency to 6M (2 * 10MHz)
    //si5351aSetFrequency(20000000); // set si5351 freq to 20MHz
    Init_si5351a() ;      // Initialize si5351 clock chip
}