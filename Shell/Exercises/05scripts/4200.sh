#4200 - Имате компилируем (a.k.a няма синтактични грешки) source file на езика C. 
#Напишете shell script, който да покaзва колко е дълбоко най-дълбокото nest-ване (влагане).
#Примерен .c файл:

##include <stdio.h>

#int main(int argc, char *argv[]) {

#  if (argc == 1) {
#		printf("There is only 1 argument");
#	} else {
#		printf("There are more than 1 arguments");
#	}
#
#	return 0;
#}
#Тук влагането е 2, понеже имаме main блок, а вътре в него if блок.

#Примерно извикване на скрипта:

#./count_nesting sum_c_code.c

#Изход:
#The deepest nesting is 2 levels

#!/bin/bash

if [[ $# -ne 1 ]]; then
  echo "Wrong input"
  exit 1
fi

file=$1

if [[ ! -f ${file} ]]; then
  echo "File not found: ${file}"
  exit 2
fi

max=0
current=0

while read line; do
  if echo "$line" | grep -q "{"; then
    current=$((current + 1))
    if [[ $current -gt $max ]]; then
      	max=$current
    fi
 elif echo "$line" | grep -q "}"; then
    current=$((current - 1))
  fi
done < ${file}

echo "The deepest nesting is $((max - 1)) levels"