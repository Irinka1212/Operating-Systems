#Зад. 21 2017-IN-01 Напишете скрипт, който приема три задължителни позицонни аргумента:
#• име на фаил
#• низ1
#• низ2
#Файлът е текстови, и съдържа редове във формат:
#ключ=стойност
#където стойност може да бъде:
#• празен низ, т.е. редът е ключ=
#• низ, състоящ се от един или повече термове, разделени с интервали, т.е., редът е ключ=t1 t2 t3
#Някъде във файла:
#• се съдържа един ред с ключ първия подаден низ (низ1 );
#• и може да се съдържа един ред с ключ втория подаден низ (низ2 ).
#Скриптът трябва да променя реда с ключ низ2 така, че обединението на термовете на редовете с ключове низ1 и низ2 да включва всеки терм еднократно.
#Примерен входен файл:
#$ cat z1.txt
#FOO=73
#BAR=42
#BAZ=
#ENABLED_OPTIONS=a b c d
#ENABLED_OPTIONS_EXTRA=c e f
#Примерно извикване:
#$ ./a.sh z1.txt ENABLED_OPTIONS ENABLED_OPTIONS_EXTRA
#Изходен файл:
#$ cat z1.txt
#FOO=73
#BAR=42
#BAZ=
#ENABLED_OPTIONS=a b c d
#ENABLED_OPTIONS_EXTRA=e f

#!/bin/bash

if [[ $# -ne 3 ]]; then
	echo "wrong input"
	exit 1
fi

if [[ ! -f "$1" ]]; then
	echo "not a file"
	exit 2
fi

file_name=$1
first_key=$2
second_key=$3

first_value=$(egrep "^$first_key=" "$file_name" | cut -d'=' -f2-)
second_value=$(egrep "^$second_key=" "$file_name" | cut -d'=' -f2-)

if [ -z "$second_value" ]; then
	echo "not found key"
  exit 3
fi 

key=()
while read character; do

	if [ $(echo "$first_value" | grep "$character" | wc -l) -eq 0 ]; then
		key+=("$character")
	fi

done < <(echo $second_value | tr ' ' '\n')

key_string=$(printf "%s " "${key[@]}")
 
sed "s/$second_key=$second_value/$second_key=$key_string/g" $file_name