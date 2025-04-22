#pragma once
#include "conta.h"
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
// Comente se não quiser delay artificial

#define DELAY_BANK 300000

typedef struct{
    conta** contas;
    int qtd_contas, cap_contas;
    int reserva;
    pthread_mutex_t mutex;
} banco;

/**
 * Inicializa váriavel de banco
 */
banco* construct_banco(int capacidade);

/**
 * Insere conta no banco e retorna o id da conta criada
 */
int add_conta(banco* faisca, conta* x);

/**
 * Libera memória de banco.
 */
void destruct_banco(banco* faisca);