/*
 *	Copyright (c) 1988-1993 Sun Microsystems Inc
 *	All Rights Reserved.
 *
 *	nisplus/getnetgrent.c -- "nisplus" backend for nsswitch "netgroup"
 *				 database
 *
 *	The API for netgroups differs sufficiently from that for the average
 *	getXXXbyYYY function that we use very few of the support routines in
 *	nisplus_common.h.
 *
 *	The implementation of setnetgrent()/getnetgrent() here follows the 4.x
 *	NIS (YP) code, inasmuch as the setnetgrent() routine does all the work
 *	of traversing the netgroup graph and building a (potentially large)
 *	list in memory, and getnetgrent() just steps down the list.
 *
 *	An alternative, and probably better, implementation would lazy-eval
 *	the netgroup graph in response to getnetgrent() calls (though
 *	setnetgrent() should still check for the top-level netgroup name
 *	and return NSS_SUCCESS / NSS_NOTFOUND).
 *
 * ===>	This file and the corresponding one in the "nis" backend contain
 *	substantial amounts of duplicate code.  If there's a tidy way to
 *	share the code, we should make use of it.
 */

#pragma ident	"@(#)getnetgrent.c	1.12	97/08/12 SMI"

#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include "nisplus_common.h"
#include "nisplus_tables.h"
#ifdef	UNDEF	/* <==== */
#include <ctype.h>
#include <malloc.h>
#endif	UNDEF	/* <==== */

/*
 * The nss_backend_t for a getnetgrent() sequence;  we actually give the
 *   netgroup frontend a pointer to one of these structures in response to
 *   a (successful) setnetgrent() call on the nis_netgr_be backend
 *   described further down in this file.
 */

/*
 * These are really "nisplus_getnetgr..." rather than "nis_getnetgr...",
 *   but they turn out to be the same, so why fiddle with them?
 */
struct nis_getnetgr_be;
typedef nss_status_t	(*nis_getnetgr_op_t)(struct nis_getnetgr_be *, void *);

struct nis_getnetgr_be {
	nis_getnetgr_op_t	*ops;
	nss_dbop_t		n_ops;
	/*
	 * State for set/get/endnetgrent()
	 */
	char			*netgroup;
	struct grouplist	*all_members;
	struct grouplist	*next_member;
};

struct grouplist {  /* One element of the list generated by a setnetgrent() */
	char			*triple[NSS_NETGR_N];
	struct	grouplist	*gl_nxt;
};

typedef const char	*ccp;
typedef int		(*top_down_lambda_t)(ccp triple[NSS_NETGR_N],
				void *iter_args, nss_status_t *return_val);

static nss_status_t getnetgr_end(struct nis_getnetgr_be *be, void *dummy);
static nss_status_t top_down(nisplus_backend_ptr_t be,
				const char **groups, int ngroups,
				top_down_lambda_t func, void *iter_args);

/* these are not ANSI: */

extern char *strdup(const char *);
extern int strcasecmp(const char *, const char *);
extern int strncasecmp(const char *, const char *, size_t);


static nss_status_t
getnetgr_set(be, a)
	struct nis_getnetgr_be	*be;
	void			*a;
{
	const char		*netgroup = (const char *) a;

	if (be->netgroup != 0 &&
	    strcmp(be->netgroup, netgroup) == 0) {
		/* We already have the member-list;  regurgitate it */
		be->next_member = be->all_members;
		return (NSS_SUCCESS);
	}
	return (NSS_NOTFOUND);
}

static nss_status_t
getnetgr_get(be, a)
	struct nis_getnetgr_be	*be;
	void			*a;
{
	struct nss_getnetgrent_args *args = (struct nss_getnetgrent_args *) a;
	struct grouplist	*mem;

	if ((mem = be->next_member) == 0) {
		args->status = NSS_NETGR_NO;
	} else {
		char			*buffer	= args->buffer;
		int			buflen	= args->buflen;
		enum nss_netgr_argn	i;

		args->status = NSS_NETGR_FOUND;

		for (i = 0;  i < NSS_NETGR_N;  i++) {
			const char	*str;
			size_t		len;

			if ((str = mem->triple[i]) == 0) {
				args->retp[i] = 0;
			} else if ((len = strlen(str) + 1) <= buflen) {
				args->retp[i] = buffer;
				memcpy(buffer, str, len);
				buffer += len;
				buflen -= len;
			} else {
				args->status = NSS_NETGR_NOMEM;
				break;
				/* %%% do we want to go to mem->gl_nxt? */
			}
		}
		be->next_member	= mem->gl_nxt;
	}
	return (NSS_SUCCESS);	/* Yup, even for end-of-list, i.e. */
				/* do NOT advance to next backend. */
}

