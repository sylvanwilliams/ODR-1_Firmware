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


int main(void)
{

    // Initialize processor and its peripherals
    Init_P33EP512MU810_osc();
    Init_P33EP512MU810_pins();
    Init_QEI_1();              // Initialize quadrature encoder interface #1
    Init_QEI_2();              // Initialize quadrature encoder interface #2
    Init_SPI2();               // Initialize SPI 2
    Init_PWM();                // Initialize PWM Module
    Init_RTCC();                // Initialize the real time clock
    Init_I2C2();                // Initialize I2C2
	
    // Initialize other board hardware
    Lcd_Init();		       // tft initialization

    si5351aSetFrequency(3600000); // set the si5351 frequency in MHz
  
    // Display and fill main screen
    Color_pallet_update();  // Set up the color pallet
    Refresh_page0();        // Refresh main screen


    while(1)  // main, loop forever
    {
        Encoder1_Update();             // Get the latest encoder status
        Encoder2_Update();             // Get the latest encoder status

        //I2C2_Byte_Write	( 170, 170 );  // Test the I2C write function

        if (current_page == 0)
        {
            Page0_pointer1_update();       // Update upper main screen data
            Page0_pointer2_update();       // Update lower main screen data
            Display_UTC_24HR();            // Update time display
        }
        else if (current_page == 1)
        {
            Page1_pointer1_update();       // Update upper main screen data
        }
        else if (current_page == 2)
        {
            Page2_pointer1_update();       // Update screen data
            if (!(page_pointer1 & 0x8000)) // Check for item focus
            {
                Page2_pointer2_update();       // Focus on item data
            }
        }
        delayms(2);                    // delay to slow the loop down for testing
    }

}
/******************************************************************************/

