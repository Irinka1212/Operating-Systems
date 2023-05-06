#Зад. 46 2022-CE-01 Описание на формат на CSV (текстови) файл:
#• CSV файлът представлява таблица, като всеки ред на таблицата е записан на отделен ред;
#• на даден ред всяко поле (колона) е разделено от останалите със запетая;
#• в полетата не може да присъства запетая, т.е. запетаята винаги е разделител между полета;
#• броят на полетата на всеки ред е константа;
#• първият ред във файла e header, който описва имената на колоните.
#Разполагате с два CSV файла със следното примерно съдържание:
#• base.csv:
#unit name,unit symbol,measure
#second,s,time
#metre,m,length
#kilogram,kg,mass
#ampere,A,electric current
#kelvin,K,thermodynamic temperature
#mole,mol,amount of substance
#candela,cd,luminous intensity
#• prefix.csv:
#prefix name,prefix symbol,decimal
#tera,T,1000000000000
#giga,G,1000000000
#mega,M,1000000
#mili,m,0.001
#nano,n,0.000000001
#Където смисълът на колоните е:
#• за base.csv
#– unit name – име на мерна единица
#– unit symbol – съкратено означение на мерната единица
#– measure – величина
#• за prefix.csv
#– prefix name – име на представка
#– prefix symbol – означение на представка
#– decimal – стойност
#Забележка: Във файловете може да има и други редове, освен показаните в примера. Приемаме, че файловете спазват описания по-горе формат, 
#т.е. не е необходимо да проверявате формата.
#Напишете shell скрипт, който приема три задължителни параметъра: число, prefix symbol и unit symbol.
#Скриптът, ако може, трябва да извежда числото в основната мерна единица без представка, добавяйки в скоби величината и името на мерната единица.
#Примерен вход и изход:
#$ ./foo.sh 2.1 M s
#2100000.0 s (time, second)
#Забележка: За изчисления може да ползвате bc.

#!/bin/bash

if [[ $# -ne 3 ]]; then
	echo "wrong input"
	exit 1
fi

if ! [[ "${1}" =~ ^[0-9]+$ ]]; then
	echo "not a number"
	exit 2
fi

if ! [[ "${2}" =~ ^[a-zA-Z]$ ]]; then
	echo "not a prefix symbol"
	exit 3
fi

if ! [[ "${3}" =~ ^[a-zA-Z]+$ ]]; then
	echo "not a unit symbol"
	exit 4
fi

number=$1
prefix_symbol=$2
unit_symbol=$3

base="base.csv"
prefix="prefix.csv"

decimal=$(grep "${prefix_symbol}" "${prefix}" | cut -d',' -f3)
measure=$(grep "$,{unit_symbol}," "${base}" | cut -d',' -f3)
unit_name=$(grep "$,{unit_symbol}," "${base}" | cut -d',' -f1)

num=$(echo "$decimal*$number" | bc)

echo "${num} ${unit_symbol} (${measure}, ${unit_name})"