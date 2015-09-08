/*
This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/



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
