#9100 - Опишете поредица от команди или напишете shell скрипт, които/който при известни две директории SOURCE и DESTINATION:
#- намира уникалните "разширения" на всички файлове, намиращи се някъде под SOURCE. 
#(За простота приемаме, че в имената на файловете може да се среща символът точка '.' максимум веднъж.)
#- за всяко "разширение" създава по една поддиректория на DESTINATION със същото име
#- разпределя спрямо "разширението" всички файлове от SOURCE в съответните поддиректории в DESTINATION

#!/bin/bash

if [[ $# -ne 2 ]]; then
	echo "wrong input"
	exit 1 
fi

if [[ ! -d $1 || ! -d $2 ]]; then
	echo "not directories"
	exit 2
fi

source=$1
destination=$2

uniq_extensions=$(find "$source" -type f -name '*.*' | awk -F'.' '{print $NF}' | sort | uniq)
all_extensions=$(find "$source" -type f -name '*.*' | awk -F'.' '{print $NF}')

for ext in $uniq_extensions; do
	mkdir -p "$destination/$ext"
done

for ext in $all_extensions; do
	files=$(find "$source" -type f -name "*.$ext")
	for file in $files; do
		dest_dir="$destination/$ext"
		#mv "$file" "$dest_dir"
	done
done
