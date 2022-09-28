/*	Copyright (c) 1984, 1986, 1987, 1988, 1989 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ident	"@(#)findgid.c	1.3	92/07/14 SMI"       /* SVr4.0 1.2 */

#include	<sys/types.h>
#include	<stdio.h>
#include	<userdefs.h>

extern gid_t findnextgid();
extern void exit();

/* return the next available gid */
main()
{
	gid_t gid = findnextgid();
	if( gid == -1 )
		exit( EX_FAILURE );
	(void) fprintf( stdout, "%ld\n", gid );
	exit( EX_SUCCESS );
	/*NOTREACHED*/
}
