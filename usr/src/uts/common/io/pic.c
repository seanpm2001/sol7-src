/*
 * Copyright (c) 1990-1993, by Sun Microsystems, Inc.
 */

#pragma ident	"@(#)pic.c	1.16	95/06/13 SMI"

#include <sys/types.h>
#include <sys/pic.h>
#include <sys/sunddi.h>

void
picsetup()
{
	char 	*bus_keyvalp;
	int	edg_trig;

	if (ddi_prop_lookup_string(DDI_DEV_T_ANY, ddi_root_node(),
		0, "bus-type", &bus_keyvalp) != DDI_PROP_SUCCESS) {
		edg_trig = 1;
	} else {
		if (strcmp(bus_keyvalp, DEVI_MCA_NEXNAME) == 0)
			edg_trig = 0;
		else
			edg_trig = 1;
		ddi_prop_free(bus_keyvalp);
	}

	/* initialize master first 				*/
	if (edg_trig) {
		/* ICW1: Edge-triggered, Cascaded, need ICW4 	*/
		(void) outb(MCMD_PORT, PIC_ICW1BASE|PIC_NEEDICW4);
	} else {
		/* ICW1: level-triggered, Cascaded, need ICW4 	*/
		(void) outb(MCMD_PORT, PIC_ICW1BASE|PIC_NEEDICW4|PIC_LTIM);
	}

	/* ICW2: start master vectors at PIC_VECTBASE 		*/
	(void) outb(MIMR_PORT, PIC_VECTBASE);

	/* ICW3: define which lines are connected to slaves 	*/
	(void) outb(MIMR_PORT, 1 << MASTERLINE);

	/* ICW4: buffered master (?), norm eoi, mcs 86 		*/
	(void) outb(MIMR_PORT, PIC_86MODE);

	/* OCW1: Start the master with all interrupts off 	*/
	(void) outb(MIMR_PORT, 0xFF);

	/* OCW3: set master into "read isr mode" 		*/
	(void) outb(MCMD_PORT, PIC_READISR);

	/* initialize the slave 				*/
	if (edg_trig) {
		/* ICW1: Edge-triggered, Cascaded, need ICW4 	*/
		(void) outb(SCMD_PORT, PIC_ICW1BASE|PIC_NEEDICW4);
	} else {
		/* ICW1: level-triggered, Cascaded, need ICW4 	*/
		(void) outb(SCMD_PORT, PIC_ICW1BASE|PIC_NEEDICW4|PIC_LTIM);
	}

	/* ICW2: set base of vectors 				*/
	outb(SIMR_PORT, PIC_VECTBASE +  8);

	/* ICW3: specify ID for this slave 			*/
	outb(SIMR_PORT, MASTERLINE);

	/* ICW4: buffered slave (?), norm eoi, mcs 86 		*/
	outb(SIMR_PORT, PIC_86MODE);

	/* OCW1: set interrupt mask 				*/
	outb(SIMR_PORT, 0xff);

	/* OCW3: set pic into "read isr mode" 			*/
	outb(SCMD_PORT, PIC_READISR);
}
