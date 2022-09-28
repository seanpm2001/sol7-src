/*
 * Copyright (c) 1992-1995, by Sun Microsystems, Inc.
 * All rights reserved.
 */

#pragma	ident	"@(#)unix_update_authtok_nisplus.c	1.4	97/01/09 SMI"	/* PAM 2.6 */

#include "unix_headers.h"

#define	BOOLSTR(b) (((b) == TRUE) ? "TRUE" : "FALSE")

#ifdef PAM_NISPLUS
static int		update_attr(pam_handle_t *, char *, char **, char *,
				int, char **, char **,
				struct passwd *, int, int,
				nis_result *, int);
static int		talk_to_npd(pam_handle_t *, char *, char **, char *,
				char *, char *, char *,
				struct passwd *, int, int,
				char **, char **,
				nis_result *, int, int, bool_t *);
static char		*reencrypt_secret(char *, char *, char *, keylen_t,
						algtype_t, int);
static nis_error	revert2oldpasswd(char *, nis_result *);


/*
 * Given a cred table entry, return the secret key, auth type, key length
 * of secret key, and algorithm type of secret key.
 *
 * The type of key must be listed in the NIS+ security cf.
 *
 * Return TRUE on success and FALSE on failure.
 */
bool_t
extract_sec_keyinfo(
	nis_object *cred_entry,	/* in */
	char **seckey,		/* out */
	char **authtype,	/* out */
	keylen_t *keylen,	/* out */
	algtype_t *algtype,	/* out */
	int debug)		/* in */
{
	char mechalias[MECH_MAXALIASNAME+1];

	if ((*authtype = ENTRY_VAL(cred_entry, 1)) == NULL) {
		syslog(LOG_ERR, "auth type field is empty for cred entry");
		return (FALSE);
	}

	/* Don't need the "local" unix system cred. */
	if (strncmp(*authtype, "LOCAL", sizeof ("LOCAL")) == 0) {
		return (FALSE);
	}

	if (!__nis_authtype2mechalias(*authtype, mechalias,
					sizeof (mechalias))) {
		syslog(LOG_ERR,
			"can't convert authtype '%s' to mechanism alias",
			*authtype);
		return (FALSE);
	}

	/* Make sure the mech is in the NIS+ security cf. */
	if (__nis_translate_mechanism(mechalias, keylen, algtype) < 0) {
		syslog(LOG_WARNING,
		"can't convert mechanism alias '%s' to keylen and algtype",
			mechalias);
		return (FALSE);
	}

	if ((*seckey = ENTRY_VAL(cred_entry, 4)) == NULL) {
		return (FALSE);
	}

	if (debug)
		syslog(LOG_DEBUG,
		"PAM: extract_sec_keyinfo: returning SUCCESS");

	return (TRUE);
}

static int
update_cred_from_client(
	char		*prognamep,
	char		*usrname,
	char		*nisdomain,
	char		*oldrpc,	/* old rpc passwd: clear version */
	char		*new,		/* new passwd: clear version */
	bool_t		revert_on_failure,
	int		privileged,
	pam_handle_t	*pamh,
	nis_result	*passwd_res,
	nis_result	*cred_res,
	bool_t		pk_creds_exist,
	int		debug)
{
	entry_col	ecol[5];
	nis_object	*eobj;
	char		mname[NIS_MAXNAMELEN];
	char 		messages[PAM_MAX_NUM_MSG][PAM_MAX_MSG_SIZE];
	nis_result	*mres;
	int reencrypt_tries = 0;
	int reencrypt_success = 0;
	int mod_entry = 0;
	int i;

	/*
	 * Bail out if user doesn't have credentials.  This allows the
	 * users at installations that want to use password aging
	 * without credentials to still change their own password.
	 */
	if (! pk_creds_exist || ! cred_res)
		return (PAM_SUCCESS);

	/* same user check? */
	if (privileged) {
		snprintf(messages[0],
				sizeof (messages[0]), " ");
		snprintf(messages[1],
				sizeof (messages[1]),
				PAM_MSG(pamh, 113,
	"The NIS+ credential information for %s will not be changed."),
				usrname);
		snprintf(messages[2],
				sizeof (messages[2]),
				PAM_MSG(pamh, 114,
	"User %s must do the following to update his/her"), usrname);
		snprintf(messages[3],
				sizeof (messages[3]),
				PAM_MSG(pamh, 115,
	"credential information:"));
		snprintf(messages[4],
				sizeof (messages[4]),
				PAM_MSG(pamh, 116,
	"Use NEW passwd for login and OLD passwd for keylogin."));
		snprintf(messages[5],
				sizeof (messages[5]),
				PAM_MSG(pamh, 117,
	"Use \"chkey -p\" to reencrypt the credentials with the"));
		snprintf(messages[6],
				sizeof (messages[6]),
				PAM_MSG(pamh, 118,
	"new login passwd."));
		snprintf(messages[7],
				sizeof (messages[7]),
				PAM_MSG(pamh, 119,
	"The user must keylogin explicitly after their next login."),
				usrname);
		snprintf(messages[8],
				sizeof (messages[8]), " ");
		(void) __pam_display_msg(pamh, PAM_ERROR_MSG, 9, messages,
						NULL);
		return (PAM_SUCCESS);
	}

	/* normal user (not privileged) */
	for (i = 0; i < cred_res->objects.objects_len; i++) {
		nis_object *cred_entry = &(cred_res->objects.objects_val[i]);
		char *oldcryptsecret;
		char *newcryptsecret;
		char *authtype;
		keylen_t keylen;
		algtype_t algtype;

		if (!extract_sec_keyinfo(cred_entry, &oldcryptsecret,
					&authtype, &keylen, &algtype, debug))
			continue;

		reencrypt_tries++;
		if (!(newcryptsecret = reencrypt_secret(oldcryptsecret,
						oldrpc, new,
						keylen, algtype, debug)))
			continue;
		reencrypt_success++;

		/* update cred at server */
		(void) memset((char *)ecol, 0, sizeof (ecol));
		ecol[4].ec_value.ec_value_val = newcryptsecret;
		ecol[4].ec_value.ec_value_len = strlen(newcryptsecret) + 1;
		ecol[4].ec_flags = EN_CRYPT|EN_MODIFIED;
		eobj = nis_clone_object(cred_entry, NULL);
		if (eobj == NULL) {
			snprintf(messages[0], sizeof (messages[0]),
				PAM_MSG(pamh, 161,
						"%s: clone object failed."),
				prognamep);
			(void) __pam_display_msg(pamh, PAM_ERROR_MSG, 1,
							messages, NULL);
			free(newcryptsecret);
			return (PAM_AUTHTOK_RECOVERY_ERR);
		}
		eobj->EN_data.en_cols.en_cols_val = ecol;
		eobj->EN_data.en_cols.en_cols_len = 5;

	/*
	 * Now, if one were stupid enough to run nispasswd as/for root
	 * on some machine, it would have looked up and modified
	 * the password entry for "root" in passwd.org_dir. Now,
	 * should we really apply this new password to the cred
	 * entry for "<machinename>.<domainname>" ?
	 *
	 * POLICY: NO. We have no way of identifying a root user in
	 * NIS+ passwd table for each root@machinename. We do not
	 * allow the one password for [name=root], passwd.org_dir
	 * to apply to all "<machinename>.<domainname>" principals.
	 * If somebody let a root entry in passwd table, it probably
	 * has modify permissions for a distinguished NIS+ principal
	 * which we let be associated only with NIS+ principal
	 * root.<domainname>. Does this make any sense ?
	 */

	/* strlen("[cname=.,auth_type=.],cred.") + null + "." = 28 */
		if ((strlen(usrname) + strlen(nisdomain) +
			strlen(NIS_RES_OBJECT(passwd_res)->zo_domain) +
			strlen(authtype) + 28) >
			(size_t) NIS_MAXNAMELEN) {
			snprintf(messages[0], sizeof (messages[0]),
					PAM_MSG(pamh, 160,
					"%s: not enough buffer space."),
				prognamep);
			(void) __pam_display_msg(pamh, PAM_ERROR_MSG, 9,
							messages, NULL);
			free(newcryptsecret);
			return (PAM_AUTHTOK_RECOVERY_ERR);
		}
		snprintf(mname, sizeof (mname),
				"[cname=%s.%s,auth_type=%s],cred.%s",
				usrname, nisdomain, authtype,
				NIS_RES_OBJECT(passwd_res)->zo_domain);
		if (mname[strlen(mname) - 1] != '.')
			(void) strcat(mname, ".");
		mres = nis_modify_entry(mname, eobj, 0);
		if (mres->status == NIS_SUCCESS)
			mod_entry++;

		free(newcryptsecret);
		/* set column stuff to NULL so that we can free eobj */
		eobj->EN_data.en_cols.en_cols_val = NULL;
		eobj->EN_data.en_cols.en_cols_len = 0;
		(void) nis_destroy_object(eobj);
		(void) nis_freeresult(mres);
	}

	if (debug)
		syslog(LOG_DEBUG,
"PAM: update_cred_from_client: %d keys tried: %d reencrypted, %d modified\n",
			reencrypt_tries, reencrypt_success, mod_entry);

	/*
	 * If we tried to reencrypt some PK creds but were not successful
	 * with any of them, we return an error.
	 */
	if (reencrypt_tries > 0 && mod_entry == 0) {
		/* attempt to revert back to the old passwd */
		if (revert_on_failure &&
		    (revert2oldpasswd(usrname, passwd_res) != NIS_SUCCESS)) {

			snprintf(messages[0], sizeof (messages[0]),
					PAM_MSG(pamh, 120,
	"WARNING: Could not reencrypt any NIS+ credentials for %s;"),
					usrname);
			snprintf(messages[1], sizeof (messages[1]),
					PAM_MSG(pamh, 121,
	"login and keylogin passwords differ."));
			snprintf(messages[2], sizeof (messages[2]),
					PAM_MSG(pamh, 122,
	"Use NEW passwd for login and OLD passwd for keylogin."));
			snprintf(messages[3], sizeof (messages[3]),
					PAM_MSG(pamh, 117,
	"Use \"chkey -p\" to reencrypt the credentials with the"));
			snprintf(messages[4], sizeof (messages[4]),
					PAM_MSG(pamh, 123,
	"You must keylogin explicitly after your next login."));
			(void) __pam_display_msg(pamh, PAM_ERROR_MSG,
							5, messages, NULL);
			return (PAM_AUTHTOK_RECOVERY_ERR);
		}

		snprintf(messages[0], sizeof (messages[0]),
				PAM_MSG(pamh, 124,
	"%s%s: couldn't change password for %s."),
				prognamep, NISPLUS_MSG, usrname);
		snprintf(messages[1], sizeof (messages[1]),
				PAM_MSG(pamh, 125,
"Reason: failed to update the cred table with reencrypted credentials."));
		snprintf(messages[2], sizeof (messages[2]),
				PAM_MSG(pamh, 126,
	"Please notify your System Administrator."));
		(void) __pam_display_msg(pamh, PAM_ERROR_MSG, 3, messages,
						NULL);

		return (PAM_AUTHTOK_RECOVERY_ERR);
	}

	/*
	 * At least one (but not all) of the PK cred entries were
	 * successfully updated.
	 */
	if (mod_entry < reencrypt_tries) {
		snprintf(messages[0], sizeof (messages[0]),
				PAM_MSG(pamh, 134,
"WARNING: Some but not all NIS+ credentials were reencrypted for user %s"),
				usrname);
		(void) __pam_display_msg(pamh, PAM_TEXT_INFO, 1,
						messages, NULL);
		return (PAM_SUCCESS);
	}

	snprintf(messages[0], sizeof (messages[0]),
			PAM_MSG(pamh, 109,
	"NIS+ credential information changed for %s"), usrname);
	(void) __pam_display_msg(pamh, PAM_TEXT_INFO, 1,
					messages, NULL);
	return (PAM_SUCCESS);
}

