/***************************************************************************
* Description:
*
* This work is licensed under the Creative Commons Attribution-ShareAlike
* 4.0 International License. To view a copy of this license, visit
* http://creativecommons.org/licenses/by-sa/4.0/ or send a letter to
* Creative Commons, PO Box 1866, Mountain View, CA 94042, USA.
****************************************************************************/



#include "p33EP512MU810.h"
#include "DSP.h"


void __attribute__((__interrupt__, no_auto_psv)) _DCIInterrupt(void)
{
int rf_I;
int rf_Q;
IFS3bits.DCIIF = 0;

rf_I = RXBUF0;
rf_Q = RXBUF1;

TXBUF0 = rf_I; 
TXBUF1 = rf_Q; 
}
