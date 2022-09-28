/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*
 *      Copyright (c) 1997, by Sun Microsystems, Inc.
 *      All rights reserved.
 */

#pragma ident	"@(#)wsyncup.c	1.7	97/06/25 SMI"	/* SVr4.0 1.6	*/

/*LINTLIBRARY*/

#include	<sys/types.h>
#include	"curses_inc.h"

/* Sync the changes in a window with its ancestors. */

void
wsyncup(WINDOW *win)
{
	short	*wbch, *wech, *pbch, *pech, bch, ech;
	int	wy, px, py, endy;
	WINDOW	*par;

	for (par = win->_parent; par != NULL; win = par, par = par->_parent) {
		py = win->_pary;
		px = win->_parx;
		endy = win->_maxy;

		wbch = win->_firstch;
		wech = win->_lastch;
		pbch = par->_firstch+ py;
		pech = par->_lastch+ py;

		/*
		 * I don't think we need check WINCHANGED first.
		 * The reasoning is that all internal calls will have come
		 * from a function that did change the window.  And assumably
		 * all external calls will work the same way.
		 */
		par->_flags |= _WINCHANGED;
		/* check each line */
		for (wy = 0; wy < endy; ++wy, ++wbch, ++wech, ++pbch, ++pech)
			if (*wbch != _INFINITY) {
				bch = px + *wbch;
				ech = px + *wech;
				if (*pbch > bch)
					*pbch = bch;
				if (*pech < ech)
					*pech = ech;
			}
	}
}

void
wcursyncup(WINDOW *win)
{
	WINDOW	*par = win->_parent;

	while (par != NULL) {
		par->_cury = win->_cury + win->_pary;
		par->_curx = win->_curx + win->_parx;
		par = (win = par)->_parent;
	}
}
