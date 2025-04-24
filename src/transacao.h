/**
 * Implementa o necessário para fazer uma transação.
 */

#pragma once

#include<unistd.h>
#include"pthread.h"
#include"banco.h"
#include <stdio.h>
#include <stdlib.h>

// Comente se não quiser delay artificial
#define DELAY_TRANSACAO 500000
// Taxa aplicada em cima das transações
#define TAXA 0.05

// Chance de receber jackpot (1 em JACKPOT_CHANCE)
#define JACKPOT_CHANCE 75
// Multiplicador da taxa que a pessoa recebe.
// TEM QUE SER MENOR QUE A CHANCE DE JACKPOT SENÃO FALIMOS!!!
#define JACKPOT_MULT 50

typedef struct {
    int id_from;
    int id_to;
    int valor; // Valor guardado em centavos
} transacao;

/**
 * Realiza transação entre duas contas de forma thread-safe.
 * @param args[0]
 * Ponteiro para o banco
 * @param args[1]
 * Ponteiro para o struct de transação
 * @param args[2]
 * Se for não-nulo, ponteiro para inteiro que será settado para 2 quando
 * a função terminar de esperar o mutex
 *
 * @return
 * Retorna NULL se a transação tem sucesso e qualquer outra coisa se não
 * há saldo suficiente na conta do pagante.
 */
void* realiza_transacao(void** args);