/*ARGSUSED*/
static nss_status_t
getnetgr_end(be, dummy)
	struct nis_getnetgr_be	*be;
	void			*dummy;
{
	struct grouplist	*gl;

	for (gl = be->all_members; gl != NULL; gl = gl->gl_nxt) {
		enum nss_netgr_argn	i;

		for (i = NSS_NETGR_MACHINE;  i < NSS_NETGR_N;  i++) {
			if (gl->triple[i] != 0) {
				free(gl->triple[i]);
			}
		}
		free(gl);
	}
	be->all_members = 0;
	be->next_member = 0;
	if (be->netgroup != 0) {
		free(be->netgroup);
		be->netgroup = 0;
	}
	return (NSS_SUCCESS);
}

/*ARGSUSED*/
static nss_status_t
getnetgr_destr(be, dummy)
	struct nis_getnetgr_be	*be;
	void			*dummy;
{
	if (be != 0) {
		getnetgr_end(be, 0);
		free (be);
	}
	return (NSS_SUCCESS);
}

static nis_getnetgr_op_t getnetgr_ops[] = {
	getnetgr_destr,
	getnetgr_end,
	getnetgr_set,
	getnetgr_get,	/* getnetgrent_r() */
};


#ifdef	UNDEF	/* <==== */
#include <rpcsvc/nis.h>
#include <syslog.h>
#include <nsswitch.h>
#include "boundary.h"
#include "nisplus_common.h"
#include "nisplus_tables.h"
#endif	UNDEF	/* <==== */

/*
 * Code to do top-down search in the graph defined by the 'netgroup' YP map
===>          ^^^^^^^^ only half of the truth
 */

/*
 * ===> This code is now used for setnetgrent(), not just innetgr().
   ===> need to document every_which_way too.
 *
 * If the easy way doesn't pan out, recursively search the 'netgroup' map.
 * In order to do this, we:
 *
 *    -	remember all the netgroup names we've seen during this search,
 *	whether or not we've expanded them yet (we want fast insertion
 *	with duplicate-detection, so use yet another chained hash table),
 *
 *    -	keep a list of all the netgroups we haven't expanded yet (we just
 *	want fast insertion and pop-first, so a linked list will do fine).
 *	If we insert at the head, we get a depth-first search;  insertion
 *	at the tail gives breadth-first (?), which seems preferable (?).
 *
 * A netgrnam struct contains pointers for both the hash-table and the list.
 * It also contains the netgroup name;  note that we embed the name at the
 * end of the structure rather than holding a pointer to yet another
 * malloc()ed region.
 *
 * A netgrtab structure contains the hash-chain heads and the head/tail
 * pointers for the expansion list.
 *
 * Most of this code is common to at least the NIS and NIS+ backends;  it
 * should be generalized and, presumably, moved into the frontend.
 * ==> Not any longer...
 */

struct netgrnam {
	struct netgrnam	*hash_chain;
	struct netgrnam	*expand_next;
	char		name[1];	/* Really [strlen(name) + 1] */
};

#define	HASHMOD	113

struct netgrtab {
	struct netgrnam	*expand_first;
	struct netgrnam	**expand_lastp;
	int		n_new;
	int		n_total;
	struct netgrnam	*hash_heads[HASHMOD];
};

static void
ngt_init(ngt)
	struct netgrtab	*ngt;
{
	memset((void *)ngt, 0, sizeof (*ngt));
	ngt->expand_lastp = &ngt->expand_first;
}

/* === ? Change ngt_init() and ngt_destroy() to malloc/free struct netgrtab */