int
update_authtok_nisplus(
	pam_handle_t *pamh,
	char *domain,
	char *field,
	char *data[],			/* Depending on field: it can store */
					/* encrypted new passwd or new */
					/* attributes */
	char *old,			/* old passwd: clear version */
	char *oldrpc,			/* old rpc passwd: clear version */
	char *new,			/* new passwd: clear version */
	int  opwcmd,			/* old passwd cmd: nispasswd */
	struct passwd *nisplus_pwd,
	int privileged,
	nis_result *passwd_res,
	nis_result *cred_res,
	int debug,
	int nowarn,
	bool_t pk_creds_exist)
{
	entry_col	ecol[8];
	nis_object	*eobj;
	nis_result	*mres;
	char		mname[NIS_MAXNAMELEN];
	nis_name	pwd_domain;
	nis_error 	niserr = 0;
	struct spwd	sp;
	char		shadow[80];
	int		rc = 0;
	char 		messages[PAM_MAX_NUM_MSG][PAM_MAX_MSG_SIZE];
	int		failover = FALSE;
	char		*gecos = NULL, *shell = NULL;
	char		*prognamep;
	char		*usrname;
	bool_t		cred_update_failed;
	int		estkey_stat;
	int		mod_creds_tries = 0;
	int		mod_creds_success = 0;
	int		reencrypt_tries = 0;
	int		reencrypt_success = 0;
	int		i;

	if ((rc = pam_get_item(pamh, PAM_SERVICE, (void **)&prognamep))
							!= PAM_SUCCESS ||
	    (rc = pam_get_item(pamh, PAM_USER, (void **)&usrname))
							!= PAM_SUCCESS)
		return (rc);

	/*
	 * Passwd is setuid program. We want the real user to send out
	 * any nis+ requests. The correct identity should have been set
	 * in ck_perm() when checking privilege.
	 */
	if (debug) {
		syslog(LOG_DEBUG,
"PAM: update_authtok_nisp: field = %s, opwcmd = %s, priv = %d,pk_creds_ex = %s",
			field ? field : "NULL",
			BOOLSTR(opwcmd),
			privileged,
			BOOLSTR(pk_creds_exist));
		syslog(LOG_DEBUG,
"PAM: update_authtok_nisplus: the effective uid while updating NIS+ pw is %d",
			geteuid());
	}

	if (opwcmd == FALSE) {
		/*
		 * Attempt to let NIS+ NPD do the password update.
		 * If the passwd entry is not present (in passwd_res)
		 *	try NPD for the local domain.
		 * If the passwd entry is present call NPD in the
		 *	domain the passwd entry resides in.
		 *	NPD wants only the domainname so strip off
		 *	the org_dir portion of the passwd directory.
		 */
		if (passwd_res == NULL || passwd_res->status != NIS_SUCCESS) {
			/*
			 * CAVEAT:
			 * Should never get here; ck_perm() should fail.
			 *
			 * It is a waste of time to try NPD for some values
			 * of passwd_res->status; additional checks advised
			 * if ever it is possible to get here.
			 */
			rc = talk_to_npd(pamh, field, data, domain, usrname,
				old, new, nisplus_pwd, failover, privileged,
				&shell, &gecos, passwd_res, debug, nowarn,
				&cred_update_failed);
		} else {
			pwd_domain = NIS_RES_OBJECT(passwd_res)->zo_domain;
			if (strcmp(nis_leaf_of(pwd_domain), "org_dir") == 0) {
				pwd_domain = nis_domain_of(
					NIS_RES_OBJECT(passwd_res)->zo_domain);
			}

			rc = talk_to_npd(pamh, field, data, pwd_domain,
						usrname, old, new, nisplus_pwd,
						failover, privileged,
						&shell, &gecos, passwd_res,
						debug, nowarn,
						&cred_update_failed);
		}

		if (rc == PAM_SUCCESS || rc == PAM_NISPLUS_PARTIAL_SUCCESS) {
			snprintf(messages[0],
				sizeof (messages[0]),
				PAM_MSG(pamh, 108,
				"NIS+ password information changed for %s"),
				usrname);
			(void) __pam_display_msg(pamh, PAM_TEXT_INFO,
				1, messages, NULL);

			if (rc == PAM_NISPLUS_PARTIAL_SUCCESS &&
			    cred_update_failed) {
				/*
				 * Attempt to establish Secure RPC credentials
				 * with the user's old password.
				 */
				estkey_stat = establish_key(NULL, geteuid(),
								oldrpc, 1,
								NULL, 0);
				if (estkey_stat == ESTKEY_NOCREDENTIALS)
					/*
					 * User has no credentias, no need
					 * to reencrypt.
					 */
					return (PAM_SUCCESS);
				else if ((estkey_stat != ESTKEY_SUCCESS) &&
						(estkey_stat !=
							ESTKEY_ALREADY)) {
					switch (estkey_stat) {
					case ESTKEY_BADPASSWD:
						snprintf(messages[0],
							sizeof (messages[0]),
								PAM_MSG(pamh,
									56,
		"Password does not decrypt any secret key for %s."),
								usrname);
						(void) __pam_display_msg(
							pamh,
							PAM_ERROR_MSG, 1,
							messages, NULL);
						break;
					case ESTKEY_CANTSETKEY:
						snprintf(messages[0],
							sizeof (messages[0]),
							PAM_MSG(pamh, 57,
	"Could not set secret key for %s. The key server may be down."),
							usrname);
						(void) __pam_display_msg(
							pamh,
							PAM_ERROR_MSG, 1,
							messages, NULL);
						break;
					case PAM_BUF_ERR:
						snprintf(messages[0],
							sizeof (messages[0]),
							PAM_MSG(pamh, 159,
				"Internal memory allocation failure."));
						(void) __pam_display_msg(
							pamh,
							PAM_ERROR_MSG, 1,
							messages, NULL);
						break;
					}

					snprintf(messages[0],
							sizeof (messages[0]),
									" ");
					snprintf(messages[1],
							sizeof (messages[1]),
							PAM_MSG(pamh, 113,
	"The NIS+ credential information for %s will not be changed."),
							usrname);
					snprintf(messages[2],
							sizeof (messages[2]),
							PAM_MSG(pamh, 114,
	"User %s must do the following to update his/her"), usrname);
					snprintf(messages[3],
							sizeof (messages[3]),
							PAM_MSG(pamh, 115,
	"credential information:"));
					snprintf(messages[4],
							sizeof (messages[4]),
							PAM_MSG(pamh, 116,
	"Use NEW passwd for login and OLD passwd for keylogin."));
					snprintf(messages[5],
							sizeof (messages[5]),
							PAM_MSG(pamh, 117,
	"Use \"chkey -p\" to reencrypt the credentials with the"));
					snprintf(messages[6],
							sizeof (messages[6]),
							PAM_MSG(pamh, 118,
	"new login passwd."));
					snprintf(messages[7],
							sizeof (messages[7]),
							PAM_MSG(pamh, 119,
	"The user must keylogin explicitly after their next login."),
							usrname);
					snprintf(messages[8],
							sizeof (messages[8]),
								" ");
					(void) __pam_display_msg(pamh,
								PAM_ERROR_MSG,
								9, messages,
								NULL);
					return (PAM_SUCCESS);
				}

				/*
				 * npd cred update failed, try updating
				 * from client.
				 */
				if (debug)
					syslog(LOG_DEBUG,
"PAM: update_authtok_nisplus: npd cred update failed, try updating from clnt");

				return (update_cred_from_client(
						prognamep, usrname,
						domain, oldrpc,
						new, FALSE, privileged,
						pamh, passwd_res, cred_res,
						pk_creds_exist, debug));
			} else if (rc == PAM_SUCCESS) {
				snprintf(messages[0],
					sizeof (messages[0]),
					PAM_MSG(pamh, 109,
				"NIS+ credential information changed for %s"),
					usrname);
				(void) __pam_display_msg(
					pamh, PAM_TEXT_INFO, 1,
					messages, NULL);
			}
			return (PAM_SUCCESS);
		}
		/* failover to use old protocol */
		if (debug)
			syslog(LOG_DEBUG,
			    "Failed to use new passwd update protocol");

		/*
		 * There are two reasons we will get here:
		 * 1. passwd, shell, gecos update failed (true failover)
		 * 2. we are updating passwd attrs other than the above
		 *    three attrs. In this case, rc is equal to PAM_PERM_DENIED
		 *    (i.e. attrs not supported by new protocol)
		 */
		failover = TRUE;
	}

	if (strcmp(field, "passwd") == 0) {
		/*
		 * Old style nisplus update
		 *
		 * Obtain the old aging information. And modify, if need be,
		 * on top. At least the lstchg field needs to be changed.
		 */
		/* old protocol requires user credential info */
		if (cred_res == NULL || cred_res->status != NIS_SUCCESS ||
		    !pk_creds_exist) {
			syslog(LOG_ERR, "%s %s: %s", prognamep, NISPLUS_MSG,
				"Failover: user credential is required.");
			return (PAM_AUTHTOK_RECOVERY_ERR);
		}

		nisplus_populate_age(NIS_RES_OBJECT(passwd_res), &sp);


		/* update passwd at server */
		(void) memset((char *)ecol, 0, sizeof (ecol));
		ecol[1].ec_value.ec_value_val = *data;
		ecol[1].ec_value.ec_value_len = strlen(*data) + 1;
		ecol[1].ec_flags = EN_CRYPT|EN_MODIFIED;

		/* update last change field */
		sp.sp_lstchg = DAY_NOW;
		if (sp.sp_max == 0) {
			/* passwd was forced to changed: turn off aging */
			sp.sp_max = -1;
			sp.sp_min = -1;
		}

		/* prepare shadow column */
		if (sp.sp_expire == -1) {
			snprintf(shadow,
				sizeof (shadow),
				"%ld:%ld:%ld:%ld:%ld::%lu",
				sp.sp_lstchg,
				sp.sp_min,
				sp.sp_max,
				sp.sp_warn,
				sp.sp_inact,
				sp.sp_flag);
		} else {
			snprintf(shadow,
				sizeof (shadow),
				"%ld:%ld:%ld:%ld:%ld:%ld:%lu",
				sp.sp_lstchg,
				sp.sp_min,
				sp.sp_max,
				sp.sp_warn,
				sp.sp_inact,
				sp.sp_expire,
				sp.sp_flag);
		}
		ecol[7].ec_value.ec_value_val = shadow;
		ecol[7].ec_value.ec_value_len = strlen(shadow) + 1;
		ecol[7].ec_flags = EN_CRYPT|EN_MODIFIED;

		/*
		 * build entry based on the one we got back from the server
		 */
		eobj = nis_clone_object(NIS_RES_OBJECT(passwd_res), NULL);
		if (eobj == NULL) {
			syslog(LOG_ERR, "%s %s: %s", prognamep, NISPLUS_MSG,
				"clone object failed");
			return (PAM_AUTHTOK_RECOVERY_ERR);
		}
		eobj->EN_data.en_cols.en_cols_val = ecol;
		eobj->EN_data.en_cols.en_cols_len = 8;

		/* strlen("[name=],.") + null + "." = 11 */
		if ((strlen(usrname) +
		    strlen(NIS_RES_OBJECT(passwd_res)->zo_name) +
		    strlen(NIS_RES_OBJECT(passwd_res)->zo_domain) + 11) >
			(size_t) NIS_MAXNAMELEN) {
			syslog(LOG_ERR, "%s %s: %s", prognamep, NISPLUS_MSG,
				"NIS+ name too long");
			return (PAM_BUF_ERR);
		}
		snprintf(mname, sizeof (mname), "[name=%s],%s.%s", usrname,
		    NIS_RES_OBJECT(passwd_res)->zo_name,
		    NIS_RES_OBJECT(passwd_res)->zo_domain);
		if (mname[strlen(mname) - 1] != '.')
			(void) strcat(mname, ".");
		mres = nis_modify_entry(mname, eobj, 0);

		/*
		 * It is possible that we have permission to modify the
		 * encrypted password but not the shadow column in the
		 * NIS+ table. In this case, we should try updating only
		 * the password field and not the aging stuff (lstchg).
		 * With the current NIS+ passwd table format, this would
		 * be the case most of the times.
		 */
		if (mres->status == NIS_PERMISSION) {
			ecol[7].ec_flags = 0;
			mres = nis_modify_entry(mname, eobj, 0);
			if (mres->status != NIS_SUCCESS) {
				snprintf(messages[0],
					sizeof (messages[0]),
					PAM_MSG(pamh, 111,
				"%s %s: Password information update failed"),
					prognamep, NISPLUS_MSG);
				(void) __pam_display_msg(pamh, PAM_ERROR_MSG,
					1, messages, NULL);

				(void) nis_freeresult(mres);
				return (PAM_AUTHTOK_RECOVERY_ERR);
			}
		}
		/* set column stuff to NULL so that we can free eobj */
		eobj->EN_data.en_cols.en_cols_val = NULL;
		eobj->EN_data.en_cols.en_cols_len = 0;
		(void) nis_destroy_object(eobj);
		(void) nis_freeresult(mres);

		snprintf(messages[0],
			sizeof (messages[0]),
			PAM_MSG(pamh, 112,
			"NIS+ password information changed for %s"),
			usrname);
		(void) __pam_display_msg(pamh, PAM_TEXT_INFO,
			1, messages, NULL);

		if (privileged) {
			snprintf(messages[0], sizeof (messages[0]), " ");
			snprintf(messages[1],
				sizeof (messages[1]),
				PAM_MSG(pamh, 113,
	"The NIS+ credential information for %s will not be changed."),
				usrname);
			snprintf(messages[2],
				sizeof (messages[2]),
				PAM_MSG(pamh, 114,
	"User %s must do the following to update his/her"), usrname);
			snprintf(messages[3],
				sizeof (messages[3]),
				PAM_MSG(pamh, 115,
	"credential information:"));
			snprintf(messages[4],
				sizeof (messages[4]),
				PAM_MSG(pamh, 116,
	"Use NEW passwd for login and OLD passwd for keylogin."));
			snprintf(messages[5],
				sizeof (messages[5]),
				PAM_MSG(pamh, 117,
	"Use \"chkey -p\" to reencrypt the credentials with the"));
			snprintf(messages[6],
				sizeof (messages[6]),
				PAM_MSG(pamh, 118,
	"new login passwd."));
			snprintf(messages[7],
				sizeof (messages[7]),
				PAM_MSG(pamh, 119,
	"The user must keylogin explicitly after their next login."),
				usrname);
			snprintf(messages[8], sizeof (messages[8]), " ");
			(void) __pam_display_msg(pamh, PAM_ERROR_MSG,
				9, messages, NULL);

			return (PAM_SUCCESS);
		}

		/* update cred(s) at server */
		for (i = 0; i < cred_res->objects.objects_len; i++) {
			nis_object *cred_entry;
			char *oldcryptsecret;
			char *newcryptsecret = NULL;
			char *authtype;
			keylen_t keylen;
			algtype_t algtype;


			cred_entry = &(cred_res->objects.objects_val[i]);
			if (!extract_sec_keyinfo(cred_entry, &oldcryptsecret,
					&authtype, &keylen, &algtype, debug))
				continue;

			reencrypt_tries++;
			if (!(newcryptsecret = reencrypt_secret(oldcryptsecret,
							oldrpc, new, keylen,
							algtype, debug)))
				continue;
			reencrypt_success++;

			(void) memset((char *)ecol, 0, sizeof (ecol));
			ecol[4].ec_value.ec_value_val = newcryptsecret;
			ecol[4].ec_value.ec_value_len = strlen(newcryptsecret)
							+ 1;
			ecol[4].ec_flags = EN_CRYPT|EN_MODIFIED;
			eobj = nis_clone_object(cred_entry, NULL);
			if (eobj == NULL) {
				syslog(LOG_ERR, "%s %s: %s",
					prognamep, NISPLUS_MSG,
					"clone object failed");
				if (newcryptsecret)
					free(newcryptsecret);
				return (PAM_AUTHTOK_RECOVERY_ERR);
			}
			eobj->EN_data.en_cols.en_cols_val = ecol;
			eobj->EN_data.en_cols.en_cols_len = 5;

		/*
		 * Now, if one were stupid enough to run nispasswd as/for root
		 * on some machine, it would have looked up and modified
		 * the password entry for "root" in passwd.org_dir. Now,
		 * should we really apply this new password to the cred
		 * entry for "<machinename>.<domainname>" ?
		 *
		 * POLICY: NO. We have no way of identifying a root user in
		 * NIS+ passwd table for each root@machinename. We do not
		 * allow the one password for [name=root], passwd.org_dir
		 * to apply to all "<machinename>.<domainname>" principals.
		 * If somebody let a root entry in passwd table, it probably
		 * has modify permissions for a distinguished NIS+ principal
		 * which we let be associated only with NIS+ principal
		 * root.<domainname>. Does this make any sense ?
		 */

		/* strlen("[cname=.,auth_type=.],.") + null + "." = 23 */
			if ((strlen(ENTRY_VAL(cred_entry, 0)) +
				strlen(cred_entry->zo_name) +
				strlen(cred_entry->zo_domain) +
				+ strlen(authtype) + 23) >
				(size_t) NIS_MAXNAMELEN) {
				syslog(LOG_ERR, "%s %s: %s",
					prognamep, NISPLUS_MSG,
					"NIS+ name too long");
				if (newcryptsecret)
					free(newcryptsecret);
				return (PAM_BUF_ERR);
			}
			snprintf(mname,
					sizeof (mname),
					"[cname=%s,auth_type=%s],%s.%s",
					ENTRY_VAL(cred_entry, 0),
					authtype,
					cred_entry->zo_name,
					cred_entry->zo_domain);
			if (mname[strlen(mname) - 1] != '.')
				(void) strcat(mname, ".");
			mod_creds_tries++;
			if (debug)
				syslog(LOG_DEBUG,
				"PAM: update_authtok_nisplus: modify '%s'",
					mname);
			mres = nis_modify_entry(mname, eobj, 0);
			if (mres->status == NIS_SUCCESS)
				mod_creds_success++;

			if (newcryptsecret)
				free(newcryptsecret);
			/* set column stuff to NULL so that we can free eobj */
			eobj->EN_data.en_cols.en_cols_val = NULL;
			eobj->EN_data.en_cols.en_cols_len = 0;
			(void) nis_destroy_object(eobj);
			(void) nis_freeresult(mres);

		}

		if (debug)
			syslog(LOG_DEBUG,
"PAM: update_authtok_nisplus:nis+ old style: %d/%d reencr succ, %d/%d mod succ",
				reencrypt_success, reencrypt_tries,
				mod_creds_success, mod_creds_tries);

		/* PK creds exist but we could not update any of them. */
		if (reencrypt_tries > 0 && mod_creds_success == 0) {

			/* attempt to revert back to the old passwd */
			niserr = revert2oldpasswd(usrname, passwd_res);

			if (niserr != NIS_SUCCESS) {
				snprintf(messages[0], sizeof (messages[0]), "");
				snprintf(messages[1],
					sizeof (messages[1]),
					PAM_MSG(pamh, 120,
		"WARNING: Could not reencrypt NIS+ credentials for %s;"),
					usrname);
				snprintf(messages[2],
					sizeof (messages[2]),
					PAM_MSG(pamh, 121,
		"login and keylogin passwords differ."));
				snprintf(messages[3],
					sizeof (messages[3]),
					PAM_MSG(pamh, 122,
		"Use NEW passwd for login and OLD passwd for keylogin."));
				snprintf(messages[4],
					sizeof (messages[4]),
					PAM_MSG(pamh, 117,
		"Use \"chkey -p\" to reencrypt the credentials with the"));
				snprintf(messages[5],
					sizeof (messages[5]),
					PAM_MSG(pamh, 118,
		"new login passwd."));
				snprintf(messages[6],
					sizeof (messages[6]),
					PAM_MSG(pamh, 123,
		"You must keylogin explicitly after your next login."));
				snprintf(messages[7], sizeof (messages[7]), "");
				(void) __pam_display_msg(pamh, PAM_ERROR_MSG,
				    8, messages, NULL);

				return (PAM_AUTHTOK_RECOVERY_ERR);
			}

			snprintf(messages[0],
				sizeof (messages[0]),
				PAM_MSG(pamh, 124,
				"%s %s: couldn't change password for %s."),
				prognamep, NISPLUS_MSG, usrname);
			snprintf(messages[1],
				sizeof (messages[1]),
				PAM_MSG(pamh, 125,
"Reason: failed to update the cred table with reencrypted credentials."));
			snprintf(messages[2],
				sizeof (messages[2]),
				PAM_MSG(pamh, 126,
"Please notify your System Administrator."));
			(void) __pam_display_msg(pamh, PAM_ERROR_MSG,
				3, messages, NULL);

			(void) nis_freeresult(mres);
			return (PAM_AUTHTOK_RECOVERY_ERR);
		}

		/*
		 * We were able to update some creds but check if all were
		 * successful.
		 */
		if (reencrypt_tries < mod_creds_success) {
			snprintf(messages[0],
				sizeof (messages[0]),
				PAM_MSG(pamh, 134,
"WARNING: Some but not all NIS+ credentials were reencrypted for user %s"),
						usrname);
				(void) __pam_display_msg(pamh, PAM_ERROR_MSG,
							1, messages, NULL);
		}

		snprintf(messages[0],
			sizeof (messages[0]),
			PAM_MSG(pamh, 109,
			"NIS+ credential information changed for %s"),
			usrname);
		(void) __pam_display_msg(pamh, PAM_ERROR_MSG,
			1, messages, NULL);
	} else
		return (update_attr(pamh, field, data, usrname, 1, &shell,
			&gecos, nisplus_pwd, failover, privileged,
			passwd_res, nowarn));

	return (PAM_SUCCESS);
}


