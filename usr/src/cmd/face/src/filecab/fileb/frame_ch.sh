#!/sbin/sh
#	Copyright (c) 1984, 1986, 1987, 1988, 1989 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.

#ident	"@(#)frame_chk.sh	1.2	89/12/04 SMI"	/* SVr4.0 1.1	*/

frame=$1
shift

if [ -z "$frame" ]
then
	echo 0; exit 0
fi

if [ -r "$frame" ]
then
	for other in "$@"
	do
		if [ "$frame" = "$other" ]
		then
			echo 2; exit 1
		fi
	done
else
	echo 1; exit 1
fi

echo 0
