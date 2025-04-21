#pragma once
#include "conta.h"

typedef struct{
    conta** contas;
    int qtd_contas, cap_contas;
} banco;

/**
 * Inicializa váriavel de banco
 */
banco* init_banco(int capacidade);

/**
 * Insere conta no banco e retorna o id da conta criada
 */
int add_conta(banco* faisca, conta* x);