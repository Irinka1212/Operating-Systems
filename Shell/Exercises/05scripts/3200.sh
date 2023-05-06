#3200 - Да се напише shell скрипт, който приканва потребителя да въведе пълното име на директория 
#и извежда на стандартния изход подходящо съобщение за броя на всички файлове и всички директории в нея.

#!/bin/bash

read -p "The name of the whole directory?" dir
files=$(find ${dir} -maxdepth 1 -type f | wc -l)
directories=$(find ${dir} -maxdepth 1 -type d | wc -l)
echo "Files:$files and directories:$directories"
