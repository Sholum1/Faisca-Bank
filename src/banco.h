/**
 * Implementa struct que representa o banco (Faísca Bank)
 */

#pragma once
#include "conta.h"
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>

#define DELAY_BANK 100000

typedef struct{
    conta** contas;
    int qtd_contas, cap_contas;
    int reserva;
    pthread_mutex_t mutex;
} banco;

/**
 * Inicializa váriavel de banco com uma certa capacidade de contas e
 * uma reserva inicial.
 */
banco* construct_banco(int capacidade, int reserva);

/**
 * Insere conta no banco e retorna o id da conta criada. Thread-safe.
 */
int add_conta(banco* faisca, conta* x);

/**
 * Aumenta reserva do banco em um valor (positivo ou negativo)
 * @param args[0]
 * Ponteiro para o bando
 * @param args[1]
 * Ponteiro para o valor em dinheiro aumentado
 * @param args[2]
 * Ponteiro para uma variável de status que é settada para 2 quando
 * é feito processamento. Caso seja NULL, essa funcionalidade é ignorada.
 */
void increase_reserva(void** args);



/**
 * Libera memória do struct banco.
 */
void destruct_banco(banco* faisca);