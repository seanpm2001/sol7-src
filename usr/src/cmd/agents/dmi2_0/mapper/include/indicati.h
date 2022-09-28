/* Copyright 08/20/96 Sun Microsystems, Inc. All Rights Reserved.
*/
#pragma ident  "@(#)indication.h	1.2 96/08/20 Sun Microsystems"

/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#ifndef _MI_INDICATE_H_RPCGEN
#define	_MI_INDICATE_H_RPCGEN

#include <rpc/rpc.h>
#include "common.h"
#ifdef __cplusplus
extern "C" {
#endif

#define  MAPPER_PROGNUM  ((unsigned long) 100249)
#define  MAPPER_VERSNUM  ((unsigned long) 0x1)
#define  RPC_THRESHOLD    10

struct DmiNodeAddress {
        DmiString_t *address;
        DmiString_t *rpc;
        DmiString_t *transport;
};
typedef struct DmiNodeAddress DmiNodeAddress;

typedef DmiNodeAddress DmiNodeAddress_t;


struct DmiDeliverEventIN {
        DmiUnsigned_t handle;
        DmiNodeAddress_t *sender;
        DmiString_t *language;
        DmiId_t compId;
        DmiTimestamp_t *timestamp;
        DmiMultiRowData_t *rowData;
};
typedef struct DmiDeliverEventIN DmiDeliverEventIN;

struct DmiComponentAddedIN {
        DmiUnsigned_t handle;
        DmiNodeAddress_t *sender;
        DmiComponentInfo_t *info;
};
typedef struct DmiComponentAddedIN DmiComponentAddedIN;

struct DmiComponentDeletedIN {
        DmiUnsigned_t handle;
        DmiNodeAddress_t *sender;
        DmiId_t compId;
};
typedef struct DmiComponentDeletedIN DmiComponentDeletedIN;

struct DmiLanguageAddedIN {
        DmiUnsigned_t handle;
        DmiNodeAddress_t *sender;
        DmiId_t compId;
        DmiString_t *language;
};
typedef struct DmiLanguageAddedIN DmiLanguageAddedIN;

struct DmiLanguageDeletedIN {
        DmiUnsigned_t handle;
        DmiNodeAddress_t *sender;
        DmiId_t compId;
        DmiString_t *language;
};
typedef struct DmiLanguageDeletedIN DmiLanguageDeletedIN;

struct DmiGroupAddedIN {
        DmiUnsigned_t handle;
        DmiNodeAddress_t *sender;
        DmiId_t compId;
        DmiGroupInfo_t *info;
};
typedef struct DmiGroupAddedIN DmiGroupAddedIN;

struct DmiGroupDeletedIN {
        DmiUnsigned_t handle;
        DmiNodeAddress_t *sender;
        DmiId_t compId;
        DmiId_t groupId;
};
typedef struct DmiGroupDeletedIN DmiGroupDeletedIN;

struct DmiSubscriptionNoticeIN {
	DmiUnsigned_t handle;
	DmiNodeAddress_t *sender;
	DmiBoolean_t expired;
	DmiRowData_t rowData;
};
typedef struct DmiSubscriptionNoticeIN DmiSubscriptionNoticeIN;

#define	DMI2_CLIENT ((unsigned long)(0x20000000))
#define	RMI2_CLIENT_VERSION ((unsigned long)(0x1))

#if defined(__STDC__) || defined(__cplusplus)
#define	_DmiDeliverEvent ((unsigned long)(0x100))
extern  DmiErrorStatus_t * _dmideliverevent_0x1(DmiDeliverEventIN *, CLIENT *);
extern  DmiErrorStatus_t * _dmideliverevent_0x1_svc(DmiDeliverEventIN *, struct svc_req *);
#define	_DmiComponentAdded ((unsigned long)(0x101))
extern  DmiErrorStatus_t * _dmicomponentadded_0x1(DmiComponentAddedIN *, CLIENT *);
extern  DmiErrorStatus_t * _dmicomponentadded_0x1_svc(DmiComponentAddedIN *, struct svc_req *);
#define	_DmiComponentDeleted ((unsigned long)(0x102))
extern  DmiErrorStatus_t * _dmicomponentdeleted_0x1(DmiComponentDeletedIN *, CLIENT *);
extern  DmiErrorStatus_t * _dmicomponentdeleted_0x1_svc(DmiComponentDeletedIN *, struct svc_req *);
#define	_DmiLanguageAdded ((unsigned long)(0x103))
extern  DmiErrorStatus_t * _dmilanguageadded_0x1(DmiLanguageAddedIN *, CLIENT *);
extern  DmiErrorStatus_t * _dmilanguageadded_0x1_svc(DmiLanguageAddedIN *, struct svc_req *);
#define	_DmiLanguageDeleted ((unsigned long)(0x104))
extern  DmiErrorStatus_t * _dmilanguagedeleted_0x1(DmiLanguageDeletedIN *, CLIENT *);
extern  DmiErrorStatus_t * _dmilanguagedeleted_0x1_svc(DmiLanguageDeletedIN *, struct svc_req *);
#define	_DmiGroupAdded ((unsigned long)(0x105))
extern  DmiErrorStatus_t * _dmigroupadded_0x1(DmiGroupAddedIN *, CLIENT *);
extern  DmiErrorStatus_t * _dmigroupadded_0x1_svc(DmiGroupAddedIN *, struct svc_req *);
#define	_DmiGroupDeleted ((unsigned long)(0x106))
extern  DmiErrorStatus_t * _dmigroupdeleted_0x1(DmiGroupDeletedIN *, CLIENT *);
extern  DmiErrorStatus_t * _dmigroupdeleted_0x1_svc(DmiGroupDeletedIN *, struct svc_req *);
#define	_DmiSubscriptionNotice ((unsigned long)(0x107))
extern  DmiErrorStatus_t * _dmisubscriptionnotice_0x1(DmiSubscriptionNoticeIN *, CLIENT *);
extern  DmiErrorStatus_t * _dmisubscriptionnotice_0x1_svc(DmiSubscriptionNoticeIN *, struct svc_req *);
extern int dmi2_client_0x1_freeresult(SVCXPRT *, xdrproc_t, caddr_t);

#else /* K&R C */
#define	_DmiDeliverEvent ((unsigned long)(0x100))
extern  DmiErrorStatus_t * _dmideliverevent_0x1();
extern  DmiErrorStatus_t * _dmideliverevent_0x1_svc();
#define	_DmiComponentAdded ((unsigned long)(0x101))
extern  DmiErrorStatus_t * _dmicomponentadded_0x1();
extern  DmiErrorStatus_t * _dmicomponentadded_0x1_svc();
#define	_DmiComponentDeleted ((unsigned long)(0x102))
extern  DmiErrorStatus_t * _dmicomponentdeleted_0x1();
extern  DmiErrorStatus_t * _dmicomponentdeleted_0x1_svc();
#define	_DmiLanguageAdded ((unsigned long)(0x103))
extern  DmiErrorStatus_t * _dmilanguageadded_0x1();
extern  DmiErrorStatus_t * _dmilanguageadded_0x1_svc();
#define	_DmiLanguageDeleted ((unsigned long)(0x104))
extern  DmiErrorStatus_t * _dmilanguagedeleted_0x1();
extern  DmiErrorStatus_t * _dmilanguagedeleted_0x1_svc();
#define	_DmiGroupAdded ((unsigned long)(0x105))
extern  DmiErrorStatus_t * _dmigroupadded_0x1();
extern  DmiErrorStatus_t * _dmigroupadded_0x1_svc();
#define	_DmiGroupDeleted ((unsigned long)(0x106))
extern  DmiErrorStatus_t * _dmigroupdeleted_0x1();
extern  DmiErrorStatus_t * _dmigroupdeleted_0x1_svc();
#define	_DmiSubscriptionNotice ((unsigned long)(0x107))
extern  DmiErrorStatus_t * _dmisubscriptionnotice_0x1();
extern  DmiErrorStatus_t * _dmisubscriptionnotice_0x1_svc();
extern int dmi2_client_0x1_freeresult();
#endif /* K&R C */

/* the xdr functions */

#if defined(__STDC__) || defined(__cplusplus)
extern  bool_t xdr_DmiNodeAddress(XDR *, DmiNodeAddress*);
extern  bool_t xdr_DmiNodeAddress_t(XDR *, DmiNodeAddress_t*);
extern  bool_t xdr_DmiDeliverEventIN(XDR *, DmiDeliverEventIN*);
extern  bool_t xdr_DmiComponentAddedIN(XDR *, DmiComponentAddedIN*);
extern  bool_t xdr_DmiComponentDeletedIN(XDR *, DmiComponentDeletedIN*);
extern  bool_t xdr_DmiLanguageAddedIN(XDR *, DmiLanguageAddedIN*);
extern  bool_t xdr_DmiLanguageDeletedIN(XDR *, DmiLanguageDeletedIN*);
extern  bool_t xdr_DmiGroupAddedIN(XDR *, DmiGroupAddedIN*);
extern  bool_t xdr_DmiGroupDeletedIN(XDR *, DmiGroupDeletedIN*);
extern  bool_t xdr_DmiSubscriptionNoticeIN(XDR *, DmiSubscriptionNoticeIN*);
extern  void dmi2_client_0x1(struct svc_req *rqstp, register SVCXPRT *transp);


#else /* K&R C */
extern bool_t xdr_DmiNodeAddress();
extern bool_t xdr_DmiNodeAddress_t();
extern bool_t xdr_DmiDeliverEventIN();
extern bool_t xdr_DmiComponentAddedIN();
extern bool_t xdr_DmiComponentDeletedIN();
extern bool_t xdr_DmiLanguageAddedIN();
extern bool_t xdr_DmiLanguageDeletedIN();
extern bool_t xdr_DmiGroupAddedIN();
extern bool_t xdr_DmiGroupDeletedIN();
extern bool_t xdr_DmiSubscriptionNoticeIN();

extern void   dmi2_client_0x1();
#endif /* K&R C */

#ifdef __cplusplus
}
#endif


#endif /* !_MI_INDICATE_H_RPCGEN */
