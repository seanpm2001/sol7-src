/* Copyright 09/11/96 Sun Microsystems, Inc. All Rights Reserved.
 */

/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "server.h"

bool_t
xdr_DmiRegisterIN(register XDR *xdrs, DmiRegisterIN *objp)
{

	register long *buf;

	if (!xdr_DmiHandle_t(xdrs, &objp->handle))
		return (FALSE);
	return (TRUE);
}

bool_t
xdr_DmiRegisterOUT(register XDR *xdrs, DmiRegisterOUT *objp)
{

	register long *buf;

	if (!xdr_DmiErrorStatus_t(xdrs, &objp->error_status))
		return (FALSE);
	if (!xdr_pointer(xdrs, (char **)&objp->handle, sizeof (DmiHandle_t), (xdrproc_t) xdr_DmiHandle_t))
		return (FALSE);
	return (TRUE);
}

bool_t
xdr_DmiUnregisterOUT(register XDR *xdrs, DmiUnregisterOUT *objp)
{

	register long *buf;

	if (!xdr_DmiErrorStatus_t(xdrs, &objp->error_status))
		return (FALSE);
	return (TRUE);
}

bool_t
xdr_DmiUnregisterIN(register XDR *xdrs, DmiUnregisterIN *objp)
{

	register long *buf;

	if (!xdr_DmiHandle_t(xdrs, &objp->handle))
		return (FALSE);
	return (TRUE);
}

bool_t
xdr_DmiGetVersionOUT(register XDR *xdrs, DmiGetVersionOUT *objp)
{

	register long *buf;

	if (!xdr_DmiErrorStatus_t(xdrs, &objp->error_status))
		return (FALSE);
	if (!xdr_pointer(xdrs, (char **)&objp->dmiSpecLevel, sizeof (DmiString_t), (xdrproc_t) xdr_DmiString_t))
		return (FALSE);
	if (!xdr_pointer(xdrs, (char **)&objp->description, sizeof (DmiString_t), (xdrproc_t) xdr_DmiString_t))
		return (FALSE);
	if (!xdr_pointer(xdrs, (char **)&objp->fileTypes, sizeof (DmiFileTypeList_t), (xdrproc_t) xdr_DmiFileTypeList_t))
		return (FALSE);
	return (TRUE);
}

bool_t
xdr_DmiGetVersionIN(register XDR *xdrs, DmiGetVersionIN *objp)
{

	register long *buf;

	if (!xdr_DmiHandle_t(xdrs, &objp->handle))
		return (FALSE);
	return (TRUE);
}

bool_t
xdr_DmiGetConfigOUT(register XDR *xdrs, DmiGetConfigOUT *objp)
{

	register long *buf;

	if (!xdr_DmiErrorStatus_t(xdrs, &objp->error_status))
		return (FALSE);
	if (!xdr_pointer(xdrs, (char **)&objp->language, sizeof (DmiString_t), (xdrproc_t) xdr_DmiString_t))
		return (FALSE);
	return (TRUE);
}

bool_t
xdr_DmiGetConfigIN(register XDR *xdrs, DmiGetConfigIN *objp)
{

	register long *buf;

	if (!xdr_DmiHandle_t(xdrs, &objp->handle))
		return (FALSE);
	return (TRUE);
}

bool_t
xdr_DmiSetConfigOUT(register XDR *xdrs, DmiSetConfigOUT *objp)
{

	register long *buf;

	if (!xdr_DmiErrorStatus_t(xdrs, &objp->error_status))
		return (FALSE);
	return (TRUE);
}

bool_t
xdr_DmiSetConfigIN(register XDR *xdrs, DmiSetConfigIN *objp)
{

	register long *buf;

	if (!xdr_DmiHandle_t(xdrs, &objp->handle))
		return (FALSE);
	if (!xdr_pointer(xdrs, (char **)&objp->language, sizeof (DmiString_t), (xdrproc_t) xdr_DmiString_t))
		return (FALSE);
	return (TRUE);
}

bool_t
xdr_DmiListComponentsOUT(register XDR *xdrs, DmiListComponentsOUT *objp)
{

	register long *buf;

	if (!xdr_DmiErrorStatus_t(xdrs, &objp->error_status))
		return (FALSE);
	if (!xdr_pointer(xdrs, (char **)&objp->reply, sizeof (DmiComponentList_t), (xdrproc_t) xdr_DmiComponentList_t))
		return (FALSE);
	return (TRUE);
}

