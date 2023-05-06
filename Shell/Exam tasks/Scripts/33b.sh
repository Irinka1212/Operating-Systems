#Зад. 33 2019-SE-01 Напишете два скрипта (по един за всяка подточка), които четат редове от STDIN.
#Скриптовете трябва да обработват само редовете, които съдържат цели положителни или отрицателни числа;
#останалите редове се игнорират. Скриптовете трябва да извежда на STDOUT:
#б) всички най-малки уникални числа от тези, които имат максимална сума на цифрите си
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
#Примерен изход за б):
#-42

#!/bin/bash

num_sums=$(mktemp)
while read line; do
	if ! [[ "$line" =~ ^(-)?[0-9]+$ ]]; then
        	continue
    	fi

	number=$(echo "$line")
	if [[ $number -lt 0 ]]; then
		copy_number=$number
        	number=$(( $number*(-1) ))
    	elif [[ "$number" -gt 0 ]]
		copy_number=$number
    	fi
        
	sum=0
        while [ $number -ne 0 ]; do
		digit=$(( $number%10 ))
        	sum=$(( $sum+$digit ))
                number=$(( $number/10 ))
        done

	echo "$copy_number $sum" >> $num_sums	
done

result=$(cat $num_sums|sort -t' ' -k2rn -k1n | head -n 1 | cut -d ' ' -f1)
echo "$result"

rm $num_sums