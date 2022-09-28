/*
 * Copyright (c) 1996-1998, by Sun Microsystems, Inc.
 * All Rights reserved.
 */
/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#pragma ident	"@(#)synonyms.h	1.69	98/01/30 SMI"

#ifndef _ASM
extern int		__threaded;
#endif

/* external data */
#define	altzone		_altzone
#define	daylight	_daylight
#define	lone		_lone
#define	lten		_lten
#define	lzero		_lzero
#define	timezone	_timezone
#define	tzname		_tzname

/* functions */
#define	Msgdb		_Msgdb
#define	a64l		_a64l
#define	access		_access
#define	acct		_acct
#define	acl		_acl
#define	addseverity	_addseverity
#define	adjtime		_adjtime
#define	alarm		_alarm
#define	ascftime	_ascftime
#define	asctime_r	_asctime_r
#define	basename	_basename
#define	brk		_brk
#define	brkbase		_brkbase
#define	bsd_signal	_bsd_signal
#define	catclose	_catclose
#define	catgets		_catgets
#define	catopen		_catopen
#define	cfgetispeed	_cfgetispeed
#define	cfgetospeed	_cfgetospeed
#define	cfree		_cfree
#define	cfsetispeed	_cfsetispeed
#define	cfsetospeed	_cfsetospeed
#define	cftime		_cftime
#define	ctime_r		_ctime_r
#define	chdir		_chdir
#define	chmod		_chmod
#define	chown		_chown
#define	chroot		_chroot
#define	close		_close
#define	closedir	_closedir
#define	closelog	_closelog
#define	countbase	_countbase
#define	creat		_creat
#define	crypt		_crypt
#define	ctermid		_ctermid
#define	cuserid		_cuserid
#define	dial		_dial
#define	dirname		_dirname
#define	drand48		_drand48
#define	dup		_dup
#define	dup2		_dup2
#define	ecvt		_ecvt
#define	edata		_edata
#define	encrypt		_encrypt
#define	end		_end
#define	endgrent	_endgrent
#define	endpwent	_endpwent
#define	endspent	_endspent
#define	endutent	_endutent
#define	endutxent	_endutxent
#define	environ		_environ
#define	erand48		_erand48
#define	etext		_etext
#define	execl		_execl
#define	execle		_execle
#define	execlp		_execlp
#define	execv		_execv
#define	execve		_execve
#define	execvp		_execvp
#define	exportfs	_exportfs
#define	facl		_facl
#define	fattach		_fattach
#define	fchdir		_fchdir
#define	fchmod		_fchmod
#define	fchown		_fchown
#define	fchroot		_fchroot
#define	fcntl		_fcntl
#define	fcvt		_fcvt
#define	fdetach		_fdetach
#define	fdopen		_fdopen
#define	ffs		_ffs
#define	fgetgrent	_fgetgrent
#define	fgetgrent_r	_fgetgrent_r
#define	fgetpwent	_fgetpwent
#define	fgetpwent_r	_fgetpwent_r
#define	fgetspent	_fgetspent
#define	fgetspent_r	_fgetspent_r
#define	finite		_finite
#define	fmtmsg		_fmtmsg
#define	fork		_fork
#define	fork1		_fork1
#define	fpathconf	_fpathconf
#define	fpclass		_fpclass
#define	fpgetmask	_fpgetmask
#define	fpgetround	_fpgetround
#define	fpgetsticky	_fpgetsticky
#define	fprintf		_fprintf
#define	fpsetmask	_fpsetmask
#define	fpsetround	_fpsetround
#define	fpsetsticky	_fpsetsticky
#define	fstat		_fstat
#define	fstatfs		_fstatfs
#define	fstatvfs	_fstatvfs
#define	fsync		_fsync
#define	ftell		_ftell
#define	ftello		_ftello
#define	fseek		_fseek
#define	fseeko		_fseeko
#define	ftok		_ftok
#define	ftruncate	_ftruncate
#define	gcvt		_gcvt
#define	getcontext	_getcontext
#define	getcwd		_getcwd
#define	getdate		_getdate
#if !defined(_REENTRANT)
#define	getdate_err	_getdate_err
#else	/* defined(_REENTRANT) */
#define	_getdate_err	getdate_err
#endif	/* !defined(_REENTRANT) */
#define	getdents	_getdents
#define	getegid		_getegid
#define	geteuid		_geteuid
#define	getexecname	_getexecname
#define	getgid		_getgid
#define	getloadavg	_getloadavg
#define	getgrent	_getgrent
#define	getgrent_r	_getgrent_r
#define	getgrgid	_getgrgid
#define	getgrgid_r	_getgrgid_r
#define	getgrnam	_getgrnam
#define	getgrnam_r	_getgrnam_r
#define	getgroups	_getgroups
#define	gethz		_gethz
#define	getitimer	_getitimer
#define	getlogin	_getlogin
#define	getmntany	_getmntany
#define	getmntent	_getmntent
#define	getmsg		_getmsg
#define	getopt		_getopt
#define	getpass		_getpass
#define	getpassphrase	_getpassphrase
#define	getpgid		_getpgid
#define	getpgrp		_getpgrp
#define	getpid		_getpid
#define	getpmsg		_getpmsg
#define	getppid		_getppid
#define	getpw		_getpw
#define	getpwent	_getpwent
#define	getpwent_r	_getpwent_r
#define	getpwnam	_getpwnam
#define	getpwnam_r	_getpwnam_r
#define	getpwuid	_getpwuid
#define	getpwuid_r	_getpwuid_r
#define	getrlimit	_getrlimit
#define	getsid		_getsid
#define	getspent	_getspent
#define	getspent_r	_getspent_r
#define	getspnam	_getspnam
#define	getspnam_r	_getspnam_r
#define	getsubopt	_getsubopt
#define	gettimeofday	_gettimeofday
#define	gettxt		_gettxt
#define	getuid		_getuid
#define	getutent	_getutent
#define	getutid		_getutid
#define	getutline	_getutline
#define	getutmp		_getutmp
#define	getutmpx	_getutmpx
#define	getutxent	_getutxent
#define	getutxid	_getutxid
#define	getutxline	_getutxline
#define	getvfsany	_getvfsany
#define	getvfsent	_getvfsent
#define	getvfsfile	_getvfsfile
#define	getvfsspec	_getvfsspec
#define	getw		_getw
#define	grantpt		_grantpt
#define	gsignal		_gsignal
#define	gtty		_gtty
#define	hasmntopt	_hasmntopt
#define	hcreate		_hcreate
#define	hdestroy	_hdestroy
#define	hsearch		_hsearch
#define	initgroups	_initgroups
#define	insque		_insque
#define	install_utrap	_install_utrap
#define	ioctl		_ioctl
#define	isaexec		_isaexec
#define	isastream	_isastream
#define	isatty		_isatty
#define	isnan		_isnan
#define	isnand		_isnand
#define	isnanf		_isnanf
#define	jrand48		_jrand48
#define	kaio		_kaio
#define	kill		_kill
#define	l64a		_l64a
#define	ladd		_ladd
#define	lchown		_lchown
#define	lckpwdf		_lckpwdf
#define	lcong48		_lcong48
#define	ldivide		_ldivide
#define	lexp10		_lexp10
#define	lfind		_lfind
#define	link		_link
#define	llog10		_llog10
#define	llseek		_llseek
#define	lmul		_lmul
#define	lockf		_lockf
#define	localtime_r	_localtime_r
#define	logb		_logb
#define	longjmp		_longjmp
#define	lrand48		_lrand48
#define	lsearch		_lsearch
#define	lseek		_lseek
#define	lshiftl		_lshiftl
#define	lsign		_lsign
#define	lstat		_lstat
#define	lsub		_lsub
#define	ltol3		_ltol3
#define	madvise		_madvise
#define	makecontext	_makecontext
#define	makeut		_makeut
#define	makeutx		_makeutx
#define	memalign	_memalign
#define	memccpy		_memccpy
#define	memcntl		_memcntl
#define	memcmp		_memcmp
#define	memcpy		_memcpy
#define	memmove		_memmove
#define	memset		_memset
#define	mincore		_mincore
#define	mkdir		_mkdir
#define	mkfifo		_mkfifo
#define	mknod		_mknod
#define	mktemp		_mktemp
#define	mkstemp		_mkstemp
#define	mlock		_mlock
#define	mlockall	_mlockall
#define	mmap		_mmap
#define	modf		_modf
#define	modff		_modff
#define	modut		_modut
#define	modutx		_modutx
#define	monitor		_monitor
#define	mount		_mount
#define	mprotect	_mprotect
#define	mrand48		_mrand48
#define	msgctl		_msgctl
#define	msgget		_msgget
#define	msgrcv		_msgrcv
#define	msgsnd		_msgsnd
#define	msync		_msync
#define	munlock		_munlock
#define	munlockall	_munlockall
#define	munmap		_munmap
#define	nextafter	_nextafter
#define	nfs_getfh	_nfs_getfh
#define	nfssvc		_nfssvc
#define	nftw		_nftw
#define	nice		_nice
#define	nrand48		_nrand48
#define	nss_delete	_nss_delete
#define	nss_endent	_nss_endent
#define	nss_getent	_nss_getent
#define	nss_search	_nss_search
#define	nss_setent	_nss_setent
#define	ntp_gettime	_ntp_gettime
#define	ntp_adjtime	_ntp_adjtime
#define	open		_open
#define	opendir		_opendir
#define	openlog		_openlog
#define	pathconf	_pathconf
#define	pause		_pause
#define	pclose		_pclose
#define	pcsample	_pcsample
#define	pipe		_pipe
#define	plock		_plock
#define	poll		_poll
#define	popen		_popen
#define	pread		_pread
#define	pset_create	_pset_create
#define	pset_destroy	_pset_destroy
#define	pset_assign	_pset_assign
#define	pset_info	_pset_info
#define	pset_bind	_pset_bind
#define	profil		_profil
#define	psiginfo	_psiginfo
#define	psignal		_psignal
#define	ptrace		_ptrace
#define	ptsname		_ptsname
#define	pwrite		_pwrite
#define	putenv		_putenv
#define	putmsg		_putmsg
#define	putpmsg		_putpmsg
#define	putpwent	_putpwent
#define	putspent	_putspent
#define	pututline	_pututline
#define	pututxline	_pututxline
#define	putw		_putw
#define	read		_read
#define	readdir		_readdir
#define	readdir_r	_readdir_r
#define	readlink	_readlink
#define	readv		_readv
#define	realpath	_realpath
#define	regcmp		_regcmp
#define	regcomp		_regcomp
#define	regerror	_regerror
#define	regex		_regex
#define	regexec		_regexec
#define	regfree		_regfree
#define	remque		_remque
#define	resolvepath	_resolvepath
#define	rmdir		_rmdir
#define	sbrk		_sbrk
#define	scalb		_scalb
#define	scrwidth	_scrwidth
#define	seed48		_seed48
#define	seekdir		_seekdir
#define	select		_select
#define	semctl		_semctl
#define	semget		_semget
#define	semop		_semop
#define	setcontext	_setcontext
#define	setegid		_setegid
#define	seteuid		_seteuid
#define	setgid		_setgid
#define	setgrent	_setgrent
#define	setgroups	_setgroups
#define	setitimer	_setitimer
#define	setjmp		_setjmp
#define	setkey		_setkey
#define	setlocale	_setlocale
#define	setlogmask	_setlogmask
#define	setpgid		_setpgid
#define	setpgrp		_setpgrp
#define	setpwent	_setpwent
#define	setregid	_setregid
#define	setreuid	_setreuid
#define	setrlimit	_setrlimit
#define	setsid		_setsid
#define	setspent	_setspent
#define	settimeofday	_settimeofday
#define	setuid		_setuid
#define	setutent	_setutent
#define	setutxent	_setutxent
#define	shmat		_shmat
#define	shmctl		_shmctl
#define	shmdt		_shmdt
#define	shmget		_shmget
#define	sig2str		_sig2str
#define	sigaction	_sigaction
#define	sigaddset	_sigaddset
#define	sigaltstack	_sigaltstack
#define	sigdelset	_sigdelset
#define	sigemptyset	_sigemptyset
#define	sigfillset	_sigfillset
#define	sigflag		_sigflag
#define	sigfpe		_sigfpe
#define	sighold		_sighold
#define	sigignore	_sigignore
#define	siginterrupt	_siginterrupt
#define	sigismember	_sigismember
#define	siglongjmp	_siglongjmp
#define	sigpause	_sigpause
#define	sigpending	_sigpending
#define	sigprocmask	_sigprocmask
#define	sigrelse	_sigrelse
#define	sigsend		_sigsend
#define	sigsendset	_sigsendset
#define	sigset		_sigset
#define	signal		_signal
#define	sigsetjmp	_sigsetjmp
#define	sigsuspend	_sigsuspend
#define	sigwait		_sigwait
#define	sleep		_sleep
#define	srand48		_srand48
#define	ssignal		_ssignal
#define	stat		_stat
#define	statfs		_statfs
#define	statvfs		_statvfs
#define	stime		_stime
#define	str2sig		_str2sig
#define	strdup		_strdup
#define	stty		_stty
#define	swab		_swab
#define	swapcontext	_swapcontext
#define	swapctl		_swapctl
#define	symlink		_symlink
#define	sync		_sync
#define	synchutmp	_synchutmp
#define	syssun		_syssun
#define	sys_errlist	_sys_errlist
#define	sys_nerr	_sys_nerr
#define	syscall		_syscall
#define	sysconf		_sysconf
#define	sysfs		_sysfs
#define	sysinfo		_sysinfo
#define	syslog		_syslog
#define	tcdrain		_tcdrain
#define	tcflow		_tcflow
#define	tcflush		_tcflush
#define	tcgetattr	_tcgetattr
#define	tcgetpgrp	_tcgetpgrp
#define	tcgetsid	_tcgetsid
#define	tcsendbreak	_tcsendbreak
#define	tcsetattr	_tcsetattr
#define	tcsetpgrp	_tcsetpgrp
#define	tdelete		_tdelete
#define	tell		_tell
#define	telldir		_telldir
#define	tempnam		_tempnam
#define	tfind		_tfind
#define	time		_time
#define	times		_times
#define	truncate	_truncate
#define	tsearch		_tsearch
#define	ttyname		_ttyname
#define	ttyslot		_ttyslot
#define	twalk		_twalk
#define	tzname		_tzname
#define	tzset		_tzset
#define	uadmin		_uadmin
#define	ulckpwdf	_ulckpwdf
#define	ulimit		_ulimit
#define	umask		_umask
#define	umount		_umount
#define	uname		_uname
#define	undial		_undial
#define	unlink		_unlink
#define	unlockpt	_unlockpt
#define	unordered	_unordered
#define	updutfile	_updutfile
#define	updutxfile	_updutxfile
#define	updutmp		_updutmp
#define	updutmpx	_updutmpx
#define	updwtmp		_updwtmp
#define	updwtmpx	_updwtmpx
#define	ustat		_ustat
#define	utime		_utime
#define	utimes		_utimes
#define	utmpname	_utmpname
#define	utmpxname	_utmpxname
#define	utssys		_utssys
#define	valloc		_valloc
#define	vfork		_vfork
#define	vhangup		_vhangup
#define	vsyslog		_vsyslog
#define	wait		_wait
#define	waitid		_waitid
#define	waitpid		_waitpid
#define	wcwidth		_wcwidth
#define	wcswidth	_wcswidth
#define	write		_write
#define	writev		_writev
#define	yield		_yield
#define	_assert		__assert
#define	_ctype		__ctype
#define	_door_create	__door_create
#define	_door_revoke	__door_revoke
#define	_door_info	__door_info
#define	_door_call	__door_call
#define	_door_return	__door_return
#define	_door_cred	__door_cred
#define	_door_bind	__door_bind
#define	_door_unbind	__door_unbind
#define	_filbuf		__filbuf
#define	_flsbuf		__flsbuf
#define	_iob		__iob
#define	_lwp_info	__lwp_info
#define	_lwp_create	__lwp_create
#define	_lwp_self	__lwp_self
#define	_lwp_kill	__lwp_kill
#define	_lwp_wait	__lwp_wait
#define	_lwp_suspend	__lwp_suspend
#define	_lwp_suspend2	__lwp_suspend2
#define	_lwp_continue	__lwp_continue
#define	_lwp_exit	__lwp_exit
#define	_lwp_mutex_lock	__lwp_mutex_lock
#define	_lwp_mutex_trylock	__lwp_mutex_trylock
#define	_lwp_mutex_unlock	__lwp_mutex_unlock
#define	_lwp_cond_wait		__lwp_cond_wait
#define	_lwp_cond_timedwait	__lwp_cond_timedwait
#define	_lwp_cond_signal	__lwp_cond_signal
#define	_lwp_cond_broadcast	__lwp_cond_broadcast
#define	_lwp_makecontext	__lwp_makecontext
#define	_lwp_getprivate		__lwp_getprivate
#define	_lwp_setprivate		__lwp_setprivate
#define	_lwp_sema_wait		__lwp_sema_wait
#define	_lwp_sema_trywait	__lwp_sema_trywait
#define	_lwp_sema_post		__lwp_sema_post
#define	_lwp_sema_init		__lwp_sema_init
#define	_lwp_sigredirect	__lwp_sigredirect
#define	_signotifywait		__signotifywait
#define	_lwp_schedctl		__lwp_schedctl

