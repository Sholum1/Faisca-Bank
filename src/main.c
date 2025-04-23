#include "transacao.h"
#include "banco.h"
#include "conta.h"
#include "impressao.h"
#include "pool.h"
#include "lista_nomes.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define QTD_CONTAS 10
#define MAX_LEN 50
#define QTD_TRANSACOES 400
#define MAX_THREADS 5

// Delay para cada print em microsegundos
#define DELAY_PRINT 1000000


// #define printf(...)

int main(){

    srand(time(NULL));
    banco* faisca = construct_banco(QTD_CONTAS, 0);

    int total_dinheiro = 0;

    for(int i = 0; i < QTD_CONTAS; i++){
        char nome[MAX_LEN];
        snprintf(nome,MAX_LEN,"%s %s", lista_nomes[rand()%QTD_NOMES], 
                lista_sobrenomes[rand()%QTD_SOBRENOMES]);

        int saldo_inicial = rand()%200000;
        add_conta(faisca,init_conta(nome,saldo_inicial,rand()));
        total_dinheiro += saldo_inicial;
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
        t[i].valor = rand() % 100000;

        printf("Transação %d:\n", i);
        char buf[20];
        cents_to_reais(t[i].valor, buf);
        printf("De %d para %d com valor de %s\n",
                t[i].id_from, t[i].id_to, buf);
    
    }

    printf("\n");

    // Faz setup dos trabalhos que serão realizados pela pool de threads
    work_pool* trabalhos = construct_work_pool(QTD_TRANSACOES);
    for(int i = 0; i < QTD_TRANSACOES; i++){
        void** args = malloc(sizeof(void*)*2);
        args[0] = faisca;
        args[1] = &t[i];
        add_work(trabalhos,realiza_transacao,args,2,i);
    }

    fprintf(stderr, "Tamanho da fila: %d\n", size_queue(trabalhos->q));

    // Guarda o id da operação sendo processada pela i-ésima thread
    int thread_work[MAX_THREADS];
    /**
     * Guarda status da thread atual:
     * 
     * 0 -> thread livre
     * 1 -> thread esperando para trabalhar
     * 2 -> thread processando
     */
    int thread_status[MAX_THREADS];
    for(int i = 0; i < MAX_THREADS; i++)
        thread_work[i] = -1;
    
    int still_working = 1;

    // Variável com thread que gerencia as outras threads
    pthread_t *manager =
        start_working(trabalhos, MAX_THREADS, thread_work, thread_status,
                      &still_working);

    while(still_working){
        situacoes_conta(faisca);
        usleep(DELAY_PRINT);
    }

    printf("Status Final:\n");
    situacoes_conta(faisca);

    total_dinheiro -= faisca->reserva;
    for(int i = 0; i < QTD_CONTAS; i++)
        total_dinheiro -= faisca->contas[i]->saldo;

    char buf[20];
    cents_to_reais(total_dinheiro, buf);

    // Caso isso dê diferente de 0, significa que algum erro de lógica fez dinheiro sumir
    printf("Diferença de dinheiro no início e fim do processo: %s\n", buf);

    pthread_join(*manager,NULL);
    free(manager);
    destruct_banco(faisca);
    destruct_work_pool(trabalhos);

    return 0;
}
