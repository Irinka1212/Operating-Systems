#7550 - Да се напише shell скрипт, който приема параметър - име на потребител. 
#Скриптът да прекратява изпълненито на всички текущо работещи процеси на дадения потребител, 
#и да извежда колко са били те.

#NB! Може да тествате по същият начин като описаният в 05-b-4300

#!/bin/bash

if [[ $# -ne 1 ]]; then
	echo "wrong input"
	exit 1
fi

count=$(ps -u $1 | wc -l)
#kill -KILL $1
echo "kill $count"