/* fp and ll functions */
#define	decimal_to_double	_decimal_to_double
#define	decimal_to_extended	_decimal_to_extended
#define	decimal_to_quadruple	_decimal_to_quadruple
#define	decimal_to_single	_decimal_to_single
#define	double_to_decimal	_double_to_decimal
#define	econvert		_econvert
#define	extended_to_decimal	_extended_to_decimal
#define	fconvert		_fconvert
#define	file_to_decimal		_file_to_decimal
#define	func_to_decimal		_func_to_decimal
#define	gconvert		_gconvert
#define	qeconvert		_qeconvert
#define	qecvt			_qecvt
#define	qfconvert		_qfconvert
#define	qfcvt			_qfcvt
#define	qgconvert		_qgconvert
#define	qgcvt			_qgcvt
#define	quadruple_to_decimal	_quadruple_to_decimal
#define	seconvert		_seconvert
#define	sfconvert		_sfconvert
#define	sgconvert		_sgconvert
#define	single_to_decimal	_single_to_decimal
#define	string_to_decimal	_string_to_decimal
#define	atoll			_atoll
#define	llabs			_llabs
#define	lldiv			_lldiv
#define	lltostr			_lltostr
#define	strtoll			_strtoll
#define	strtoull		_strtoull
#define	ulltostr		_ulltostr

