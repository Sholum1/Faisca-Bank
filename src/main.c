#include "transacao.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/sysinfo.h>
#include <string.h>

int main(){
    int Thread_avail = get_nprocs();

    banco* b = malloc(sizeof(banco));
    b->num = 10;
    b->contas = malloc(sizeof(conta) * 10);
    for (int i = 0; i < 10; i++){
        b->contas[i].id = i;
        b->contas[i].valor = 0;
    }
    b->contas[0].valor = 100000;
    for (int i = 1; i < 10; i++){
        b->contas[i].valor = rand()%50000 + 1;
        printf("Conta %d = %d\n",b->contas[i].id, b->contas[i].valor);
    }

    int i = 0;
    while(i < 5){
        transacao* t = malloc(sizeof(transacao));
        t->id_from = rand() % 10;
        t->id_to = rand() % 10;
        while (t->id_from == t->id_to){
            t->id_to = rand() % 10;
        }
        t->valor = rand() % 1000;

        void** args = malloc(sizeof(void*) * 2);
        args[0] = b;
        args[1] = t;
        printf("pointerss: %p %p, que ta aq: %p\n", args[0], args[1], args);
        pthread_t thread;
        pthread_create(&thread, NULL, (void*)realiza_transacao, args);
        pthread_join(thread, NULL);
        i++;
    }
    return 0;
}