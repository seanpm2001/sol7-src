#pragma ident	"@(#)strstr.c	1.4	95/05/31 SMI"
	  /* from UCB 4.1 80/12/21 */

/*
 * Get matching substring
 */

#pragma weak strstr = _strstr

char *
_strstr(s1, s2)
	register char *s1, *s2;
{
	int s2len = strlen(s2);	/* length of the second string */
	/*
	 * If the length of the second string is 0, 
	 *  return the first argument.
	 */
	if (s2len == 0)
		return (s1);

	while (strlen(s1) >= s2len) { 
		if (strncmp(s1, s2, s2len) == 0)
			return (s1);
		s1++;
	}
	return (0);
}
