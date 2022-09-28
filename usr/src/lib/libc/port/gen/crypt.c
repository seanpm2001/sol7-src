/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/
/*
 * Copyright(c) 1996, by Sun Microsystems, Inc
 * All rights reserved.
 */

#pragma ident	"@(#)crypt.c	1.22	97/10/08 SMI"	/* SVr4.0 1.11	*/

/*LINTLIBRARY*/
/*
 * This program implements a data encryption algorithm to encrypt passwords.
 */
#pragma weak crypt = _crypt
#pragma weak encrypt = _encrypt
#pragma weak setkey = _setkey
#include "synonyms.h"

char *
crypt(const char *pw, const char *salt)
{
	return (0);
}


/*ARGSUSED*/
void
encrypt(char *block, int fake)
{
}


void
setkey(const char *key)
{
}
