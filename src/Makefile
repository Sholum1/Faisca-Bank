SOURCE = main

#SETUP PARA IMPLICIT RULES
CC = gcc
CFLAGS = -std=c11 -Wall -Wvla -g -lm

objects = transacao.o conta.o impressao.o banco.o
libs = transacao.h conta.h impressao.h banco.h

all: $(SOURCE) $(libs)

$(SOURCE): $(objects) 

clean:
	rm -rf *.o *.gch $(SOURCE)