/*	Copyright (c) 1984, 1986, 1987, 1988, 1989 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ident	"@(#)getrusage.c	1.3	96/10/17 SMI"	/* SVr4.0 1.1	*/

/*******************************************************************

		PROPRIETARY NOTICE (Combined)

This source code is unpublished proprietary information
constituting, or derived under license from AT&T's UNIX(r) System V.
In addition, portions of such source code were derived from Berkeley
4.3 BSD under license from the Regents of the University of
California.



		Copyright Notice 

Notice of copyright on this source code product does not indicate 
publication.

	(c) 1986,1987,1988,1989  Sun Microsystems, Inc
	(c) 1983,1984,1985,1986,1987,1988,1989  AT&T.
	          All rights reserved.
********************************************************************/ 


/*
 * Compatibility lib for BSD's getrusgae(). Only the
 * CPU time usage is supported, and hence does not
 * fully support BSD's rusage semantics.
 */

#include <sys/types.h>
#include <sys/time.h>
#include <sys/times.h>
#include <sys/param.h>
#include <sys/errno.h>
#include "resource.h"

extern	errno;

getrusage(who, rusage)
	int	who;
	struct	rusage	*rusage;
{
	struct	tms	tms;

	if ( times(&tms) == -1 )
		return -1;		/* errno set by times() */

	if (rusage)
		memset((void *)rusage, 0, sizeof(struct rusage));

	switch (who) {

		case RUSAGE_SELF:
			rusage->ru_utime.tv_sec = tms.tms_utime / HZ;
			rusage->ru_utime.tv_usec = (tms.tms_utime % HZ) /
				HZ * 1000000;
			rusage->ru_stime.tv_sec = tms.tms_stime / HZ;
			rusage->ru_stime.tv_usec = (tms.tms_stime % HZ) /
				HZ * 1000000;
			return 0;

		case RUSAGE_CHILDREN:
			rusage->ru_utime.tv_sec = tms.tms_cutime / HZ;
			rusage->ru_utime.tv_usec = (tms.tms_cutime % HZ) /
				HZ * 1000000;
			rusage->ru_stime.tv_sec = tms.tms_cstime / HZ;
			rusage->ru_stime.tv_usec = (tms.tms_cstime % HZ) /
				HZ * 1000000;
			return 0;

		default:
			errno = EINVAL;
			return -1;
	}
}

