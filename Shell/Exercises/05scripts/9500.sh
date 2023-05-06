#9500 - (Цветно принтиране) Напишете shell script color_print, който взима два параметъра.

#Първият може да е измежду "-r", "-g" "-b", а вторият е произволен string.
#На командата "echo" може да се подаде код на цвят, който ще оцвети текста в определения цвят.
#В зависимост от първия аргумент, изпринтете втория аргумен в определения цвят:

#"-r" е червено. Кодът на червеното е '\033[0;31m' (echo -e "\033[0;31m This is red")
#"-g" е зелено. Кодът на зеленото е '\033[0;32m' (echo -e "\033[0;32m This is green")
#"-b" е синьо. Кодът на синьото е '\033[0;34m' (echo -e "\033[0;34m This is blue")
#Ако е подадена друга буква изпишете "Unknown colour", а ако изобщо не е подаден аргумент за цвят, просто изпишете текста.

#Hint:

#В края на скрипта си напишете: echo '\033[0m', за да не се прецакат цветовете на терминала. Това е цветът на "няма цвят".

#!/bin/bash

if [[ $# -eq 0 ]] || [[ $# -gt 2 ]]; then
	echo "wrong input"
	exit 1
fi

if [[ $# -eq 2 ]]; then

	color=$1
	string=$2

	if ! [[ "$string" =~ ^[a-zA-Z]+$ ]]; then
		echo "wrong second argument"
		exit 2
	fi

	if [[ "$color" == "-r" ]]; then
		echo -e "\033[0;31m $string"
	elif [[ "$color" == "-g" ]]; then
		echo -e "\033[0;32m $string"
	elif [[ "$color" == "-b" ]]; then
		echo -e "\033[0;34m $string"
	elif ! [[ "$color" =~ "-[rgb]" ]]; then
		echo "Unknown colour"
	fi
fi

if [[ $# -eq 1 ]]; then
	if ! [[ "$1" =~ ^[a-zA-Z]+$ ]]; then
		echo "wrong second argument"
		exit 2
	fi
	echo "$1";
fi