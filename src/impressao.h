#pragma once
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include"banco.h"
#include"transacao.h"

void cents_to_reais(int valor, char* buf);

void situacoes_conta(banco* faisca);

void print_jackpot(int value);
