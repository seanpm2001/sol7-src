/*	Copyright (c) 1993-96 Sun Microsystems, Inc.	*/
/*	  All Rights Reserved  	*/


#pragma ident	"@(#)t_strerror.c	1.5	97/08/12 SMI"	/* SVr4.0 1.2 */


/*LINTLIBRARY*/
#include <unistd.h>
#include <stdio.h>
#include <stddef.h>
#include <libintl.h>
#include <stropts.h>
#include <xti.h>
#include "timt.h"
#include "tx.h"

static const char __nsl_dom[]  = "SUNW_OST_NETNSL";

static char *_xti_errlist[] = {
	"No Error",					/*  0 */
	"Incorrect address format",			/*  1 - TBADADDR */
	"Incorrect options format",			/*  2 - TBADOPT */
	"Illegal permissions",				/*  3 - TACCES */
	"Illegal file descriptor",			/*  4 - TBADF */
	"Couldn't allocate address",			/*  5 - TNOADDR */
	"Routine will place interface out of state",    /*  6 - TOUTSTATE */
	"Illegal called/calling sequence number",	/*  7 - TBADSEQ */
	"System error",					/*  8 - TSYSERR */
	"An event requires attention",			/*  9 - TLOOK */
	"Illegal amount of data",			/* 10 - TBADDATA */
	"Buffer not large enough",			/* 11 - TBUFOVFLW */
	"Can't send message - (blocked)",		/* 12 - TFLOW */
	"No message currently available",		/* 13 - TNODATA */
	"Disconnect message not found",			/* 14 - TNODIS */
	"Unitdata error message not found",		/* 15 - TNOUDERR */
	"Incorrect flags specified",			/* 16 - TBADFLAG */
	"Orderly release message not found",		/* 17 - TNOREL */
	"Primitive not supported by provider",		/* 18 - TNOTSUPPORT */
	"State is in process of changing",		/* 19 - TSTATECHNG */

	/* Following error codes are new in XTI */

	"Unsupported structure type requested",		/* 20 - TNOSTRUCTYPE */
	"Invalid transport provider name",		/* 21 - TBADNAME */
	"Listener queue length limit is zero",		/* 22 - TBADQLEN */
	"Transport address is in use",			/* 23 - TADDRBUSY */
	"Outstanding connection indications",		/* 24 - TINDOUT */
	"Listener-acceptor transport provider mismatch",
							/* 25 - TPROVMISMATCH */
	"Connection acceptor has listen queue length limit greater than zero",
							/* 26 - TRESQLEN */
"Connection acceptor-listener address not same but required by transport",
							/* 27 - TRESADDR */
	"Incoming connection queue is full",		/* 28 - TQFULL */
	"Protocol error on transport primitive",	/* 29 - TPROTO */
};

static int _xti_nerr = A_CNT(_xti_errlist)-1; /* take off entry t_errno 0 */

const char *
_tx_strerror(int errnum, int api_semantics)
{
	static char buf[BUFSIZ];

	if (api_semantics == TX_XTI_API) {
		if (errnum <= _xti_nerr && errnum >= 0)
			return (dgettext(__nsl_dom, _xti_errlist[errnum]));
		else {
			sprintf(buf, "%d: %s", errnum,
				dgettext(__nsl_dom, "error unknown"));
			return (buf);
		}
	} else {		/* TX_TLI_API */
		/*
		 * This code for TLI only. It uses "t_nerr" and "t_errlist"
		 * which are exposed interfaces in the t_error() man page.
		 * XTI uses different array to avoid binary compatibility
		 * issues in using the exposed array. [ XTI t_error() does
		 * not mention the error message list array ]
		 *
		 * For the moment we simply index into the t_errlist[] array.
		 * When the array fills (we cannot allow it to expand in size
		 * or binary compatibility will be broken), this code will need
		 * modification.  See the comment in _errlst.c.
		 */
		if (errnum < t_nerr && errnum >= 0)
			return (dgettext(__nsl_dom, t_errlist[errnum]));
		else {
			sprintf(buf, "%d: %s", errnum,
				dgettext(__nsl_dom, "error unknown"));
			return (buf);
		}
	}
}
