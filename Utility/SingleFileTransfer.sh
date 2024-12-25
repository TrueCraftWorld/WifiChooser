#!/bin/bash

# Опции: -f (файл), -d (директория), -h (помощь)
while getopts ":i:f:h" opt; do
  case $opt in
    i)
      targetIp="$OPTARG"
      ;;
    f)
      file="$OPTARG"
      ;;
    h)
      echo "Использование: $0 -i <IP адрес одноплатника> -f <имя передаваемого файла> -h <Вывод этого сообщения>"
      exit 0
      ;;
    \?)
      echo "Неизвестная опция: -$OPTARG" >&2
      exit 1
      ;;
    :)
      echo "Опция -$OPTARG требует аргумента." >&2
      exit 1
      ;;
  esac
done

directory="$PWD"

rsync -av "$file" kikorik@"$targetIp":/usr/share/qtpr/


echo "Файл: $file"
echo "Директория: $directory"

