#include "transacao.h"
#include "pthread.h"
#include "stdio.h"
#include "stdlib.h"    

// make a 
void* realiza_transacao(void** args){
    banco* b = args[0];
    transacao* t = args[1];
    printf("pointers: %p %p\n", b, t);
    printf("Transaçãoo de %d para %d no valor de %d\n", t->id_from, t->id_to, t->valor);
    


    int waiting = 1;
    while(waiting){
        if (b->contas[t->id_from].being_used == 1 || b->contas[t->id_to].being_used == 1){
        } else {
            waiting = 0;
        }
    }

    b->contas[t->id_from].being_used = 1;
    b->contas[t->id_to].being_used = 1;
    
    if (b->contas[t->id_from].valor < t->valor){
        printf("Conta %d não tem saldo suficiente\n", t->id_from);
        b->contas[t->id_from].being_used = 0;
        b->contas[t->id_to].being_used = 0;
        return;
    }
    
    b->contas[t->id_from].valor -= t->valor;
    b->contas[t->id_to].valor += t->valor;
    printf("Transação de %d para %d no valor de %d\n", t->id_from, t->id_to, t->valor);
    b->contas[t->id_from].being_used = 0;
    b->contas[t->id_to].being_used = 0;
    return;
}
