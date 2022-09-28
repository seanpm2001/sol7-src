/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*
 * Copyright (c) 1997, by Sun Mircrosystems, Inc.
 * All rights reserved.
 */

#pragma	ident	"@(#)menuwin.c	1.4	97/07/09 SMI"	/* SVr4.0 1.2	*/

/*LINTLIBRARY*/

#include <sys/types.h>
#include "private.h"

int
set_menu_win(MENU *m, WINDOW *win)
{
	if (m) {
		if (Posted(m)) {
			return (E_POSTED);
		}
		UserWin(m) = win;
		/* Call scale because the menu subwindow may not be defined */
		_scale(m);
	} else {
		UserWin(Dfl_Menu) = win;
	}
	return (E_OK);
}

WINDOW *
menu_win(MENU *m)
{
	return (UserWin((m) ? m : Dfl_Menu));
}
