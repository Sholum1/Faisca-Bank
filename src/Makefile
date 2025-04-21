all: main.o transacao.o
	gcc -o main main.o transacao.o	

main.o: main.c
	gcc -c main.c transacao.h

transação.o: transacao.c
	gcc -c transacao.c transacao.h

clean:
	rm -f *.o *.gch

