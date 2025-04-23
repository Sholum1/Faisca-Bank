#pragma once
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include"banco.h"
#include"transacao.h"

void cents_to_reais(int valor, char* buf);

void situacao_contas(banco* faisca);

// Esse void* é uma gambiarra pq por algum motivo ele n reconhece o tipo transacao na hora de compilar
void situacao_threads(int qtd_threads, int thread_work[], int thread_status[], void* transacoes, conta* contas[]);

void print_jackpot(int value);
