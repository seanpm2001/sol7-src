#ident "@(#)signotifywait.s	1.1	96/12/04"

	.file "signotifywait.s"

#include <sys/asm_linkage.h>

	ANSI_PRAGMA_WEAK(_signotifywait,function)

#include	"SYS.h"

	ENTRY(_signotifywait)
	SYSTRAP(signotifywait)
	SYSCERROR
	RET
	SET_SIZE(_signotifywait)
