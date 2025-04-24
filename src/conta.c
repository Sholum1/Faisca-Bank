#include"conta.h"

/**
 * Cria uma struct de conta e retorna o ponteiro para ela.
 * Note que não é necessario uma função para liberar essa struct, dado
 * que basta dar um free.
 */
conta* init_conta(char* nome, int saldo_inicial, int seed){
    conta* ret = malloc(sizeof(conta));

    strcpy(ret->nome, nome);
    ret->saldo = saldo_inicial;
    ret->seed = seed;
    pthread_mutex_init(&ret->mutex,NULL);
    return ret;
}