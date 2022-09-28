/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*
 *      Copyright (c) 1997, by Sun Microsystems, Inc.
 *      All rights reserved.
 */

#pragma ident	"@(#)notimeout.c	1.9	97/06/25 SMI" /* SVr4.0 1.7 */

#include	<sys/types.h>
#include	"curses_inc.h"

int
notimeout(WINDOW *win, bool bf)
{
	win->_notimeout = (bf) ? TRUE : FALSE;
	return (OK);
}
