#pragma once
#include"pthread.h"
#include<stdlib.h>
#include<string.h>

#define MAX_LEN 25

typedef struct {
    // Nome do usuário
    char nome[MAX_LEN];
    // Guarda saldo de cada conta em centavos (use impressao.c para
    // transformar em reais)
    int saldo;
    // Seed para o cálculo do faísca cashback (e delay aleatório)
    unsigned int seed;
    pthread_mutex_t mutex;   
} conta;

/**
 * Cria uma struct de conta e retorna o ponteiro para ela.
 * Note que não é necessario uma função para liberar essa struct, dado
 * que basta dar um free.
 */
conta* init_conta(char* nome, int saldo_inicial, int seed);
