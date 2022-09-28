/*	Copyright (c) 1984, 1986, 1987, 1988, 1989 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ident	"@(#)get_names.c	1.8	97/05/13 SMI"	/* SVr4.0 1.4	*/

/*
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * 		PROPRIETARY NOTICE (Combined)
 *
 * This source code is unpublished proprietary information
 * constituting, or derived under license from AT&T's UNIX(r) System V.
 * In addition, portions of such source code were derived from Berkeley
 * 4.3 BSD under license from the Regents of the University of
 * California.
 *
 *
 *
 * 		Copyright Notice
 *
 * Notice of copyright on this source code product does not indicate
 * publication.
 *
 * 	(c) 1986,1987,1988.1989  Sun Microsystems, Inc
 * 	(c) 1983,1984,1985,1986,1987,1988,1989  AT&T.
 *	All rights reserved.
 *
 */


#include "talk.h"
#include "ctl.h"
#include <locale.h>
#include <pwd.h>
#include <sys/systeminfo.h>

char *getlogin(), *ttyname(int);

extern CTL_MSG msg;

/*
 * Determine the local and remote user, tty, and machines
 */

struct hostent *gethostbyname();

void
get_names(argc, argv)
int argc;
char *argv[];
{
	char hostname[HOST_NAME_LENGTH + 1];
	char *rem_name;
	char *my_name;
	char *my_machine_name;
	char *rem_machine_name;
	char *my_tty;
	char *rem_tty;
	char *ptr;
	int name_length;

	if (argc < 2) {
		fprintf(stderr,
		    "Usage: talk %s\n", gettext("address [terminal]"));
		exit(1);
	}
	if (!isatty(0)) {
		fprintf(stderr,
	gettext("Standard input must be a tty, not a pipe or a file\n"));
		exit(1);
	}

	if (!isatty(1)) {
		fprintf(stderr,
	gettext("Standard output must be a tty, not a pipe or a file\n"));
		exit(1);
	}

	if ((my_name = getlogin()) == NULL) {
	struct passwd *pass = getpwuid(getuid());
	if (pass != NULL)
		my_name = pass->pw_name;
	}
	if (my_name == NULL) {
		fprintf(stderr,
	gettext("Who are you?  You have no entry in /etc/utmp!  Aborting..\n"));
		exit(1);
	}

	name_length = HOST_NAME_LENGTH;
	(void) sysinfo(SI_HOSTNAME, hostname, name_length);
	my_machine_name = hostname;

	my_tty = strrchr(ttyname(0), '/') + 1;

	/*
	* check for, and strip out, the machine name of the target
	*/

	for (ptr = argv[1]; *ptr != '\0' &&
			 *ptr != '@' &&
			 *ptr != ':' &&
			 *ptr != '!' &&
			 *ptr != '.'; ptr++) {
	}

	if (*ptr == '\0') {

		/* this is a local to local talk */

	rem_name = argv[1];
	rem_machine_name = my_machine_name;

	} else {

	if (*ptr == '@') {
		/* user@host */
		rem_name = argv[1];
		rem_machine_name = ptr + 1;
	} else {
		/* host.user or host!user or host:user */
		rem_name = ptr + 1;
		rem_machine_name = argv[1];
	}
	*ptr = '\0';
	}


	if (argc > 2) {
	rem_tty = argv[2];	/* tty name is arg 2 */
	} else {
	rem_tty = "";
	}

	get_addrs(my_machine_name, rem_machine_name);

	/* Load these useful values into the standard message header */

	msg.id_num = 0;

	strncpy(msg.l_name, my_name, NAME_SIZE);
	msg.l_name[NAME_SIZE - 1] = '\0';

	strncpy(msg.r_name, rem_name, NAME_SIZE);
	msg.r_name[NAME_SIZE - 1] = '\0';

	strncpy(msg.r_tty, rem_tty, TTY_SIZE);
	msg.r_tty[TTY_SIZE - 1] = '\0';
}
