/*	Copyright (c) 1984, 1986, 1987, 1988, 1989 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*******************************************************************

		PROPRIETARY NOTICE (Combined)

This source code is unpublished proprietary information
constituting, or derived under license from AT&T's UNIX(r) System V.
In addition, portions of such source code were derived from Berkeley
4.3 BSD under license from the Regents of the University of
California.



		Copyright Notice 

Notice of copyright on this source code product does not indicate 
publication.

	(c) 1986,1987,1988,1989,1997  Sun Microsystems, Inc
	(c) 1983,1984,1985,1986,1987,1988,1989  AT&T.
	          All rights reserved.
********************************************************************/ 

#ident	"@(#)stdio.h	1.9	97/11/22 SMI"	/* SVr4.0 1.8	*/

/*
* User-visible pieces of the ANSI C standard I/O package.
*/
#ifndef _STDIO_H /* if not defined then stdio.h has not yet been included */
#define _STDIO_H

#include <sys/va_list.h>	/* Required because not all architectures */
				/* have va_list defined as a "void *".    */

#ifndef BSD
#define BSD
#endif


#ifndef _SIZE_T
#define _SIZE_T
typedef unsigned int 	size_t;
#endif 

typedef long	fpos_t;

#ifndef NULL
#define NULL            0
#endif 

#if defined(__STDC__)

#if #machine(pdp11)
#   define BUFSIZ		512
#   define _STDIO_REVERSE
#elif #machine(u370)
#   define BUFSIZ		4096
#   define _STDIO_REVERSE
#   define _STDIO_ALLOCATE
#else
#   define BUFSIZ		1024
#endif

#else	/* !defined(__STDC__) */

#if pdp11 || u370

#if pdp11
#   define BUFSIZ		512
#   define _STDIO_REVERSE
#else 	/* u370 */
#   define BUFSIZ		4096
#   define _STDIO_REVERSE
#   define _STDIO_ALLOCATE
#endif

#else
#   define BUFSIZ		1024
#endif

#endif	/* __STDC__ */

#if defined(i386) || defined(__i386)
#define _NFILE	60	/* initial number of streams */
#elif defined(sparc) || defined(__sparc)
#define _NFILE	20	/* initial number of streams */
#else
#error Unknown architecture!
#endif
#define _SBFSIZ	8	/* compatibility with shared libs */

#define _IOFBF		0000	/* full buffered */
#define _IOLBF		0100	/* line buffered */
#define _IONBF		0004	/* not buffered */
#define _IOEOF		0020	/* EOF reached on read */
#define _IOERR		0040	/* I/O error from system */

#define _IOREAD		0001	/* currently reading */
#define _IOWRT		0002	/* currently writing */
#define _IORW		0200	/* opened for reading and writing */
#define _IOMYBUF	0010	/* stdio malloc()'d buffer */

#ifndef EOF
#   define EOF	(-1)
#endif

#define FOPEN_MAX	_NFILE
#define FILENAME_MAX    1024	/* max # of characters in a path name */

#define SEEK_SET	0
#define SEEK_CUR	1
#define SEEK_END	2
#define TMP_MAX		17576	/* 26 * 26 * 26 */

#if __STDC__ - 0 == 0 || defined(_POSIX_SOURCE) || defined(_XOPEN_SOURCE)
#define L_ctermid	9
#define L_cuserid	9
#define P_tmpdir	"/var/tmp/"
#endif

#define L_tmpnam	25	/* (sizeof(P_tmpdir) + 15) */

#if defined(__STDC__)
#define stdin	(&__iob[0])
#define stdout	(&__iob[1])
#define stderr	(&__iob[2])
#else
#define stdin	(&_iob[0])
#define stdout	(&_iob[1])
#define stderr	(&_iob[2])
#endif

typedef struct	/* needs to be binary-compatible with old versions */
{
#ifdef _STDIO_REVERSE
	unsigned char	*_ptr;	/* next character from/to here in buffer */
	int		_cnt;	/* number of available characters in buffer */
#else
	int		_cnt;	/* number of available characters in buffer */
	unsigned char	*_ptr;	/* next character from/to here in buffer */
#endif
	unsigned char	*_base;	/* the buffer */
	unsigned char	_flag;	/* the state of the stream */
	unsigned char	_file;	/* UNIX System file descriptor */
} FILE;

#if defined(__STDC__)
extern FILE		__iob[_NFILE];
#else
extern FILE		_iob[_NFILE];
#endif
extern FILE		*_lastbuf;
extern unsigned char 	*_bufendtab[];
#ifndef _STDIO_ALLOCATE
extern unsigned char	 _sibuf[], _sobuf[];
#endif

/* Large file interfaces */
#if (_FILE_OFFSET_BITS == 64)
#ifdef __PRAGMA_REDEFINE_EXTNAME
#pragma redefine_extname	fopen		fopen64
#pragma redefine_extname	freopen		freopen64
#else
#define	fopen			fopen64
#define	freopen			freopen64
#endif
#endif

#if defined(__STDC__)

