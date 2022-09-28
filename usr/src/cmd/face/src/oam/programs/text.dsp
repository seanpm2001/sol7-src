#ident	"@(#)Text.dspell	1.2	92/07/14 SMI"	/* SVr4.0 1.5	*/

title="Removal of Spell Checker"

init=false

`indicator -w;
fmlgrep '^vmsys:' /etc/passwd | fmlcut -f6 -d: |set -e VMSYS;
if fmlgrep 'name="Spell Checker"' $VMSYS/lib/services > /dev/null;
then
	if $VMSYS/bin/delmenu "Spell Checker" > /dev/null;
	then
		message "Spell Checker has been removed.";
	else
		message "Spell Checker has not been removed.";
	fi;
else
	message "Spell Checker is not in the Programs Menu.";
fi`
