#include"banco.h"

/**
 * Inicializa váriavel de banco
 */
banco* construct_banco(int capacidade){
    banco* ret = malloc(sizeof(banco));

    ret->qtd_contas = 0;
    ret->cap_contas = capacidade;
    ret->contas = malloc(capacidade*sizeof(conta*));
    pthread_mutex_init(&ret->mutex, NULL);

    return ret;
}

/**
 * Insere conta no banco e retorna o id da conta criada
 */
int add_conta(banco* faisca, conta* x){
    pthread_mutex_lock(&faisca->mutex);
    assert(faisca->qtd_contas < faisca->cap_contas);
    faisca->contas[faisca->qtd_contas] = x;
    faisca->qtd_contas++;
    int ret = faisca->qtd_contas-1;
    pthread_mutex_unlock(&faisca->mutex);
    return ret;
}

void increase_reserva(void** args){
    banco* faisca = args[0];
    int x = *(int *)args[1];
    int* status = args[2];
    pthread_mutex_lock(&faisca->mutex);
    *status = 2;
    
    // Simulando processamento pesado (por exemplo query em API)
    usleep(DELAY_BANK);
    
    faisca->reserva+=x;
    pthread_mutex_unlock(&faisca->mutex);
    free(args);
}

/**
 * Libera memória de banco.
 */
void destruct_banco(banco* faisca){
    for(int i = 0; i < faisca->qtd_contas; i++)
        free(faisca->contas[i]);
    free(faisca->contas);
    free(faisca);
}