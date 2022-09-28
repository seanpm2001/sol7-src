/*	Copyright (c) 1984, 1986, 1987, 1988, 1989 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ident	"@(#)strsave.c	1.11	97/08/25 SMI"	/* SVr4.0 1.1	*/

#include "uucp.h"
#include <rpc/trace.h> 

/* #include <errno.h> */
/* #include <malloc.h> */
/* #include <string.h> */
/* #include <sys/types.h> */
/* #include <sys/stat.h> */

#if defined(sparc)
#define _STAT _stat
#else  /* !sparc */
#define _STAT stat
#endif /* sparc */

extern int _stat(const char *, struct stat *);

/* copy str into data space -- caller should report errors. */

GLOBAL char *
strsave(str)
#if defined(__STDC__)
register const char *str;
#else
register char *str;
#endif
{
	register char *rval;

	trace1(TR_strsave, 0);
	rval = malloc(strlen(str) + 1);
	if (rval != 0)
		strcpy(rval, str);
	trace1(TR_strsave, 1);
	return (rval);
}

/*	Determine if the effective user id has the appropriate permission
	on a file.  Modeled after access(2).
	amode:
		00	just checks for file existence.
		04	checks read permission.
		02	checks write permission.
		01	checks execute/search permission.
		other bits are ignored quietly.
*/

GLOBAL int
eaccess(path, amode)
char		*path;
register mode_t	amode;
{
	struct stat	s;
	uid_t euid;

	trace1(TR_eaccess, 0);
	if (_STAT(path, &s) == -1) {
		trace1(TR_eaccess, 1);
		return (-1);		/* can't stat file */
	}
	amode &= 07;

	if ((euid = geteuid()) == 0) {		/* root can do all */
		trace1(TR_eaccess, 1);
	        return (0);
	}
	if (euid == s.st_uid)
	    s.st_mode >>= 6;		/* use owner bits */
	else if (getegid() == s.st_gid)
	    s.st_mode >>= 3;		/* use group bits */

	if ((amode & s.st_mode) == amode) {
		trace1(TR_eaccess, 1);
		return (0);		/* access permitted */
	}
	errno = EACCES;
	trace1(TR_eaccess, 1);
	return (-1);
}
