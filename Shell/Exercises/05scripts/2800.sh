#2800 - Да се напише shell скрипт, който приема точно един параметър и проверява дали подаденият му параметър се състои само от букви и цифри.

#!/bin/bash

if [[ $# -ne 1 ]]; then
	echo "wrong input"
	exit 1
fi

if [[ "$1" =~ ^[a-zA-Z0-9]+$ ]]; then
    echo "yes"
else
    echo "no"
fi
