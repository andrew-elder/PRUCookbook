// Control a ws2812 (neo pixel) display, green, red, blue, green, ...
#include <stdint.h>
#include <pru_cfg.h>
#include "resource_table_empty.h"

#define STR_LEN 3
#define	oneCyclesOn		700/5	// Stay on 700ns
#define oneCyclesOff	800/5
#define zeroCyclesOn	350/5
#define zeroCyclesOff	600/5
#define resetCycles		60000/5	// Must be at least 50u, use 60u
#define out 1		// Bit number to output one

volatile register uint32_t __R30;
volatile register uint32_t __R31;

void main(void)
{
	/* Clear SYSCFG[STANDBY_INIT] to enable OCP master port */
	CT_CFG.SYSCFG_bit.STANDBY_INIT = 0;
	
	uint32_t color[STR_LEN] = {0x0f0000, 0x000f00, 0x0000f};	// green, red, blue
	int i, j;

	for(j=0; j<STR_LEN; j++) {
		for(i=23; i>=0; i--) {
			if(color[j] & (0x1<<i)) {
				__R30 |= 0x1<<out;		// Set the GPIO pin to 1
				__delay_cycles(oneCyclesOn-1);
				__R30 &= ~(0x1<<out);	// Clear the GPIO pin
				__delay_cycles(oneCyclesOff-2);
			} else {
				__R30 |= 0x1<<out;		// Set the GPIO pin to 1
				__delay_cycles(zeroCyclesOn-1);
				__R30 &= ~(0x1<<out);	// Clear the GPIO pin
				__delay_cycles(zeroCyclesOff-2);
			}
		}
	}
	// Send Reset
	__R30 &= ~(0x1<<out);	// Clear the GPIO pin
	__delay_cycles(resetCycles);
	
	__halt();
}
