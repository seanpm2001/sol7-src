/*
 * Copyright (c) 1994-1997, by Sun Microsystems, Inc.
 * All rights reserved
 */

#ifndef _SYS_AIO_H
#define	_SYS_AIO_H

#pragma ident	"@(#)aio.h	1.6	96/08/22 SMI"

#ifdef	__cplusplus
extern "C" {
#endif

typedef struct aio_result_t {
	ssize_t aio_return;		/* return value of read or write */
	int aio_errno;		/* errno generated by the IO */
} aio_result_t;

#ifdef	_SYSCALL32
typedef struct aio_result32_t {
	int aio_return;		/* return value of read or write */
	int aio_errno;		/* errno generated by the IO */
} aio_result32_t;
#endif /* _SYSCALL32 */

#define	AIOREAD		0	/* opcodes for aio calls */
#define	AIOWRITE	1
#define	AIOWAIT		2
#define	AIOCANCEL	3
#define	AIONOTIFY	4
#define	AIOINIT		5
#define	AIOSTART	6
#define	AIOLIO		7
#define	AIOSUSPEND	8
#define	AIOERROR	9
#define	AIOLIOWAIT	10
#define	AIOAREAD	11
#define	AIOAWRITE	12
#ifdef _LARGEFILE64_SOURCE
#if	defined(_LP64) && !defined(_KERNEL)
#define	AIOLIO64	AIOLIO
#define	AIOSUSPEND64	AIOSUSPEND
#define	AIOERROR64	AIOERROR
#define	AIOLIOWAIT64	AIOLIOWAIT
#define	AIOAREAD64	AIOAREAD
#define	AIOAWRITE64	AIOAWRITE
#define	AIOCANCEL64	AIOCANCEL
#else
#define	AIOLIO64	13
#define	AIOSUSPEND64	14
#define	AIOERROR64	15
#define	AIOLIOWAIT64	16
#define	AIOAREAD64	17
#define	AIOAWRITE64	18
#define	AIOCANCEL64	19
#endif /* defined(_LP64) && !defined(_KERNEL) */
#endif /* _LARGEFILE64_SOURCE */

#define	AIOFSYNC	20

#define	AIO_POLL_BIT	0x20	/* opcode filter for AIO_INPROGRESS */
#ifdef	__cplusplus
}
#endif

#endif /* _SYS_AIO_H */
