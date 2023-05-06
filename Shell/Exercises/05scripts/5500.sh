#5500 - Да се напише shell script, който генерира HTML таблица съдържаща описание на потребителите във виртуалката ви. Таблицата трябва да има:
#- заглавен ред с имената нa колоните
#- колони за username, group, login shell, GECOS field (https://en.wikipedia.org/wiki/Gecos_field)

#Пример:
#$ ./passwd-to-html.sh > table.html
#$ cat table.html
#<table>
#  <tr>
#    <th>Username</th>
#    <th>group</th>
#    <th>login shell</th>
#    <th>GECOS</th>
#  </tr>
#  <tr>
#    <td>root</td>
#    <td>root</td>
#    <td>/bin/bash</td>
#   <td>GECOS here</td>
#  </tr>
#  <tr>
#    <td>ubuntu</td>
#    <td>ubuntu</td>
#    <td>/bin/dash</td>
#    <td>GECOS 2</td>
#  </tr>
#</table>

#!/bin/bash

filename='/etc/passwd'

echo "<table>"
echo "	<tr>"
echo "	<th>Username</th>"
echo "	<th>Group</th>"
echo "	<th>Login Shell</th>"
echo "	<th>GECOS Field</th>"
echo "	</tr>"
while IFS=':' read username group gecos shell
#while IFS=':' read -r username _ group _ gecos shell; do
do
    echo "	<tr>"
    echo "	<th>$username</th>"
    echo "	<th>$group</th>"
    echo "	<th>$shell</th>"
    echo "	<th>$gecos</th>"
    echo "	</tr>"
done < <(cut -d: -f1,3,5,7 /etc/passwd)
#done < "$filename"
echo "</table>"