#Зад. 38 2020-SE-03 Под пакет ще разбираме директория, която има следната структура:
#<name>
#|-- version
#|-- tree
#|...
#Където <name> е името на пакета, version е текстов файл, който съдържа низ от вида 1.2.3-4 и нищо друго, а tree е директория с произволно съдържание.
#За да получим архив на пакет, архивираме (tar) и компресираме (xz) съдържанието на директорията tree.
#Под хранилище ще разбираме директория, която има следната структура:
#<repo name>
#|-- db
#|-- packages
#|...
#Където <repo name> е името на хранилището, db е текстов файл, чиито редове имат вида <package name>-<package version> <package checksum> и са сортирани лексикографски. 
#Директорията packages съдържа архиви с имена <package checksum>.tar.xz, които съответстват на редове в db. Под <package checksum> имаме предвид sha256 сумата на архива на пакета.
#Напишете скрипт repo_add.sh, който приема два аргумента - път до хранилище и път до пакет, който добавя пакета в хранилището. Ако същата версия на пакет вече съществува, 
#архивът сезаменя с новата версия. В противен случай, новата версия се добавя заедно с другите.
#Заб: Първо си проектирайте общия алгоритъм на работа.
#Примерно хранилище:
#myrepo
#|-- db
#|-- packages
#|-- 6e3549438bc246b86961b2e8c3469321ca22eabd0a6c487d086de7a43a0ef766.tar.xz
#|-- 66b28e48161ba01ae25433b9ac4086a83b14d2ee49a62f2659c96514680ab6e8.tar.xz
#|-- 99c934ad80bd9e49125523c414161e82716b292d4ed2f16bb977d6db7e13d9bc.tar.xz
#Със съдържание на db:
#glibc-2.31-2 6e3549438bc246b86961b2e8c3469321ca22eabd0a6c487d086de7a43a0ef766
#zlib-1.1.15-8 66b28e48161ba01ae25433b9ac4086a83b14d2ee49a62f2659c96514680ab6e8
#zlib-1.2.11-4 99c934ad80bd9e49125523c414161e82716b292d4ed2f16bb977d6db7e13d9bc
#Примерен пакет:
#zlib
#|-- version # contains ’1.2.11-3’
#|-- tree
#|...
#Съдържание на хранилището след изпълнение на ./repo-add.sh myrepo zlib
#myrepo
#|-- db
#|-- packages
#|-- 6e3549438bc246b86961b2e8c3469321ca22eabd0a6c487d086de7a43a0ef766.tar.xz
#|-- 66b28e48161ba01ae25433b9ac4086a83b14d2ee49a62f2659c96514680ab6e8.tar.xz
#|-- b839547ee0aed82c74a37d4129382f1bd6fde85f97c07c5b705eeb6c6d69f162.tar.xz
#|-- 99c934ad80bd9e49125523c414161e82716b292d4ed2f16bb977d6db7e13d9bc.tar.xz
#Със съдържание на db:
#glibc-2.31-2 6e3549438bc246b86961b2e8c3469321ca22eabd0a6c487d086de7a43a0ef766
#zlib-1.1.15-8 66b28e48161ba01ae25433b9ac4086a83b14d2ee49a62f2659c96514680ab6e8
#zlib-1.2.11-3 b839547ee0aed82c74a37d4129382f1bd6fde85f97c07c5b705eeb6c6d69f162
#zlib-1.2.11-4 99c934ad80bd9e49125523c414161e82716b292d4ed2f16bb977d6db7e13d9bc

#!/bin/bash

if [[ $# -ne 2 ]]; then
	echo "wrong input"
	exit 1
fi

repository=$1
package=$2

package_name=$(basename "${package}")
version=$(cat "${package}/version")
archive=$(tar cJf - -C "${package}/tree" .)
sum=$(echo "${archive}" | sha256sum | awk '{print $1}')

archive_filename="${sum}.tar.xz"
echo "${archive}" | xz > "${archive_filename}"

existing_entry=$(grep "^${package_name}-${version} " "${repository}/db")

if [[ -n "${existing_entry}" ]]; then 
    sed -i "/^${package_name}-${version} /d" "${repository}/db"
fi

echo "${package_name}-${version} ${sum}" >> "${repository}/db" 
sort -o "${repository}/db" "${repository}/db"

mv "${archive_filename}" "${repository}/packages"