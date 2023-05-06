#Зад. 42 2021-SE-01 Разполагате с машина, на която е инсталиран специализиран софтуер, който ползва два потребителски акаунта – oracle и grid. 
#Всеки от потребителите би трябвало да има environment променлива ORACLE_HOME, която указва абсолютен път до директория във формат /path/to/dir. 
#В поддиректория bin на зададената директория би трябвало да има изпълним файл с име adrci. Всеки от двата потребителя има собствена директория diag_dest, 
#която е във вида /u01/app/потребител. Когато някой от потребителите изпълни неговото копие на командата adrci с параметър 
#exec="show homes" може да получи на STDOUT един от следните два изхода:
#• вариант 1: (неуспех): No ADR homes are set
#• вариант 2: (успех):
#ADR Homes:
#diag/rdbms/orclbi/orclbi1
#diag/rdbms/orclbi/orclbi2
#И в двата случая командата приключва с exit code 0. Ако командата се изпълни успешно, тя връща списък с един или повече ADR Homes, 
#които са релативни имена на директории спрямо diag_dest на съответният потребител.
#Напишете скрипт, който може да се изпълнява само от някой от тези два акаунта, и извежда на STDOUT размера в мегабайти и абсолютният път на всеки ADR Home.
#Примерен изход:
#0 /u01/app/oracle/diag/rdbms/orclbi/orclbi1
#389 /u01/app/oracle/diag/rdbms/orclbi/orclbi2

#!/bin/bash

if [[ "$(whoami)" != "oracle" ]] && [[ "$(whoami)" != "grid" ]]; then
	echo "not a valid account"
	exit 1
fi

#export ORACLE_HOME=/path/to/dir
#ADR=$(find "$ORACLE_HOME" | egrep "/bin/adrci$" | exec="show homes")
ADR=$($ORACLE_HOME/bin/adrci exec="show homes")

if ! echo "${ADR}" | grep -q "^ADR Homes:"; then
	exit 0
fi

diag_dest="/u01/app/$(whoami)"
while read file;do

	find "${diag_dest}" -printf '%s %p\n'| egrep "${file}$"

done < <(echo "$ADR" | tail -n +2)