bool_t
xdr_DmiListComponentsIN(register XDR *xdrs, DmiListComponentsIN *objp)
{

	register long *buf;

	if (!xdr_DmiHandle_t(xdrs, &objp->handle))
		return (FALSE);
	if (!xdr_DmiRequestMode_t(xdrs, &objp->requestMode))
		return (FALSE);
	if (!xdr_DmiUnsigned_t(xdrs, &objp->maxCount))
		return (FALSE);
	if (!xdr_DmiBoolean_t(xdrs, &objp->getPragma))
		return (FALSE);
	if (!xdr_DmiBoolean_t(xdrs, &objp->getDescription))
		return (FALSE);
	if (!xdr_DmiId_t(xdrs, &objp->compId))
		return (FALSE);
	return (TRUE);
}

bool_t
xdr_DmiListComponentsByClassOUT(register XDR *xdrs, DmiListComponentsByClassOUT *objp)
{

	register long *buf;

	if (!xdr_DmiErrorStatus_t(xdrs, &objp->error_status))
		return (FALSE);
	if (!xdr_pointer(xdrs, (char **)&objp->reply, sizeof (DmiComponentList_t), (xdrproc_t) xdr_DmiComponentList_t))
		return (FALSE);
	return (TRUE);
}

bool_t
xdr_DmiListComponentsByClassIN(register XDR *xdrs, DmiListComponentsByClassIN *objp)
{

	register long *buf;

	if (!xdr_DmiHandle_t(xdrs, &objp->handle))
		return (FALSE);
	if (!xdr_DmiRequestMode_t(xdrs, &objp->requestMode))
		return (FALSE);
	if (!xdr_DmiUnsigned_t(xdrs, &objp->maxCount))
		return (FALSE);
	if (!xdr_DmiBoolean_t(xdrs, &objp->getPragma))
		return (FALSE);
	if (!xdr_DmiBoolean_t(xdrs, &objp->getDescription))
		return (FALSE);
	if (!xdr_DmiId_t(xdrs, &objp->compId))
		return (FALSE);
	if (!xdr_pointer(xdrs, (char **)&objp->className, sizeof (DmiString_t), (xdrproc_t) xdr_DmiString_t))
		return (FALSE);
	if (!xdr_pointer(xdrs, (char **)&objp->keyList, sizeof (DmiAttributeValues_t), (xdrproc_t) xdr_DmiAttributeValues_t))
		return (FALSE);
	return (TRUE);
}

bool_t
xdr_DmiListLanguagesOUT(register XDR *xdrs, DmiListLanguagesOUT *objp)
{

	register long *buf;

	if (!xdr_DmiErrorStatus_t(xdrs, &objp->error_status))
		return (FALSE);
	if (!xdr_pointer(xdrs, (char **)&objp->reply, sizeof (DmiStringList_t), (xdrproc_t) xdr_DmiStringList_t))
		return (FALSE);
	return (TRUE);
}

bool_t
xdr_DmiListLanguagesIN(register XDR *xdrs, DmiListLanguagesIN *objp)
{

	register long *buf;

	if (!xdr_DmiHandle_t(xdrs, &objp->handle))
		return (FALSE);
	if (!xdr_DmiUnsigned_t(xdrs, &objp->maxCount))
		return (FALSE);
	if (!xdr_DmiId_t(xdrs, &objp->compId))
		return (FALSE);
	return (TRUE);
}

bool_t
xdr_DmiListClassNamesOUT(register XDR *xdrs, DmiListClassNamesOUT *objp)
{

	register long *buf;

	if (!xdr_DmiErrorStatus_t(xdrs, &objp->error_status))
		return (FALSE);
	if (!xdr_pointer(xdrs, (char **)&objp->reply, sizeof (DmiClassNameList_t), (xdrproc_t) xdr_DmiClassNameList_t))
		return (FALSE);
	return (TRUE);
}