/*
 * The function uses the new protocol to update passwd attributes via
 * passwd daemon.
 */
static int
talk_to_npd(pam_handle_t *pamh, char *field, char **data, char *domain,
	char *user, char *oldpass, char *newpass,
	struct passwd *nisplus_pwd, int failover, int privileged,
	char **shell, char **gecos,
	nis_result *passwd_res, int debug, int nowarn,
	bool_t *cred_update_failed)
{
	CLIENT		*clnt = NULL;
	char		*old_passwd = NULL;
	char		*srv_pubkey = NULL;  /* server public key hex string */
	char		*u_pubkey = NULL;    /* user public key hex string */
	char		*u_seckey = NULL;    /* user secret key hex string */
	des_block	deskeys[3];
	unsigned long 	ident = 0, randval = 0;
	int 		error = 0, status;
	int		retcode = PAM_SYSTEM_ERR;
	char		messages[PAM_MAX_NUM_MSG][PAM_MAX_MSG_SIZE];
	nispasswd_error		*errlist = NULL;
	nispasswd_error		*p = NULL;
	char			*key_type = NULL; /* key len/type "alias" str */
	keylen_t		srv_keylen;	/* server key lenth (bits) */
	algtype_t		srv_keyalgtype;	/* server key algorithm type */

	if (user == NULL || domain == NULL || *domain == '\0') {
		retcode = PAM_AUTHTOK_RECOVERY_ERR;
		goto out;
	}

	if (debug)
		syslog(LOG_DEBUG, "PAM: talk_to_npd: domain=%s, user=%s",
			domain, user);

	/*
	 * Let's do a quick check whether the attrs are really of interest.
	 * We don't want to prompt for user passwd which is sure not to be
	 * used.
	 */
	retcode = update_attr(pamh, field, data, NULL, 0, NULL, NULL,
			nisplus_pwd, failover, privileged,
			passwd_res, nowarn);
	if (retcode != PAM_SUCCESS)
		goto out;

	if (oldpass == NULL || oldpass[0] == '\0') {
		/*
		 * This is possible from unix_set_authtokattr().
		 * Old passwd is required to change any attributes.
		 * This is imposed by new protocol to support users
		 * without credentials.
		 */

		retcode = __pam_get_authtok(pamh, PAM_PROMPT, 0, PASSWORD_LEN,
			PAM_MSG(pamh, 63, "Enter login(NIS+) password: "),
			&old_passwd);
		if (retcode != PAM_SUCCESS)
			goto out;
	} else {
		old_passwd = strdup(oldpass);
		if (old_passwd == NULL) {
			retcode = PAM_BUF_ERR;
			goto out;
		}
	}

	/* get gecos, shell and other */
	retcode = update_attr(pamh, field, data, user, 0, shell, gecos,
		nisplus_pwd, failover, privileged,
		passwd_res, nowarn);
	if (retcode != PAM_SUCCESS)
		goto out;

	if (npd_makeclnthandle(domain, &clnt, &srv_pubkey,
				&srv_keylen, &srv_keyalgtype,
				&key_type) == FALSE) {
		syslog(LOG_ERR,
		"Couldn't make a client handle to NIS+ password daemon");
		retcode = PAM_AUTHTOK_RECOVERY_ERR;
		goto out;
	}

	if ((u_pubkey = malloc(BITS2NIBBLES(srv_keylen) + 1)) == NULL) {
		syslog(LOG_CRIT, "malloc failed");
		retcode = PAM_AUTHTOK_RECOVERY_ERR;
		goto out;
	}
	if ((u_seckey = malloc(BITS2NIBBLES(srv_keylen) + 1)) == NULL) {
		syslog(LOG_CRIT, "malloc failed");
		retcode = PAM_AUTHTOK_RECOVERY_ERR;
		goto out;
	}

	if (debug)
		syslog(LOG_DEBUG,
	"PAM: talk_to_npd: generate key-pair for key-alg %d-%d (key_type = %s)",
			srv_keylen, srv_keyalgtype,
			key_type ? key_type : "NULL");

	/* generate a key-pair for this user */
	/* again: doesn't need to generate a new pair of keys (?) */
	if (__gen_dhkeys_g(u_pubkey, u_seckey, srv_keylen, srv_keyalgtype,
				old_passwd) == 0) {
		syslog(LOG_ERR,
		"Couldn't generate a D-H key-pair (len = %d, type = %d)",
			srv_keylen, srv_keyalgtype);
		retcode = PAM_AUTHTOK_RECOVERY_ERR;
		goto out;
	}

	/*
	 * get the common des key(s) from the servers' pubkey and
	 * the users secret key
	 */
	if (__gen_common_dhkeys_g(srv_pubkey, u_seckey, srv_keylen,
				srv_keyalgtype, deskeys,
				AUTH_DES_KEY(srv_keylen, srv_keyalgtype)
							? 1 : 3) == 0) {
		syslog(LOG_ERR,
		"Couldn't get a common DES key (keylen = %d, algtype = %d)",
			srv_keylen, srv_keyalgtype);
		retcode = PAM_AUTHTOK_RECOVERY_ERR;
		goto out;
	}

again:
	status = nispasswd_auth(user, domain, old_passwd, u_pubkey, key_type,
				srv_keylen, srv_keyalgtype, deskeys, clnt,
				&ident, &randval, &error);
	if (status == NPD_FAILED) {
		switch (error) {
		case NPD_NOTMASTER:
			syslog(LOG_ALERT,
	"Password update daemon is not running with NIS+ master server");
			retcode = PAM_AUTHTOK_RECOVERY_ERR;
			goto out;
		case NPD_SYSTEMERR:
			syslog(LOG_ALERT, "NIS+ system error");
			retcode = PAM_AUTHTOK_RECOVERY_ERR;
			goto out;
		case NPD_IDENTINVALID:
			syslog(LOG_ALERT, "NIS+ identifier invalid");
			retcode = PAM_AUTHTOK_RECOVERY_ERR;
			goto out;
		case NPD_PASSINVALID:
			syslog(LOG_ALERT, "NIS+ password invalid");
			retcode = PAM_AUTHTOK_RECOVERY_ERR;
			goto out;
		case NPD_NOSUCHENTRY:
			syslog(LOG_ALERT, "No NIS+ password entry for %s",
				user);
			retcode = PAM_AUTHTOK_RECOVERY_ERR;
			goto out;
		case NPD_NISERROR:
			syslog(LOG_ALERT, "NIS+ error");
			retcode = PAM_AUTHTOK_RECOVERY_ERR;
			goto out;
		case NPD_CKGENFAILED:
			syslog(LOG_ALERT,
			    "Couldn't generate a common DES key");
			retcode = PAM_AUTHTOK_RECOVERY_ERR;
			goto out;
		case NPD_NOPASSWD:
			syslog(LOG_ALERT, "No NIS+ password for %s", user);
			retcode = PAM_AUTHTOK_RECOVERY_ERR;
			goto out;
		case NPD_NOTAGED:
			syslog(LOG_ALERT, "NIS+ passwd has not aged enough");
			retcode = PAM_AUTHTOK_RECOVERY_ERR;
			goto out;
		case NPD_NOSHDWINFO:
			syslog(LOG_ALERT, "No shadow password information");
			retcode = PAM_AUTHTOK_RECOVERY_ERR;
			goto out;
		default:
			syslog(LOG_ALERT, "NIS+ fatal error: %d", error);
			retcode = PAM_AUTHTOK_RECOVERY_ERR;
			goto out;
		}
	}
	if (status == NPD_TRYAGAIN) {
		/*
		 * call nispasswd_auth() again after getting another
		 * passwd. Note that ident is now non-zero.
		 */
		if (debug)
			syslog(LOG_DEBUG,
			    "status=tryagain; ident=%ld, randval=%ld",
			    ident, randval);

		if (old_passwd) {
			memset(old_passwd, 0, strlen(old_passwd));
			free(old_passwd);
		}

		/* wrong passwd: get auth token again */
		snprintf(messages[0],
			sizeof (messages[0]),
			PAM_MSG(pamh, 127,
			"NIS+ Password incorrect: try again"));
		(void) __pam_display_msg(pamh, PAM_ERROR_MSG, 1,
		    messages, NULL);

		retcode = __pam_get_authtok(pamh, PAM_PROMPT, 0, PASSWORD_LEN,
			PAM_MSG(pamh, 63, "Enter login(NIS+) password: "),
			&old_passwd);
		if (retcode != PAM_SUCCESS)
			goto out;

		goto again;
	}
	if (status == NPD_SUCCESS) {
		/* send the new passwd & other changes */
		if (debug)
			syslog(LOG_DEBUG,
		"PAM: talk_to_npd: status=success; ident=%ld, randval=%ld",
				ident, randval);
		if (newpass == NULL) {
			/*
			 * This is possible from unix_set_authtokattr().
			 * Just use the same passwd so that we have a
			 * meaningful passwd field.
			 */
			newpass = old_passwd;
		}

		/* gecos and shell could be NULL if we just change passwd */
		status = nispasswd_pass(clnt, ident, randval, &deskeys[0],
				newpass, *gecos, *shell, &error, &errlist);

		if (status == NPD_FAILED) {
			snprintf(messages[0],
				sizeof (messages[0]),
				PAM_MSG(pamh, 128,
				"NIS+ password information update failed \
while talking to NIS+ passwd daemon"));
			(void) __pam_display_msg(pamh, PAM_ERROR_MSG,
				1, messages, NULL);
			if (debug)
				syslog(LOG_DEBUG, "PAM: talk_to_npd: error=%d",
					error);
			retcode = PAM_AUTHTOK_RECOVERY_ERR;
			goto out;
		}
		/*
		 * WHAT SHOULD BE DONE FOR THE PARTIAL SUCCESS CASE ??
		 * I'll just print out some messages
		 */
		if (status == NPD_PARTIALSUCCESS) {
			syslog(LOG_ALERT,
			"Password information is partially updated.");
			for (p = errlist; p != NULL; p = p->next) {
				if (p->npd_field == NPD_GECOS) {
					snprintf(messages[0],
						sizeof (messages[0]),
						PAM_MSG(pamh, 129,
	"GECOS information was not updated: check NIS+ permissions."));
					(void) __pam_display_msg(pamh,
						PAM_ERROR_MSG, 1,
						messages, NULL);
				} else if (p->npd_field == NPD_SHELL) {
					snprintf(messages[0],
						sizeof (messages[0]),
						PAM_MSG(pamh, 130,
	"SHELL information was not updated: check NIS+ permissions."));
					(void) __pam_display_msg(pamh,
						PAM_ERROR_MSG, 1,
						messages, NULL);
				} else if (p->npd_field == NPD_SECRETKEY) {
					*cred_update_failed = TRUE;
					snprintf(messages[0],
						sizeof (messages[0]),
						PAM_MSG(pamh, 131,
			"NIS+ Credential information was not updated."));
					(void) __pam_display_msg(pamh,
						PAM_ERROR_MSG, 1,
						messages, NULL);
				}
			}
			/* check for collision with PAM_* return code */
			(void) __npd_free_errlist(errlist);
			retcode = PAM_NISPLUS_PARTIAL_SUCCESS;
			goto out;
		}
		(void) __npd_free_errlist(errlist);
	}
	retcode = PAM_SUCCESS;
out:
	if (u_pubkey)
		free(u_pubkey);
	if (u_seckey)
		free(u_seckey);
	if (srv_pubkey)
		free(srv_pubkey);
	if (key_type)
		free(key_type);
	if (old_passwd) {
		memset(old_passwd, 0, strlen(old_passwd));
		free(old_passwd);
	}
	return (retcode);
}


