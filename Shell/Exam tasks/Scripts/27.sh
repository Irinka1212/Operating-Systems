#Зад. 27 2017-SE-04 Напишете shell script, който получава задължителен първи позиционен параметър – директория и незадължителен втори – име на файл. 
#Скриптът трябва да намира в подадената директория и нейните под-директории всички symlink-ове 
#и да извежда (при подаден аргумент файл – добавяйки към файла, а ако не е – на стандартния изход) за тях следната информация:
#• ако destination-a съществува – името на symlink-а -> името на destination-а;
#• броя на symlink-овете, чийто destination не съществува.
#Примерен изход:
#lbaz -> /foo/bar/baz
#lqux -> ../../../qux
#lquux -> /foo/quux
#Broken symlinks: 34

#!/bin/bash

if [[ $# -gt 2 ]] || [[ $# -eq 0 ]]; then
	echo "wrong input"
	exit 1
fi

if [[ ! -d "$1" ]]; then
	echo "not a dir"
	exit 2
fi

dir=$1

OUT="/dev/stdout"

if [[ -n "${2}" ]]; then
	OUT="${2}"
fi

while read -d $'\0' FILE; do
	if realpath -qe "${FILE}" &> /dev/null; then
		#readlink = find "$dir" -type l -printf '%l\n'
		echo "$(basename "${FILE}") -> $(readlink ${FILE}" >> ${OUT}
	else
		BROKEN=$((BROKEN+1))
	fi
done < <(find "${dir}" -type l -print0)

echo "Broken symlinks:${BROKEN}" >> ${OUT}

