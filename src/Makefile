SOURCE = main

#SETUP PARA IMPLICIT RULES
CC = gcc
CFLAGS = -std=gnu11 -Wall -Wvla -Werror -g -lm

objects = transacao.o conta.o impressao.o banco.o pool.o fila.o
libs = transacao.h conta.h impressao.h banco.h pool.h fila.h

all: $(SOURCE) $(libs)

$(SOURCE): $(objects) 

clean:
	rm -rf *.o $(SOURCE)