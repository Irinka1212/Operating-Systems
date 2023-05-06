#Зад. 44 2021-SE-03 Напишете shell скрипт, който приема два позиционни параметъра – имена на файлове. Примерно извикване: $ ./foo.sh input.bin output.h
#Файлът input.bin е двоичен файл с елементи uint16_t числа, създаден на little-endian машина.
#Вашият скрипт трябва да генерира C хедър файл, дефиниращ масив с име arr, който:
#• съдържа всички елементи от входния файл;
#• няма указана големина;
#• не позволява промяна на данните.
#Генерираният хедър файл трябва да:
#• съдържа и uint32_t променлива arrN, която указва големината на масива;
#• бъде валиден и да може да се #include-ва без проблеми от C файлове, очакващи да “виждат” arr и arrN.
#За да е валиден един входен файл, той трябва да съдържа не повече от 524288 елемента.
#За справка, dump на съдържанието на примерен input.bin:
#00000000: 5555 5655 5955 5a55 6555 6655 6955 6a55 UUVUYUZUeUfUiUjU
#00000010: 9555 9655 9955 9a55 a555 a655 a955 aa55 .U.U.U.U.U.U.U.U

#!/bin/bash

if [[ $# -ne 2 ]]; then
	echo "wrong input"
	exit 1
fi

if [[ ! -f "${1}" ]] || [[ ! -f "${2}" ]]; then 
	exit "not files"
	exit 2
fi

input=$1
output=$2

size=$( stat -c "%s" "${input}" )

echo -e "#include <stdio.h>\n" >> ${output}

arrN=$( echo "$size/2" | bc )

echo -e "const uint32_t arrN = $arrN\n" >> ${output}
echo -e "const uint16_t arr[] = {\n" >> ${output}

for num in $( xxd ${input} | cut -c 10-49 ); do
	last=$( echo "${num}" | egrep -o "..$" )
	first=$( echo "${num}" | egrep -o "^.." )

	new="0x$last$first"

	if [[ ${count} -eq 0 ]]; then
		echo -n "${new}" >> ${output}
	else
		echo -ne ",${new}" >> ${output}
	fi
done
echo -e "};" >> ${output}
