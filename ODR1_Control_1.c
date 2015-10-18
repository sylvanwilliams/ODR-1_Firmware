/***************************************************************************
* This code is used for ODR-1 control functions
*
* This work is licensed under the Creative Commons Attribution-ShareAlike
* 4.0 International License. To view a copy of this license, visit
* http://creativecommons.org/licenses/by-sa/4.0/ or send a letter to
* Creative Commons, PO Box 1866, Mountain View, CA 94042, USA.
****************************************************************************/

#include "p33EP512MU810.h"
#include "DSPIC33E_hardware.h"
#include "ODR1_Control_1.h"
#include "ODR1_Meters.h"
#include "si5351a.h"
#include "TLV320AIC3204.h"
#include "UI_page0.h"
#include "LCD_driver.h"

int32 radio_freq = 10000000;    // Radio frequency Default to 10MHz

// Arrays

// local oscillator drift in ppb, temporarily stored as constant later
// to be moved to EEPROM but we still need a default set of values.

const int16 LOdrift[19] =
{             // Serial # 1 LO Drift Values in ppb
    -2917,    // -20C
    -4354,    // -15C
    -5187,    // -10C
    -5485,    // -5C
    -5319,    //  0C
    -4758,    //  5C
    -3871,    //  10C
    -2729,    //  15C
    -1400,    //  20C
        0,    //  25C
     1538,    //  30C
     3008,    //  35C
     4386,    //  40C
     5602,    //  45C
     6587,    //  50C
     7271,    //  55C
     7584,    //  60C
     7458,    //  65C
     6821,    //  70C
};

/*
const int16 LOdrift[19] =
{             // Serial # 2 and average LO Drift Values in ppb
    -3559,    // -20C
    -4877,    // -15C
    -5694,    // -10C
    -6003,    // -5C
    -5883,    //  0C
    -5269,    //  5C
    -4276,    //  10C
    -3030,    //  15C
    -1557,    //  20C
        0,    //  25C
     1679,    //  30C
     3214,    //  35C
     4543,    //  40C
     5570,    //  45C
     6379,    //  50C
     6856,    //  55C
     7027,    //  60C
     6879,    //  65C
     6286,    //  70C
};
*/

/*
const int16 LOdrift[19] =
{             // Serial # 3 LO Drift Values in ppb
    -4200,    // -20C
    -5400,    // -15C
    -6200,    // -10C
    -6521,    // -5C
    -6447,    //  0C
    -5780,    //  5C
    -4680,    //  10C
    -3330,    //  15C
    -1713,    //  20C
        0,    //  25C
     1820,    //  30C
     3420,    //  35C
     4700,    //  40C
     5537,    //  45C
     6170,    //  50C
     6440,    //  55C
     6470,    //  60C
     6300,    //  65C
     5750,    //  70C
};
*/

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

    // radio_freq = 10000000; // Set the Default radio frequency to 10MHz
    Init_si5351a() ;       // Initialize si5351 clock chip
}

/******************************************************************************
 * Function:       Set_bandpass_Filters
 *
 * PreCondition:   None
 *
 * Input:          None
 *
 * Output:         None
 *
 * Side Effects:   None
 *
 * Overview:       Select the bandpass filter based on frequency
 *****************************************************************************/
void Set_bandpass_Filters(int32 freq)
{
    if (freq < 2690001)                             // < 2.69MHz
    {
        BPF_S0  = 1;
        BPF_S1  = 1;
        BPF_CS0 = 0;
        BPF_CS1 = 1;
    }
    else if ((freq > 2690000)&&(freq < 4800001))   // 2.69-4.80MHz
    {
        BPF_S0  = 1;
        BPF_S1  = 1;
        BPF_CS0 = 1;
        BPF_CS1 = 0;
    }
    else if ((freq > 4800000)&&(freq < 7650001))   // 4.80-7.65MHz
    {
        BPF_S0  = 0;
        BPF_S1  = 1;
        BPF_CS0 = 0;
        BPF_CS1 = 1;
    }
    else if ((freq > 7650000)&&(freq < 11000001))  // 7.65-11.0MHz
    {
        BPF_S0  = 0;
        BPF_S1  = 1;
        BPF_CS0 = 1;
        BPF_CS1 = 0;
    }
    else if ((freq > 11000000)&&(freq < 16100001)) // 11.0-16.1MHz
    {
        BPF_S0  = 1;
        BPF_S1  = 0;
        BPF_CS0 = 0;
        BPF_CS1 = 1;
    }
    else if ((freq > 16100000)&&(freq < 22800001)) // 16.1-22.8MHz
    {
        BPF_S0  = 1;
        BPF_S1  = 0;
        BPF_CS0 = 1;
        BPF_CS1 = 0;
    }
    else if ((freq > 22800000)&&(freq < 31600001)) // 22.8-31.6MHz
    {
        BPF_S0  = 0;
        BPF_S1  = 0;
        BPF_CS0 = 0;
        BPF_CS1 = 1;
    }
    else                                           // > 31.6Mhz
    {
        BPF_S0  = 0;
        BPF_S1  = 0;
        BPF_CS0 = 1;
        BPF_CS1 = 0;
    }
}