static struct netgrnam **
ngt_prevp(ngt, name, namelen)
	struct netgrtab	*ngt;
	const char	*name;
	size_t		namelen;
{
	unsigned	hashval;
	size_t		i;
	struct netgrnam	*cur;
	struct netgrnam	**prevp;

	if (namelen < 1)
		return (0);

	for (hashval = 0, i = 0;  i < namelen;  i++) {
		hashval = (hashval << 2) + hashval +
			((const unsigned char *)name)[i];
	}
	for (prevp = &ngt->hash_heads[hashval % HASHMOD];
	    (cur = *prevp) != 0; prevp = &cur->hash_chain) {
		if (strncmp(cur->name, name, namelen) == 0 &&
		    cur->name[namelen] == 0) {
			/* Found it */
			break;
		}
	}
	return (prevp);
}

#define	ngt_found(ngt, name, namelen)	(*ngt_prevp(ngt, name, namelen) != 0)

static void
/* ==> ? Should return 'failed' (out-of-memory) status ? */
ngt_insert(ngt, name, namelen, breadth_first)
	struct netgrtab	*ngt;
	const char	*name;
	size_t		namelen;
	int		breadth_first;
{
	struct netgrnam	*cur;
	struct netgrnam	**prevp;

	if (namelen < 1)
		return;

	prevp = ngt_prevp(ngt, name, namelen);
	if (*prevp != 0) {		/* Already in table, do nothing */
		return;
	}

#define	dummy ((struct netgrnam *)0)

	/* Create new netgrnam struct */
	cur = (struct netgrnam *)
		malloc(namelen + 1 + (char *)&dummy->name[0] - (char *)dummy);
	if (cur == 0) {
		return;			/* Out of memory, too bad */
	}
	memcpy(cur->name, name, namelen);
	cur->name[namelen] = 0;

	/* Insert in hash table */
	cur->hash_chain = *prevp;
	*prevp = cur;

	/* Insert in expansion list, at start (depth-first) or end (breadth) */
	if (breadth_first || ngt->expand_first == 0) {
		cur->expand_next = 0;
		*ngt->expand_lastp = cur;
		ngt->expand_lastp = &cur->expand_next;
	} else {
		cur->expand_next = ngt->expand_first;
		ngt->expand_first = cur;
	}
	ngt->n_new++;
	ngt->n_total++;
}

static const char *
ngt_next(ngt)
	struct netgrtab	*ngt;
{
	struct netgrnam	*first;

	if ((first = ngt->expand_first) == 0) {
		return (0);
	}
	if ((ngt->expand_first = first->expand_next) == 0) {
		ngt->expand_lastp = &ngt->expand_first;
	}
	ngt->n_new--;
	return (first->name);
}

static void
ngt_destroy(ngt)
	struct netgrtab	*ngt;
{
	struct netgrnam	*cur;
	struct netgrnam *next;
	int		i;

	for (i = 0;  i < HASHMOD;  i++) {
		for (cur = ngt->hash_heads[i];
		    cur != 0; /* no action */) {
			next = cur->hash_chain;
			free(cur);
			cur = next;
		}
	}
	/* Don't bother zeroing pointers;  must do init if we want to reuse */
}

static int
domcmp(n1, n2)
	const char	*n1;	/* See if n1 is n2 or an ancestor of it     */
	const char	*n2;	/* (in string terms, n1 is a suffix of n2)  */
				/* Returns ZERO for success, -1 for failure */
{
#define	PASS	0
#define	FAIL	-1

	size_t		l1, l2;

	if ((n1 == NULL) || (n2 == NULL))
		return (-1);

	l1 = strlen(n1);
	l2 = strlen(n2);

	/* Turn a blind eye to the presence or absence of trailing periods */
	if (l1 != 0 && n1[l1 - 1] == '.') {
		--l1;
	}
	if (l2 != 0 && n2[l2 - 1] == '.') {
		--l2;
	}
	if (l1 > l2) {		/* Can't be a suffix */
		return (FAIL);
	} else if (l1 == 0) {	/* Trivially a suffix; */
				/* (do we want this case?) */
		return (PASS);
	}
	/* So 0 < l1 <= l2 */
	if (l1 < l2 && n2[l2 - l1 - 1] != '.') {
		return (FAIL);
	}
	if (strncasecmp(n1, &n2[l2 - l1], l1) == 0) {
		return (PASS);
	} else {
		return (FAIL);
	}
}


