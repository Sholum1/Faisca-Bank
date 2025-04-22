#pragma once

#include<unistd.h>
#include "pthread.h"
#include "banco.h"
#include "impressao.h"
#include <stdio.h>
#include <stdlib.h>

// Comente se não quiser delay artificial
#define DELAY_TRANSACAO 500000


typedef struct {
    int id_from;
    int id_to;
    int valor; // Lembra de dividir por 100;   
} transacao;

/**
 * Dado um banco e um struct de transação, realiza transação entre duas contas
 * 
 * Retorna NULL se a transação tem sucesso e qualquer outra coisa se não há saldo suficiente na conta do pagante.
 */
void* realiza_transacao(void** args);


