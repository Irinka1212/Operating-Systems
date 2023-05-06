#Зад. 39 2020-SE-04 Напишете скрипт, който приема два аргумента - имена на директории. Първата (SRC ) съществува, докато втората (DST) трябва да бъде създадена от скрипта. 
#Директорията SRC и нейните поддиректории може да съдържат файлове, чиито имена завършат на .jpg. Имената на файловете може да съдържат интервали, както и поднизове, оградени със скоби, например:
#A single (very ugly) tree (Outdoor treks) 2.jpg
#Falcons.jpg
#Gorgonzola (cheese).jpg
#Leeches (two different ones) (Outdoor treks).jpg
#Pom Pom Pom.jpg
#За даден низ ще казваме, че е почистен, ако от него са премахнати leading и trailing интервалите и всички последователни интервали са сведени до един.
#За всеки файл дефинираме следните атрибути:
#• заглавие - частта от името преди .jpg, без елементи оградени в скоби, почистен. Примери:
#A single tree 2
#Falcons
#Gorgonzola
#Leeches
#Pom Pom Pom
#• албум - последният елемент от името, който е бил ограден в скоби, почистен. Ако албум е празен стринг, ползваме стойност по подразбиране misc. Примери:
#Outdoor treks
#misc
#cheese
#Outdoor treks
$misc
#• дата - времето на последна модификация на съдържанието на файла, във формат YYYYMM-DD
#• хеш - първите 16 символа от sha256 сумата на файла. 
#Забележка: приемаме, че в тази идеална вселена първите 16 символа от sha256 сумата са уникални за всеки файл от тези, които ще се наложи да обработваме.
#Скриптът трябва да създава в директория DST необходимата структура от под-директории, файлове и symlink–ове, така че да са изпълнени следните условия за всеки файл от SRC :
#• DST/images/хеш.jpg - копие на съответния файл
#• следните обекти са относителни symlink–ове към хеш.jpg:
#– DST/by-date/дата/by-album/албум/by-title/заглавие.jpg
#– DST/by-date/дата/by-title/заглавие.jpg
#– DST/by-album/албум/by-date/дата/by-title/заглавие.jpg
#– DST/by-album/албум/by-title/заглавие.jpg
#– DST/by-title/заглавие.jpg

#!/bin/bash

if [[ $# -ne 2 ]]; then 
	echo "wrong input"
	exit 1
fi

if [[ ! -d "${1}" ]]; then
	echo "not a dir"
	exit 2
fi


SRC=$1
DST=$2

mkdir -p ${DST}/images
mkdir -p ${DST}/by-title

for file in "${SRC}"/*.jpg; do
	title=$(basename "${file}" | cut -d'.' -f1 | sed 's/([^)]*)//g' | sed 's/)//g' | tr -s ' ') 
	if grep -q "(" "${file}"; then
		album=$(echo "${file}" | rev | cut -d'(' -f1 | cut -d')' -f2 | rev)
	else
		album="misc"
	fi
	date=$(date -d "@$(stat -c "%Y" "${file}")" "+%Y%m-%d")
	hash_=$(sha256sum "${file}" | cut -c1-16)
	
	mkdir -p "${DST}/by-date/${date}/by-album/${album}/by-title"
	mkdir -p "${DST}/by-date/${date}/by-title"
	mkdir -p "${DST}/by-album/${album}/by-date/${date}/by-title"
	mkdir -p "${DST}/by-album/${album}/by-title"

	cp "${file}" "${DST}/images/${hash_}.jpg"

	ln -s $(realpath ${DST}/images/${hash_}.jpg) "${DST}/by-date/${date}/by-album/${album}/by-title/${title}.jpg"
	ln -s $(realpath ${DST}/images/${hash_}.jpg) "${DST}/by-date/${date}/by-title/${title}.jpg"
	ln -s $(realpath ${DST}/images/${hash_}.jpg) "${DST}/by-album/${album}/by-date/${date}/by-title/${title}.jpg"
	ln -s $(realpath ${DST}/images/${hash_}.jpg) "${DST}/by-album/${album}/by-title/${title}.jpg"
	ln -s $(realpath ${DST}/images/${hash_}.jpg) "${DST}/by-title/${title}.jpg"
done	