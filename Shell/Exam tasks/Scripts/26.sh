#Зад. 26 2017-SE-03 Напишете скрипт, който ако се изпълнява от root потребителя:
#а) извежда обобщена информация за броя и общото количество активна памет (RSS - resident set size, non-swaped physical memory that a task has used) на текущите процеси на всеки потребител;
#б) ако процесът с най-голяма активна памет на даден потребител използва два пъти повече памет от средното за потребителя, то скриптът да прекратява изпълнението му по подходящ начин.
#За справка:
#$ ps aux | head -5
#USER PID %CPU %MEM VSZ RSS TTY STAT START TIME COMMAND
#root 1 0.0 0.0 15820 1052 ? Ss Apr21 0:06 init [2]
#root 2 0.0 0.0 0 0 ? S Apr21 0:00 [kthreadd]
#root 3 0.0 0.0 0 0 ? S Apr21 0:02 [ksoftirqd/0]
#root 5 0.0 0.0 0 0 ? S< Apr21 0:00 [kworker/0:0H]
#Алтернативно, може да ползвате изхода от ps -e -o uid,pid,rss

#!/bin/bash

#if [[ $(whoami) != "root" ]] ...

num_rss=$(ps -e -o rss= | wc -l)
all_rss=$(ps -e -o rss= | paste -sd+ - | bc)

ps -e -o user= | sort | uniq | while read user; do
	sum=$(ps -u $user -o rss= | paste -sd+ - | bc)
	#без paste?
	lines=$(ps -u $user -o rss= | wc -l)
	average=$(echo "scale=2; $sum/$lines" | bc)
	thebiggest=$(ps -u $user -o rss= --sort rss | tail -n 1)
	if (( $(echo "$thebiggest > 2*$average" | bc -l) )); then
	# if $( echo $thebiggest | bc ) -gt (( 2 * $average )); then
		#kill -KILL $(ps -u $user pid= --sort rss=| tail -n 1) 
		echo "killing $(ps -u $user -o pid= --sort rss | tail -n 1)"
	fi
	echo "$user : $sum : $lines"
done

#kill -TERM $pid
#sleep 2
#kill -KILL $pid