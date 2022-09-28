/* Copyright 08/05/96 Sun Microsystems, Inc. All Rights Reserved.
*/
#pragma ident  "@(#)mi_indicate_svc.c	1.1 96/08/05 Sun Microsystems"

/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "mi_indicate.h"
#include <stdio.h>
#include <stdlib.h> /* getenv, exit */
#include <rpc/pmap_clnt.h> /* for pmap_unset */
#include <string.h> /* strcmp */
#include <signal.h>
#ifdef __cplusplus
#include <sysent.h> /* getdtablesize, open */
#endif /* __cplusplus */
#include <unistd.h> /* setsid */
#include <sys/types.h>
#include <memory.h>
#include <stropts.h>
#include <netconfig.h>
#include <sys/resource.h> /* rlimit */
#include <syslog.h>

#ifndef SIG_PF
#define	SIG_PF void(*)(int)
#endif

#ifdef DEBUG
#define	RPC_SVC_FG
#endif

#define	_RPCSVC_CLOSEDOWN 120
static int _rpcpmstart;		/* Started by a port monitor ? */

/* States a server can be in wrt request */

#define	_IDLE 0
#define	_SERVED 1

static int _rpcsvcstate = _IDLE;	/* Set when a request is serviced */
static int _rpcsvccount = 0;		/* Number of requests being serviced */

void _msgout(char* msg)
{
#ifdef RPC_SVC_FG
	if (_rpcpmstart)
		syslog(LOG_ERR, msg);
	else
		(void) fprintf(stderr, "%s\n", msg);
#else
	syslog(LOG_ERR, msg);
#endif
}

#if 0
void
closedown(int sig)
{
	if (_rpcsvcstate == _IDLE && _rpcsvccount == 0) {
		extern fd_set svc_fdset;
		static int size;
		int i, openfd;
		struct t_info tinfo;

		if (!t_getinfo(0, &tinfo) && (tinfo.servtype == T_CLTS))
			exit(0);
		if (size == 0) {
			struct rlimit rl;

			rl.rlim_max = 0;
			getrlimit(RLIMIT_NOFILE, &rl);
			if ((size = rl.rlim_max) == 0) {
				return;
			}
		}
		for (i = 0, openfd = 0; i < size && openfd < 2; i++)
			if (FD_ISSET(i, &svc_fdset))
				openfd++;
		if (openfd <= 1)
			exit(0);
	} else
		_rpcsvcstate = _IDLE;

	(void) signal(SIGALRM, (SIG_PF) closedown);
	(void) alarm(_RPCSVC_CLOSEDOWN/2);
}
#endif

void
dmi2_client_0x1(struct svc_req *rqstp, register SVCXPRT *transp)
{
	union {
		DmiDeliverEventIN _dmideliverevent_0x1_arg;
		DmiComponentAddedIN _dmicomponentadded_0x1_arg;
		DmiComponentDeletedIN _dmicomponentdeleted_0x1_arg;
		DmiLanguageAddedIN _dmilanguageadded_0x1_arg;
		DmiLanguageDeletedIN _dmilanguagedeleted_0x1_arg;
		DmiGroupAddedIN _dmigroupadded_0x1_arg;
		DmiGroupDeletedIN _dmigroupdeleted_0x1_arg;
		DmiSubscriptionNoticeIN _dmisubscriptionnotice_0x1_arg;
	} argument;
	char *result;
	xdrproc_t _xdr_argument, _xdr_result;
	char *(*local)(char *, struct svc_req *);

	_rpcsvccount++;
	switch (rqstp->rq_proc) {
	case NULLPROC:
		(void) svc_sendreply(transp,
			(xdrproc_t) xdr_void, (char *)NULL);
		_rpcsvccount--;
		_rpcsvcstate = _SERVED;
		return;

	case _DmiDeliverEvent:
		_xdr_argument = (xdrproc_t) xdr_DmiDeliverEventIN;
		_xdr_result = (xdrproc_t) xdr_DmiErrorStatus_t;
		local = (char *(*)(char *, struct svc_req *)) _dmideliverevent_0x1_svc;
		break;

	case _DmiComponentAdded:
		_xdr_argument = (xdrproc_t) xdr_DmiComponentAddedIN;
		_xdr_result = (xdrproc_t) xdr_DmiErrorStatus_t;
		local = (char *(*)(char *, struct svc_req *)) _dmicomponentadded_0x1_svc;
		break;

	case _DmiComponentDeleted:
		_xdr_argument = (xdrproc_t) xdr_DmiComponentDeletedIN;
		_xdr_result = (xdrproc_t) xdr_DmiErrorStatus_t;
		local = (char *(*)(char *, struct svc_req *)) _dmicomponentdeleted_0x1_svc;
		break;

	case _DmiLanguageAdded:
		_xdr_argument = (xdrproc_t) xdr_DmiLanguageAddedIN;
		_xdr_result = (xdrproc_t) xdr_DmiErrorStatus_t;
		local = (char *(*)(char *, struct svc_req *)) _dmilanguageadded_0x1_svc;
		break;

	case _DmiLanguageDeleted:
		_xdr_argument = (xdrproc_t) xdr_DmiLanguageDeletedIN;
		_xdr_result = (xdrproc_t) xdr_DmiErrorStatus_t;
		local = (char *(*)(char *, struct svc_req *)) _dmilanguagedeleted_0x1_svc;
		break;

	case _DmiGroupAdded:
		_xdr_argument = (xdrproc_t) xdr_DmiGroupAddedIN;
		_xdr_result = (xdrproc_t) xdr_DmiErrorStatus_t;
		local = (char *(*)(char *, struct svc_req *)) _dmigroupadded_0x1_svc;
		break;

	case _DmiGroupDeleted:
		_xdr_argument = (xdrproc_t) xdr_DmiGroupDeletedIN;
		_xdr_result = (xdrproc_t) xdr_DmiErrorStatus_t;
		local = (char *(*)(char *, struct svc_req *)) _dmigroupdeleted_0x1_svc;
		break;

	case _DmiSubscriptionNotice:
		_xdr_argument = (xdrproc_t) xdr_DmiSubscriptionNoticeIN;
		_xdr_result = (xdrproc_t) xdr_DmiErrorStatus_t;
		local = (char *(*)(char *, struct svc_req *)) _dmisubscriptionnotice_0x1_svc;
		break;

	default:
		svcerr_noproc(transp);
		_rpcsvccount--;
		_rpcsvcstate = _SERVED;
		return;
	}
	(void) memset((char *)&argument, 0, sizeof (argument));
	if (!svc_getargs(transp, _xdr_argument, (caddr_t) &argument)) {
		svcerr_decode(transp);
		_rpcsvccount--;
		_rpcsvcstate = _SERVED;
		return;
	}
	result = (*local)((char *)&argument, rqstp);
	if (result != NULL && !svc_sendreply(transp, _xdr_result, result)) {
		svcerr_systemerr(transp);
	}
	if (!svc_freeargs(transp, _xdr_argument, (caddr_t) &argument)) {
		_msgout("unable to free arguments");
		exit(1);
	}
	_rpcsvccount--;
	_rpcsvcstate = _SERVED;
	return;
}

