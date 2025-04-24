/**
 * Implementa uma pool de trabalhos para a pool de threads executar
 */

#pragma once
#include "fila.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct{
    void *(*func)(void**);
    void ** args;
    int qtd_args;
    int id;
} work;

typedef struct {
    queue* q;
    pthread_mutex_t mutex;
} work_pool;


/**
 * Cria pool de trabalho com capacidade de max_proc processos.
 */
work_pool* construct_work_pool(int max_proc);

/**
 * Adiciona trabalho na pool. Thread-safe. Potencialmente mais trabalhos
 * podem ser adicionados enquanto as threads estão executando, mas isso não
 * é usado no projeto.
 */
void add_work(work_pool *pool, void *(*func)(void **), void **args,
              int qtd_args, int id);

/**
 * Faz setup de pool de threads para executar as tarefas. Essa função cria uma thread
 * para gerenciar thread_count outras threads para executar os trabalhos de pool, mantendo
 * atualizado os vetores de thread_work e thread_status como definidos em main.c. Além disso
 * deixa atualizado a flag still_working que é settada para 1 enquanto ainda trabaho para ser
 * executado pela pool de threads.
 * 
 * @return Retona ponteiro para uma pthread que gerencia a pool de threads.
 */
pthread_t *start_working(work_pool *pool, int thread_count, int thread_work[],
                         int thread_status[], int* still_working);

/**
 * Libera espaço ocupado pela pool de trabalhos
 */
void destruct_work_pool(work_pool* pool);