static int
update_attr(pam_handle_t *pamh, char *field, char **data, char *usrname,
	int opwcmd, char **sh_p, char **gecos_p,
	struct passwd *nisplus_pwd, int failover, int privileged,
	nis_result *passwd_res, int nowarn)
{
	entry_col	ecol[8];
	nis_object	*eobj;
	nis_result	*mres;
	char		mname[NIS_MAXNAMELEN];
	struct spwd	sp;		/* new attr values in here */
	char		*value;
	int		maxdate;
	int		mindate;
	int		warndate;
	static char	lkstring[] = "*LK*"; /* ??? in header */
	int		flag = 0;	/* any change in shadow column */
	char		**data_p = data;
	char		shadow[80];
	char		*newhome;
	char		*newgecos;
	char		*newsh;
	char 		messages[PAM_MAX_NUM_MSG][PAM_MAX_MSG_SIZE];

	if (strcmp(field, "attr") == 0) {
		/*
		 * Obtain the old aging information. And modify, if need be,
		 * on top.
		 */
		if (opwcmd)
			nisplus_populate_age(NIS_RES_OBJECT(passwd_res), &sp);

		(void) memset((char *)ecol, 0, sizeof (ecol));
		while (*data != NULL) {
			/* AUTHTOK_DEL: not applicable */

			/* check attribute: AUTHTOK_LK */
			if ((value = attr_match("AUTHTOK_LK", *data))
								!= NULL) {
				/* new update protocol doesn't support this */
				if (opwcmd == FALSE)
					return (PAM_PERM_DENIED);

				if (strcmp(value, "1") == 0) {
					/* lock password */
					ecol[1].ec_value.ec_value_val =
					    &lkstring[0];
					ecol[1].ec_value.ec_value_len =
					    strlen(&lkstring[0]) + 1;
					ecol[1].ec_flags = EN_CRYPT|EN_MODIFIED;

					if (!(attr_find
					    ("AUTHTOK_EXP", data_p))) {
						sp.sp_lstchg = DAY_NOW;
						flag = 1;
					}
				}
				data++;
				continue;
			}

			/* check attribute: AUTHTOK_EXP */
			if ((value = attr_match("AUTHTOK_EXP", *data))
			    != NULL) {
				/* new update protocol doesn't support this */
				if (opwcmd == FALSE)
					return (PAM_PERM_DENIED);

				if (strcmp(value, "1") == 0) {
					/* expire password */
					sp.sp_lstchg = (long) 0;
					flag = 1;
				}
				data++;
				continue;
			}

			/* check attribute: AUTHTOK_MAXAGE */
			if ((value = attr_match("AUTHTOK_MAXAGE", *data))
			    != NULL) {
				/* new update protocol doesn't support this */
				if (opwcmd == FALSE)
					return (PAM_PERM_DENIED);

				/* set max field */
				maxdate = (int)atol(value);
				if (!(attr_find
					("AUTHTOK_MINAGE", data_p)) &&
					sp.sp_min == -1)
					sp.sp_min = 0;
				if (maxdate == -1) {	/* turn off aging */
					sp.sp_min = -1;
					sp.sp_warn = -1;
				} else if (sp.sp_max == -1)
					sp.sp_lstchg = DAY_NOW;

				sp.sp_max = maxdate;
				flag = 1;
				data++;
				continue;
			}

			/* check attribute: AUTHTOK_MINAGE */
			if ((value = attr_match("AUTHTOK_MINAGE", *data))
			    != NULL) {
				/* new update protocol doesn't support this */
				if (opwcmd == FALSE)
					return (PAM_PERM_DENIED);

				/* set min field */
				mindate = (int)atol(value);
				if (!(attr_find
					("AUTHTOK_MAXAGE", data_p)) &&
				    sp.sp_max == -1 && mindate != -1)
					return (PAM_AUTHTOK_ERR);
				sp.sp_min = mindate;
				flag = 1;
				data++;
				continue;
			}

			/* check attribute: AUTHTOK_WARNDATE */
			if ((value = attr_match("AUTHTOK_WARNDATE", *data))
			    != NULL) {
				/* new update protocol doesn't support this */
				if (opwcmd == FALSE)
					return (PAM_PERM_DENIED);

				/* set warn field */
				warndate = (int)atol(value);
				if (sp.sp_max == -1 && warndate != -1)
					return (PAM_AUTHTOK_ERR);
				sp.sp_warn = warndate;
				flag = 1;
				data++;
				continue;
			}

			if ((value = attr_match("AUTHTOK_SHELL", *data))
			    != NULL) {
				/* see if quick check */
				if (usrname == NULL)
					return (PAM_SUCCESS);

				if (nisplus_pwd == NULL && opwcmd) {
				    if (!nowarn) {
					snprintf(messages[0],
						sizeof (messages[0]),
						PAM_MSG(pamh, 132,
						"No NIS+ record"));
					(void) __pam_display_msg(pamh,
						PAM_ERROR_MSG, 1,
						messages, NULL);
				    }
				    return (PAM_AUTHTOK_RECOVERY_ERR);
				}

				/*
				 * If failover, we already got the shell info
				 * in "shell". Don't ask again.
				 */
				if (failover)
					newsh = *sh_p;
				else
					newsh = getloginshell(pamh,
					    nisplus_pwd->pw_shell,
					    privileged, nowarn);

				/* if NULL, shell unchanged */
				if (newsh == NULL)
					return (PAM_SUCCESS);

				if (opwcmd || failover) {
					ecol[6].ec_value.ec_value_val = newsh;
					ecol[6].ec_value.ec_value_len =
					    strlen(newsh) + 1;
					ecol[6].ec_flags = EN_MODIFIED;
				} else
					*sh_p = newsh;
				data++;
				continue;
			}

			if ((value = attr_match("AUTHTOK_HOMEDIR", *data))
			    != NULL) {
				/* new update protocol doesn't support this */
				if (opwcmd == FALSE)
					return (PAM_PERM_DENIED);

				/* home directory */
				if (nisplus_pwd == NULL) {
				    if (!nowarn) {
					snprintf(messages[0],
						sizeof (messages[0]),
						PAM_MSG(pamh, 132,
						"No NIS+ record"));
					(void) __pam_display_msg(pamh,
						PAM_ERROR_MSG, 1,
						messages, NULL);
				    }
				    return (PAM_AUTHTOK_RECOVERY_ERR);
				}
				newhome = gethomedir(pamh, nisplus_pwd->pw_dir,
						nowarn);
				/* if NULL, homedir unchanged */
				if (newhome == NULL)
					return (PAM_SUCCESS);
				ecol[5].ec_value.ec_value_val = newhome;
				ecol[5].ec_value.ec_value_len =
				    strlen(newhome) + 1;
				ecol[5].ec_flags = EN_MODIFIED;
				data++;
				continue;
			}

			if ((value = attr_match("AUTHTOK_GECOS", *data))
			    != NULL) {
				/* see if quick check */
				if (usrname == NULL)
					return (PAM_SUCCESS);

				/* finger information */
				if (nisplus_pwd == NULL && opwcmd) {
				    if (!nowarn) {
					snprintf(messages[0],
						sizeof (messages[0]),
						PAM_MSG(pamh, 132,
						"No NIS+ record"));
					(void) __pam_display_msg(pamh,
						PAM_ERROR_MSG, 1,
						messages, NULL);
				    }
				    return (PAM_AUTHTOK_RECOVERY_ERR);
				}
				if (failover) {
					newgecos = *gecos_p;
				} else {
					newgecos = getfingerinfo(pamh,
						nisplus_pwd->pw_gecos, nowarn);
				}

				/* if NULL, gecos unchanged */
				if (newgecos == NULL)
					return (PAM_SUCCESS);

				if (opwcmd || failover) {
					ecol[4].ec_value.ec_value_val =
					    newgecos;
					ecol[4].ec_value.ec_value_len =
					    strlen(newgecos) + 1;
					ecol[4].ec_flags = EN_MODIFIED;
				} else
					*gecos_p = newgecos;
				data++;
				continue;
			}
		} /* while */

		if (usrname == NULL)
			return (PAM_SUCCESS);

		if (flag && opwcmd) {
			/* prepare shadow column */
			if (sp.sp_expire == -1) {
				snprintf(shadow,
					sizeof (shadow),
					"%ld:%ld:%ld:%ld:%ld::%lu",
					sp.sp_lstchg,
					sp.sp_min,
					sp.sp_max,
					sp.sp_warn,
					sp.sp_inact,
					sp.sp_flag);
			} else {
				snprintf(shadow,
					sizeof (shadow),
					"%ld:%ld:%ld:%ld:%ld:%ld:%lu",
					sp.sp_lstchg,
					sp.sp_min,
					sp.sp_max,
					sp.sp_warn,
					sp.sp_inact,
					sp.sp_expire,
					sp.sp_flag);
			}
			ecol[7].ec_value.ec_value_val = shadow;
			ecol[7].ec_value.ec_value_len = strlen(shadow) + 1;
			ecol[7].ec_flags = EN_CRYPT|EN_MODIFIED;
		}

		if (opwcmd || failover) {
			eobj = nis_clone_object(NIS_RES_OBJECT(passwd_res),
			    NULL);
			if (eobj == NULL) {
				syslog(LOG_ERR, "NIS+ clone object failed");
				return (PAM_AUTHTOK_RECOVERY_ERR);
			}
			eobj->EN_data.en_cols.en_cols_val = ecol;
			eobj->EN_data.en_cols.en_cols_len = 8;

			/* strlen("[name=],.") + null + "." = 17 */
			if ((strlen(usrname) +
			    strlen(NIS_RES_OBJECT(passwd_res)->zo_name) +
			    strlen(NIS_RES_OBJECT(passwd_res)->zo_domain) +
			    17) > (size_t) NIS_MAXNAMELEN) {
				syslog(LOG_ERR, "NIS+ name too long");
				return (PAM_BUF_ERR);
			}
			snprintf(mname,
				sizeof (mname),
				"[name=%s],%s.%s", usrname,
				NIS_RES_OBJECT(passwd_res)->zo_name,
				NIS_RES_OBJECT(passwd_res)->zo_domain);
			if (mname[strlen(mname) - 1] != '.')
				(void) strcat(mname, ".");
			mres = nis_modify_entry(mname, eobj, 0);
			if (mres->status != NIS_SUCCESS) {
				snprintf(messages[0],
					sizeof (messages[0]),
					PAM_MSG(pamh, 133,
		"NIS+ password information update failed (update_attr)"));
				(void) __pam_display_msg(pamh,
					PAM_ERROR_MSG, 1, messages, NULL);

				return (PAM_AUTHTOK_RECOVERY_ERR);
			}

			snprintf(messages[0],
				sizeof (messages[0]),
				PAM_MSG(pamh, 112,
				"NIS+ password information changed for %s"),
				usrname);
			(void) __pam_display_msg(pamh, PAM_TEXT_INFO,
				1, messages, NULL);
		}
	}
	return (PAM_SUCCESS);
}

