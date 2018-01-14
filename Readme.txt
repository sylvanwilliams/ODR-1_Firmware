This code is utilized within the ODR-1 (Open Design Radio - 1) which is a stand-alone "Firmware" defined QRP transceiver.The design goal for the radio is to cover from 1.8MHz (160 meters) up to 50 MHz (6 meters) though it may be able to cover up to 70MHz (4 meters). The radio uses a common quadrature switching mixer for both transmit and receive. The main chip sets used in the radio are:

Main Processor        - Microchip DSPIC33EP512MU810-1
LCD Display           - Tianma TM022HDH26 with integrated ILI9341 controller 240x320 color
Ethernet Controller   - Microchip ENC28J60-I/SS
Audio Codec           - Texas Instruments TLV320AIC3204
VFO (Clock Generator) - Silicon Labs Si5351A

The first build phase will consist of the Processor Board, Oscillator and mixer Board and the
Display Board. This will constitute a 10mW transceiver. The final board will contain the main post amplifier and low pass filters generating ~5W of output power.

Hardware - https://github.com/N7IFC/ODR-1_Hardware
Firmware - https://github.com/sylvanwilliams/ODR-1_Firmware


This work is licensed under the Creative Commons Attribution-ShareAlike 4.0 International License.To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/4.0/ or send a letter to Creative Commons, PO Box 1866, Mountain View, CA 94042, USA.