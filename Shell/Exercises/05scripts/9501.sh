#9501 - Този път програмата ви ще приема само един параметър, който е измежду ("-r", "-b", "-g", "-x").
#Напишете shell script, който приема редовете от stdin и ги изпринтва всеки ред с редуващ се цвят. 
#Цветовете вървят RED-GREEN-BLUE и цветът на първия ред се определя от аргумента.
#Ако е подаден аргумент "-x", то не трябва да променяте цветовете в терминала (т.е., все едно сте извикали командата cat).

#Hint: Не забравяйте да връщате цветовете в терминала.

#!/bin/bash

if [[ $# -ne 1 ]]; then
	echo "wrong input"
	exit 1
fi

if ! [[ "$1" =~ ^(-r|-b|-g|-x)$ ]]; then
	echo "wrong command"
	exit 2
fi

next_color=""
while read line; do
	if [[ "$1" == "-r" ]]; then
		echo -e "\033[0;31m $line\033[0m"
		next_color="-g"
	elif [[ "$1" == "-g" ]]; then
		echo -e "\033[0;32m $line\033[0m"
		next_color="-b"
	elif [[ "$1" == "-b" ]]; then
		echo -e "\033[0;34m $line\033[0m"
		next_color="-r"
	elif [[ "$1" == "-x" ]]; then
		echo "$line"
		next_color="-x"
	fi

	shift
	if [[ "$next_color" != "-x" ]]; then
		set -- "$@" "$next_color"
	fi
done