/*
 * top_down() -- do innetgr() search down from netgroups toward triples.
 *
 * We use this when:
 *
 *	- we're executing a setnetgrent() and so have to enumerate all
 *	  the triples that belong to a given netgroup,
 *
 *	- we're executing a __multi_innetgr() and believe that this will
 *	  perform better than the every_which_way() routine below
 *	  (e.g. both 'hosts' and 'users' are wildcarded).
 */

static nss_status_t
top_down(be, groups, ngroups, func, iter_args)
	nisplus_backend_ptr_t	be;
	const char		**groups;
	int			ngroups;
	top_down_lambda_t	func;
	void			*iter_args;
{
	struct netgrtab		*ngt;
	/*
	 * netgrtab goes on the heap, not the stack, because it's large and
	 * stacks may not be all that big in multi-threaded programs.
	 * The same goes for search_crit[].
	 * ===> Should we just put them both in the backend struct and
	 *	be done with it?
	 */
	char			*search_crit;

	const char		*group;
	int			nfound;
	int			done;
	nss_status_t		result;

	if ((search_crit = (char *) malloc(NIS_MAXNAMELEN)) == 0) {
		return (NSS_UNAVAIL);
	}
	if ((ngt = (struct netgrtab *) malloc(sizeof (*ngt))) == 0) {
		free(search_crit);
		return (NSS_UNAVAIL);
	}
	ngt_init(ngt);

	while (ngroups > 0) {
		ngt_insert(ngt, *groups, strlen(*groups), 1);		/* %%% check 4th arg value */
		groups++;
		ngroups--;
	}

	done	= 0;	/* Set to 1 to indicate that we cut the iteration  */
			/*   short (and 'result' holds the return value)   */
	nfound	= 0;	/* Number of successful netgroup nis_list calls	   */

	while (!done && (group = ngt_next(ngt)) != 0) {
		nis_result	*r;
		int		i;

		sprintf(search_crit, "[%s=%s]%s",
			NETGR_TAG_NAME, group, be->table_name);		/* %%% was NETGR_NDX_NAME */
		result = _nss_nisplus_list(search_crit, 0, &r);

		if (result != NSS_SUCCESS) {
			if (result == NSS_NOTFOUND) {
#ifdef	DEBUG
				syslog(LOG_WARNING,
					"innetgr: no such NIS+ netgroup as %s",
					group);
#endif	DEBUG
			} else {
#ifdef	DEBUG
				syslog(LOG_WARNING,
					"innetgr: nis_list returned [%s]",
					(r == 0) ? "A null pointer !?" :
					nis_sperrno(NIS_RES_STATUS(r)));
#endif	DEBUG
				done = 1;	/* Give up, return result */
			}
			if (r != 0) {
				nis_freeresult(r);
			}
			continue;
		}

		nfound++;

		for (i = 0;  !done && i < NIS_RES_NUMOBJ(r);  i++) {
			ccp		triple[NSS_NETGR_N];
			nis_object	*obj = NIS_RES_OBJECT(r) + i;
			char		*val;
			size_t		len;

			if (__type_of(obj) != NIS_ENTRY_OBJ ||
			    obj->EN_data.en_cols.en_cols_len < NETGR_COL) {
				/* Not a netgroup entry !? */
				continue;
			}

			len = ENTRY_LEN(obj, NETGR_NDX_GROUP);
			val = ENTRY_VAL(obj, NETGR_NDX_GROUP);
			if (len != 0 && val[0] != '\0') {
				/* Recursive member, not triple */
				ngt_insert(ngt, val, len - 1, 0);
				continue;
			}

#define	COPY_NAME(entry_ndx, triple_ndx)				\
/* do */ {								\
	len = ENTRY_LEN(obj, entry_ndx);				\
	val = ENTRY_VAL(obj, entry_ndx);				\
	if (len == 0 || val[0] == '\0') {				\
		triple[triple_ndx] = 0;					\
	} else {							\
		triple[triple_ndx] = val;				\
	}								\
} /* while (0) */

			COPY_NAME(NETGR_NDX_HOST,	NSS_NETGR_MACHINE);
			COPY_NAME(NETGR_NDX_USER,	NSS_NETGR_USER);
			COPY_NAME(NETGR_NDX_DOMAIN,	NSS_NETGR_DOMAIN);

			if (!(*func)(triple, iter_args, &result)) {
				/* Return result, good or bad */
				done = 1;
				break;
			}
		}
		/* End of inner loop over NIS_RES_OBJECT(r)[] */
		nis_freeresult(r);
	}
	/* End of outer loop (!done && ngt_next(ngt) != 0) */

	free(search_crit);
	ngt_destroy(ngt);
	free(ngt);

	if (done) {
		return (result);
	} else if (nfound > 0) {
		/* ==== ? Should only do this if all the top-level groups */
		/*	  exist in NIS+?				  */
		return (NSS_SUCCESS);
	} else {
		return (NSS_NOTFOUND);
	}
}


