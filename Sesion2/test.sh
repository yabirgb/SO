#!/bin/bash

gcc -o ejer3 ejercicio3.c
mkdir -p test
cd test
touch test1
touch test2
chmod 777 test1 test2
mkdir innertest
cd innertest
touch test3
echo "I'm test 3" >> test3
chmod 777 test3
mkdir bikinibottom
cd bikinibottom
touch patrik
chmod 777 patrik
echo "I'm patrik" >> patrik
cd ..
cd ..
cd ..
chmod +x ejer3
./ejer3 test/
rm -rf test/

