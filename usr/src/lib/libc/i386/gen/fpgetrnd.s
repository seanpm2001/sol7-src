/ fpgetrnd() returns the 80[23]87 coprocessor's current rounding mode.

	.file	"fpgetrnd.s"
	.ident	"@(#)fpgetrnd.s	1.1	92/04/17 SMI"
	.text
	.set	FPRC, 0xC00	/ FPRC (rounding control) from <sys/fp.h>
	.globl	fpgetrnd
	.align	4
_fgdef_(fpgetrnd):
	pushl	%ecx
	fstcw	0(%esp)
	movl	0(%esp),%eax
	andl	$FPRC,%eax
	popl	%ecx
	ret	
	.align	4
	.size	fpgetrnd,.-fpgetrnd
