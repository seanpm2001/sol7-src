/*	Copyright (c) 1984, 1986, 1987, 1988, 1989 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*
 * Copyright  (c) 1986 AT&T
 *	All Rights Reserved
 */
#ident	"@(#)system.c	1.4	92/07/14 SMI"       /* SVr4.0 1.2 */

#include	<curses.h>
#include	"wish.h"

int
vt_system(string)
register char	*string;
{
	register int	retval;
	char	buf;

	(void) vt_before_fork();
	putchar('\n');
	fflush(stdout);
	retval = system(string);
	printf("Please hit ENTER to continue: ");
	fflush(stdout);
	read(0, &buf, 1);
	(void) vt_after_fork();
	return retval;
}
