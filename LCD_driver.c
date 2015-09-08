/*******************************************************************************
* Simple graphics and text library for a 320x240 display using an ILI9341
* display controller connected via 5-wire SPI (MISO, MOSI, SCK, CS, DS)
*
* This work is licensed under the Creative Commons Attribution-ShareAlike
* 4.0 International License. To view a copy of this license, visit
* http://creativecommons.org/licenses/by-sa/4.0/ or send a letter to
* Creative Commons, PO Box 1866, Mountain View, CA 94042, USA.
*******************************************************************************/

#include "LCD_fonts.h"
#include "LCD_driver.h"
#include "main.h"
#include "p33EP512MU810.h"
#include "DSPIC33E_hardware.h"

uint16 BACK_COLOR;    // Background color
uint16 POINT_COLOR;   // Pen color
uint8 num32_char[10] = {0,0,0,0,0,0,0,0,0,0}; // 32bit number to character array:

//Function prototypes (private)
static void LCD_Write_Bus(short command);
static void LCD_FrameAddr(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2);
static void LCD_WR_DATA8(char da);
static void LCD_WR_DATA(int da);
static void LCD_WR_REG(char da);

/*******************************************************************************
* 1mS Delay
* Edit "count" based on processor speed for 1ms
*
*******************************************************************************/
void delayms(uint16 count)
{
    int i,j;
    for(i=0;i<count;i++)
    {
        //Debug_0=0;
        for(j=0;j<6000;j++);    //Edit count for 1ms
        //Debug_0=1;
    }
}

/*******************************************************************************
* SPI Bus Routine
* Modify to fit the bus connected to LCD
* LCD on SPI2 by itself
*******************************************************************************/
void LCD_Write_Bus(short command)
{	
    short temp;

    LCD_CS = 0;                     // lower the slave select line
    temp = SPI2BUF;                 // dummy read of the SPI1BUF register to clear the SPIRBF flag
    SPI2BUF = command;              // write the data out to the SPI peripheral
    while (!SPI2STATbits.SPIRBF);   // wait for the data to be sent out
    LCD_CS = 1;                     // raise the slave select line
}

/*******************************************************************************
*
*
*
*******************************************************************************/
void LCD_WR_DATA8(char da) //Transmit Data - 8-bit parameter
{
    LCD_DC=1;
    LCD_Write_Bus(da);
}

/*******************************************************************************
*
*
*
*******************************************************************************/
 void LCD_WR_DATA(int da)
{
    LCD_DC=1;
    LCD_Write_Bus(da>>8);    // Right shift 8 send high order byte
    LCD_Write_Bus(da);       // Send low order byte
}

/*******************************************************************************
*
*
*
*******************************************************************************/
void LCD_WR_REG(char da)	 
{
    LCD_DC=0;
    LCD_Write_Bus(da);
}

/*******************************************************************************
*
*
*
*******************************************************************************/
 void LCD_WR_REG_DATA(int reg,int da)
{
    LCD_WR_REG(reg);
    LCD_WR_DATA(da);
}

/*******************************************************************************
* Set active LCD Frame Memory area
*
*******************************************************************************/
void LCD_FrameAddr(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2)
{  
   LCD_WR_REG(0x2a);    // Column Address Set
   LCD_WR_DATA8(x1>>8); // SC15-09 Start Column address byte 1
   LCD_WR_DATA8(x1);    // SC08-01 Start Column address byte 2
   LCD_WR_DATA8(x2>>8); // EC15-09 End Column address byte 1
   LCD_WR_DATA8(x2);    // EC08-01 End Column address byte 2
  
   LCD_WR_REG(0x2b);    // Page Address Set
   LCD_WR_DATA8(y1>>8); // SP15-09 Start Page address byte 1
   LCD_WR_DATA8(y1);    // SP08-01 Start Page address byte 2
   LCD_WR_DATA8(y2>>8); // EP15-09 End Page address byte 1
   LCD_WR_DATA8(y2);    // EP08-01 End Page address byte 2

   LCD_WR_REG(0x2C);    // Memory Write

}

