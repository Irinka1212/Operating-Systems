#Зад. 33 2019-SE-01 Напишете два скрипта (по един за всяка подточка), които четат редове от STDIN.
#Скриптовете трябва да обработват само редовете, които съдържат цели положителни или отрицателни числа;
#останалите редове се игнорират. Скриптовете трябва да извежда на STDOUT:
#а) всички уникални числа, чиято абсолютна стойност е равна на максималната абсолютна стойност сред всички числа
#Примерен вход:
#We don’t
#n11d n0
#educat10n
#12.3
#6
#33
#-42
#-42
#111
#111
#-111
#Примерен изход за а):
#-111
#111

#!/bin/bash

nums=()
max_value=0
while read line; do
	if ! [[ "$line" =~ ^(-)?[0-9]+$ ]]; then
		continue
	fi
	
	if [[ "$line" -lt 0 ]]; then
		line=$(( $line*(-1) ))
		
		if [[ "$line" -gt "$max_value" ]]; then
			max_value=$line
		fi

		line=$(( $line*(-1) ))
	elif [[ "$line" -gt 0 ]]; then
		if [[ "$line" -gt "$max_value" ]]; then
			max_value=$line
		fi
	fi

	nums+=("$line")
done

values=$(mktemp)
for num in "${nums[@]}"; do
	
	if [[ "$num" -lt 0 ]]; then
		num=$(( $num*(-1) ))
		
		if [[ "$num" -eq "$max_value" ]]; then
			num=$(( $num*(-1) ))
			echo "$num" >> $values
		fi

	elif [[ "$num" -gt 0 ]]; then
		if [[ "$num" -eq "$max_value" ]]; then
			echo "$num" >> $values 
		fi
	fi
done

result=$(cat "$values" | sort | uniq)
echo "$result"

rm $values
