/***************************************************************************
* This code is used for ODR-1 Meter functions
* Meters such as Battery Volts, S-meter, SWR...
* Meter values are calculated here but displayed via UI_pages
*
* This work is licensed under the Creative Commons Attribution-ShareAlike
* 4.0 International License. To view a copy of this license, visit
* http://creativecommons.org/licenses/by-sa/4.0/ or send a letter to
* Creative Commons, PO Box 1866, Mountain View, CA 94042, USA.
****************************************************************************/

#include "p33EP512MU810.h"
#include "DSPIC33E_hardware.h"
#include "ODR1_Meters.h"
#include "LCD_driver.h" // needed to display test data
#include "UI_main.h" // needed to display test data
/******************************************************************************
 * Function:       Osc_Temperature
 *
 * PreCondition:   None
 *
 * Input:          None
 *
 * Output:         None
 *
 * Side Effects:   None
 *
 * Overview: Grabs A-D value and returns Oscillator Temperature
 *
 *****************************************************************************/
void Osc_Temperature(void)
{
    uint16 temp;
    temp = ADC1BUF0; // Read the AN20 temperature sensore value
}

/******************************************************************************
 * Function:       Battery_Volts
 *
 * PreCondition:   None
 *
 * Input:          None
 *
 * Output:         Uint16 voltage in 10s of milivolts
 *
 * Side Effects:   None
 *
 * Overview: Grabs A-D value and returns battery volts in mV
 *
 *****************************************************************************/
uint16 Battery_Volts(void)
{
    // uint16 temp = 255;
    return (ADC1BUF1 >>1); // Read the AN22 battery voltage/2
    //Debug screen printing
    // POINT_COLOR = char_norm_color;
    // BACK_COLOR = field_color;
    // LCD_16wz_Num(160,10,temp,5);
}