#ident "@(#)sigwait.s 1.1 96/12/04"

	.file "sigwait.s"

#include <sys/asm_linkage.h>

#include	"SYS.h"

#define NULLP	0

	ENTRY(_libc_sigwait)
	mov	NULLP, %o1
	mov	NULLP, %o2
	SYSTRAP(sigtimedwait)
	SYSCERROR
	RET

	SET_SIZE(_libc_sigwait)
