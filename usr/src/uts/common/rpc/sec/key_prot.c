#pragma ident	"@(#)key_prot.c	1.11	97/04/29 SMI"
/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include <rpc/key_prot.h>
/* @(#)key_prot.x	1.10 90/01/03 Copyright (c)  1990, 1991 SMI */

/*
 * Compiled from key_prot.x using rpcgen.
 * DO NOT EDIT THIS FILE!
 * This is NOT source code!
 */

bool_t
xdr_keystatus(XDR *xdrs, keystatus *objp)
{
	if (!xdr_enum(xdrs, (enum_t *)objp))
		return (FALSE);
	return (TRUE);
}

bool_t
xdr_keybuf(XDR *xdrs, keybuf objp)
{
	if (!xdr_opaque(xdrs, objp, HEXKEYBYTES))
		return (FALSE);
	return (TRUE);
}

bool_t
xdr_netnamestr(XDR *xdrs, netnamestr *objp)
{
	if (!xdr_string(xdrs, objp, MAXNETNAMELEN))
		return (FALSE);

	return (TRUE);
}

bool_t
xdr_cryptkeyarg(XDR *xdrs, cryptkeyarg *objp)
{
	if (!xdr_netnamestr(xdrs, &objp->remotename))
		return (FALSE);
	if (!xdr_des_block(xdrs, &objp->deskey))
		return (FALSE);
	return (TRUE);
}

bool_t
xdr_cryptkeyarg2(XDR *xdrs, cryptkeyarg2 *objp)
{
	if (!xdr_netnamestr(xdrs, &objp->remotename))
		return (FALSE);
	if (!xdr_netobj(xdrs, &objp->remotekey))
		return (FALSE);
	if (!xdr_des_block(xdrs, &objp->deskey))
		return (FALSE);
	return (TRUE);
}

bool_t
xdr_cryptkeyres(XDR *xdrs, cryptkeyres *objp)
{
	if (!xdr_keystatus(xdrs, &objp->status))
		return (FALSE);
	switch (objp->status) {
	case KEY_SUCCESS:
		if (!xdr_des_block(xdrs, &objp->cryptkeyres_u.deskey))
			return (FALSE);
		break;
	}
	return (TRUE);
}

bool_t
xdr_unixcred(XDR *xdrs, unixcred *objp)
{
	if (!xdr_u_int(xdrs, &objp->uid))
		return (FALSE);
	if (!xdr_u_int(xdrs, &objp->gid))
		return (FALSE);
	if (!xdr_array(xdrs, (char **)&objp->gids.gids_val,
			(u_int *) &objp->gids.gids_len, MAXGIDS,
			sizeof (u_int), (xdrproc_t) xdr_u_int))
		return (FALSE);
	return (TRUE);
}

bool_t
xdr_getcredres(XDR *xdrs, getcredres *objp)
{
	if (!xdr_keystatus(xdrs, &objp->status))
		return (FALSE);
	switch (objp->status) {
	case KEY_SUCCESS:
		if (!xdr_unixcred(xdrs, &objp->getcredres_u.cred))
			return (FALSE);
		break;
	}
	return (TRUE);
}

bool_t
xdr_key_netstarg(XDR *xdrs, key_netstarg *objp)
{
	if (!xdr_keybuf(xdrs, objp->st_priv_key))
		return (FALSE);
	if (!xdr_keybuf(xdrs, objp->st_pub_key))
		return (FALSE);


	if (!xdr_netnamestr(xdrs, &objp->st_netname))
		return (FALSE);

	return (TRUE);
}

bool_t
xdr_key_netstres(XDR *xdrs, key_netstres *objp)
{
	if (!xdr_keystatus(xdrs, &objp->status))
		return (FALSE);
	switch (objp->status) {
	case KEY_SUCCESS:
		if (!xdr_key_netstarg(xdrs, &objp->key_netstres_u.knet))
			return (FALSE);
		break;
	}
	return (TRUE);
}
