#!/bin/sh

#Script para executar o Faísca Bank
#Uso: ./run.sh ou bash run.sh

(cd src && make clean && make)
./src/main 2> log