/* transitional large file interfaces */
#define	creat64		_creat64
#define	ftello64	_ftello64
#define	fseeko64	_fseeko64
#define	fstat64		_fstat64
#define	ftw64		_ftw64
#define	fstatvfs64	_fstatvfs64
#define	ftruncate64	_ftruncate64
#define	getdents64	_getdents64
#define	getrlimit64	_getrlimit64
#define	lockf64		_lockf64
#define	lseek64		_lseek64
#define	lstat64		_lstat64
#define	mkstemp64	_mkstemp64
#define	mmap64		_mmap64
#define	nftw64		_nftw64
#define	open64		_open64
#define	pread64		_pread64
#define	pwrite64	_pwrite64
#define	readdir64	_readdir64
#define	readdir64_r	_readdir64_r
#define	seekdir64	_seekdir64
#define	tell64		_tell64
#define	telldir64	_telldir64
#define	setrlimit64	_setrlimit64
#define	stat64		_stat64
#define	statvfs64	_statvfs64
#define	truncate64	_truncate64

/* names that need to be hidden in the min-ABI library */
#if defined(ABI)

#include <sys/feature_tests.h>

#if defined(_LP64) || \
	(_FILE_OFFSET_BITS  != 64 && !defined(_LARGEFILE64_SOURCE))