bool_t
xdr_DmiListClassNamesIN(register XDR *xdrs, DmiListClassNamesIN *objp)
{

	register long *buf;

	if (!xdr_DmiHandle_t(xdrs, &objp->handle))
		return (FALSE);
	if (!xdr_DmiUnsigned_t(xdrs, &objp->maxCount))
		return (FALSE);
	if (!xdr_DmiId_t(xdrs, &objp->compId))
		return (FALSE);
	return (TRUE);
}

bool_t
xdr_DmiListGroupsOUT(register XDR *xdrs, DmiListGroupsOUT *objp)
{

	register long *buf;

	if (!xdr_DmiErrorStatus_t(xdrs, &objp->error_status))
		return (FALSE);
	if (!xdr_pointer(xdrs, (char **)&objp->reply, sizeof (DmiGroupList_t), (xdrproc_t) xdr_DmiGroupList_t))
		return (FALSE);
	return (TRUE);
}

bool_t
xdr_DmiListGroupsIN(register XDR *xdrs, DmiListGroupsIN *objp)
{

	register long *buf;

	if (!xdr_DmiHandle_t(xdrs, &objp->handle))
		return (FALSE);
	if (!xdr_DmiRequestMode_t(xdrs, &objp->requestMode))
		return (FALSE);
	if (!xdr_DmiUnsigned_t(xdrs, &objp->maxCount))
		return (FALSE);
	if (!xdr_DmiBoolean_t(xdrs, &objp->getPragma))
		return (FALSE);
	if (!xdr_DmiBoolean_t(xdrs, &objp->getDescription))
		return (FALSE);
	if (!xdr_DmiId_t(xdrs, &objp->compId))
		return (FALSE);
	if (!xdr_DmiId_t(xdrs, &objp->groupId))
		return (FALSE);
	return (TRUE);
}

bool_t
xdr_DmiListAttributesOUT(register XDR *xdrs, DmiListAttributesOUT *objp)
{

	register long *buf;

	if (!xdr_DmiErrorStatus_t(xdrs, &objp->error_status))
		return (FALSE);
	if (!xdr_pointer(xdrs, (char **)&objp->reply, sizeof (DmiAttributeList_t), (xdrproc_t) xdr_DmiAttributeList_t))
		return (FALSE);
	return (TRUE);
}

bool_t
xdr_DmiListAttributesIN(register XDR *xdrs, DmiListAttributesIN *objp)
{

	register long *buf;

	if (!xdr_DmiHandle_t(xdrs, &objp->handle))
		return (FALSE);
	if (!xdr_DmiRequestMode_t(xdrs, &objp->requestMode))
		return (FALSE);
	if (!xdr_DmiUnsigned_t(xdrs, &objp->maxCount))
		return (FALSE);
	if (!xdr_DmiBoolean_t(xdrs, &objp->getPragma))
		return (FALSE);
	if (!xdr_DmiBoolean_t(xdrs, &objp->getDescription))
		return (FALSE);
	if (!xdr_DmiId_t(xdrs, &objp->compId))
		return (FALSE);
	if (!xdr_DmiId_t(xdrs, &objp->groupId))
		return (FALSE);
	if (!xdr_DmiId_t(xdrs, &objp->attribId))
		return (FALSE);
	return (TRUE);
}

bool_t
xdr_DmiAddComponentOUT(register XDR *xdrs, DmiAddComponentOUT *objp)
{

	register long *buf;

	if (!xdr_DmiErrorStatus_t(xdrs, &objp->error_status))
		return (FALSE);
	if (!xdr_DmiId_t(xdrs, &objp->compId))
		return (FALSE);
	if (!xdr_pointer(xdrs, (char **)&objp->errors, sizeof (DmiStringList_t), (xdrproc_t) xdr_DmiStringList_t))
		return (FALSE);
	return (TRUE);
}

bool_t
xdr_DmiAddComponentIN(register XDR *xdrs, DmiAddComponentIN *objp)
{

	register long *buf;

	if (!xdr_DmiHandle_t(xdrs, &objp->handle))
		return (FALSE);
	if (!xdr_pointer(xdrs, (char **)&objp->fileData, sizeof (DmiFileDataList_t), (xdrproc_t) xdr_DmiFileDataList_t))
		return (FALSE);
	return (TRUE);
}

