#!/bin/bash

if [ ! $1 ]; then
	echo Usage: $0 [constant]
	exit
fi

file=clear_cipher_couples_$1.txt
constant=$1$1$1$1$1$1$1$1$1$1$1$1$1$1$1$1$1$1$1$1$1$1$1$1$1$1$1$1

counter=0
echo 000$counter$constant >"./in/$file"
counter=1
for counter in {1..9}; do
	echo 000$counter$constant >>"./in/$file"
	counter=$((counter + 1))
done

counter=10

echo 000a$constant >>"./in/$file"
echo 000b$constant >>"./in/$file"
echo 000c$constant >>"./in/$file"
echo 000d$constant >>"./in/$file"
echo 000e$constant >>"./in/$file"
echo 000f$constant >>"./in/$file"

counter=16
var=0
content=0

for counter in {16..255}; do
	var=$(echo "obase=16; $counter" | bc)
	content=$(echo $var | tr '[:upper:]' '[:lower:]')
	echo 00$content$constant >>"./in/$file"
	counter=$((counter + 1))
done