#define	abs		_abi_abs
#define	atoi		_abi_atoi
#define	atol		_abi_atol
#define	difftime	_abi_difftime
#undef endgrent
#define	endgrent	_abi_endgrent
#undef endpwent
#define	endpwent	_abi_endpwent
#undef fgetgrent
#define	fgetgrent	_abi_fgetgrent
#undef fgetpwent
#define	fgetpwent	_abi_fgetpwent
#undef ftruncate
#define	ftruncate	_abi_ftruncate
#undef getdents
#define	getdents	_abi_getdents
#undef getgrent
#define	getgrent	_abi_getgrent
#undef getpwent
#define	getpwent	_abi_getpwent
#define	getenv		_abi_getenv
#define	gmtime		_abi_gmtime
#undef isatty
#define	isatty		_abi_isatty
#define	labs		_abi_labs
#define	localtime	_abi_localtime
#define	memchr		_abi_memchr
#undef memcmp
#define	memcmp		_abi_memcmp
#undef memcpy
#define	memcpy		_abi_memcpy
#undef memmove
#define	memmove		_abi_memmove
#undef memset
#define	memset		_abi_memset
#define	mktime		_abi_mktime
#undef setgrent
#define	setgrent	_abi_setgrent
#undef setpwent
#define	setpwent	_abi_setpwent
#undef sleep
#define	sleep		_abi_sleep
#define	strcat		_abi_strcat
#define	strchr		_abi_strchr
#define	strcmp		_abi_strcmp
#define	strcpy		_abi_strcpy
#define	strcspn		_abi_strcspn
#define	strlen		_abi_strlen
#define	strncmp		_abi_strncmp
#define	strncpy		_abi_strncpy
#define	strtol		_abi_strtol
#undef syscall
#define	syscall		_abi_syscall
#undef sysi86
#define	sysi86		_abi_sysi86
#undef truncate
#define	truncate	_abi_truncate
#undef ttyslot
#define	ttyslot		_abi_ttyslot
#undef tzset
#define	tzset		_abi_tzset

