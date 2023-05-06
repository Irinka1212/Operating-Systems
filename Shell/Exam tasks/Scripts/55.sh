# Зад. 55 2023-IN-02 Вашите колеги от съседната лаборатория ползват специализиран софтуер за оптометрични изследвания, който записва резултатите от всяко измерване в отделен файл. 
# Файловете имат уникално съдържание, по което се определя за кое измерване се отнася файла. За съжаление, тъй като колегите ви ползват бета версия на софтуера, 
# той понякога записва по няколко пъти резултатите от дадено измерване в произволна комбинация от следните варианти:
# • нула или повече отделни обикновенни файла с еднакво съдържание;
# • нула или повече групи от hardlink-ове, като всяка група съдържа две или повече имена на даден файл с измервания.
# Помогнете на колегите си, като напишете shell скрипт, който приема параметър – име на директория, съдържаща файлове с измервания. 
# Скриптът трябва да извежда на стандартния изход списък с имена на файлове, кандидати за изтриване, по следните критерии:
# • ако измерването е записано само в отделни файлове, трябва да остане един от тях;
# • ако измерването е записано само в групи от hardlink-ове, всяка група трябва да се намали с едно име;
# • ако измерването е записано и в групи, и като отделни файлове, за групите се ползва горния критерий, а всички отделни файлове се премахват.

#!/bin/bash

if [[ $# -ne 1 ]]; then
	echo "wrong input"
	exit 1
fi

if [[ ! -d "${1}" ]]; then
	echo "not a dir"
	exit 2
fi

T=$(mktemp -d)

while read inode num file; do
	hash_sum=$(sha1sum "$file" | cut -d' ' -f1)
	echo "$inode" $num >> "$T/$hash_sum"
done < <(find "$1" -type f -printf '%i %n %p\n')

while read file;do
	data=$(sort "$file" | uniq)
	count=$(echo "$data" | cut -d' ' -f2)

	if [[ -z "(echo "$data" | egrep -v ' 1$')" ]]; then
		while read inode; do
			find "$1" -type f -inum "$inode"
		done < <(echo "$data" | tail -n +2 | cut -d' ' -f1)
	elif ! echo "$count" | egrep -q ' 1$'; then
		while read inode; do
			find "$1" -type f -inum "$inode" | tail -n 1
		done < <(echo "$data" |  cut -d' ' -f1)
	else
		while read inode; do
			find "$1" -type f -inum "$inode" | tail -n 1
		done < <(echo "$data" | egrep ' 1$' | cut -d' ' -f1)
		
		while read inode; do
			find "$1" -type f -inum "$inode" | tail -n 1
		done < <(echo "$data" | egrep -v ' 1$' | cut -d' ' -f1)
	fi
done < <(find "$T" -type f)

rm -r "$T"