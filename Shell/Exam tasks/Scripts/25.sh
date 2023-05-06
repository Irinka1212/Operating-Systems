#Зад. 25 2017-SE-02 Напишете скрипт, който приема три задължителни позиционни параметра - директория SRC, директория DST (която не трябва да съдържа файлове) и низ АBC. 
#Ако скриптът се изпълнява от root потребителя, то той трябва да намира всички файлове в директорията SRC и нейните под-директории, които имат в името си като под-низ АBC, 
#и да ги мести в директорията DST, запазвайки директорийната структура (но без да запазва мета-данни като собственик и права,
#т.е. не ни интересуват тези параметри на новите директории, които скриптът би генерирал в DST).
#Пример:
#• в SRC (/src) има следните файлове:
#/src/foof.txt
#/src/1/bar.txt
#/src/1/foo.txt
#/src/2/1/foobar.txt
#/src/2/3/barf.txt
#• DST (/dst) е празна директория
#• зададения низ е foo
#Резултат:
#• в SRC има следните файлове:
#/src/1/bar.txt
#/src/2/3/barf.txt
#• в DST има следните файлове:
#/dst/foof.txt
#/dst/1/foo.txt
#/dst/2/1/foobar.txt

#!/bin/bash

if [[ $# -ne 3 ]]; then
	echo "wrong input"
	exit 1
fi

if [[ ! -d $1 ]] || [[ ! -d $2 ]] || ! [[ "$3" =~ ^[a-zA-Z]+$ ]]; then
	echo "not dir and string"
	exit 2
fi

find_dest=$(find ${2} -maxdepth 0 -empty)
if [[ -z "$find_dest" ]]; then
	echo "not empty dir"
	exit 3
fi

# if [[ $(whoami) != "root" ]] ...

SRC=$1
DST=$2
ABC=$3

for file in $(find $SRC -type f -name "*${ABC}*" -printf "%P\n"); do
	dir=$(dirname $file) 
	f=$(basename $file) 
	mkdir -p $DST/$dir
	#mv $file $DST/$dir/$f
	cp $file $DST/$dir/$f
done	