/*
 * Code for setnetgrent()
 */

/*
 * Iterator function for setnetgrent():  copy triple, add to be->all_members
 */
static int
save_triple(trippp, headp, return_val)
	ccp			trippp[NSS_NETGR_N];
	struct grouplist	**headp;
	nss_status_t		*return_val;
{
	struct grouplist	*gl;
	enum nss_netgr_argn	i;

	if ((gl = (struct grouplist *)malloc(sizeof (*gl))) == 0) {
		/* Out of memory */
		*return_val = NSS_UNAVAIL;
		return (0);
	}
	for (i = NSS_NETGR_MACHINE;  i < NSS_NETGR_N;  i++) {
		if (trippp[i] == 0) {
			/* Wildcard */
			gl->triple[i] = 0;
		} else if ((gl->triple[i] = strdup(trippp[i])) == 0) {
			/* Out of memory.  Free any we've allocated */
			enum nss_netgr_argn	j;

			for (j = NSS_NETGR_MACHINE;  j < i;  j++) {
				if (gl->triple[j] != 0) {
					free(gl->triple[j]);
				}
			}
			*return_val = NSS_UNAVAIL;
			return (0);
		}
	}
	gl->gl_nxt = *headp;
	*headp = gl;
	return (1);	/* Tell top_down() to keep iterating */
}

static nss_status_t
netgr_set(be, a)
	nisplus_backend_ptr_t	be;
	void			*a;
{
	struct nss_setnetgrent_args *args = (struct nss_setnetgrent_args *) a;
	struct nis_getnetgr_be	*get_be;
	nss_status_t		res;

	get_be = (struct nis_getnetgr_be *) malloc(sizeof (*get_be));
	if (get_be == 0) {
		return (NSS_UNAVAIL);
	}

	get_be->all_members = 0;
	res = top_down(be, &args->netgroup, 1, (top_down_lambda_t)save_triple,
		&get_be->all_members);

	if (res == NSS_SUCCESS) {
		get_be->ops		= getnetgr_ops;
		get_be->n_ops		= sizeof (getnetgr_ops) /
						sizeof (getnetgr_ops[0]);
		get_be->netgroup	= strdup(args->netgroup);
		get_be->next_member	= get_be->all_members;

		args->iterator		= (nss_backend_t *) get_be;
	} else {
		args->iterator		= 0;
		free(get_be);
	}
	return (res);
}


/*
 * Code for innetgr()
 */

/*
 * Iterator function for innetgr():  Check whether triple matches args
 */
static int
match_triple(triple, ia, return_val)
	ccp			triple[NSS_NETGR_N];
	struct nss_innetgr_args	*ia;
	nss_status_t		*return_val;
{
	enum nss_netgr_argn	i;

	for (i = NSS_NETGR_MACHINE;  i < NSS_NETGR_N;  i++) {
		int		(*cmpf)(const char *, const char *);
		char		**argv;
		int		n;
		const char	*name = triple[i];
		int		argc = ia->arg[i].argc;

		if (argc == 0 || name == 0) {
			/* Wildcarded on one side or t'other */
			continue;
		}
		argv = ia->arg[i].argv;
		switch (i) {
		    case NSS_NETGR_MACHINE:
			cmpf = strcasecmp;
			break;
		    case NSS_NETGR_USER:
			cmpf = strcmp;
			break;
		    case NSS_NETGR_DOMAIN:
			cmpf = domcmp;
			break;
		}
		for (n = 0;  n < argc;  n++) {
			if ((*cmpf)(argv[n], name) == 0) {
				break;
			}
		}
		if (n >= argc) {
			/* Match failed, tell top_down() to keep looking */
			return (1);
		}
	}
	/* Matched on all three, so quit looking and declare victory */

	ia->status = NSS_NETGR_FOUND;
	*return_val = NSS_SUCCESS;
	return (0);
}


