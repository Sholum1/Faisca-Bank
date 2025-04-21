#include "transação.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_THREADS sysconf(_SC_NPROCESSORS_ONLN)

int main(){

    banco* b = malloc(sizeof(banco));
    b->num = 0;
    b->contas = malloc(sizeof(conta) * 10);
    for (int i = 0; i < 10; i++){
        b->contas[i].id = i;
        b->contas[i].valor = 0;
        sprintf(b->contas[i].nome, "Conta %d", i);
    }
    b->contas[0].valor = 10000;

    



    return 0;
}