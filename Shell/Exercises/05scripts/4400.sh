#4400 - Напишете shell script, който да приема параметър име на директория, от която взимаме файлове, 
#и опционално експлицитно име на директория, в която ще копираме файлове. Скриптът да копира файловете
#със съдържание, променено преди по-малко от 45 мин, от първата директория във втората директория. Ако
#втората директория не е подадена по име, нека да получи такова от днешната дата във формат, който ви
#е удобен. При желание новосъздадената директория да се архивира.

#!/bin/bash

if [[ $# -gt 2 ]] || [[ $# -eq 0 ]]; then
     	echo "Wrong input"
	exit 1
fi

if [[ ! -d "$1" ]]; then
    echo "Not a dir"
    exit 2
fi

directory=$1

if [[ $# -eq 2 ]]; then
  copy_dir=$2
else
  copy_dir=$(date +%Y-%m-%d)
fi

if [[ ! -d $copy_dir ]]; then
  mkdir $copy_dir
fi

for file in $(find "$directory" -type f -mmin -45); do
  cp $file $copy_dir
done

read -p "Do you want to archive the directory?" answer
if [[ $answer == "yes"]]; then
	tar cJf copy_dir.tar.xz $copy_dir
fi
