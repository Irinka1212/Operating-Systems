#7000 - Да се напише shell скрипт, който приема произволен брой аргументи - имена на файлове. 
#Скриптът да прочита от стандартния вход символен низ и за всеки от зададените файлове 
#извежда по подходящ начин на стандартния изход броя на редовете, които съдържат низа.

#NB! Низът може да съдържа интервал.

#!/bin/bash

for file in ${@}; do
    if [[ ! -f "$file" ]]; then
        echo "$file is not a file"
        exit 1
    fi
done

read -p "Give me a string:" string

for file in ${@}; do
	if grep -q "$string" "$file"; then
	    grep -c "$string" "$file"
	fi
done
