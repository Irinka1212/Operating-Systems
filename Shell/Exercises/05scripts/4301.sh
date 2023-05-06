#4301 - Напишете shell script, който автоматично да попълва файла указател от предната задача по подадени аргументи: 
#име на файла указател, пълно име на човека (това, което очакваме да е в /etc/passwd) и избран за него nickname.
#Файлът указател нека да е във формат:
#<nickname, който лесно да запомните> <username в os-server>
#// може да сложите и друг delimiter вместо интервал

#Примерно извикване:
#./pupulate_address_book myAddressBook "Ben Dover" uncleBen

#Добавя към myAddressBook entry-то:
#uncleBen <username на Ben Dover в os-server>

#***Бонус: Ако има няколко съвпадения за въведеното име (напр. има 10 човека Ivan Petrov в /etc/passwd), 
#всички те да се показват на потребителя, заедно с пореден номер >=1, след което той да 
#може да въведе някой от номерата (или 0 ако не си хареса никого), и само избраният да бъде добавен към указателя.

#!/bin/bash

if [[ $# -ne 3 ]]; then
	echo "Wrong input"
	exit 1
fi

if [[ ! -f ${1} ]]; then
	echo "File not found"
	exit 2
fi

file=$1
full_name=$2
nickname=$3
filename='/etc/passwd'

if grep -q "$full_name" "$filename"; then
	username=$(grep "$full_name" "$filename" | cut -d':' -f1)
fi

count=$(grep -c "$full_name" "$filename")
if [[ -z $count ]]; then
	echo "Name not found"
	exit 3
fi

if [[ $count -gt 1 ]]; then
	grep -n "$full_name" "$filename"
	read -p "Choose a num:" num;
	if [[ $num -eq 0 ]]; then
	echo "Zero can be an input"
	exit 4
	fi

	username=$(grep -n "$full_name" "$filename" | sed -n "$nump" | cut -d':' -f2)
fi

echo "${nickname}:${username}" >> ${file}