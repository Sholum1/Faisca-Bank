#pragma once
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include"banco.h"
#include"transacao.h"

/**
 * Dado um valor em centavos, escreve uma string em buffer indicando o valor
 * em reais (por exemplo, se valor = 123, será escrito "R$1,23" em buf).
 */
void cents_to_reais(int valor, char* buf);

/**
 * Imprime uma tabela indicando para conta o nome do titular, saldo e 
 * se ela está sendo acessada nesse momento.
 * 
 * Essa função *causa* uma data race, pois lê de variáveis que estão sendo
 * escritas por outras threads. Entretanto, isso é *esperado*, pois o intuito
 * da função é mostar uma snapshot do sistema, mesmo que ele esteja dessincronizado.
 * 
 * Garantir que essa função é síncrona altera significativamente o funcionamento
 * das threads e, ao meu ver, vai contra o espírito do projeto. Por exemplo, por não
 * ser síncrona, é possível ver que o total de dinheiro no sistema não é constante para
 * todo instante (pois entre os writes e reads pode haver falta de sincronia pontual), 
 * mas que ao final do programa o dinheiro é conservado
 */
void situacao_contas(banco* faisca);

/**
 * Imprime uma tabela indicando para thread o nome dos envolvidos na transação
 * que ela está processando, e o status da operação (se ela está esperando outra
 * thread, processando a transação ou livre).
 * 
 * Essa função *causa* uma data race, pois lê de variáveis que estão sendo
 * escritas por outras threads. Entretanto, isso é *esperado*, pois o intuito
 * da função é mostar uma snapshot do sistema, mesmo que ele esteja dessincronizado.
 */
void situacao_threads(int qtd_threads, int thread_work[], int thread_status[], transacao* transacoes, conta* contas[]);


/**
 * Se value for maior ou igual a 0, imprime na saída de erro um caça níquel
 * mostrando que o usuário perdeu (e portanto pagará taxa). Caso contrário,
 * mostra caça níquel indicando que o usuário ganhou o faísca cashback.
 */
void print_jackpot(int value);
