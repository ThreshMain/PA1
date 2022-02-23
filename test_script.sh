#!/usr/bin/env bash
COLOR_OFF=$'\033[0m'
RED=$'\033[0;31m'
GREEN=$'\033[0;32m'
BLUE=$'\033[0;34m'
YELLOW=$'\033[0;33m'

CODE_FILE=$(find . -type f -name "*.[ch]" | tr "\n" " ")
PROG="/tmp/a.out"

echo "$BLUE!Compiling! $CODE_FILE $COLOR_OFF"

CMAKE_FLAGS="-g -std=c++14 -Wall -pedantic -Wno-long-long -O2 -Werror"
command time -f "%U user %S system %P CPU max %MKB" g++ "$CODE_FILE" $CMAKE_FLAGS -o "$PROG" || (echo "$RED""FAILED COMPILING!" && kill -SIGPIPE $$)
echo
echo "$YELLOW""Testing $COLOR_OFF"

FAILED=0
SOLVED=0

start_time=$(date +%s.%6N)
for IN_FILE in {basic,sample}/CZE/*_in.txt;
do
  OUT_FILE=$(echo -n "$IN_FILE" | sed -e 's/in/out/g')
  command time -f "%U user %S system %P CPU max %MKB"  $PROG < "$IN_FILE" > tmp_out.txt
  if ! diff "$OUT_FILE" tmp_out.txt;
  then
    ((FAILED++))
    echo "$RED""Habibi, habibi $IN_FILE $COLOR_OFF";
  else
    ((SOLVED++))
    echo "$GREEN""Swag $IN_FILE"$'\e[0m';
  fi
  echo
done
end_time=$(date +%s.%6N)

COLOR=""
if (( FAILED != 0 )); then
  COLOR=$RED
fi
echo "$COLOR""Failed($FAILED),"$'\e[1;32m'" Solved($SOLVED) $COLOR_OFF";

# elapsed time with second resolution
elapsed=$(echo "scale=6; $end_time - $start_time" | bc)
echo "Total time: $elapsed"

rm tmp_out.txt