/*
 * every_which_way() -- assumes that at least one of 'hosts' and 'users' is
 *	NOT wildcarded.  This makes it reasonable to start searching from
 *	the bottom (i.e. from triples up to groups).  It's easy enough to
 *	search from top and bottom toward the middle, but for now we just
 *	search bottom-up.  One reason for doing this is that mountd hands us
 *	long lists of putative netgroups, most of which are in fact hosts;
 *	it would be a shame to pay for a NIS+ lookup on each bogus netgroup.
 */

#undef	SEARCH_DOWN_TOO

static nss_status_t
every_which_way(be, args)
	nisplus_backend_ptr_t	be;
	struct nss_innetgr_args	*args;
{
	struct netgrtab		*top;	/* Search down from netgroup names   */
	struct netgrtab		*bottom;/* Search up from (host,user,domain) */
					/*   triples			     */
	char			*search_crit;

	nss_innetgr_argc	ngroups, nhosts, nusers, ndomains;
	nss_innetgr_argv	pgroups, phosts, pusers, pdomains;
	char			*hosts, *users;
	nis_result		*r;
	int			i;
	enum { BAD, NO, YES }	verdict;

/* Copy NIS+ search value; ==== Should really do NIS+ quoting */
#define	catquoted(to, from)	strcat(to, from)

/*
====>
 * remember to set errp in all cases;
 * getting NOTFOUND vs SUCCESS/0 right may be expensive;
 * should do negative caching of group-names.
 */

	if ((search_crit = (char *) malloc(NIS_MAXNAMELEN)) == 0) {
		return (NSS_UNAVAIL);
	}
	if ((top = (struct netgrtab *) malloc(sizeof (*top))) == 0) {
		free(search_crit);
		return (NSS_UNAVAIL);
	}
	if ((bottom = (struct netgrtab *) malloc(sizeof (*bottom))) == 0) {
		free(search_crit);
		free(top);
		return (NSS_UNAVAIL);
	}

	ngt_init(top);

	ngroups = args->groups.argc;	/* Guaranteed >= 1 */
	pgroups = args->groups.argv;
	do {
		/* ==== ?? check validity ?? */
		ngt_insert(top, *pgroups, strlen(*pgroups), 1);
		pgroups++;
		ngroups--;
	} while (ngroups != 0);
	/* ==== if none valid, bag it */

	/*
	 * For users and hosts:
	 *	if (argc == 0) {
	 *		wild-card (i.e. don't specify search criterion)
	 *	} else {
	 *		argc:	try argv[0]
	 *		argc-1:	try argv[1]
	 *		...
	 *		1:	try argv[argc - 1]
	 *		0:	try the null string (wildcard in the table)
	 *	}
	 * For domains we want the same logical behaviour, but do it
	 * differently:  since the odds are that every entry in the
	 * netgroup table has domain set to either the null string or one
	 * value, we eschew search criteria and do the filtering at our end.
	 *
	 * Similarly, we don't use search criteria to check that the 'group'
	 * column is null;  we check it on the client side.
	 */

	ngt_init(bottom);

	search_crit[0] = '\0';