/*******************************************************************************
* Initialize LCD Display
*
*
*******************************************************************************/
void Lcd_Init(void)
{
    LCD_REST=1;
    delayms(5);	
    LCD_REST=0;
    delayms(5);
    LCD_REST=1;
    LCD_CS=1;
    delayms(5);
    LCD_CS =0;  //Open Chip Enable

    LCD_WR_REG(0xCB);       //Power Control A
    LCD_WR_DATA8(0x39);     // Parameter 1
    LCD_WR_DATA8(0x2C);     // Parameter 2
    LCD_WR_DATA8(0x00);     // Parameter 3
    LCD_WR_DATA8(0x34);     // REG_VD Vcore control
    LCD_WR_DATA8(0x02);     // VBC ddvdh control

    LCD_WR_REG(0xCF);       // Power Control B
    LCD_WR_DATA8(0x00);     // Parameter 1
    LCD_WR_DATA8(0XC1);     // power control
    LCD_WR_DATA8(0X30);     // DC_ena ESD protection
 
    LCD_WR_REG(0xE8);       // Driver Timing Control A
    LCD_WR_DATA8(0x85);
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(0x78);
 
    LCD_WR_REG(0xEA);       // Driver Timing Control B
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(0x00);
 
    LCD_WR_REG(0xED);       // Power On Sequence Control
    LCD_WR_DATA8(0x64);
    LCD_WR_DATA8(0x03);
    LCD_WR_DATA8(0X12);
    LCD_WR_DATA8(0X81);

    LCD_WR_REG(0xF7);       // Pump Ratio Control
    LCD_WR_DATA8(0x20);
  
    LCD_WR_REG(0xC0);       // Power control 1
    LCD_WR_DATA8(0x23);     // VRH[5:0]
 
    LCD_WR_REG(0xC1);       // Power control 2
    LCD_WR_DATA8(0x10);     // SAP[2:0];BT[3:0]
 
    LCD_WR_REG(0xC5);       // VCOM control 1
    LCD_WR_DATA8(0x3e);     // VMH Contrast Adjustment
    LCD_WR_DATA8(0x28);     // VML VCOML voltage
 
    LCD_WR_REG(0xC7);       // VCOM control 2
    LCD_WR_DATA8(0x86);     // VMF
 
    LCD_WR_REG(0x36);       // Memory Access Control Display Orientation
    LCD_WR_DATA8(0x28);     // Portrate = 48 or 68, Landscape = 28 or E8

    LCD_WR_REG(0x3A);       // COLMOD: Pixel Format Set
    LCD_WR_DATA8(0x55);     // 16 bits per pixel

    LCD_WR_REG(0xB1);       // Frame Rate Control
    LCD_WR_DATA8(0x00);     // DIVA
    LCD_WR_DATA8(0x1B);     // RTNA 18=79Hz, 1B=70Hz
 
    LCD_WR_REG(0xB6);       // Display Function Control
    LCD_WR_DATA8(0x08);
    LCD_WR_DATA8(0x82);
    LCD_WR_DATA8(0x27);
 
    LCD_WR_REG(0xF2);       // 3Gamma Function Disable
    LCD_WR_DATA8(0x00);
 
    LCD_WR_REG(0x26);       //Gamma curve selected
    LCD_WR_DATA8(0x01);
 
    LCD_WR_REG(0xE0);       //Set Gamma
    LCD_WR_DATA8(0x0F);
    LCD_WR_DATA8(0x31);
    LCD_WR_DATA8(0x2B);
    LCD_WR_DATA8(0x0C);
    LCD_WR_DATA8(0x0E);
    LCD_WR_DATA8(0x08);
    LCD_WR_DATA8(0x4E);
    LCD_WR_DATA8(0xF1);
    LCD_WR_DATA8(0x37);
    LCD_WR_DATA8(0x07);
    LCD_WR_DATA8(0x10);
    LCD_WR_DATA8(0x03);
    LCD_WR_DATA8(0x0E);
    LCD_WR_DATA8(0x09);
    LCD_WR_DATA8(0x00);

    LCD_WR_REG(0XE1);       //Set Gamma
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(0x0E);
    LCD_WR_DATA8(0x14);
    LCD_WR_DATA8(0x03);
    LCD_WR_DATA8(0x11);
    LCD_WR_DATA8(0x07);
    LCD_WR_DATA8(0x31);
    LCD_WR_DATA8(0xC1);
    LCD_WR_DATA8(0x48);
    LCD_WR_DATA8(0x08);
    LCD_WR_DATA8(0x0F);
    LCD_WR_DATA8(0x0C);
    LCD_WR_DATA8(0x31);
    LCD_WR_DATA8(0x36);
    LCD_WR_DATA8(0x0F);
 
    LCD_WR_REG(0x11);       //Exit Sleep
    delayms(120);
				
    LCD_WR_REG(0x29);       //Display on
    LCD_WR_REG(0x2c);
}

