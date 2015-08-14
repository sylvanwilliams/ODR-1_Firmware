/*******************************************************************************
* Header File for LV320AIC3204.c audio codec driver
*
*
* Authors: Bill Peterson,
* Changes:
* 2015-Aug Initial code writing
*
*******************************************************************************/

#ifndef __LV320AIC3204_H
#define __LV320AIC3204_H

#define	uint8 unsigned char             // AKA uint8_t
#define	uint16 unsigned int             // AKA uint16_t

uint8 Codec_Read_SPI(uint8 address);
void Codec_Write_SPI(uint8 address, uint8 data);
void Init_Audio_Codec(void);    // Initialize Audio Codec

#endif