#if 0
main()
{
	pid_t pid;
	int i;

	(void) sigset(SIGPIPE, SIG_IGN);

	/*
	 * If stdin looks like a TLI endpoint, we assume
	 * that we were started by a port monitor. If
	 * t_getstate fails with TBADF, this is not a
	 * TLI endpoint.
	 */
	if (t_getstate(0) != -1 || t_errno != TBADF) {
		char *netid;
		struct netconfig *nconf = NULL;
		SVCXPRT *transp;
		int pmclose;

		_rpcpmstart = 1;
		openlog("mi_indicate", LOG_PID, LOG_DAEMON);

		if ((netid = getenv("NLSPROVIDER")) == NULL) {
		/* started from inetd */
			pmclose = 1;
		} else {
			if ((nconf = getnetconfigent(netid)) == NULL)
				_msgout("cannot get transport info");

			pmclose = (t_getstate(0) != T_DATAXFER);
		}
		if ((transp = svc_tli_create(0, nconf, NULL, 0, 0)) == NULL) {
			_msgout("cannot create server handle");
			exit(1);
		}
		if (nconf)
			freenetconfigent(nconf);
		if (!svc_reg(transp, DMI2_CLIENT, RMI2_CLIENT_VERSION, dmi2_client_0x1, 0)) {
			_msgout("unable to register (DMI2_CLIENT, RMI2_CLIENT_VERSION).");
			exit(1);
		}
		if (pmclose) {
			(void) signal(SIGALRM, (SIG_PF) closedown);
			(void) alarm(_RPCSVC_CLOSEDOWN/2);
		}
		svc_run();
		exit(1);
		/* NOTREACHED */
	}	else {
#ifndef RPC_SVC_FG
		int size;
		struct rlimit rl;
		pid = fork();
		if (pid < 0) {
			perror("cannot fork");
			exit(1);
		}
		if (pid)
			exit(0);
		rl.rlim_max = 0;
		getrlimit(RLIMIT_NOFILE, &rl);
		if ((size = rl.rlim_max) == 0)
			exit(1);
		for (i = 0; i < size; i++)
			(void) close(i);
		i = open("/dev/null", 2);
		(void) dup2(i, 1);
		(void) dup2(i, 2);
		setsid();
		openlog("mi_indicate", LOG_PID, LOG_DAEMON);
#endif
	}
	if (!svc_create(dmi2_client_0x1, DMI2_CLIENT, RMI2_CLIENT_VERSION, "netpath")) {
		_msgout("unable to create (DMI2_CLIENT, RMI2_CLIENT_VERSION) for netpath.");
		exit(1);
	}

	svc_run();
	_msgout("svc_run returned");
	exit(1);
	/* NOTREACHED */
}
#endif

