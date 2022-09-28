#ident	"@(#)Text.aspell	1.2	92/07/14 SMI"	/* SVr4.0 1.5	*/

title="Installation of Spell Checker"

init=false

`indicator -w;
fmlgrep '^vmsys:' /etc/passwd | fmlcut -f6 -d: |set -e VMSYS;
if fmlgrep 'name="Spell Checker"' $VMSYS/lib/services > /dev/null;
then
	message "Spell Checker is already in the Programs Menu.";
elif [ ! -x /usr/bin/spell ];
then
	message "The SPELL Utilities do not exist on your system yet.";
else
	if $VMSYS/bin/addmenu "Spell Checker" "OBJECTS/spell/Form.spell" > /dev/null;
	then
		message "Spell Checker has been added.";
	else
		message "Spell Checker has not been added.";
	fi;
fi`
