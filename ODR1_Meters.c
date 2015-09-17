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

//Battery voltage averaging array and pointer seeded with 12.00 volts
uint16 batt_volts_array[10] = {2400,2400,2400,2400,2400,2400,2400,2400,2400,2400};
uint16 batt_volts_ptr = 0;

uint16 batt_volts_avg = 1200;
uint16 batt_volts_cnt = 0;

int16 osc_temp_avg = 270; // Average temperature value
uint16 osc_temp_cnt = 0;  // Temperature anti-flicker filter counter

/******************************************************************************
 * Function:       Osc_Temperature using the STLM20 temperature sensor
 *
 * PreCondition:   None
 *
 * Input:          None
 *
 * Output:         Temperature in 10s of a degree C int16
 *
 * Side Effects:   None
 *
 * Overview: Grabs A-D value and returns Oscillator Temperature
 * 1st order linear equasion used for best accuracy -40C to +85C.
 * Sensor Vo = ?11.67 mV/°C * T + 1.8583 V
 * Sensor Vo = 1.8583V - 0.01167V * T
 * With an ADC reference of 2.5V use the following equasions
 * 10bit Equasion = Temp x10 deg C = (1592400 - (ADCval * 2092)) / 1000
 * 12bit Equasion = Temp x10 deg C = (1592400 - (ADCval *  523)) / 1000
 *****************************************************************************/
int16 Osc_Temperature(void)
{
    int32 temp1;
    int16 temp2;
    
    temp1 = ADC1BUF0;     // Read the AN20 temperature sensor value
    temp1 = (temp1 * 523);
    temp2 = (int16)((1592400 - temp1)/1000); // Signed temperature value * 10

    // Temperature flicker filter
    if (temp2 != osc_temp_avg)    // Test if new temp != avg temp
    {
       osc_temp_cnt ++;           // Increment the no match counter
    }
    else  osc_temp_cnt = 0;       // New sample and average temp is the same
    if (osc_temp_cnt > 10)        // If temp is different xx times
    {
        osc_temp_avg = temp2;     // Set the new average temperature
        osc_temp_cnt = 0;
    }

    return osc_temp_avg;

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
 * Overview: Grabs A-D value from AN22 input and then runs it through
 * a 10 value averaging array. The more frequently this function is called
 * the less effective the filtering. 1/2 second intervals are effective.
 *
 *****************************************************************************/
uint16 Battery_Volts(void)
{
    uint8 t=0;
    uint16 volts = 0;

    batt_volts_ptr ++;            // Increment array pointer
    if (batt_volts_ptr > 9)
    {
        batt_volts_ptr = 0;       // Roll over the pointer
    }
    batt_volts_array[batt_volts_ptr] = ADC1BUF1; // Grab new sample AN22
    for(t=0;t<10;t++)  // increment through the voltage buffer
    {
        volts = (volts + batt_volts_array[t]);   // Add up last 10 samples
    }
    volts = volts / 10;                      // Get the average

    return (volts >>1);                      // Return battery voltage/2

//    return ADC1BUF1 >>1;    // Return raw battery value  /2
}