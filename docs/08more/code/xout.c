#include <stdint.h>
#include <pru_intc.h>
#include "rsc_table_pru0.h"

#define PRU0

volatile register uint32_t __R30;
volatile register uint32_t __R31;

typedef struct {
	uint32_t reg5;
	uint32_t reg6;
	uint32_t reg7;
	uint32_t reg8;
	uint32_t reg9;
	uint32_t reg10;
} bufferData;

bufferData dmemBuf;

/* PRU-to-ARM interrupt */
#define PRU1_PRU0_INTERRUPT (18)
#define PRU0_ARM_INTERRUPT (19+16)

void main(void)
{
	bufferData buf;

	/* Clear the status of all interrupts */
	CT_INTC.SECR0 = 0xFFFFFFFF;
	CT_INTC.SECR1 = 0xFFFFFFFF;

	/* Load the buffer with default values to transfer */
	dmemBuf.reg5 = 0xEEADBEEF;
	dmemBuf.reg6 = 0xAAAAAAAA;
	dmemBuf.reg7 = 0x12345678;
	dmemBuf.reg8 = 0xBBBBBBBB;
	dmemBuf.reg9 = 0x87654321;
	dmemBuf.reg10 = 0xCCCCCCCC;

	/* Load data from DRAM */
	buf = dmemBuf;

	/* Poll until R31.30 (PRU0 interrupt) is set
	 * This signals PRU1 is initialized */
// 	while ((__R31 & (1<<30)) == 0) {
// 	}

	/* XFR registers R5-R10 from PRU0 to PRU1 */
	/* 14 is the device_id that signifies a PRU to PRU transfer */
	__xout(10, 5, 0, buf);

	/* Clear the status of the interrupt */
	CT_INTC.SICR = PRU1_PRU0_INTERRUPT;

	/* Halt the PRU core */
	__halt();
}