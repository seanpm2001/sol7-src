/*	Copyright (c) 1984, 1986, 1987, 1988, 1989 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ident	"@(#)gettab.c	1.7	97/07/28 SMI"

/*
 * Copyright (c) 1994,1997, by Sun Microsystems, Inc.
 * All rights reserved.
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <dirent.h>
#include <stdlib.h>
#include <unistd.h>
#include <libintl.h>
#include <fcntl.h>
#include <string.h>
#include "usl_iconv.h"

static void	checkmagic(struct kbd_header *);
static void	do_link(int, struct kbd_tab *, char *, char *,
	int *, char *);

extern int	search_dbase(char *, char *, char *, char *,
	char *, char *, char *);

struct kbd_tab *
gettab(char *file, char *table, char *d_data_base,
	char *f_data_base, int no_composites)
{
	int i;
	int j;
	int fd;
	char f_name[MAXNAMLEN];
	struct kbd_header hdr;
	struct kbd_tab *tb;
	struct cornode *n;
	char *text;
	int found;


	(void) sprintf(f_name, "%s/%s", d_data_base, file);
	fd = open(f_name, 0);
	if (fd < 0)  {
		fprintf(stderr, gettext("Cannot access table.\n"));
		exit(1);
	}

	if (read(fd, &hdr, sizeof (struct kbd_header)) !=
		sizeof (struct kbd_header)) {
		fprintf(stderr, gettext("Can't read header.\n"));
		exit(1);
	}

	checkmagic(&hdr);

	/*
	 * ok so far
	 * so malloc a tb
	 */
	tb = (struct kbd_tab *) malloc(sizeof (struct kbd_tab));
	if (!tb) {
		fprintf(stderr, gettext("Out of Space(1)\n"));
		exit(1);
	}

	/*
	 * For each table, do the obvious.
	 */
	for (i = 0; i < hdr.h_ntabs; i++) {

		found = 0;

		if ((j = read(fd, tb, sizeof (struct kbd_tab))) !=
			sizeof (struct kbd_tab)) {
			fprintf(stderr, gettext("Can't access table "
				"header %d\n"), errno);
			exit(1);
		}

		/*
		 * test its the table we want
		 */
		if (strncmp((const char *)tb->t_name, table, KBDNL) == 0)
			found = 1;

		if (tb->t_flag == KBD_COT) {
			/*
			 * might now be found
			 * COT tables do not have names in them.
			 */
			do_link(fd, tb, d_data_base,
				f_data_base, &found, table);
			if (no_composites && found) {
				fprintf(stderr, gettext("Double composite\n"));
				exit(1);
			}
			if (found)
				return (tb);
			continue;
		}

		/*
		 * Is it a simple one to one mapping
		 */
		if (tb->t_flag & KBD_ONE) {

			if (!found)
				lseek(fd, (long)256, SEEK_CUR);
			else {
				tb->t_oneone = (unsigned char *)malloc(256);
				if (tb->t_oneone == (unsigned char *)NULL) {
					fprintf(stderr, gettext("Out of "
						"Space(2)\n"));
					exit(1);
				}
				if (read(fd, tb->t_oneone, 256) != 256) {
					fprintf(stderr, gettext("Can't access "
						"one to one map table\n"));
					exit(1);
				}
				return (tb);
			}
		}
		/*
		 * Is it a tree
		 */
		if (tb->t_nodes) {
			if (!found)
				lseek(fd, (long)(tb->t_nodes *
					sizeof (struct cornode)), SEEK_CUR);
			else {
				tb->t_nodep = (struct cornode *)
				    malloc(tb->t_nodes *
					sizeof (struct cornode));
				if (tb->t_nodep == (struct cornode *)NULL) {
					fprintf(stderr, gettext("Out of "
						"Space(3)\n"));
					exit(1);
				}
				j =  tb->t_nodes * sizeof (struct cornode);
				if (read(fd, tb->t_nodep, j) != j) {
					fprintf(stderr, gettext("Bad map "
						"table\n"));
					exit(1);
				}
			}
		}
		if (tb->t_text) {
			if (!found)
				lseek(fd, (long)tb->t_text, SEEK_CUR);
			else {
				tb->t_textp =
					(unsigned char *)malloc(tb->t_text);
				if (!tb->t_textp) {
					fprintf(stderr, gettext("Out of "
						"Space(4)\n"));
					exit(1);
				}

				if (read(fd, tb->t_textp, tb->t_text) !=
					tb->t_text) {
					fprintf(stderr, gettext("Bad text "
						"table\n"));
					exit(1);
				}
			}
		}

		if (found)
			return (tb);

	}

	return ((struct kbd_tab *)NULL);
}

static void
checkmagic(struct kbd_header *h)
{
	if (strncmp((const char *)h->h_magic, KBD_MAGIC,
		strlen(KBD_MAGIC)) != 0) {
		fprintf(stderr, gettext("bad magic (not a kbd object)\n"));
		exit(1);
	}
	if (h->h_magic[KBD_HOFF] > KBD_VER) {
		fprintf(stderr, gettext("Version mismatch; this is %d, "
			"file is %d\n"), KBD_VER, (int) (h->h_magic[KBD_HOFF]));
		exit(1);
	}
}

/*
 * do_link	Make a link table.  On disk, a link table is a "kbd_tab"
 *		followed immediatedly by a STRING that is "t.t_max"
 *		bytes long.  We just take the string and start looking
 * 		for the maps in the known database
 *		Because the map does not have its name in it here
 * 		we must do another 'found' test.
 */

static void
do_link(int ifd, struct kbd_tab *t, char *d_data_base,
	char *f_data_base, int *found, char *i_table)
{
	int i;
	int j;
	char *s;
	char *table[KBDLNX];
	char file[KBDNL];
	char n_table[KBDNL];

	s = malloc(t->t_max);
	if (s == (char *)NULL) {
		fprintf(stderr, gettext("Cannot Allocate Link Table, No "
			"Space\n"));
		exit(1);
	}
	if (read(ifd, s, t->t_max) != t->t_max) {
		fprintf(stderr, gettext("Invalid Composite Map\n"));
		exit(1);
	}
	/*
	 * If s we
	 * have a composite list
	 * we must know traverse
	 * the list.
	 */

	/*
	 * save name of table
	 */
	i = 0;
	table[0] = s;
	while (i++ < (int)t->t_max && *s != ':')
		s++;
	*s++ = '\0';
	if (i == (int)t->t_max) {
		fprintf(stderr, gettext("Invalid Table\n"));
		exit(1);
	}

	j = 1;
	while (i < (int)t->t_max && j < KBDLNX) {
		table[j] = s;
		while (i++ < (int)t->t_max && *s != ',') {
			s++;
		}
		if (i < (int)t->t_max)
			*s++ = '\0';
		j++;
	}

	if (*found == 0) {

		if (strncmp(table[0], i_table, KBDNL)) {
			free(table[0]);
			return;
		}

		*found = 1;

	}
	/*
	 * SIlently ignore
	 * greater tha KBDLNX composites
	 */
	for (i = 1; i < j; i++) {

		if (search_dbase(file, n_table, d_data_base,
			f_data_base, table[i], (char *)NULL, (char *)NULL)) {
			/*
			 * get the table
			 */
			t->t_next = gettab(file, n_table, d_data_base,
				f_data_base, 1);
			t = t->t_next;

		} else {

			fprintf(stderr, gettext("Can't Access Composite "
				"Table\n"));
			exit(1);

		}
	}
}