bool_t
xdr_DmiAddLanguageOUT(register XDR *xdrs, DmiAddLanguageOUT *objp)
{

	register long *buf;

	if (!xdr_DmiErrorStatus_t(xdrs, &objp->error_status))
		return (FALSE);
	if (!xdr_pointer(xdrs, (char **)&objp->errors, sizeof (DmiStringList_t), (xdrproc_t) xdr_DmiStringList_t))
		return (FALSE);
	return (TRUE);
}

bool_t
xdr_DmiAddLanguageIN(register XDR *xdrs, DmiAddLanguageIN *objp)
{

	register long *buf;

	if (!xdr_DmiHandle_t(xdrs, &objp->handle))
		return (FALSE);
	if (!xdr_pointer(xdrs, (char **)&objp->fileData, sizeof (DmiFileDataList_t), (xdrproc_t) xdr_DmiFileDataList_t))
		return (FALSE);
	if (!xdr_DmiId_t(xdrs, &objp->compId))
		return (FALSE);
	return (TRUE);
}

bool_t
xdr_DmiAddGroupOUT(register XDR *xdrs, DmiAddGroupOUT *objp)
{

	register long *buf;

	if (!xdr_DmiErrorStatus_t(xdrs, &objp->error_status))
		return (FALSE);
	if (!xdr_DmiId_t(xdrs, &objp->groupId))
		return (FALSE);
	if (!xdr_pointer(xdrs, (char **)&objp->errors, sizeof (DmiStringList_t), (xdrproc_t) xdr_DmiStringList_t))
		return (FALSE);
	return (TRUE);
}

bool_t
xdr_DmiAddGroupIN(register XDR *xdrs, DmiAddGroupIN *objp)
{

	register long *buf;

	if (!xdr_DmiHandle_t(xdrs, &objp->handle))
		return (FALSE);
	if (!xdr_pointer(xdrs, (char **)&objp->fileData, sizeof (DmiFileDataList_t), (xdrproc_t) xdr_DmiFileDataList_t))
		return (FALSE);
	if (!xdr_DmiId_t(xdrs, &objp->compId))
		return (FALSE);
	return (TRUE);
}

bool_t
xdr_DmiDeleteComponentOUT(register XDR *xdrs, DmiDeleteComponentOUT *objp)
{

	register long *buf;

	if (!xdr_DmiErrorStatus_t(xdrs, &objp->error_status))
		return (FALSE);
	return (TRUE);
}

bool_t
xdr_DmiDeleteComponentIN(register XDR *xdrs, DmiDeleteComponentIN *objp)
{

	register long *buf;

	if (!xdr_DmiHandle_t(xdrs, &objp->handle))
		return (FALSE);
	if (!xdr_DmiId_t(xdrs, &objp->compId))
		return (FALSE);
	return (TRUE);
}

bool_t
xdr_DmiDeleteLanguageOUT(register XDR *xdrs, DmiDeleteLanguageOUT *objp)
{

	register long *buf;

	if (!xdr_DmiErrorStatus_t(xdrs, &objp->error_status))
		return (FALSE);
	return (TRUE);
}

bool_t
xdr_DmiDeleteLanguageIN(register XDR *xdrs, DmiDeleteLanguageIN *objp)
{

	register long *buf;

	if (!xdr_DmiHandle_t(xdrs, &objp->handle))
		return (FALSE);
	if (!xdr_pointer(xdrs, (char **)&objp->language, sizeof (DmiString_t), (xdrproc_t) xdr_DmiString_t))
		return (FALSE);
	if (!xdr_DmiId_t(xdrs, &objp->compId))
		return (FALSE);
	return (TRUE);
}

bool_t
xdr_DmiDeleteGroupOUT(register XDR *xdrs, DmiDeleteGroupOUT *objp)
{

	register long *buf;

	if (!xdr_DmiErrorStatus_t(xdrs, &objp->error_status))
		return (FALSE);
	return (TRUE);
}

bool_t
xdr_DmiDeleteGroupIN(register XDR *xdrs, DmiDeleteGroupIN *objp)
{

	register long *buf;

	if (!xdr_DmiHandle_t(xdrs, &objp->handle))
		return (FALSE);
	if (!xdr_DmiId_t(xdrs, &objp->compId))
		return (FALSE);
	if (!xdr_DmiId_t(xdrs, &objp->groupId))
		return (FALSE);
	return (TRUE);
}