/*
 * Return reencrypted secret key.
 *
 * The memory for the returned reencrypted secret key should be set free
 * by the caller.
 */
static char *
reencrypt_secret(
	char *oldsecret,
	char *oldpass,
	char *newpass,
	keylen_t keylen,
	algtype_t algtype,
	int debug)
{
	char netname[MAXNETNAMELEN+1];
	char *encrypted_secret;

	if (!getnetname(netname)) {
		syslog(LOG_ERR,
			"PAM: reencrypt_secret:  getnetname failed");
		return (NULL);
	}

	if (debug)
		syslog(LOG_DEBUG,
"PAM: reencrypt_secret: call xdecrypt_g: keylen=%d, algtype=%d, netname=%s",
		keylen, algtype, netname);

	if (!xdecrypt_g(oldsecret, keylen, algtype, oldpass, netname, TRUE)) {
		syslog(LOG_INFO,
			"secret key decrypt failed for %s/%d-%d",
			netname, keylen, algtype);
		return (NULL);
	}

	if (!xencrypt_g(oldsecret, keylen, algtype, newpass, netname,
			&encrypted_secret, TRUE)) {
		syslog(LOG_ERR,
		"secret key encrypt failed for user '%s' for keytype %d-%d",
			netname, keylen, algtype);
		return (NULL);
	}

	return (encrypted_secret);
}