/*******************************************************************************
* Clear screen function
* Color:To clear the screen filled with color
*
*******************************************************************************/
void LCD_Clear(uint16 Color)
{
    uint8 VH,VL;
    uint16 i,j;
    VH=Color>>8;
    VL=Color;
    LCD_FrameAddr(0,0,LCD_W-1,LCD_H-1);
    for(i=0;i<LCD_W;i++)
    {
        for (j=0;j<LCD_H;j++)
        {
            LCD_WR_DATA8(VH);
            LCD_WR_DATA8(VL);
        }
    }
}

/*******************************************************************************
* Display 40x40 Icon using 5R-6G-5B color format
*
*
*******************************************************************************/
void LCD_DrawIcon(uint16 x,uint16 y)
{
    int i;

    LCD_FrameAddr(x,y,x+39,y+39);         // Icon size drawing window
    for(i=0;i<1600;i++)                 // Loop for total 2-byte color array
    {
        //LCD_WR_DATA8(image[i*2+1]);   // Swap these two lines if 565 color data is wrong
        LCD_WR_DATA8(image[i*2]);
        LCD_WR_DATA8(image[i*2+1]);     // Send color data
    }
}

/*******************************************************************************
* Draw point of one pixel
* POINT_COLOR:Color this point
*
*******************************************************************************/
void LCD_DrawPoint(uint16 x,uint16 y)
{
	LCD_FrameAddr(x,y,x,y);//Set cursor position
	LCD_WR_DATA(POINT_COLOR); 	    
}

/*******************************************************************************
* Draw a big point 2x2 pixels
* POINT_COLOR:Color this point
*
*******************************************************************************/
void LCD_DrawPoint_big(uint16 x,uint16 y)
{
	LCD_Fill(x-1,y-1,x+1,y+1,POINT_COLOR);
}

/*******************************************************************************
* Fill in the designated area specified color
* Area size: (x1:y1) Start, (x2:y2) End
* (x2-x1)*(y2-y1)
*******************************************************************************/
void LCD_Fill(uint16 x1,uint16 y1,uint16 x2,uint16 y2,uint16 color)
{          
	uint16 i,j;
	LCD_FrameAddr(x1,y1,x2,y2);      //Set cursor position
	for(i=y1;i<=y2;i++)
	{													   	 	
            for(j=x1;j<=x2;j++)LCD_WR_DATA(color);//Set cursor position
	} 					  	    
}

