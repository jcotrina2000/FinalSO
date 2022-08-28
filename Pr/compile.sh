#! /bin/bash
#
cp ziggurat.h /$HOME/include
#
gcc -c -Wall ziggurat.c
if [ $? -ne 0 ]; then
  echo "Compile error."
  exit
fi
gcc -o tiktoker tiktoker.c ziggurat.o -lm

