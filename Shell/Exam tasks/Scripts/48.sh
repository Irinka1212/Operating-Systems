#Зад. 48 2022-IN-01 Както знаете, при отваряне на файл с редактора vi, той създава в същата директория временен файл с име в следния формат: 
#точка, името на оригиналния файл, точка, swp. Например, при редактиране на файл с име foo.txt ще се създаде временен файл с име .foo.txt.swp.
#Напишете shell скрипт, който приема два задължителни позиционни аргумента – имена на директории. Примерно извикване: ./foo.sh ./dir1 /path/to/dir2/
#В dir1 може да има файлове/директории, директорията dir2 трябва да е празна. Скриптът трябва да копира всички обикновенни файлове от dir1 (и нейните под-директории) в dir2, 
#запазвайки директорийната структура, но без да копира временните файлове, създадени от редактора vi (по горната дефиниция).
#Забележка: За удобство приемаме, че не ни вълнува дали метаданните на обектите (owner, group, permissions, etc.) ще се запазят.
#Примерни обекти:
#dir1/
#dir1/a
#dir1/.a.swp
#dir1/b
#dir1/c/d
#dir1/c/.bar.swp
#Обекти след изпълнението:
#dir2/
#dir2/a
#dir2/b
#dir2/c/d
#dir2/c/.bar.swp

#!/bin/bash

if [[ $# -ne 2 ]]; then
	echo "wrong input"
	exit 1
fi

if [[ ! -d "${1}" ]] || [[ ! -d "${2}" ]]; then
	echo "not directories"
	exit 2
fi


find_dest=$(find ${2} -maxdepth 0 -empty)
if [[ -z "$find_dest" ]]; then
	echo "not empty dir"
	exit 3
fi

from=$1
to=$2

for file in $(find "${from}" -type f); do
	dir=$(dirname "${file}" | sed -E "s/(.*)\($file)\\///")
	f=$(basename "${file}")			
	if [[ "${f}" =~ \.swp$ ]]; then
		swp_name=$(echo "${f}" | cut -d'.' -f2)
		file_swp=$(find "${from}" -type f -name "${swp_name}")
		if [[ -z "${file_swp}" ]]; then
			mkdir -p "${to}/${dir}"
			cp "${file}" "${to}/${dir}/${f}"
		fi
	else
		mkdir -p "${to}/${dir}"
		cp "${file}" "${to}/${dir}/${f}"
	fi
done	