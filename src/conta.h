#pragma once
#include"pthread.h"
#include<stdlib.h>
#include<string.h>

typedef struct {
    // Nome do usuário
    char nome[50];
    // Guarda saldo de cada conta em centavos (use impressao.c para transformar em reais)
    int saldo;
    // Seed para o cálculo do faísca cashback (e delay aleatório)
    unsigned int seed;
    pthread_mutex_t mutex;   
} conta;

conta* init_conta(char* nome, int saldo_inicial, int seed);
