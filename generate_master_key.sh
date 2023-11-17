#!/bin/bash

file=key.txt

counter=0
mod=65536

echo -n >"./in/$file"

for counter in {0..7}; do
	var=$(($RANDOM % $mod))
	if (($var < 16)); then
		var=$(echo "obase=16; $var" | bc)
		content=$(echo $var | tr '[:upper:]' '[:lower:]')
		echo -n 000$content >>"./in/$file"

	elif (($var > 15 && $var < 256)); then
		var=$(echo "obase=16; $var" | bc)
		content=$(echo $var | tr '[:upper:]' '[:lower:]')
		echo -n 00$content >>"./in/$file"

	elif (($var > 255 && $var < 4096)); then
		var=$(echo "obase=16; $var" | bc)
		content=$(echo $var | tr '[:upper:]' '[:lower:]')
		echo -n 0$content >>"./in/$file"

	elif (($var > 4095)); then
		var=$(echo "obase=16; $var" | bc)
		content=$(echo $var | tr '[:upper:]' '[:lower:]')
		echo -n $content >>"./in/$file"
	fi
done

echo "" >>"./in/$file"
