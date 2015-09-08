



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
