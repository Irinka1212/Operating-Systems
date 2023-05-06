#7100 - Да се напише shell скрипт, който приема два параметъра - име на директория и число. 
#Скриптът да извежда на стандартния изход имената на всички обикновени файлове във директорията, които имат размер, по-голям от подаденото число.

#!/bin/bash

if [[ $# -ne 2 ]]; then
	echo "wrong input"
	exit 1
fi

if [[ ! -d $1 ]]; then
	echo "Not a dir"
	exit 2
fi

dir=$1
num=$2

for file in $(find "$dir" -type f -size +${num}); do
	echo $(basename $file)
done