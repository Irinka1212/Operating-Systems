#Зад. 19 2016-SE-04 Файловете във вашата home директория съдържат информация за музикални албуми и имат специфична структура. 
#Началото на всеки ред е годината на издаване на албума, анепосредствено, след началото на всеки ред следва името на изпълителя на песента. 
#Имената на файловете се състоят от една дума, която съвпада с името на изпълнителя.
#Примерно съдържание на файл с име "Bonnie":
#2005г. Bonnie - "God Was in the Water" (Randall Bramblett, Davis Causey) – 5:17
#2005г. Bonnie - "Love on One Condition" (Jon Cleary) – 3:43
#2005г. Bonnie - "So Close" (Tony Arata, George Marinelli, Pete Wasner) – 3:22
#2005г. Bonnie - "Trinkets" (Emory Joseph) – 5:02
#2005г. Bonnie - "Crooked Crown" (David Batteau, Maia Sharp) – 3:49
#2005г. Bonnie - "Unnecessarily Mercenary" (Jon Cleary) – 3:51
#2005г. Bonnie - "I Will Not Be Broken" - "Deep Water" (John Capek, Marc Jordan) – 3:58

#Да се напише shell скрипт приемащ два параметъра, които са имена на файлове от вашата home директория. 
#Скриптът сравнява, кой от двата файла има повече на брой редове, съдържащи неговотоиме (на файла). За файлът победител изпълнете следните действия:
#• извлечете съдържанието му, без годината на издаване на албума и без името на изпълнителя
#• сортирайте лексикографски извлеченото съдържание и го запишете във файл с име ’изпълнител.songs’
#Примерен изходен файл (с име Bonnie.songs):
#"Crooked Crown" (David Batteau, Maia Sharp) – 3:49
#"God Was in the Water" (Randall Bramblett, Davis Causey) – 5:17
#"I Will Not Be Broken" - "Deep Water" (John Capek, Marc Jordan) – 3:58
#"Love on One Condition" (Jon Cleary) – 3:43
#"So Close" (Tony Arata, George Marinelli, Pete Wasner) – 3:22
#"Trinkets" (Emory Joseph) – 5:02
#"Unnecessarily Mercenary" (Jon Cleary) – 3:51

#!/bin/bash

if [[ $# -ne 2 ]]; then
	echo "wrong input"
	exit 1
fi

if [[ ! -f "$1" ]] || [[ ! -f "$2" ]]; then
	echo "not files"
	exit 2
fi

if ! [[ "$1" =~ ^[a-zA-Z]+$ ]] || ! [[ "$2" =~ ^[a-zA-Z]+$ ]]; then
	echo "not strings"
	exit 3
fi

nums_first_file=$(egrep "$(basename "$1")" ${1} | wc -l)
nums_second_file=$(egrep "$(basename "$2")" ${2} | wc -l)

if [[ ${nums_first_file} -gt ${nums_second_file} ]]; then
	cut -d'-' -f2- "$1" | cut -c 2- | sort > $(basename "$1").songs
elif [[ ${nums_first_file} -lt ${nums_second_file} ]]; then
	cut -d'-' -f2- "$2" | cut -c 2- | sort > $(basename "$2").songs
fi