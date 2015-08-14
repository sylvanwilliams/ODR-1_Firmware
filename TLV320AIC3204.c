/*******************************************************************************
* Driver Code the Texas Instruments TLV320AIC3204 audio codec
*
*
* Authors: Bill Peterson,
* Changes:
* 2015-Aug Initial code writing
*
*******************************************************************************/
#include "TLV320AIC3204.h"
#include "p33EP512MU810.h"
#include "DSPIC33E_hardware.h"
#include "LCD_driver.h" // only needed for the delay function

/*******************************************************************************
* Initialize Audio Codec
*
*
*******************************************************************************/
void Init_Audio_Codec(void)
{
    // Reset Audio Codec
    CODEC_RST = 0;
    delayms(10);    // Delay for 10mS for reset of codec
    CODEC_RST = 1;

}

