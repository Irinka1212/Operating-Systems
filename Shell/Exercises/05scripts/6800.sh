#6800 - Да се напише shell скрипт, който получава единствен аргумент директория и отпечатва списък с всички файлове и директории в нея (без скритите).
#До името на всеки файл да седи размера му в байтове, а до името на всяка директория да седи броят на елементите в нея 
#(общ брой на файловете и директориите, без скритите).

#Добавете параметър -a, който указва на скрипта да проверява и скритите файлове и директории.

#Пример:
#$ ./list.sh .
#asdf.txt (250 bytes)
#Documents (15 entries)
#empty (0 entries)
#junk (1 entry)
#karh-pishtov.txt (8995979 bytes)
#scripts (10 entries)

#!/bin/bash

if [[ $# -eq 0 ]] || [[ $# -gt 2 ]]; then
	echo "wrong input"
	exit 1
fi

if [[ ! -d $1 ]]; then
	echo "not a dir"
	exit 2
fi

if [[ $# -eq 1 ]]; then

for file in $(find "$1" -maxdepth 1 -type f -not -path '*/\.*'); do
	bytes=$(wc -c $file | awk '{print $1}')
	echo "File:$(basename $file) and bytes: $bytes"
done

for dir in $(find "$1" -maxdepth 1 -type d -not -path '*/\.*'); do
	num_dir=$(find "$dir" -type d -not -path '*/\.*'| wc -l)
	num_files=$(find "$dir" -type f -not -path '*/\.*'| wc -l)
	num_all=$(($num_dir + $num_files))
	echo "Dir:$(basename $dir) and count:$num_all"
done
fi

if [[ $# -eq 2 ]]; then
	if [[ "$2" != "-a" ]]; then
		echo "invalid argument"
		exit 3
	fi

for file in $(find "$1" -maxdepth 1 -type f); do
	bytes=$(wc -c $file | awk '{print $1}')
	echo "File:$(basename $file) and bytes: $bytes"
done

for dir in $(find "$1" -maxdepth 1 -type d); do
	num_dir=$(find "$dir" -type d| wc -l)
	num_files=$(find "$dir" -type f| wc -l)
	num_all=$(($num_dir + $num_files))
	echo "Dir:$(basename $dir) and count:$num_all"
done
fi