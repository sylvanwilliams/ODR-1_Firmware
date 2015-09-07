/*******************************************************************************
* Header file for LCD_driver.c
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
*******************************************************************************/

#ifndef __LCD_DRIVER_H
#define __LCD_DRIVER_H		

#define	int8 signed char                // AKA int8_t
#define	uint8 unsigned char             // AKA uint8_t
#define	int16 signed int                // AKA int16_t
#define	uint16 unsigned int             // AKA uint16_t
#define	int32 signed long int           // AKA int32_t
#define	uint32 unsigned long int        // AKA uint32_t
#define	int64 signed long long int      // AKA int64_t
#define	uint64 unsigned long long int   // AKA uint64_t

//FIFO State
#define UNLOCKED    0
#define LOCKED      1

//Define the size of the LCD	
//#define LCD_W 240   // Portrait
//#define LCD_H 320   // Portrait
#define LCD_W 320   // Landscape
#define LCD_H 240   // Landscape

#define DATA 1
#define COMMAND 0

// variables available for external access
extern  uint16 BACK_COLOR;      // Background color
extern  uint16 POINT_COLOR;     // Pen color
extern  uint8 num32_char[10];  // 32bit number to character array:

// Functions
void delayms(uint16 count);          // 1ms delay routine used during display init
void Lcd_Init(void); 
void LCD_Clear(uint16 Color);


void LCD_DrawPoint(uint16 x,uint16 y);                                  // Draw points
void LCD_DrawPoint_big(uint16 x,uint16 y);                              // Draw a big point
uint16  LCD_ReadPoint(uint16 x,uint16 y);                               // Reading point
void Draw_Circle(uint16 x0,uint16 y0,uint8 r);                          // Draw Circle
void LCD_DrawLine(uint16 x1, uint16 y1, uint16 x2, uint16 y2);          // Draw Line
void LCD_DrawRectangle(uint16 x1, uint16 y1, uint16 x2, uint16 y2);     // Draw Rectangle
void LCD_Fill(uint16 x1,uint16 y1,uint16 x2,uint16 y2,uint16 color);    // Fill Screen With Color
void LCD_8x16_Char(uint16 x,uint16 y,uint8 num,uint8 mode);             // Display a small character
void LCD_16nz_Num(uint16 x,uint16 y,uint16 num,uint8 len);              // Display 16x24 16 bit number no leading zeros
void LCD_16nz8_Num(uint16 x,uint16 y,uint16 num,uint8 len);             // Display 8x16 16 bit number no leading zeros
void LCD_16wz_Num(uint16 x,uint16 y,uint16 num,uint8 len);              // Display 16x24 16 bit number with leading zeros
void LCD_16wz8_Num(uint16 x,uint16 y,uint16 num,uint8 len);             // Display 8x16 16 bit number with leading zeros
void LCD_32nz_Num(uint16 x,uint16 y,uint32 num,uint8 len);              // Display 32 bit number no leading zeros
void LCD_8x16_String(uint16 x,uint16 y,char *p);                        // Display a string 8x16 font
void LCD_16x24_String(uint16 x,uint16 y,char *p);                       // Display a string 16x24 font
void LCD_16x24_Char(uint16 x,uint16 y,uint8 num,uint8 mode);            // Display a large character
void NumToCharArray(uint32 num);                                        // Convert 32bit number to character array
void LCD_DrawIcon(uint16 x,uint16 y);                                   // Draw a 40x40 Icon with 565 color


//Pen color
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 		 0XFFE0
#define GBLUE		 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 		 0XBC40
#define BRRED 		 0XFC07
#define GRAY  		 0X8430

//GUI colors
#define DARKBLUE      	 0X01CF
#define LIGHTBLUE      	 0X7D7C 
#define GRAYBLUE       	 0X5458


//More than three colors for the color PANEL
#define LIGHTGREEN     	 0X841F         //Light Green
#define LGRAY 		 0XC618         //Light gray (PANEL), form the background color

#define LGRAYBLUE        0XA651         //Powder blue (intermediate layer color)
#define LBBLUE           0X2B12         //Light brown Blue (select entry opposite color)
					  		 
#endif  
	 
	 



