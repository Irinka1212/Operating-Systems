#7200 - Да се напише shell скрипт, който приема произволен брой аргументи - имена на файлове или директории. 
#Скриптът да извежда за всеки аргумент подходящо съобщение:
#	- дали е файл, който може да прочетем
#	- ако е директория - имената на файловете в нея, които имат размер, по-малък от броя на 
#	  файловете в директорията.

#!/bin/bash

for arg in ${@}; do
    if [[ ! -f "$arg" ]] && [[ ! -d "$arg" ]]; then
        echo "wrong input"
        exit 1
    fi

    if [[ -f "$arg" ]]; then
	if [[ -r "$arg" ]]; then
    		echo "$arg is readable"
	else
    		echo "$arg is not readable"
	fi
    fi

    if [[ -d "$arg" ]]; then
	num_files=$(find "$arg" -maxdepth 1 -type f | wc -l)
        for file in $(find "$arg" -maxdepth 1 -type f -size -${num_files}); do
	         echo "$(basename $file) from the $arg"
    	done
    fi
done
