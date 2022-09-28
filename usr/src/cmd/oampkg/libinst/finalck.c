/*	Copyright (c) 1984, 1986, 1987, 1988, 1989 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*LINTLIBRARY*/
#ident	"@(#)finalck.c	1.7	93/03/09 SMI"	/* SVr4.0 1.6.1.1	*/

#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <sys/types.h>
#include <pkgstrct.h>
#include <locale.h>
#include <libintl.h>
#include <pkglib.h>

#define	ERR_ATTR	"ERROR: attribute verification of <%s> failed"
#define	ERR_CONT	"ERROR: content verification of <%s> failed"

extern int	warnflag;
extern char	errbuf[];

int
finalck(struct cfent *ept, int attrchg, int contchg)
{
	int	errflg;
	int	n;

	errflg = 0;
	if (attrchg || contchg) {
		/* verify change, or fix if possible */
		if (n = averify(1, &ept->ftype, ept->path, &ept->ainfo)) {
			logerr(gettext(ERR_ATTR), ept->path);
			logerr(errbuf);
			errflg++;
			warnflag++;
			if (n == VE_EXIST)
				return (1); /* no need to check contents */
		}
	}
	if (contchg && strchr("fev", ept->ftype)) {
		/* verify change was executed properly */
		if ((contchg < 0) || (ept->ftype == 'e')) {
			ept->cinfo.modtime = BADCONT;
			ept->cinfo.size = BADCONT;
			ept->cinfo.cksum = BADCONT;
		}
		if (cverify(1, &ept->ftype, ept->path, &ept->cinfo)) {
			logerr(gettext(ERR_CONT), ept->path);
			logerr(errbuf);
			errflg++;
			warnflag++;
		}
	}
	return (errflg);
}
