/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*
 * Copyright (c) 1996-1997, by Sun Microsystems, Inc.
 * All rights reserved.
 */

#ifndef _STDARG_H
#define	_STDARG_H

#pragma ident	"@(#)stdarg.h	1.41	97/11/22 SMI"	/* SVr4.0 1.8	*/

#if defined(__STDC__)

#include <sys/va_list.h>

#ifdef	__cplusplus
extern "C" {
#endif

/*
 * The type associated with va_list is defined in <sys/va_list.h> under the
 * implementation name __va_list.  This protects the ANSI-C, POSIX and
 * XPG namespaces.  Including this file allows (requires) the name va_list
 * to exist in the these namespaces.
 */
#ifndef	_VA_LIST
#define	_VA_LIST
typedef __va_list va_list;
#endif

/*
 * Many compilation systems depend upon the use of special functions
 * built into the the compilation system to handle variable argument
 * lists and stack allocations.  The method to obtain this in SunOS
 * is to define the feature test macro "__BUILTIN_VA_ARG_INCR" which
 * enables the following special built-in functions:
 *	__builtin_alloca
 *	__builtin_va_alist
 *	__builtin_va_arg_incr
 * It is intended that the compilation system define this feature test
 * macro, not the user of the system.
 *
 * The tests on the processor type are to provide a transitional period
 * for existing compilation systems, and may be removed in a future
 * release.
 */
#if (defined(__BUILTIN_VA_ARG_INCR) || defined(sparc) || defined(__sparc) || \
    defined(i386) || defined(__i386)) && !(defined(lint) || defined(__lint))

/*
 * Instruction set architectures which use a simple pointer to the
 * argument save area share a common implementation.
 */
#define	va_start(list, name)	(void) (list = (va_list) &__builtin_va_alist)
#define	va_arg(list, mode)	((mode *)__builtin_va_arg_incr((mode *)list))[0]

#else	/* defined(__BUILTIN_VA_ARG_INCR) && !(defined(lint) || ... ) */

/*
 * The following are appropriate implementations for most implementations
 * which have completely stack based calling conventions.  These are also
 * appropriate for lint usage on all systems where a va_list is a simple
 * pointer.
 */
#if __STDC__ != 0	/* -Xc compilation */
#define	va_start(list, name) (void) (list = (void *)((char *)&name + \
	((sizeof (name) + (sizeof (int) - 1)) & ~(sizeof (int) - 1))))
#else
#define	va_start(list, name) (void) (list = (void *)((char *)&...))
#endif	/* __STDC__ != 0 */
#define	va_arg(list, mode) \
	((mode *)(list = (void *)((char *)list + sizeof (mode))))[-1]

#endif	/* defined(__BUILTIN_VA_ARG_INCR) && !(defined(lint) || ... ) */

extern void va_end(va_list);
#define	va_end(list) (void)0

/*
 * va_copy is a Solaris extension to provide a portable way to perform
 * a variable argument list ``bookmarking'' function.
 */
#if defined(__EXTENSIONS__) || ((__STDC__ - 0 == 0) && \
	!defined(_POSIX_C_SOURCE) && !defined(_XOPEN_SOURCE))

#define	va_copy(to, from)	((to) = (from))

#endif	/* defined(__EXTENSIONS__) || ((__STDC__ - 0 == 0) && ... ) */

#ifdef	__cplusplus
}
#endif

#else	/* __STDC__ */

#include <varargs.h>

#endif	/* __STDC__ */

#endif	/* _STDARG_H */