extern int	remove(const char *);
extern int	rename(const char *, const char *);
extern int	fclose(FILE *);
extern int	fflush(FILE *);
extern FILE	*fopen(const char *, const char *);
extern FILE	*freopen(const char *, const char *, FILE *);
extern void	setbuf(FILE *, char *);
extern int	setvbuf(FILE *, char *, int, size_t);
/* PRINTFLIKE2 */
extern int	fprintf(FILE *, const char *, ...);
/* SCANFLIKE2 */
extern int	fscanf(FILE *, const char *, ...);
/* PRINTFLIKE1 */
extern int	printf(const char *, ...);
/* SCANFLIKE1 */
extern int	scanf(const char *, ...);
/* PRINTFLIKE2 */
extern char	*sprintf(const char *, const char *, ...);
/* SCANFLIKE2 */
extern int	sscanf(const char *, const char *, ...);
extern int	vfprintf(FILE *, const char *, __va_list);
extern int	vprintf(const char *, __va_list);
extern char	*vsprintf(char *, char *, __va_list);
extern int	fgetc(FILE *);
extern char	*fgets(char *, int, FILE *); 
extern int	fputc(int, FILE *);
extern int	fputs(const char *, FILE *);
extern int	getc(FILE *);
extern int	getchar(void);
extern char	*gets(char *);
extern int	putc(int, FILE *);
extern int	putchar(int);
extern int	puts(const char *);
extern int	ungetc(int, FILE *);
extern size_t	fread(void *, size_t, size_t, FILE *);
	#pragma int_to_unsigned fread
extern size_t	fwrite(const void *, size_t, size_t, FILE *);
	#pragma int_to_unsigned fwrite
extern int	fgetpos(FILE *, fpos_t *);
extern int	fseek(FILE *, long, int);
extern int	fsetpos(FILE *, const fpos_t *);
extern long	ftell(FILE *);
extern void	rewind(FILE *);
extern void	clearerr(FILE *);
extern int	feof(FILE *);
extern int	ferror(FILE *);
extern void	perror(const char *);

extern int	__filbuf(FILE *);
extern int	__flsbuf(int, FILE *);

#if !#lint(on)
#define getc(p)		(--(p)->_cnt < 0 ? __filbuf(p) : (int)*(p)->_ptr++)
#define putc(x, p)	(--(p)->_cnt < 0 ? __flsbuf((x), (p)) \
				: (int)(*(p)->_ptr++ = (x)))
#define getchar()	getc(stdin)
#define putchar(x)	putc((x), stdout)
#define clearerr(p)	((void)((p)->_flag &= ~(_IOERR | _IOEOF)))
#define feof(p)		((p)->_flag & _IOEOF)
#define ferror(p)	((p)->_flag & _IOERR)
#endif

#if __STDC__ == 0 || defined(_POSIX_SOURCE) || defined(_XOPEN_SOURCE) /* non-ANSI standard compilation */

extern FILE    *fdopen(int, const char *);
extern FILE    *popen(const char *, const char *);
extern char    *ctermid(char *);
extern char    *cuserid(char *);
extern char    *tempnam(const char *, const char *);
extern int     getw(FILE *);
extern int     putw(int, FILE *);
extern int     pclose(FILE *);
extern int     system(const char *);
extern int	fileno(FILE *);

#if !#lint(on)
#define fileno(p)	(p)->_file
#endif

#endif	/* __STDC__ == 0 */

#else	/* !defined __STDC__ */
#define _bufend(p)      _bufendtab[(p)->_file]
#define _bufsiz(p)      (_bufend(p) - (p)->_base)

#ifndef lint
#define getc(p)         (--(p)->_cnt < 0 ? _filbuf(p) : (int) *(p)->_ptr++)
#define putc(x, p)      (--(p)->_cnt < 0 ? \
                        _flsbuf((unsigned char) (x), (p)) : \
                        (int) (*(p)->_ptr++ = (unsigned char) (x)))
#define getchar()       getc(stdin)
#define putchar(x)      putc((x), stdout)
#define clearerr(p)     ((void) ((p)->_flag &= ~(_IOERR | _IOEOF)))
#define feof(p)         ((p)->_flag & _IOEOF)
#define ferror(p)       ((p)->_flag & _IOERR)
#define fileno(p)       (p)->_file
#endif	/* lint */

extern FILE     *fopen(), *fdopen(), *freopen(), *popen();
extern long     ftell();
extern void     rewind(), setbuf();
extern char     *ctermid(), *cuserid(), *fgets(), *gets(), *sprintf(),
		*vsprintf();
extern int      fclose(), fflush(), fread(), fwrite(), fseek(), fgetc(),
                getw(), pclose(), printf(), fprintf(),
                vprintf(), vfprintf(), fputc(), putw(),
                puts(), fputs(), scanf(), fscanf(), sscanf(),
                setvbuf(), system(), ungetc();

#endif	/* __STDC__ */

#if	defined(_LARGEFILE64_SOURCE) && !((_FILE_OFFSET_BITS == 64) && \
	    !defined(__PRAGMA_REDEFINE_EXTNAME))
#if defined(__STDC__)
extern FILE	*fopen64(const char *, const char *);
extern FILE	*freopen64(const char *, const char *, FILE *);
#else
extern FILE     *fopen64(), *freopen64();
#endif
#endif	/* _LARGEFILE64_SOURCE... */

#endif  /* _STDIO_H */
