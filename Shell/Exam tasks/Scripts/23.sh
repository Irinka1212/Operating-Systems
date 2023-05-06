#Зад. 23 2017-IN-03 Напишете скрипт, който извежда името на потребителския акаунт, в чиято home директория има най-скоро променен обикновен файл и кой е този файл. 
#Напишете скрипта с подходящите проверки, така че да бъде валиден инструмент.

#!/bin/bash

homedirs=(/home/*)

most_recent_file_name=""
most_recent_time=0
for dir in "${homedirs[@]}"; do 
	time=$(find "$dir" -type f -printf '%T@ %u %f\n' | sort -n | tail -n 1 | cut -d' ' -f1)
	file_name=$(find "$dir" -type f -printf '%T@ %u %f\n' | sort -n | tail -n 1 | cut -d'' -f2-)
	if (( $(echo "$time > $most_recent_time" | bc -l) )); then #float
		most_recent_file_name="$file_name"
        	most_recent_time="$time"
	fi
done

echo "$file_name"
