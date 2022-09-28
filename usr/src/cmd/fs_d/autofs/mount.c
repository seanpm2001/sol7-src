/*
 *	autofs mount.c
 *
 *	Copyright (c) 1988-1996 Sun Microsystems Inc
 *	All Rights Reserved.
 */

#pragma ident	"@(#)mount.c	1.17	96/04/05 SMI"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <sys/mntent.h>
#include <sys/mnttab.h>
#include <sys/mount.h>
#include <sys/utsname.h>
#include <sys/tiuser.h>
#include <string.h>
#include <fslib.h>
#include "automount.h"

#define	MNTTAB_OPTS	"ignore,nest"

static void usage();
static void process_opts(char *options, int *directp);
static char *concat_opts(const char *opts1, const char *opts2);
static int  ro_given(char *options);

main(argc, argv)
	int argc;
	char **argv;
{
	int error;
	int c;
	int mntflags = 0;
	int nmflg = 0;
	int roflg = 0;
	char *mntpnt, *mapname;
	struct utsname utsname;
	char autofs_addr[MAXADDRLEN];
	struct autofs_args fni;
	char *options = "";
	int mount_timeout = AUTOFS_MOUNT_TIMEOUT;
	struct mnttab mnt;

	while ((c = getopt(argc, argv, "o:mr")) != EOF) {
		switch (c) {
		case '?':
			usage();
			exit(1);
			/* NOTREACHED */

		case 'o':
			options = optarg;
			break;

		case 'm':
			nmflg++;
			break;
		case 'r':	/* converted to -o ro always */
			roflg++;
			break;

		default:
			usage();
		}
	}
	if (argc - optind != 2)
		usage();

	if (geteuid() != 0) {
		(void) fprintf(stderr, "must be root\n");
		exit(1);
	}

	mapname = argv[optind];
	mntpnt  = argv[optind + 1];

	if (strcmp(mntpnt, "/-") == 0) {
		(void) fprintf(stderr, "invalid mountpoint: /-\n");
		exit(1);
	}

	if (uname(&utsname) < 0) {
		perror("uname");
		exit(1);
	}
	(void) strcpy(autofs_addr, utsname.nodename);
	(void) strcat(autofs_addr, ".autofs");

	process_opts(options, &fni.direct);

	if (roflg && !ro_given(options))
		options = concat_opts(options, "ro");

	fni.addr.buf	= autofs_addr;
	fni.addr.len	= strlen(fni.addr.buf);
	fni.addr.maxlen	= fni.addr.len;
	fni.path	= mntpnt;
	fni.opts	= options;
	fni.map		= mapname;
	fni.subdir	= "";
	if (fni.direct)
		fni.key = mntpnt;
	else
		fni.key	= "";
	fni.mount_to	= mount_timeout;
	fni.rpc_to	= AUTOFS_RPC_TIMEOUT;

	error = mount("", mntpnt, mntflags | MS_DATA,
			MNTTYPE_AUTOFS, &fni, sizeof (fni));
	if (error < 0) {
		perror("autofs mount");
		exit(1);
	}

	mnt.mnt_special = fni.map;
	mnt.mnt_mountp = mntpnt;
	mnt.mnt_fstype = MNTTYPE_AUTOFS;
	mnt.mnt_mntopts = concat_opts(options,
		fni.direct ? MNTTAB_OPTS ",direct" : MNTTAB_OPTS ",indirect");
	fsaddtomtab(&mnt);

	return (0);
}

static void
usage()
{
	(void) fprintf(stderr,
	    "Usage: autofs mount [-r] [-o opts]  map  dir\n");
	exit(1);
}

/*
 * Remove pseudo-options "direct", "indirect", "nest", and "ignore" from
 * option list.  Set *directp to 1 if "direct" is found, and 0 otherwise
 * (mounts are indirect by default).  If both "direct" and "indirect" are
 * found, the last one wins.
 */
static void
process_opts(char *options, int *directp)
{
	char *opt;
	char *opts;

	if ((opts = strdup(options)) == NULL) {
		(void) fprintf(stderr,
				"autofs mount: memory allocation failed\n");
		exit(1);
	}
	options[0] = '\0';
	*directp = 0;

	while ((opt = strtok(opts, ",")) != NULL) {
		opts = NULL;
		while (isspace(*opt)) {
			opt++;
		}
		if (strcmp(opt, "direct") == 0) {
			*directp = 1;
		} else if (strcmp(opt, "indirect") == 0) {
			*directp = 0;
		} else if ((strcmp(opt, "nest") != 0) &&
				(strcmp(opt, "ignore") != 0)) {
			if (options[0] != '\0') {
				(void) strcat(options, ",");
			}
			(void) strcat(options, opt);
		}
	};
}

/*
 * Concatenate two options strings, with a comma between them.
 */
static char *
concat_opts(const char *opts1, const char *opts2)
{
	char *opts = malloc(strlen(opts1) + strlen(opts2) + 2);
	if (opts == NULL) {
		(void) fprintf(stderr,
			"autofs mount: memory allocation failed\n");
		exit(1);
	}
	strcpy(opts, opts1);
	if (opts1[0] != '\0' && opts2[0] != '\0') {
		strcat(opts, ",");
	}
	return (strcat(opts, opts2));
}

/*
 * check the options string for 'ro' options
 * if present returns 1 otherwise return 0;
 */

static int
ro_given(char *options)
{
	char	*op = options;

	if (!*op)
		return (0);

	while (op != 0) {
		if (*op == 'r' && *(op+1) == 'o' &&
			(*(op+2) == ',' || *(op+2) == '\0'))
			return (1);

		if ((op = strchr(op, ',')) != NULL)
			op++;
	}


	return (0);
}
