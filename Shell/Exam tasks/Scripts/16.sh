#Зад. 16 2016-SE-02 Напишете shell скрипт, който приема един позиционен параметър - число. 
#Ако скриптът се изпълнява като root, да извежда обобщена информация за общото количество активна памет 
#(RSS - resident set size, non-swapped physical memory that a task has used) на процесите на всеки потребител.
#Ако за някой потребител обобщеното число надвишава подадения параметър, да изпраща подходящи сигнали за прекратяване на процеса с най-много активна памет на потребителя.
#Забележка: Приемаме, че изхода в колоната RSS е число в същата мерна единица, като числото, подадено като аргумент. 
#Примерен формат:
#USER PID %CPU %MEM VSZ RSS TTY STAT START TIME COMMAND
#root 1 0.0 0.0 15816 1884 ? Ss May12 0:03 init [2]
#root 2 0.0 0.0 0 0 ? S May12 0:00 [kthreadd]
#root 3 0.0 0.0 0 0 ? S May12 0:02 [ksoftirqd/0]
#Забележка: Алтернативно може да ползвате изхода от ps -e -o uid,pid,rss

#!/bin/bash

# if [[ $(whoami) != "root" ]] ...

if [[ $# -ne 1 ]]; then
	echo "Wrong input"
	exit 1
fi

if ! [[ "$1" =~ ^[0-9]+$ ]]; then
	echo "not a number"
	exit 2
fi

ps -e -o user= | sort | uniq | while read user; do
	sum=$(ps -u $user -o rss= | paste -sd+ - | bc)
	if [[ ${sum} -gt ${1} ]]; then
		#kill -KILL $(ps -u $user pid= --sort rss | tail -n 1) 
		echo "killing $(ps -u $user -o pid= --sort rss | tail -n 1)"
	fi
done
