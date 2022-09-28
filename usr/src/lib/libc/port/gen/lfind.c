/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*
 * Copyright (c) 1996, by Sun Microsystems, Inc.
 * All rights reserved.
 */

#pragma	ident	"@(#)lfind.c	1.10	96/11/27 SMI"	/* SVr4.0 1.9	*/

/*LINTLIBRARY*/
/*
 * Linear search algorithm, generalized from Knuth (6.1) Algorithm Q.
 *
 * This version no longer has anything to do with Knuth's Algorithm Q,
 * which first copies the new element into the table, then looks for it.
 * The assumption there was that the cost of checking for the end of the
 * table before each comparison outweighed the cost of the comparison, which
 * isn't true when an arbitrary comparison function must be called and when the
 * copy itself takes a significant number of cycles.
 * Actually, it has now reverted to Algorithm S, which is "simpler."
 */

#pragma weak lfind = _lfind

#include "synonyms.h"
#include <stdlib.h>
#include <mtlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <thread.h>
#include <synch.h>
#include <search.h>

#ifdef _REENTRANT
extern mutex_t __lsearch_lock;
#endif _REENTRANT

void *
lfind(const void *ky, const void *bs, size_t *nelp, size_t width, int (*compar)())
{
	char * key = (char *)ky;
	char * base = (char *)bs;
	char * next = base + *nelp * width;	/* End of table */

	(void) _mutex_lock(&__lsearch_lock);
	for (; base < next; base += width)
		if ((*compar)(key, base) == 0) {
			(void) _mutex_unlock(&__lsearch_lock);
			return (base);	/* Key found */
		}
	(void) _mutex_unlock(&__lsearch_lock);
	return (NULL);
}
