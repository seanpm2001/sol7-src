/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any	*/
/*	actual or intended publication of such source code.	*/

/*	Copyright (c) 1989 by Sun Microsystems, Inc.		*/

.ident	"@(#)utime.s	1.1	96/12/04 SMI"	/* SVr4.0 1.6	*/

/* C library -- utime						*/
/* int utime (const char *path, const struct utimbuf *times);	*/
 
	.file	"utime.s"

#include <sys/asm_linkage.h>

	ANSI_PRAGMA_WEAK(utime,function)

#include "SYS.h"

	SYSCALL(utime)
	RET

	SET_SIZE(utime)
