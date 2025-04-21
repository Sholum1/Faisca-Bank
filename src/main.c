#include "transacao.h"
#include "banco.h"
#include "conta.h"
#include "impressao.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/sysinfo.h>
#include <string.h>

#define QTD_CONTAS 10
#define MAX_LEN 50


int main(){
    banco* faisca = init_banco(QTD_CONTAS);

    for(int i = 0; i < QTD_CONTAS; i++){
        char nome[MAX_LEN];
        snprintf(nome,MAX_LEN,"Conta %d", i);

        add_conta(faisca,init_conta(nome,rand()%2000,rand()));
    }

    int i = 0;
    while(i < 5){
        printf("Estado para i = %d:\n", i);
        situacoes_conta(faisca);

        transacao* t = malloc(sizeof(transacao));
        t->id_from = rand() % 10;
        t->id_to = rand() % 10;
        while (t->id_from == t->id_to){
            t->id_to = rand() % 10;
        }
        t->valor = rand() % 1000;

        void** args = malloc(sizeof(void*) * 2);
        args[0] = faisca;
        args[1] = t;
        pthread_t thread;
        pthread_create(&thread, NULL, (void*)realiza_transacao, args);
        int ok;
        pthread_join(thread, (void*)&ok);
        i++;

        printf("Status de execução de transaçãoo: %d\n\n", ok);
        
    }
    return 0;
}