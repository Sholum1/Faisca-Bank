#include"conta.h"

conta* init_conta(char* nome, int saldo_inicial, int seed){
    conta* ret = malloc(sizeof(conta));

    strcpy(ret->nome, nome);
    ret->saldo = saldo_inicial;
    ret->seed = seed;
    pthread_mutex_init(&ret->mutex,NULL);
    return ret;
}