/******************************************************************************
 * Function:       Set_RFGain
 *
 * PreCondition:   None
 *
 * Input:          int16 with value of 0 to 3
 *
 * Output:         None
 *
 * Side Effects:   None
 *
 * Overview:       Set RF gain on mixer board
 *****************************************************************************/
void Set_RFGain(int16 gain)
{
    switch(gain) // Action based on gain selected
    {
        case 0:  // Select -24dB RF Gain
            ATT_S0  = 0;    // RF Attenuator Select 0
            ATT_S1  = 1;    // RF Attenuator Select 1
        break;
        case 1:  // Select -6dB RF Gain
            ATT_S0  = 1;    // RF Attenuator Select 0
            ATT_S1  = 0;    // RF Attenuator Select 1
        break;
        case 2:  // Select 0dB RF Gain
            ATT_S0  = 0;    // RF Attenuator Select 0
            ATT_S1  = 0;    // RF Attenuator Select 1
        break;
        case 3:  // Select +15dB RF Gain
            ATT_S0  = 1;    // RF Attenuator Select 0
            ATT_S1  = 1;    // RF Attenuator Select 1
        break;
        default: // Default 0dB RF Gain
            ATT_S0  = 0;    // RF Attenuator Select 0
            ATT_S1  = 0;    // RF Attenuator Select 1
    }
}

/******************************************************************************
 * Function:       Radio_Receive
 *
 * PreCondition:   None
 *
 * Input:          int16 with value of 0 to 3
 *
 * Output:         None
 *
 * Side Effects:   None
 *
 * Overview:       Set radio to receive
 *****************************************************************************/
void Radio_Receive()
{
    Codec_HP_Gain(-7);    // Mute headphones
    TX_RX   = 0;    // RX/TX Control line, 0=Receive
    PTT_OUT = 0;    // Rear PTT Output

    switch(rxtx_mode) // Action based on rxtx mode radio is in
    {
        case 0:  // Mode 0 CW
            Codec_Config_PT1_RX();      // Configure CODEC for receive
        break;
        case 1:  // Mode 1 USB
            Codec_Config_PT1_RX();      // Configure CODEC for receive
        break;
        case 2:  // Mode 2 LSB
            Codec_Config_PT1_RX();      // Configure CODEC for receive
        break;
        case 3:  // Mode 3 AM
            Codec_Config_PT1_RX();      // Configure CODEC for receive
        break;
        case 4:  // Mode 4 Analog Pass Through
            Codec_Config_PT1_RX();      // Configure CODEC for receive
        break;
        case 5:  // Mode 5 ADC-DAC Pass Through
            Codec_Config_PT2_RX();      // Configure CODEC for receive
        break;
        case 6:  // Mode 6 ADC-DSP-DAC Pass Through
            Codec_Config_PT3_RX();      // Configure CODEC for receive
        break;
    }
    Codec_HP_Gain(af_gain);    // Restore headphones gain 
}

/******************************************************************************
 * Function:       Radio_Transmit
 *
 * PreCondition:   None
 *
 * Input:          int16 with value of 0 to 3
 *
 * Output:         None
 *
 * Side Effects:   None
 *
 * Overview:       Set radio to transmit
 *****************************************************************************/
