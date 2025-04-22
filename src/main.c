#include "transacao.h"
#include "banco.h"
#include "conta.h"
#include "impressao.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define QTD_CONTAS 5
#define MAX_LEN 50
#define QTD_TRANSACOES 10

// #define printf(...)

int main(){

    banco* faisca = init_banco(QTD_CONTAS);

    for(int i = 0; i < QTD_CONTAS; i++){
        char nome[MAX_LEN];
        snprintf(nome,MAX_LEN,"Conta %d", i);

        add_conta(faisca,init_conta(nome,rand()%2000,rand()));
    }

    printf("Status Inicial:\n");
    situacoes_conta(faisca);

    transacao t[QTD_TRANSACOES];
    for(int i = 0; i < QTD_TRANSACOES; i++){
        t[i].id_from = rand() % QTD_CONTAS;
        t[i].id_to = rand() % QTD_CONTAS;
        while (t[i].id_from == t[i].id_to){
            t[i].id_to = rand() % QTD_CONTAS;
        }
        t[i].valor = rand() % 1000;

        printf("Transação %d:\n", i);
        printf("De %d para %d com valor de %s\n", t[i].id_from, t[i].id_to, cents_to_reais(t[i].valor));
    }

    printf("\n");

    pthread_t threads[QTD_TRANSACOES];

    for(int i = 0; i < QTD_TRANSACOES; i++){
        void** args = malloc(sizeof(void*)*2);
        args[0] = faisca;
        args[1] = &t[i];
        pthread_create(&threads[i], NULL, (void*)realiza_transacao, args);
    }

    for(int i = 0; i < QTD_TRANSACOES; i++){
        int ok;

        printf("Vou forçar a thread %d a terminar\n", i);
        situacoes_conta(faisca);
        
        pthread_join(threads[i], (void*)&ok);

        printf("Forcei a transação %d\n", i);
        printf("Status de execução de transação: %d\n", ok);
    }

    printf("Status Final:\n");
    situacoes_conta(faisca);


    destruct_banco(faisca);

    return 0;
}