/*******************************************************************************
* Draw lines
* x1,y1:Starting point coordinates
* x2,y2:End coordinates
*******************************************************************************/
void LCD_DrawLine(uint16 x1, uint16 y1, uint16 x2, uint16 y2)
{
	uint16 t;
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 

	delta_x=x2-x1; //Calculate the coordinates increment
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //Set single-step directions
	else if(delta_x==0)incx=0;//Vertical line
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//Level 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //Select the basic incremental axes
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//Draw lines output
	{  
		LCD_DrawPoint(uRow,uCol);//Draw points
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}

/*******************************************************************************
* Draw a rectangle corner to corner
* (x1, y1): Upper left corner
* (x2, y2): Lower right corner
*******************************************************************************/
void LCD_DrawRectangle(uint16 x1, uint16 y1, uint16 x2, uint16 y2)
{
	LCD_DrawLine(x1,y1,x2,y1);
	LCD_DrawLine(x1,y1,x1,y2);
	LCD_DrawLine(x1,y2,x2,y2);
	LCD_DrawLine(x2,y1,x2,y2);
}

/*******************************************************************************
* Specify the size of a circle drawn in the specified location
* (x,y):The center
* r    :Radius
*******************************************************************************/
void Draw_Circle(uint16 x0,uint16 y0,uint8 r)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //Judge signs the next point position
	while(a<=b)
	{
		LCD_DrawPoint(x0-b,y0-a);             //3           
		LCD_DrawPoint(x0+b,y0-a);             //0           
		LCD_DrawPoint(x0-a,y0+b);             //1       
		LCD_DrawPoint(x0-b,y0-a);             //7           
		LCD_DrawPoint(x0-a,y0-b);             //2             
		LCD_DrawPoint(x0+b,y0+a);             //4               
		LCD_DrawPoint(x0+a,y0-b);             //5
		LCD_DrawPoint(x0+a,y0+b);             //6 
		LCD_DrawPoint(x0-b,y0+a);             
		a++;
		//Circle using Bresenham algorithm   
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 
		LCD_DrawPoint(x0+a,y0+b);
	}
} 

/*******************************************************************************
* Displays a character at the specified position
* num:Characters to be displayed:" "--->"~"
* mode:Preserve background (1) Overwrite background (0)
*******************************************************************************/
void LCD_8x16_Char(uint16 x,uint16 y,uint8 num,uint8 mode)
{
    uint8 temp;
    uint8 pos,t;

    if(x>LCD_W-16||y>LCD_H-16)return;	    // If character runs off screen, exit
		   
    num=num-' ';                                // Character number = char number - "space" char for offset into array. We can use fixed number here.
    LCD_FrameAddr(x,y,x+7,y+15);                //Set write window position
    if(!mode)  //Overwrite background
    {
        for(pos=0;pos<16;pos++)                 // Loop for each byte of the character
        {
            temp=asc2_1608[(uint16)num*16+pos];    //Grab byte out of font array
            for(t=0;t<8;t++)
            {
                //if(temp&0x01)                 // Test the LSb for a 1 can be used to mirror character
                if(temp&0x80)                   // Test the MSb for a 1
                {
                    LCD_WR_DATA(POINT_COLOR);
		}
                else                            // LSb was a 0
                {
                    LCD_WR_DATA(BACK_COLOR);
                }
                //temp>>=1;                     // Right shift the byte can be used to mirror character
                temp<<=1;                       // Left shift the byte
            }

        }
    }
    else    //Preserve background
    {
        for(pos=0;pos<16;pos++)
	{
            temp=asc2_1608[(uint16)num*16+pos];    //Call 1608 fonts
            for(t=0;t<8;t++)
            {
                // if(temp&0x01)                // Test the LSb for a 1 can be used to mirror character
                if(temp&0x80)                   // Test the MSb for a 1
                {
                    LCD_WR_DATA(POINT_COLOR);   //Draw a point
                }
                //temp>>=1;                     // Right shift the byte can be used to mirror character
                temp<<=1;                       // Left shift the byte
            }
	}
    }
}

/*******************************************************************************
* m^n function
*
*******************************************************************************/
uint32 mypow(uint8 m,uint8 n)
{
	uint32 result=1;
	while(n--)result*=m;    
	return result;
}

