This is a demo application connecting a SnadPIC Rev 1.03 board (DSPIC33EP512MU810 processor) to a 320x240 262K color Tianma TM022HDH26 V1.0 LCD with an integrated ILITEK ILI9341 LCD Driver.

The DSPic demo board was purchased on Ebay from seller eleckropic with the following board description:
PIC32 PIC24 dsPIC33 EASY USB OTG Development Board Starter kit TQFP100 SD Card

The LCD board was purchased on Ebay from seller egochina8848 with the following board description:
"2.2" Serial SPI TFT Color LCD Module Display 240X320 w/ PCB Adpater / SD Socket"

The LCD pins were attached as follows:
MISO -  Unused, not connected
LED -   Tied through a 10 Ohm resistor to +3.3V to operate the back-light.
SCK -   To demo board RG6
MOSI -  To demo board RG8
D/C -   To demo board RG13
RESET - To demo board RG1
CS -    To demo board RG14
GND -   To demo board GND
VCC -   To demo board +3.3V

The code was compiled on MPLAB X IDE v2.00

Programming was done via PICkit-3