#Зад. 17 2016-SE-03 Напишете shell скрипт който, ако се изпълнява от root, проверява кои потребители на системата нямат homedir или не могат да пишат в него.
#Примерен формат:
#root:x:0:0:root:/root:/bin/bash
#daemon:x:1:1:daemon:/usr/sbin:/usr/sbin/nologin
#bin:x:2:2:bin:/bin:/usr/sbin/nologin

#!/bin/bash

# if [[ $(whoami) != "root" ]] ...

while IFS=':' read user homedir; do
    if [[ -d "$homedir" ]]; then
        perm=$(stat -c '%a' "$homedir" | sed 's/[0-9]/& /g')
        echo "Permissions of $homedir: $perm"
    else
        echo "$homedir doesnt't exist"
    fi

    if ! [[ "$perm" =~ "[2367]" ]]; then
	# [[ ! -w "$homedir" ]]
        echo "User $user does not have write permission to $homedir"
    fi
done < <(cut -d':' -f1,6 /etc/passwd)
