#7700 - Да се напише shell скрипт, който приема два параметъра - име на директория и число. 
#Скриптът да извежда сумата от размерите на файловете в директорията, които имат размер, по-голям от подаденото число.

#!/bin/bash

if [[ $# -ne 2 ]]; then
	echo "wrong input"
	exit 1
fi

if [[ ! -d $1 ]]; then
	echo "not a dir"
	exit 2
fi

dir=$1
num=$2

sum=0
for file in $(find "$dir" -maxdepth 1 -type f -size +${num}); do
	sum=$(( sum + $(stat -c %s "$file") ))
done

echo "Sum:$sum"

