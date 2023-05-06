#7800 - Да се напише shell скрипт, който намира броя на изпълнимите файлове в PATH.
#Hint: Предполага се, че няма спейсове в имената на директориите

#Hint2: Ако все пак искаме да се справим с този случай, да се разгледа IFS променливата и констуркцията while read -d

#!/bin/bash

count=0
IFS=$'\n'

for dir in $(echo "$PATH" | tr ':' ' '); do
    while read -d '' file; do
    #for file in "$dir"/*; do
        if [[ -x "$file" && ! -d "$file" ]]; then
            count=$(( count + 1 ))
        fi
    done < <(find "$dir" -maxdepth 1 -type f -print0)
    #done
done

echo "Count:$count"