	users  = search_crit;
	nusers = args->arg[NSS_NETGR_USER].argc;	/* 0 ==> wildcard */
	pusers = nusers ? args->arg[NSS_NETGR_USER].argv : 0;
	do {
		if (pusers != 0) {
			strcpy(users, ",user=");
			if (nusers != 0) {
				catquoted(users, *pusers);
				pusers++;
			} /* Else look for null strings (wildcards) in table */
		} /* Else accept any value */

		hosts  = users + strlen(users);
		nhosts = args->arg[NSS_NETGR_MACHINE].argc;
		phosts = nhosts ? args->arg[NSS_NETGR_MACHINE].argv : 0;
		do {
			if (phosts != 0) {
				strcpy(hosts, ",host=");
				if (nhosts != 0) {
					catquoted(hosts, *phosts);
					phosts++;
				}
			}

			search_crit[0] = '[';	/* Was temporarily a comma */
			strcat(search_crit, "]");
			strcat(search_crit, be->table_name);
			switch (_nss_nisplus_list(search_crit, 0, &r)) {
			    case NSS_SUCCESS:
				break;
			    case NSS_NOTFOUND:
				nis_freeresult(r);
				continue;
			    default:
				if (r != 0) {
					nis_freeresult(r);
				}
				verdict = BAD;
				goto wrapup;
			}
			for (i = 0;  i < NIS_RES_NUMOBJ(r);  i++) {
				nis_object	*obj = NIS_RES_OBJECT(r) + i;
				char		*val;
				size_t		len;

				if (__type_of(obj) != NIS_ENTRY_OBJ ||
				    obj->EN_data.en_cols.en_cols_len
				    < NETGR_COL) {
					/* Not a netgroup entry !? */
					continue;
				}
				len = ENTRY_LEN(obj, NETGR_NDX_GROUP);
				val = ENTRY_VAL(obj, NETGR_NDX_GROUP);
				if (len != 0 && val[0] != '\0') {
					/* Recursive member, not triple */
					continue;
				}

				ndomains = args->arg[NSS_NETGR_DOMAIN].argc;
				pdomains = args->arg[NSS_NETGR_DOMAIN].argv;
				len = ENTRY_LEN(obj, NETGR_NDX_DOMAIN);
				val = ENTRY_VAL(obj, NETGR_NDX_DOMAIN);

				if (ndomains != 0 &&
				    len != 0 && val[0] != '\0') {
					do {
						if (domcmp(val, *pdomains++)
						    == 0) {
							/* Got a match */
							break;
						}
					} while (--ndomains != 0);
					if (ndomains == 0) {
						/* better luck with next obj */
						continue;
					}
				}
				/*
				 * Got one, either by wildcarding (one of the
				 *   "...!= 0" tests failed) or by a match
				 *   in the domain list.
				 */
				len = ENTRY_LEN(obj, NETGR_NDX_NAME);
				val = ENTRY_VAL(obj, NETGR_NDX_NAME);
				if (ngt_found(top, val, len - 1)) {
					/* Good day to buy a lottery ticket */
					nis_freeresult(r);
					verdict = YES;
					goto wrapup;
				}
				ngt_insert(bottom, val, len - 1, 1);
			}
			nis_freeresult(r);
		} while (nhosts-- != 0);	/* decrements unsigned just */
	} while (nusers-- != 0);		/* past zero; sleazy but OK */

