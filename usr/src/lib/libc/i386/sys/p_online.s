	.ident	"@(#)p_online.s	1.1	97/07/28 SMI"


	.file	"p_online.s"

	.text

	.globl	__cerror

_fwdef_(`p_online'):
	MCOUNT			/ subroutine entry counter if profiling
	movl	$P_ONLINE,%eax
	lcall   $SYSCALL_TRAPNUM,$0
	jae	noerror
	_prologue_
_m4_ifdef_(`DSHLIB',
	`pushl	%eax',
	`'
)
	jmp	_fref_(__cerror)
noerror:
	ret
