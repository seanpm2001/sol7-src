/*	Copyright (c) 1984, 1986, 1987, 1988, 1989 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*
 *		PROPRIETARY NOTICE (Combined)
 *
 *
 * This source code is unpublished proprietary information
 * constituting, or derived under license from AT&T's UNIX(r) System V.
 * In addition, portions of such source code were derived from Berkeley
 * 4.3 BSD under license from the Regents of the University of
 * California.
 *
 *
 *		Copyright Notice
 *
 * Notice of copyright on this source code product does not indicate
 * publication.
 *
 * 	Copyright (c) 1986-1989,1996-1998 by Sun Microsystems, Inc.
 *	All rights reserved.
 *
 * 	Copyright (c) 1983-1989 AT&T.
 *	All rights reserved.
 */

#ifndef	_SYS_PARAM_H
#define	_SYS_PARAM_H

#pragma ident	"@(#)param.h	1.7	98/01/06 SMI"	/* SVr4.0 1.3	*/

/*
 * Fundamental variables; don't change too often.
 *
 * This file is included here for compatibility with
 * SunOS, but it does *not* include all the values
 * define in the SunOS version of this file.
 */

#include <limits.h>
#include <unistd.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef	MAX_INPUT
#define	MAX_INPUT	512	/* Maximum bytes stored in the input queue */
#endif

#ifndef	MAX_CANON
#define	MAX_CANON	256	/* Maximum bytes for canoical processing */
#endif

#define	UID_NOBODY	60001
#define	GID_NOBODY	60001

#define	UID_NOACCESS	60002	/* user ID no access */

#define	MAXPID	((pid_t)sysconf(_SC_MAXPID))	/* max process id */
#define	MAXUID	2147483647	/* max user id */
#define	MAXLINK	32767		/* max links */

#define	NMOUNT		40	/* est. of # mountable fs for quota calc */

/* The values below are defined in limits.h */
#define	NOFILE	OPEN_MAX	/* max open files per process */

#define	CANBSIZ	256		/* max size of typewriter line	*/
#define	HZ	((int)sysconf(_SC_CLK_TCK))  /* ticks/second of the clock */
#define	TICK	(1000000000/((int)sysconf(_SC_CLK_TCK)))

#define	NCARGS	0x100000	/* # characters in exec arglist */
				/*   must be multiple of NBPW.  */
/*
 * These define the maximum and minimum allowable values of the
 * configurable parameter NGROUPS_MAX.
 */
#define	NGROUPS_UMAX	32
#define	NGROUPS_UMIN	8

#define	NGROUPS		NGROUPS_MAX	/* max number groups, from limits.h */
#define	NOGROUP		-1	/* marker for empty group set member */

/*
 * Priorities.  Should not be altered too much.
 */

#define	PMASK	0177
#define	PCATCH	0400
#define	PNOSTOP	01000
#define	PSWP	0
#define	PINOD	10
#define	PSNDD	PINOD
#define	PAMAP	PINOD
#define	PPMAP	PINOD
#define	PRIBIO	20
#define	PZERO	25
#define	PMEM	0
#define	NZERO	20
#define	PPIPE	26
#define	PVFS	27
#define	PWAIT	30
#define	PLOCK	35
#define	PSLEP	39
#define	PUSER	60
#define	PIDLE	127

/*
 * Fundamental constants of the implementation--cannot be changed easily.
 */

#define	NBPS	0x20000		/* Number of bytes per segment */
#define	NBPW	sizeof (int)	/* number of bytes in an integer */
#define	NULL	0
#define	CMASK	0		/* default mask for file creation */
#define	CDLIMIT	(1L<<11)	/* default max write address */
#define	NODEV	(dev_t)(-1)
#define	NBPSCTR		512	/* Bytes per disk sector.	*/
#define	UBSIZE		512	/* unix block size.		*/
#define	SCTRSHFT	9	/* Shift for BPSECT.		*/

#define	lobyte(X)	(((unsigned char *)&(X))[1])
#define	hibyte(X)	(((unsigned char *)&(X))[0])
#define	loword(X)	(((ushort_t *)&(X))[1])
#define	hiword(X)	(((ushort_t *)&(X))[0])

/* REMOTE -- whether machine is primary, secondary, or regular */
#define	SYSNAME 9		/* # chars in system name */
#define	PREMOTE 39

/*
 * MAXPATHLEN defines the longest permissible path length,
 * including the terminating null, after expanding symbolic links.
 * MAXSYMLINKS defines the maximum number of symbolic links
 * that may be expanded in a path name. It should be set high
 * enough to allow all legitimate uses, but halt infinite loops
 * reasonably quickly.
 * MAXNAMELEN is the length (including the terminating null) of
 * the longest permissible file (component) name.
 */
