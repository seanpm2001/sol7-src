#ident	"@(#)Form.prin	1.2	92/07/14 SMI"	/* SVr4.0 1.4	*/

form="Change Print Options for $LOGNAME"

help=OPEN TEXT OBJECTS/Text.h "Print Command" T.hprin1

framemsg="Enter a print command with appropriate options, then press SAVE when finished."

done=`indicator -w;set PRINCMD1="$F1" PRINCMD\2="$F2" PRINCMD3="$F3"`
close=`unset -l PRINT`

name="Print Command #1:"
nrow=1
ncol=1
rows=1
columns=45
frow=1
fcol=20
value=const $PRINCMD1 
valid=`indicator -w;echo "${F1}" | fmlcut -f1 -d" "  | set -l PRINT ; $VMSYS/bin/chexec "${PRINT}"`
invalidmsg=`regex -v "$PRINT" '^$' "A value must be entered for this field."
				'.*' "$PRINT is an invalid command."`
scroll=true

name="Print Command #2:"
nrow=3
ncol=1
frow=3
fcol=20
rows=1
columns=45
value=const $PRINCMD2 
valid=`indicator -w;echo "${F2}" | fmlcut -f1 -d" "  | set -l PRINT ; $VMSYS/bin/chexec "${PRINT}"`
invalidmsg=`regex -v "$PRINT" '^$' "A value must be entered for this field."
				'.*' "$PRINT is an invalid command."`
scroll=true

name="Print Command #3:"
nrow=5
ncol=1
frow=5
fcol=20
rows=1
columns=45
value=const $PRINCMD3 
valid=`indicator -w;echo "${F3}" | fmlcut -f1 -d" "  | set -l PRINT ; $VMSYS/bin/chexec "${PRINT}"`
invalidmsg=`regex -v "$PRINT" '^$' "A value must be entered for this field."
				'.*' "$PRINT is an invalid command."`
scroll=true

name=RESET
button=8
action=reset