bool_t
xdr_DmiAddRowOUT(register XDR *xdrs, DmiAddRowOUT *objp)
{

	register long *buf;

	if (!xdr_DmiErrorStatus_t(xdrs, &objp->error_status))
		return (FALSE);
	return (TRUE);
}

bool_t
xdr_DmiAddRowIN(register XDR *xdrs, DmiAddRowIN *objp)
{

	register long *buf;

	if (!xdr_DmiHandle_t(xdrs, &objp->handle))
		return (FALSE);
	if (!xdr_pointer(xdrs, (char **)&objp->rowData, sizeof (DmiRowData_t), (xdrproc_t) xdr_DmiRowData_t))
		return (FALSE);
	return (TRUE);
}

bool_t
xdr_DmiDeleteRowOUT(register XDR *xdrs, DmiDeleteRowOUT *objp)
{

	register long *buf;

	if (!xdr_DmiErrorStatus_t(xdrs, &objp->error_status))
		return (FALSE);
	return (TRUE);
}

bool_t
xdr_DmiDeleteRowIN(register XDR *xdrs, DmiDeleteRowIN *objp)
{

	register long *buf;

	if (!xdr_DmiHandle_t(xdrs, &objp->handle))
		return (FALSE);
	if (!xdr_pointer(xdrs, (char **)&objp->rowData, sizeof (DmiRowData_t), (xdrproc_t) xdr_DmiRowData_t))
		return (FALSE);
	return (TRUE);
}

bool_t
xdr_DmiGetMultipleOUT(register XDR *xdrs, DmiGetMultipleOUT *objp)
{

	register long *buf;

	if (!xdr_DmiErrorStatus_t(xdrs, &objp->error_status))
		return (FALSE);
	if (!xdr_pointer(xdrs, (char **)&objp->rowData, sizeof (DmiMultiRowData_t), (xdrproc_t) xdr_DmiMultiRowData_t))
		return (FALSE);
	return (TRUE);
}

bool_t
xdr_DmiGetMultipleIN(register XDR *xdrs, DmiGetMultipleIN *objp)
{

	register long *buf;

	if (!xdr_DmiHandle_t(xdrs, &objp->handle))
		return (FALSE);
	if (!xdr_pointer(xdrs, (char **)&objp->request, sizeof (DmiMultiRowRequest_t), (xdrproc_t) xdr_DmiMultiRowRequest_t))
		return (FALSE);
	return (TRUE);
}

bool_t
xdr_DmiSetMultipleOUT(register XDR *xdrs, DmiSetMultipleOUT *objp)
{

	register long *buf;

	if (!xdr_DmiErrorStatus_t(xdrs, &objp->error_status))
		return (FALSE);
	return (TRUE);
}

bool_t
xdr_DmiSetMultipleIN(register XDR *xdrs, DmiSetMultipleIN *objp)
{

	register long *buf;

	if (!xdr_DmiHandle_t(xdrs, &objp->handle))
		return (FALSE);
	if (!xdr_DmiSetMode_t(xdrs, &objp->setMode))
		return (FALSE);
	if (!xdr_pointer(xdrs, (char **)&objp->rowData, sizeof (DmiMultiRowData_t), (xdrproc_t) xdr_DmiMultiRowData_t))
		return (FALSE);
	return (TRUE);
}

bool_t
xdr_DmiGetAttributeOUT(register XDR *xdrs, DmiGetAttributeOUT *objp)
{

	register long *buf;

	if (!xdr_DmiErrorStatus_t(xdrs, &objp->error_status))
		return (FALSE);
	if (!xdr_pointer(xdrs, (char **)&objp->value, sizeof (DmiDataUnion_t), (xdrproc_t) xdr_DmiDataUnion_t))
		return (FALSE);
	return (TRUE);
}

bool_t
xdr_DmiGetAttributeIN(register XDR *xdrs, DmiGetAttributeIN *objp)
{

	register long *buf;

	if (!xdr_DmiHandle_t(xdrs, &objp->handle))
		return (FALSE);
	if (!xdr_DmiId_t(xdrs, &objp->compId))
		return (FALSE);
	if (!xdr_DmiId_t(xdrs, &objp->groupId))
		return (FALSE);
	if (!xdr_DmiId_t(xdrs, &objp->attribId))
		return (FALSE);
	if (!xdr_pointer(xdrs, (char **)&objp->keyList, sizeof (DmiAttributeValues_t), (xdrproc_t) xdr_DmiAttributeValues_t))
		return (FALSE);
	return (TRUE);
}

