#6600 - Да се напише shell скрипт, който получава единствен аргумент директория и изтрива всички 
#повтарящи се (по съдържание) файлове в дадената директория. Когато има няколко еднакви файла, да се 
#остави само този, чието име е лексикографски преди имената на останалите дублирани файлове.

#Примери:
#$ ls .
#f1 f2 f3 asdf asdf2
## asdf и asdf2 са еднакви по съдържание, но f1, f2, f3 са уникални

#$ ./rmdup .
#$ ls .
#f1 f2 f3 asdf
## asdf2 е изтрит

#!/bin/bash

if [[ $# -eq 0 ]] || [[ $# -gt 1 ]]; then
	echo "wrong input"
	exit 1
fi

if [[ ! -d $1 ]]; then
	echo "not a dir"
	exit 2
fi


for file1 in $(find "$1" -type f); do
 	for file2 in $(find "$1" -type f); do
		if cmp -s "$file1" "$file2"; then
			if [[ "$file1" < "$file2" ]]; then
				rm "$file2"
			else
				if [[ "$file1" != "$file2" ]]; then
					rm "$file1"
				fi
			fi
		fi
	done
done
