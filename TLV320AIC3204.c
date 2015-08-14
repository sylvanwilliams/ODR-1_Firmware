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
* Audio Codec SPI read function
* The codec is on SPI1 bus all by itself
* A 16 bit word can be used here to clock an 8-bit command followed by 8-bit data
* First byte is 7-bit register address plus low order bit set to 1 for read
* Second byte is set to all zeros which will clock in the data on MISO
*******************************************************************************/
uint8 Codec_Read_SPI(uint8 address)
{
    uint16 temp;
    temp = ((address << 1)+1);      // Shift 7-bit address left 1 bit add 1
    temp = (temp << 8);             // Shift the command to the high order byte
    SPI1BUF = temp;                 // send read command
    while (!SPI1STATbits.SPIRBF);   // wait for the data to be sent out
    return SPI1BUF;                 // Standard mode read
}


/*******************************************************************************
* Audio Codec SPI write function
* The codec is on SPI1 bus all by itself
* A 16 bit word can be used here to clock an 8-bit command followed by 8-bit data
* First byte is 7-bit register address plus low order bit set to 0 for write
* Second byte is the 8-bit data to store in the register
*******************************************************************************/
void Codec_Write_SPI(uint8 address, uint8 data)
{
    uint16 temp;
    temp = (address << 1);          // Shift 7-bit address left 1 bit
    temp = ((temp << 8)+ data);     // Shift command to high order byte, add data
    SPI1BUF = temp;                 // send command
    while (!SPI1STATbits.SPIRBF);   // wait for the data to be sent out

}


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

