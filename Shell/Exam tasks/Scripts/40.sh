#Зад. 40 2020-SE-05 Напишете shell скрипт, който приема 3 позиционни аргумента – две имена на файлове и име на директория. Примерно извикване:
#$ ./foo.sh foo.pwd config.cfg cfgdir/
#В директорията cfgdir/ и нейните под-директории може да има файлове с имена завършващи на .cfg. 
#За да са валидни, тези файлове трябва да съдържат редове само в следните формати (редовете започващи с # са коментари):
#Във файла foo.pwd има описани потребителски имена (username) и MD5 хеш суми на паролите им, с по един запис на ред, в следният формат:
#username:password_hash
#Също така, разполагате с команда pwgen, която генерира и извежда на STDOUT случайни пароли, и знаете, че поддържа следните два аргумента:
#$ pwgen [ password_length ] [ number_of_passwords ]
#Вашият скрипт трябва да валидира cfg файловете в директорията, и за тези, които не са валидни, 
#да извежда на STDOUT името на файла и номерирани редовете, които имат проблем, в следния формат:
#Error in filename.cfg:
#Line 1:XXXX
#Line 37:YYYY
#където XXXX и YYYY е съдържанието на съответния ред.
#За валидните файлове, скриптът трябва да:
#• генерира config.cfg като обединение на съдържанието им;
#• името на файла, без частта .cfg дефинира потребителско име. Ако във файла с паролите не съществува запис за този потребител, 
#то такъв да се добави и на стандартния изход да се изведе потребителското име и паролата (поне 16 символа) разделени с един интервал.

#!/bin/bash

if [[ $# -ne 3 ]]; then
	echo "wrong input"
	exit 1
fi

if [[ ! -f "${1}" ]] || [[ ! -f "${2}" ]]; then
	echo "not files"
	exit 2
fi

if [[ ! -d "${3}" ]]; then
	echo "not a dir"
	exit 3
fi

dir=$3

for file in "$dir"/*.cfg; do
  if [[ -f "${file}" ]]; then
   	errors=0
    while read line; do
      	if [[ "${line}" =~ ^[[:space:]]*#.*$ ]]; then
        	continue  
      	elif [[ "${line}" =~ ^[[:space:]]*[{][[:space:]]*(no-production|volatile|run-all)[[:space:]]*[}][[:space:]]*[;][[:space:]]*$ ]]; then
        	continue  
      	else
        	echo "Error in ${file}:"
        	echo "Line ${errors}: ${line}"
        	errors=$((errors+1))
  	fi
    done < "${file}"

    if [[ ${errors} -eq 0 ]]; then
    	cat "${file}" >> "${2}"
	username=$(basename "${file}" | cut -d'.' -f1)
      	
	if ! grep -q "^${username}:" "${1}"; then
        	password=$(pwgen 16 1)
        	echo "${username}:${password}" >> "${1}"
        	echo "New user added: ${username} ${password}"
      	fi
    fi
  fi
done