bool_t
xdr_DmiSetAttributeOUT(register XDR *xdrs, DmiSetAttributeOUT *objp)
{

	register long *buf;

	if (!xdr_DmiErrorStatus_t(xdrs, &objp->error_status))
		return (FALSE);
	return (TRUE);
}

bool_t
xdr_DmiSetAttributeIN(register XDR *xdrs, DmiSetAttributeIN *objp)
{

	register long *buf;

	if (!xdr_DmiHandle_t(xdrs, &objp->handle))
		return (FALSE);
	if (!xdr_DmiId_t(xdrs, &objp->compId))
		return (FALSE);
	if (!xdr_DmiId_t(xdrs, &objp->groupId))
		return (FALSE);
	if (!xdr_DmiId_t(xdrs, &objp->attribId))
		return (FALSE);
	if (!xdr_pointer(xdrs, (char **)&objp->keyList, sizeof (DmiAttributeValues_t), (xdrproc_t) xdr_DmiAttributeValues_t))
		return (FALSE);
	if (!xdr_DmiSetMode_t(xdrs, &objp->setMode))
		return (FALSE);
	if (!xdr_pointer(xdrs, (char **)&objp->value, sizeof (DmiDataUnion_t), (xdrproc_t) xdr_DmiDataUnion_t))
		return (FALSE);
	return (TRUE);
}

bool_t
xdr_DmiRegisterCiIN(register XDR *xdrs, DmiRegisterCiIN *objp)
{

	register long *buf;

	if (!xdr_pointer(xdrs, (char **)&objp->regInfo, sizeof (DmiRegisterInfo_t), (xdrproc_t) xdr_DmiRegisterInfo_t))
		return (FALSE);
	return (TRUE);
}

bool_t
xdr_DmiRegisterCiOUT(register XDR *xdrs, DmiRegisterCiOUT *objp)
{

	register long *buf;

	if (!xdr_DmiErrorStatus_t(xdrs, &objp->error_status))
		return (FALSE);
	if (!xdr_pointer(xdrs, (char **)&objp->handle, sizeof (DmiHandle_t), (xdrproc_t) xdr_DmiHandle_t))
		return (FALSE);
	if (!xdr_pointer(xdrs, (char **)&objp->dmiSpecLevel, sizeof (DmiString_t), (xdrproc_t) xdr_DmiString_t))
		return (FALSE);
	return (TRUE);
}

bool_t
xdr_DmiUnregisterCiIN(register XDR *xdrs, DmiUnregisterCiIN *objp)
{

	register long *buf;

	if (!xdr_DmiHandle_t(xdrs, &objp->handle))
		return (FALSE);
	return (TRUE);
}

bool_t
xdr_DmiUnregisterCiOUT(register XDR *xdrs, DmiUnregisterCiOUT *objp)
{

	register long *buf;

	if (!xdr_DmiErrorStatus_t(xdrs, &objp->error_status))
		return (FALSE);
	return (TRUE);
}

bool_t
xdr_DmiOriginateEventIN(register XDR *xdrs, DmiOriginateEventIN *objp)
{

	register long *buf;

	if (!xdr_DmiId_t(xdrs, &objp->compId))
		return (FALSE);
	if (!xdr_pointer(xdrs, (char **)&objp->language, sizeof (DmiString_t), (xdrproc_t) xdr_DmiString_t))
		return (FALSE);
	if (!xdr_pointer(xdrs, (char **)&objp->timestamp, sizeof (DmiTimestamp_t), (xdrproc_t) xdr_DmiTimestamp_t))
		return (FALSE);
	if (!xdr_pointer(xdrs, (char **)&objp->rowData, sizeof (DmiRowData_t), (xdrproc_t) xdr_DmiRowData_t))
		return (FALSE);
	return (TRUE);
}

bool_t
xdr_DmiOriginateEventOUT(register XDR *xdrs, DmiOriginateEventOUT *objp)
{

	register long *buf;

	if (!xdr_DmiErrorStatus_t(xdrs, &objp->error_status))
		return (FALSE);
	return (TRUE);
}