	while (top->n_new != 0 && bottom->n_new != 0) {
		const char *group;

#ifdef	SEARCH_DOWN_TOO
		if (top->n_total < bottom->n_total /* and a fudge factor? */) {
			group = ngt_next(top);
			sprintf(search_crit, "[%s=%s]%s",
				NETGR_NDX_NAME, group, be->table_name);
			switch (_nss_nisplus_list(search_crit, 0, &r)) {
			    case NSS_SUCCESS:
				break;
			    case NSS_NOTFOUND:
				/* sysadmin SNAFU?  This group doesn't exist */
				nis_freeresult(r);
				continue;
			    default:
				if (r != 0) {
					nis_freeresult(r);
				}
				verdict = BAD;
				goto wrapup;
			}

			for (i = 0;  i < NIS_RES_NUMOBJ(r);  i++) {
				nis_object	*obj = NIS_RES_OBJECT(r) + i;
				char		*val;
				unsigned	len;

				if (__type_of(obj) != NIS_ENTRY_OBJ ||
				    obj->EN_data.en_cols.en_cols_len
				    < NETGR_COL) {
					/* Not a netgroup entry? */
					continue;
				}
				len = ENTRY_LEN(obj, NETGR_NDX_GROUP);
				val = ENTRY_VAL(obj, NETGR_NDX_GROUP);
				if (len == 0 || val[0] == '\0') {
					/* Triple, not recursive netgroup */
					continue;
				}
				if (ngt_found(bottom, val, len - 1)) {
					/* Eureka */
					nis_freeresult(r);
					verdict = YES;
					goto wrapup;
				}
				ngt_insert(top, val, len - 1, 1);
			}
		} else {
#endif	SEARCH_DOWN_TOO
			group = ngt_next(bottom);
			sprintf(search_crit, "[%s=%s],%s",
				NETGR_TAG_GROUP, group, be->table_name);	/* %%% was NETGR_NDX_GROUP */
			switch (_nss_nisplus_list(search_crit, 0, &r)) {
			    case NSS_SUCCESS:
				break;
			    case NSS_NOTFOUND:
				/* This group isn't included in any others */
				nis_freeresult(r);
				continue;
			    default:
				nis_freeresult(r);
				verdict = BAD;
				goto wrapup;
			}

			for (i = 0;  i < NIS_RES_NUMOBJ(r);  i++) {
				nis_object	*obj = NIS_RES_OBJECT(r) + i;
				char		*val;
				size_t		len;

				if (__type_of(obj) != NIS_ENTRY_OBJ ||
				    obj->EN_data.en_cols.en_cols_len
				    < NETGR_COL) {
					/* Not a netgroup entry !? */
					continue;
				}
				len = ENTRY_LEN(obj, NETGR_NDX_NAME);
				val = ENTRY_VAL(obj, NETGR_NDX_NAME);
				if (ngt_found(top, val, len - 1)) {
					/* Eureka */
					nis_freeresult(r);
					verdict = YES;
					goto wrapup;
				}
				ngt_insert(bottom, val, len - 1, 0);
			}
			nis_freeresult(r);
#ifdef	SEARCH_DOWN_TOO
		}
#endif	SEARCH_DOWN_TOO
	}
	/*
	 * We arrive here if we didn't find any matches, i.e. none of the
	 *   initial triples are in any of the initial groups.  This could
	 *   happen because either:
	 *
	 *    (1) none of the initial groups are stored in NIS+, in which
	 *	  case we should return NSS_NOTFOUND so that the switch
	 *	  will try the next source (if any),
	 *
	 * or (2) at least one of the initial groups is stored in NIS+,
	 *	  in which case we should set ia->status = NETGR_NO and
	 *	  return NSS_SUCCESS ("don't try any more sources").
	 *
	 * However, distinguishing between these cases is expensive and quite
	 * likely to be wasted effort, so we'll pretend they're both (1).
	 */
	verdict = NO;

wrapup:
	free(search_crit);
	ngt_destroy(top);
	free(top);
	ngt_destroy(bottom);
	free(bottom);

	switch (verdict) {
	    case BAD:
		return (NSS_UNAVAIL);
	    case NO:
		return (NSS_NOTFOUND);
	    case YES:
		args->status = NSS_NETGR_FOUND;
		return (NSS_SUCCESS);
	}
}


static nss_status_t
netgr_in(be, a)
	nisplus_backend_ptr_t	be;
	void			*a;
{
	struct nss_innetgr_args	*ia = (struct nss_innetgr_args *) a;

	ia->status = NSS_NETGR_NO;

	if (ia->arg[NSS_NETGR_MACHINE].argc == 0 &&
		ia->arg[NSS_NETGR_USER].argc == 0) {
		return (top_down(be, (const char **)ia->groups.argv,
				ia->groups.argc,
				(top_down_lambda_t)match_triple, ia));
	} else {
		return (every_which_way(be, ia));
	}
}

/*ARGSUSED*/
static int
bogus_object2ent(nobj, obj, argp)
	int		nobj;
	nis_object	*obj;
	nss_XbyY_args_t	*argp;
{
	/*
	 * This should never get used in the netgroup backend;
	 *   if it does, we should complain loudly.
	 */
	syslog(LOG_ERR, "logic bug in nsswitch nisplus/netgroup backend");
	return (NSS_STR_PARSE_PARSE);
}


static nisplus_backend_op_t netgroup_ops[] = {
	_nss_nisplus_destr,
	0,		/* No endent, because no setent/getent */
	0,		/* No setent;  setnetgrent() is really a getXbyY() */
	0,		/* No getent in the normal sense */

	netgr_in,	/* innetgr() */
	netgr_set,	/* setnetgrent() */
};

/*ARGSUSED*/
nss_backend_t *
_nss_nisplus_netgroup_constr(dummy1, dummy2, dummy3)
	const char	*dummy1, *dummy2, *dummy3;
{
	return (_nss_nisplus_constr(netgroup_ops,
				sizeof(netgroup_ops) / sizeof(netgroup_ops[0]),
				NETGR_TBLNAME, bogus_object2ent));
}
