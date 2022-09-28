/*	Copyright (c) 1984, 1986, 1987, 1988, 1989 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ident	"@(#)killall.c	1.6	92/07/14 SMI"	/* SVr4.0 1.21	*/

#include <sys/types.h>
#include <sys/procset.h>
#include <signal.h>
#include <errno.h>

char usage[] = "usage: killall [signal]\n";
char perm[] = "permission denied\n";

main(argc,argv)
int argc;
char **argv;
{
	int sig;
	procset_t set;

	switch(argc) {
		case 1:
			sig = SIGTERM;
			break;
		case 2:
			if (str2sig(argv[1], &sig) == 0)
				break;
		default:
			write(2,usage,sizeof(usage)-1);
			exit(1);
	}

	setprocset(&set, POP_DIFF, P_ALL, P_MYID, P_SID, P_MYID);
	if ((sigsendset(&set, sig) < 0)  &&  (errno != ESRCH)) {
		write(2,perm,sizeof(perm)-1);
		exit(1);
	}

	exit(0);
}
