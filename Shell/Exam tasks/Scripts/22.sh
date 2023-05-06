#Зад. 22 2017-IN-02 Напишете скрипт, който приема задължителен позиционен аргумент - име на потребител FOO. 
#Ако скриптът се изпълнява от root:
#а) да извежда имената на потребителите, които имат повече на брой процеси от FOO, ако има такива;
#б) да извежда средното време (в секунди), за което са работили процесите на всички потребители на системата (TIME, във формат HH:MM:SS);
#в) ако съществуват процеси на FOO, които са работили над два пъти повече от средното време, скриптът да прекратява изпълнението им по подходящ начин.
#За справка:
#$ ps -e -o user,pid,%cpu,%mem,vsz,rss,tty,stat,time,command | head -5
#USER PID %CPU %MEM VSZ RSS TT STAT TIME COMMAND
#root 1 0.0 0.0 15820 1920 ? Ss 00:00:05 init [2]
#root 2 0.0 0.0 0 0 ? S 00:00:00 [kthreadd]
#root 3 0.0 0.0 0 0 ? S 00:00:01 [ksoftirqd/0]
#root 5 0.0 0.0 0 0 ? S< 00:00:00 [kworker/0:0H]

#!/bin/bash

if [[ $# -ne 1 ]]; then
	echo "wrong input"
	exit 1
fi

FOO=$1

num_FOO=$(ps -u $FOO -o user= | wc -l)

ps -e -o user= | sort | uniq -c | tr -s ' ' | awk  '$1 > $num_FOO {print $2}'

all_time=$(ps -e -o time= | awk -F: '{ print ($1 * 3600) + ($2 * 60) + $3 }' | paste -sd+ - | bc) 
all_users=$(ps -e -o time= | wc -l)
average_time=$(( $all_time / $all_users ))
#average_time=$(echo "$all_time / $all_users" | bc)

echo "Average time: $(date -d@$average_time -u +%H:%M:%S)"

ps -u $FOO -o user= | while read user; do
	user_time=$(ps -u $FOO -o time= | awk -F: '{ print ($1 * 3600) + ($2 * 60) + $3 }')
	if (( $(echo "$user_time > 2*$average_time" | bc -l) )); then
		#kill -KILL $(ps -u $FOO -o pid=) 
		echo "killing $(ps -u $FOO -o pid=)"
	fi
done