void Radio_Transmit()
{
    Codec_HP_Gain(-7);    // Mute headphones

    switch(rxtx_mode) // Action based on rxtx mode radio is in
    {
        case 0:  // Mode 0 CW
            Codec_Config_PT1_TX();      // Configure CODEC for transmit
        break;
        case 1:  // Mode 1 USB
            Codec_Config_TX_Mic();      // Configure CODEC for transmit
        break;
        case 2:  // Mode 2 LSB
            Codec_Config_TX_Mic();      // Configure CODEC for transmit
        break;
        case 3:  // Mode 3 AM
            Codec_Config_TX_Mic();      // Configure CODEC for transmit
        break;
        case 4:  // Mode 4 Analog Pass Through
            Codec_Config_PT1_TX();      // Configure CODEC for transmit
        break;
        case 5:  // Mode 5 ADC-DAC Pass Through
            Codec_Config_PT1_TX();      // Configure CODEC for transmit
        break;
        case 6:  // Mode 6 ADC-DSP-DAC Pass Through
            Codec_Config_PT1_TX();      // Configure CODEC for transmit
        break;
    }
    Codec_HP_Gain(af_gain);    // Restore headphones gain
    TX_RX   = 1;    // RX/TX Control line, 0=Receive
    PTT_OUT = 1;    // Rear PTT Output
}

/******************************************************************************
 * Function:       Freq_Error_Comp
 *
 * PreCondition:   None
 *
 * Input:          None
 *
 * Output:         Frequency Error in Hertz
 *
 * Side Effects:   None
 *
 * Overview:       Calculates the number of Hertz to add to the oscillator
 *                 commannd to compensate for initial error and thermal drift.
 *                 The method uses a look-up table indexed by temperature. The
 *                 table holds the thermal error in ppb. lineal interpolation
 *                 is used for temperatures compensation between entries.
 *****************************************************************************/
int16 Freq_Error_Comp()
{
    int16 tempA;
    int16 tempB;
    int32 driftA;
    int32 driftB;
    int16 init_freq_error = 21866; // SER# 1 Initial freq error in ppb
//    int16 init_freq_error = 15000; // SER# 2 Initial freq error in ppb
//    int16 init_freq_error = 21850; // SER# 3 Initial freq error in ppb
    tempA = Osc_Temperature();    // Get the latest oscilator temperature
    if (tempA < -200)             // Check for temperature out of bounds low
    {
        tempA = -200;
    }
    else if (tempA > 699)         // Check for temperature out of bounds high
    {
        tempA = 699;              // Cap at 699 so array index is !> 17
    }
    tempB = tempA + 200;          // Shift temperature positive by 20.0 degrees
    tempB = tempB / 50;           // Create index into LOdrift array
    driftA = LOdrift[tempB];      // Grab first drift value from array
    driftB = LOdrift[tempB +1];   // Grab next drift value from array
    driftB = driftB - driftA;     // Find the difference in the drift values
    driftB = driftB * 2;          // Weight per 1/10 degree = diff * 100/50, simplify to * 2
    tempB = tempB * 50;           // Index * 50
    tempB = tempB - 200;          // Get back to the rounded DriftA temperature
    tempA = tempA - tempB;        // Find the difference in temperature
    driftB = driftB * tempA;      // Interpolation difference * 100
    driftB = driftB / 100;        // Convert difference to ppb
    driftB = driftB + driftA;     // Interpolated error in ppb

    driftB = (driftB + init_freq_error); // Add initial error in ppb for total
    driftA = (radio_freq / 10000);       // Radio frequency scale down
    driftB = (driftB * driftA);          // Multiply total ppb * frequency
    driftB = (driftB / 100000);          // Scale total error to Hz
    return   (int16)driftB;              // Return error correction in Hertz
}

/******************************************************************************
 * Function:       Change_Freq
 *
 * PreCondition:   None
 *
 * Input:          None
 *
 * Output:         Other Functional calls
 *
 * Side Effects:   None
 *
 * Overview:       Change the Oscillator Frequency and set the bandpass
 *                 filters based on frequency then update the display frequency
 *****************************************************************************/

void Change_Freq()
{
    int32 freq_cmd;   // Frequency command to be set to oscillator
    // Don't let radio go out of frequency bounds
    if (radio_freq > radio_freq_max)
    {
        radio_freq = radio_freq_max;
    }
    else if (radio_freq < radio_freq_min)
    {
        radio_freq = radio_freq_min;
    }
    freq_cmd = (int32)Freq_Error_Comp();     // Get frequency error compensation
    freq_cmd = (freq_cmd + radio_freq);      // Compensate frequency
    freq_cmd = (freq_cmd * 2);               // Double the oscillator frequency
    si5351aSetFrequency((uint32)freq_cmd);   // Set oscillator frequency

    Set_bandpass_Filters(radio_freq);        // Select bandpass filter on mixer bd
}