/*******************************************************************************
* Display 16 bit number with no leading zeros using 16x24 font
* x,y :Starting point coordinates
* len :Number of digits to show
* num:Value (0 to 65535);
*******************************************************************************/
void LCD_16nz_Num(uint16 x,uint16 y,uint16 num,uint8 len)
{         	
   uint8 t,temp;
   uint8 enshow=0;            // Leading zero suppression flag
   for(t=0;t<len;t++)      // One digit at a time till display length is hit
   {
      temp=(num/mypow(10,len-t-1))%10;
      if(enshow==0&&t<(len-1))
      {
         if(temp==0)
         {
            LCD_16x24_Char(x+16*t,y,' ',0);     // 16 pixle wide font
            continue;  // Force loop to next iteration
         }
         else enshow=1;
      }
      LCD_16x24_Char(x+16*t,y,temp+48,0);    // 16 pixle wide font
   }
}

/*******************************************************************************
* Display 16 bit number with no leading zeros using 8x16 font
* x,y :Starting point coordinates
* len :Number of digits to show
* num:Value (0 to 65535);
*******************************************************************************/
void LCD_16nz8_Num(uint16 x,uint16 y,uint16 num,uint8 len)
{
   uint8 t,temp;
   uint8 enshow=0;            // Leading zero suppression flag
   for(t=0;t<len;t++)      // One digit at a time till display length is hit
   {
      temp=(num/mypow(10,len-t-1))%10;
      if(enshow==0&&t<(len-1))
      {
         if(temp==0)
         {
            LCD_8x16_Char(x+8*t,y,' ',0);    // 8 pixle wide font
            continue;  // Force loop to next iteration
         }
         else enshow=1;
      }
      LCD_8x16_Char(x+8*t,y,temp+48,0);    // 8 pixle wide font
   }
}

/*******************************************************************************
* Display 16 bit number with leading zeros using 16x24 font
* x,y:Starting point coordinates
* num:Value (0 to 65535)
*******************************************************************************/
void LCD_16wz_Num(uint16 x,uint16 y,uint16 num,uint8 len)
{         	
   uint8 t,temp;
   for(t=0;t<len;t++)       // One digit at a time till display length is hit
   {
      temp=(num/mypow(10,len-t-1))%10;
      LCD_16x24_Char(x+16*t,y,temp+'0',0);
   }
}

/*******************************************************************************
* Display 16 bit number with leading zeros using 8x16 font
* x,y:Starting point coordinates
* num:Value (0 to 65535)
*******************************************************************************/
void LCD_16wz8_Num(uint16 x,uint16 y,uint16 num,uint8 len)
{
   uint8 t,temp;
   for(t=0;t<len;t++)       // One digit at a time till display length is hit
   {
      temp=(num/mypow(10,len-t-1))%10;
      LCD_8x16_Char(x+8*t,y,temp+'0',0);
   }
}

/*******************************************************************************
* Display 32 bit number with no leading zeros
* x,y: Starting point coordinates
* num: Number to display (0 to 4,294,967,295)
* len: Number of digits to show. Truncates most significant end
*******************************************************************************/
void LCD_32nz_Num(uint16 x,uint16 y,uint32 num,uint8 len)
{
    uint8 t;          // Character position
    uint8 enshow=0;   // Leading zero suppression flag
 
    // Convert number to characters and store in 10-byte wide array
    for(t=10;t>0;)                       // loop for each character in the aray
    {
        t--;                             // Decrement to store least significant at high end of array
        num32_char[t] = (num %10)+ 48; // Store least significant decimal as character
        num = num /10;                   // Divide by 10 to position next decimal to convert
    }

    // Display array characters on screen suppressing leading zeros
    for(t=(10-len);t<10;t++)                        // loop for each character in the aray
    {

        if((enshow==0) && (num32_char[t]=='0'))     // test for leading zeros
        {
            LCD_16x24_Char(x,y,' ',0);              // display space instead of zero
            x = x+16;                               // increment character position
        }
        else
        {
            enshow = 1;                             // leading zero flag off
            LCD_16x24_Char(x,y,num32_char[t],0);    // display number character
            x = x+16;                               // Increment character position
        }
    }
}

