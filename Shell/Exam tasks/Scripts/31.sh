#Зад. 31 2018-SE-02 Напишете скрипт, който приема два позиционни аргумента – име на текстови файл и директория.
#Директорията не трябва да съдържа обекти, а текстовият файл (US-ASCII) е стенограма и всеки ред е в следния формат:
#ИМЕ ФАМИЛИЯ (уточнения): Реплика
#където:
#• ИМЕ ФАМИЛИЯ присъстват задължително;
#• ИМЕ и ФАМИЛИЯ се състоят само от малки/главни латински букви и тирета;
#• (уточнения) не е задължително да присъстват;
#• двоеточието ‘:’ присъства задължително;
#• Репликата не съдържа знаци за нов ред;
#• в стринга преди двоеточието ‘:’ задължително има поне един интервал между ИМЕ и ФАМИЛИЯ;
#• наличието на други интервали където и да е на реда е недефинирано.
#Примерен входен файл:
#John Lennon (The Beatles): Time you enjoy wasting, was not wasted.
#Roger Waters: I’m in competition with myself and I’m losing.
#John Lennon:Reality leaves a lot to the imagination.
#Leonard Cohen:There is a crack in everything, that’s how the light gets in.
#Скриптът трябва да:
#• създава текстови файл dict.txt в посочената директория, който на всеки ред да съдържа:
#ИМЕ ФАМИЛИЯ;НОМЕР
#където:
#– ИМЕ ФАМИЛИЯ е уникален участник в стенограмата (без да се отчитат уточненията);
#– НОМЕР е уникален номер на този участник, избран от вас.
#• създава файл НОМЕР.txt в посочената директория, който съдържа всички (и само) редовете на дадения участник.

#!/bin/bash

if [[ $# -ne 2 ]]; then
	echo "wrong input"
	exit 1
fi

if ! [[ "$1" =~ ^[a-zA-Z-]+$ ]]; then
	echo "not a string"
	exit 2
fi

if [[ ! -d "$2" ]]; then
	echo "not a dir"
	exit 3
fi

find_dir=$(find "$2" -maxdepth 0 -empty)
if [[ -z "$find_dir" ]]; then
	echo "not empty"
	exit 4
fi

file=$1
dir=$2

lines=$(egrep "(^[a-zA-Z-]+[[:space:]]+([a-zA-Z-]+)[[:space:]]?(:?\([a-zA-Z-]+\))?:([^\\n]+)" "$file")

touch $dir/dict.txt

count=1
for line in "${lines}"; do
	if echo "$line" | grep -q "("; then
		names=$(echo "$line" | cut -d':' -f1 | cut -d'(' -f1) 
	else
		names=$(echo "$line" | cut -d':' -f1)
	fi
	
	read -p "Give me a name: " name
	if echo "$names" | grep -q "$name"; then
		file_num="$count.txt"
		touch "$dir/$file_num"
		echo "$lines" | grep "${name}" >> "$dir/$file_num"
	fi
done

count=1
while read -r line; do
  if echo "$line" | grep -q "("; then
    names=$(echo "$line" | cut -d':' -f1 | cut -d'(' -f1) 
  else
    names=$(echo "$line" | cut -d':' -f1)
  fi

  echo "${names};${count}" >> "$dir/dict.txt"
  count=$(( count+1 ))

done <<< "$lines"