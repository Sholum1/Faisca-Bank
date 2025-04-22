#pragma once
#include "fila.h"
#include <pthread.h>
#include <stdlib.h>

typedef struct{
    void *(*func)(void**);
    void ** args;
    int id;
} work;

typedef struct {
    queue* q;
    pthread_mutex_t mutex;
} work_pool;


work_pool* construct_work_pool(int max_proc);

void add_work(work_pool* pool, void* (*func)(void**), void** args, int id);

void start_working(work_pool* pool, int thread_count, int current_work[]);

void destruct_work_pool(work_pool* pool);