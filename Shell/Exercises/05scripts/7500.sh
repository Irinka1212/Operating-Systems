#7500 - Напишете shell script guess, която си намисля число, което вие трябва да познате. 
#В зависимостот вашия отговор, програмата трябва да ви казва "надолу" или "нагоре", докато не познате числото. 
#Когато го познаете, програмата да ви казва с колко опита сте успели.

#./guess (програмата си намисля 5)

#Guess? 22
#...smaller!
#Guess? 1
#...bigger!
#Guess? 4
#...bigger!
#Guess? 6
#...smaller!
#Guess? 5
#RIGHT! Guessed 5 in 5 tries!

#Hint: Един начин да направите рандъм число е с $(( (RANDOM % b) + a  )), което ще генерира число в 
#интервала [a, b]. Може да вземете a и b като параметри, но не забравяйте да направите проверката.

#!/bin/bash

read -p "Give me parameters a and b:" a b;

if [[ $a -gt $b ]]; then
	echo "a cant be greater than b"
fi

num=$(( (RANDOM % $b) + $a  ))

read -p "Guess?" input
#input if is in interval 4600.sh

count=0;
while [ ${input} -ne ${num} ]; do
	if [[ ${input} -lt ${num} ]]; then
		echo ".....bigger!"
		count=$(( count + 1 ))
		read -p "Guess?" input
	fi
	if [[ ${input} -gt ${num} ]]; then
		echo ".....smaller!"
		count=$(( count + 1 ))
		read -p "Guess?" input
	fi
done

echo "Right! Guessed $count"