#pragma ident	"@(#)getw.c	1.10	92/07/20 SMI"  /* from S5R2 1.3 */

/*LINTLIBRARY*/
/*
 * The intent here is to provide a means to make the order of
 * bytes in an io-stream correspond to the order of the bytes
 * in the memory while doing the io a `word' at a time.
 */
#include <stdio.h>

int
getw(stream)
register FILE *stream;
{
	int w;
	register char *s = (char *)&w;
	register int i = sizeof(int);

	while (--i >= 0)
		*s++ = getc(stream);
	return (feof(stream) || ferror(stream) ? EOF : w);
}
