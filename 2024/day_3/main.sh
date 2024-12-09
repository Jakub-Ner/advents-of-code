#!/bin/bash

FILE="input.txt"

echo "Part 1"
cat $FILE | \
  egrep -o 'mul\([0-9]+,[0-9]+\)' | \
  awk -F'[(,)]' '{sum += $2 * $3} END {print "Sum = "sum}'

echo -e "\nPart 2"
elements=$(cat $FILE | tr -d '\n' | \
  egrep -o "mul\([0-9]+,[0-9]+\)|do\(\)|don't\(\)" | \
  awk -F'[(,)]'  '{print $1, $2*$3}')

skip=false
sum=0
for element in $elements; do
  if [ "$element" = "mul" ] || [ "$skip" = true ]; then
    continue
  fi

  if [ "$element" = "do" ]; then
    skip=false
    continue
  fi

  if [ "$element" = "don't" ]; then
    skip=true
    continue
  fi

  sum=$((sum + element))
done

echo "Sum = $sum"