#define	MAXPATHLEN	1024
#define	MAXSYMLINKS	20
#define	MAXNAMELEN	256

#ifndef	NADDR
#define	NADDR 13
#endif

/*
 * The following are defined to be the same as
 * defined in /usr/include/limits.h.  They are
 * needed for pipe and FIFO compatibility.
 */
#ifndef	PIPE_BUF	/* max # bytes atomic in write to a pipe */
#define	PIPE_BUF	5120
#endif	/* PIPE_BUF */

#ifndef	PIPE_MAX	/* max # bytes written to a pipe in a write */
#define	PIPE_MAX	5120
#endif	/* PIPE_MAX */

#define	PIPEBUF PIPE_BUF	/* pipe buffer size */

#ifndef	NBBY
#define	NBBY	8			/* number of bits per byte */
#endif

/* macros replacing interleaving functions */
#define	dkblock(bp)	((bp)->b_blkno)
#define	dkunit(bp)	(minor((bp)->b_dev) >> 3)

/*
 * File system parameters and macros.
 *
 * The file system is made out of blocks of at most MAXBSIZE units,
 * with smaller units (fragments) only in the last direct block.
 * MAXBSIZE primarily determines the size of buffers in the buffer
 * pool. It may be made larger without any effect on existing
 * file systems; however making it smaller make make some file
 * systems unmountable.
 *
 * Note that the blocked devices are assumed to have DEV_BSIZE
 * "sectors" and that fragments must be some multiple of this size.
 */
#define	MAXBSIZE	8192
#define	DEV_BSIZE	512
#define	DEV_BSHIFT	9		/* log2(DEV_BSIZE) */
#define	MAXFRAG 	8
#define	MAXOFF_T	((off_t)(((ulong_t)1 << NBBY * sizeof (off_t) - 1) - 1))
#define	MAXOFFSET_T	((1LL << NBBY * sizeof (daddr_t) + DEV_BSHIFT - 1) - 1)

#define	btodb(bytes)			/* calculates (bytes / DEV_BSIZE) */ \
	((unsigned)(bytes) >> DEV_BSHIFT)
#define	dbtob(db)			/* calculates (db * DEV_BSIZE) */ \
	((unsigned)(db) << DEV_BSHIFT)

/*
 * PAGES* describes the logical page size used by the system.
 */

#define	PAGESIZE	(sysconf(_SC_PAGESIZE))	/* All the above, for logical */
#define	PAGEOFFSET	(PAGESIZE - 1)
#define	PAGEMASK	(~PAGEOFFSET)

/*
 * Some random macros for units conversion.
 */

/*
 * pages to bytes, and back (with and without rounding)
 */
#define	ptob(x)		((x) * PAGESIZE)
#define	btop(x)		(((unsigned)(x)) / PAGESIZE)
#define	btopr(x)	((((unsigned)(x) + PAGEOFFSET) / PAGESIZE))

/*
 * Signals
 */
#include <sys/signal.h>

#include <sys/types.h>

/*
 * bit map related macros
 */
#define	setbit(a, i)	((a)[(i)/NBBY] |= 1<<((i)%NBBY))
#define	clrbit(a, i)	((a)[(i)/NBBY] &= ~(1<<((i)%NBBY)))
#define	isset(a, i)	((a)[(i)/NBBY] & (1<<((i)%NBBY)))
#define	isclr(a, i)	(((a)[(i)/NBBY] & (1<<((i)%NBBY))) == 0)

/*
 * Macros for fast min/max.
 */
#ifndef	MIN
#define	MIN(a, b)	(((a) < (b))?(a):(b))
#endif
#ifndef	MAX
#define	MAX(a, b)	(((a) > (b))?(a):(b))
#endif

#define	howmany(x, y)	(((x)+((y)-1))/(y))
#define	roundup(x, y)	((((x)+((y)-1))/(y))*(y))

/*
 * Scale factor for scaled integers used to count
 * %cpu time and load averages.
 */
#define	FSHIFT	8		/* bits to right of fixed binary point */
#define	FSCALE	(1<<FSHIFT)

/*
 * Maximum size of hostname recognized and stored in the kernel.
 * Same as in /usr/include/netdb.h
 */
#ifndef	MAXHOSTNAMELEN
#define	MAXHOSTNAMELEN	256
#endif

#ifdef __cplusplus
}
#endif

#endif	/* _SYS_PARAM_H */