#else	/* _LP64 || (_FILE_OFFSET_BITS  != 64 && !_LARGEFILE64_SOURCE) */

#undef ftruncate64
#define	ftruncate64	_abi_ftruncate64
#undef getdents64
#define	getdents64	_abi_getdents64
#undef truncate64
#define	truncate64	_abi_truncate64

#endif /* _LP64 || (_FILE_OFFSET_BITS  != 64 && !_LARGEFILE64_SOURCE) */

#else /* !defined(ABI) */

/* names that need to be hidden in the shared library */
#if defined(DSHLIB) && (defined(i386) || defined(__i386))

#include <sys/feature_tests.h>

#if defined(_LP64) || \
	(_FILE_OFFSET_BITS != 64 && !defined(_LARGEFILE64_SOURCE))

#undef closelog
#define	closelog	_abi_closelog
#undef ecvt
#define	ecvt		_abi_ecvt
#undef endgrent
#define	endgrent	_abi_endgrent
#undef endpwent
#define	endpwent	_abi_endpwent
#undef fcvt
#define	fcvt		_abi_fcvt
#undef fgetgrent
#define	fgetgrent	_abi_fgetgrent
#undef fgetpwent
#define	fgetpwent	_abi_fgetpwent
#undef ftruncate
#define	ftruncate	_abi_ftruncate
#undef getdents
#define	getdents	_abi_getdents
#undef getgrent
#define	getgrent	_abi_getgrent
#undef gethz
#define	gethz		_abi_gethz
#undef gettimeofday
#define	gettimeofday	_abi_gettimeofday
#undef getpwent
#define	getpwent	_abi_getpwent
#undef openlog
#define	openlog		_abi_openlog
#undef select
#define	select		_abi_select
#undef setgrent
#define	setgrent	_abi_setgrent
#undef setlogmask
#define	setlogmask	_abi_setlogmask
#undef settimeofday
#define	settimeofday	_abi_settimeofday
#undef setpwent
#define	setpwent	_abi_setpwent
#undef syscall
#define	syscall		_abi_syscall
#undef sysi86
#define	sysi86		_abi_sysi86
#undef syslog
#define	syslog		_abi_syslog
#undef truncate
#define	truncate	_abi_truncate
#undef ttyslot
#define	ttyslot		_abi_ttyslot
#undef vfork
#define	vfork		_abi_vfork
#undef vsyslog
#define	vsyslog		_abi_vsyslog

#else /* _LP64 || (_FILE_OFFSET_BITS != 64 && !_LARGEFILE64_SOURCE) */

#undef	ftruncate64
#define	ftruncate64	_abi_ftruncate64
#undef	truncate64
#define	truncate64	_abi_truncate64

#endif /* _LP64 || (_FILE_OFFSET_BITS != 64 && !_LARGEFILE64_SOURCE) */

#endif /* DSHLIB && (i386 || __i386) */

#endif /* defined(ABI) */
