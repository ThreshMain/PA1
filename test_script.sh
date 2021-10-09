#!/usr/bin/env bash
PROG="./a.out"

for IN_FILE in sample/CZE/*_in.txt;
do
  OUT_FILE=`echo -n $IN_FILE | sed -e 's/in/out/g'`
  \time -f "%U user %S system %P CPU max %MKB"  $PROG < "$IN_FILE" > tmp_out.txt
  if ! diff $OUT_FILE tmp_out.txt;
  then
    echo $'\e[1;31m'"Habibi, habibi $IN_FILE"$'\e[0m';
  else
    echo $'\e[1;32m'"Swag $IN_FILE"$'\e[0m';
  fi
  echo
done

rm tmp_out.txt