/*******************************************************************************
* Convert 32bit number to a 10 character array.
* This is useful for adding format characters between numbers...
* least significant is at high end of array
*******************************************************************************/
void NumToCharArray(uint32 num)
{
    uint8 t;          // Character position
    // Convert number to characters and store in 10-byte wide array
    for(t=10;t>0;)                       // loop for each character in the aray
    {
        t--;                             // Decrement to store least significant at high end of array
        num32_char[t] = (num %10)+ 48;   // Store least significant decimal as character
        num = num /10;                   // Divide by 10 to position next decimal to convert
    }
}

/*******************************************************************************
* Display string with 8x16 fonts
* x,y:Starting point coordinates
* *p:String starting address
*******************************************************************************/
void LCD_8x16_String(uint16 x,uint16 y,char *p)
{         
    while(*p!='\0')                 // While not at end of string
    {       
        if(x>LCD_W-8){x=0;y+=16;}   // Test if position > display width & char wrap
        if(y>LCD_H-16){y=x=0;}      // Test if position > display height
        LCD_8x16_Char(x,y,*p,0);    // pos x, pos y, char#, mode=0
        x+=8;                       // Increment by the font width
        p++;
    }  
}

/*******************************************************************************
* Display string with 16x24 fonts
* x,y:Starting point coordinates
* *p:String starting address
*******************************************************************************/
void LCD_16x24_String(uint16 x,uint16 y,char *p)
{
    while(*p!='\0')                 // While not at end of string
    {
        if(x>LCD_W-16){x=0;y+=24;}  // Test if position > display width & char wrap
        if(y>LCD_H-24){y=x=0;}      // Test if position > display height
        LCD_16x24_Char(x,y,*p,0);   // pos x, pos y, char#, mode=0
        //LCD_16x24_Char(x,y,*p,1); // pos x, pos y, char#, mode=1
        x+=16;                      // Increment by the font width
        p++;
    }
}

/*******************************************************************************
* Displays a character at the specified position
* num:Characters to be displayed:" "--->"~"
* mode:Preserve background (1) Overwrite background (0)
*******************************************************************************/
void LCD_16x24_Char(uint16 x,uint16 y,uint8 num,uint8 mode)
{
    uint8 temp;
    uint8 pos,t;

    if(x>LCD_W-16||y>LCD_H-24)return;           // Test the character position to see if fit on screen note this test is redundant to the string write function test.

    num=num-' ';                                // Character number = char number minus "space" character to give offset into array. We can use fixed number here.
    LCD_FrameAddr(x,y,x+15,y+23);               //Set write window position

    if(!mode)   //Overwrite background
    {
        for(pos=0;pos<48;pos++)                 // Loop for each byte of the character
	{
            temp=Font_1624[(uint16)num*48+pos];    //Grab byte out of font array

            for(t=0;t<8;t++)                    // Loop for each pixel bit in byte
            {
                //if(temp&0x01)                 // Test the LSb for a 1 can be used to mirror character
                if(temp&0x80)                   // Test the MSb for a 1
                {
                    LCD_WR_DATA(POINT_COLOR);
		}
                else                            // LSb was a 0
                {
                    LCD_WR_DATA(BACK_COLOR);
                }
                //temp>>=1;                       // Right shift the byte can be used to mirror character
                temp<<=1;                       // Left shift the byte

            }
        }
    }
    else  //Preserve background
    {
        for(pos=0;pos<48;pos++)
	{
            temp=Font_1624[(uint16)num*48+pos];        //Grab byte out of font array
            for(t=0;t<8;t++)                        // Loop for each pixel bit in byte
            {
                //if(temp&0x01)                     // Test the LSb for a 1 can be used to mirror character
                if(temp&0x80)                       // Test the MSb for a 1
                {
                    LCD_WR_DATA(POINT_COLOR);      //Draw a point
                }
                //temp>>=1;                        // Right shift the byte can be used to mirror character
                temp<<=1;                          // Left shift the byte
            }
        }
    }
}