/*
 * Revert back to the old passwd
 */
static nis_error
revert2oldpasswd(char *usrname, nis_result *passwd_res)
{
	entry_col ecol[8];
	nis_object *eobj;
	nis_result *mres;
	char mname[NIS_MAXNAMELEN];

	/*
	 * clear column data
	 */
	(void) memset((char *) ecol, 0, sizeof (ecol));

	/*
	 * passwd (col 1)
	 */
	ecol[1].ec_value.ec_value_val =
		ENTRY_VAL(NIS_RES_OBJECT(passwd_res), 1);
	ecol[1].ec_value.ec_value_len =
		ENTRY_LEN(NIS_RES_OBJECT(passwd_res), 1);
	ecol[1].ec_flags = EN_CRYPT|EN_MODIFIED;

	/*
	 * build entry based on the global "passwd_res"
	 */
	eobj = nis_clone_object(NIS_RES_OBJECT(passwd_res), NULL);
	if (eobj == NULL)
		return (NIS_SYSTEMERROR);
	eobj->EN_data.en_cols.en_cols_val = ecol;
	eobj->EN_data.en_cols.en_cols_len = 8;

	snprintf(mname,
		sizeof (mname),
		"[name=%s],%s.%s", usrname,
		NIS_RES_OBJECT(passwd_res)->zo_name,
		NIS_RES_OBJECT(passwd_res)->zo_domain);

	mres = nis_modify_entry(mname, eobj, 0);
	return (mres->status);
}

#endif /* PAM_NISPLUS */
