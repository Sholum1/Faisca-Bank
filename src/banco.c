#include"banco.h"
#include<stdlib.h>

/**
 * Inicializa váriavel de banco
 */
banco* init_banco(int capacidade){
    banco* ret = malloc(sizeof(banco));

    ret->qtd_contas = 0;
    ret->cap_contas = capacidade;
    ret->contas = malloc(capacidade*sizeof(conta*));

    return ret;
}

/**
 * Insere conta no banco e retorna o id da conta criada
 */
int add_conta(banco* faisca, conta* x){
    faisca->contas[faisca->qtd_contas] = x;
    faisca->qtd_contas++;
    return faisca->qtd_contas-1;
}