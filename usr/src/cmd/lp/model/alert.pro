#ident	"@(#)alert.proto	1.3	90/03/01 SMI"	/* SVr4.0 1.5	*/
#
#	Pardon the bizarre variable names, but we must not
#	conflict with any variables that the user gives us.
#

-ENVIRONMENT-

cd -PWD-

if [ "`ulimit`" -gt -ULIMIT- ]
then
	ulimit -ULIMIT-
fi

umask -UMASK-

AlErT () {
	2>&1 (
-CMD-
	) <$1 1>${TmPsTdErR} || return 1
	return 0
}

TmPsTdErR=${TMPDIR:-/usr/tmp}/`/bin/uname -n`$$
trap 'rm -f ${TmPsTdErR}; exit 0' 0 1 2 15

while AlErT $1
do
	if [ -INTERVAL- -le 0 ]
	then
		exit 0
	else
		TiCk=0
		while [ "${TiCk}" -lt -INTERVAL- ]
		do
			TiCk=`expr "${TiCk}" + 1`
			sleep 60
		done
	fi
done

ec=$?
if [ "${ec}" -ne 0 ]
then
	{

		echo "
One of your print service alerts failed with exit code ${ec}.
The standard output and standard error were:
	"
		cat ${TmPsTdErR}
		echo '

The alert message is:
'
		cat $1

	} | mailx -s 'Failed print service alert' -USER-
fi
