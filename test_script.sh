#!/usr/bin/env bash
PROG="./a.out"

FAILED=0
SOLVED=0

for IN_FILE in sample/CZE/*_in.txt;
do
  OUT_FILE=`echo -n $IN_FILE | sed -e 's/in/out/g'`
  \time -f "%U user %S system %P CPU max %MKB"  $PROG < "$IN_FILE" > tmp_out.txt
  if ! diff $OUT_FILE tmp_out.txt;
  then
    ((FAILED++))
    echo $'\e[1;31m'"Habibi, habibi $IN_FILE"$'\e[0m';
  else
    ((SOLVED++))
    echo $'\e[1;32m'"Swag $IN_FILE"$'\e[0m';
  fi
  echo
done

COLOR=""
if (( FAILED != 0 )); then
  COLOR=$'\e[1;31m'
fi
echo "$COLOR""Failed($FAILED),"$'\e[1;32m'" Solved($SOLVED)"$'\e[0m';
rm tmp_out.txt
