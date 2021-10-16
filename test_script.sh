#!/usr/bin/env bash
COLOR_OFF=$'\033[0m'
RED=$'\033[0;31m'
GREEN=$'\033[0;32m'
BLUE=$'\033[0;34m'
YELLOW=$'\033[0;33m'

CODE_FILE=$(find . -type f -name "*.c")

echo "$BLUE!Compiling! $CODE_FILE $COLOR_OFF"
g++ "$CODE_FILE" --std=c++14 -Wall -pedantic -Wno-long-long -g -o /tmp/a.out || (echo "$RED""FAILED COMPILING!" && kill -SIGPIPE $$)
echo
echo "$YELLOW""Testing $COLOR_OFF"
PROG="/tmp/a.out"

FAILED=0
SOLVED=0

for IN_FILE in sample/CZE/*_in.txt;
do
  OUT_FILE=`echo -n $IN_FILE | sed -e 's/in/out/g'`
  \time -f "%U user %S system %P CPU max %MKB"  $PROG < "$IN_FILE" > tmp_out.txt
  if ! diff $OUT_FILE tmp_out.txt;
  then
    ((FAILED++))
    echo "$RED""Habibi, habibi $IN_FILE $COLOR_OFF";
  else
    ((SOLVED++))
    echo "$GREEN""Swag $IN_FILE"$'\e[0m';
  fi
  echo
done

COLOR=""
if (( FAILED != 0 )); then
  COLOR=$RED
fi
echo "$COLOR""Failed($FAILED),"$'\e[1;32m'" Solved($SOLVED) $COLOR_OFF";
rm tmp_out.txt
