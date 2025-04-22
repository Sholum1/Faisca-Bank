#!/bin/bash

#Script para executar o Faísca Bank
#Uso: ./run.sh ou bash run.sh

(cd src && make src)
./src/main 2> log