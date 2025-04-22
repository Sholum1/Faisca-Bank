#include"pool.h"

work_pool* construct_work_pool(int max_proc){
    work_pool* ret = malloc(sizeof(work_pool));
    ret->q = construct_queue(max_proc);
    pthread_mutex_init(&ret->mutex,NULL);
    return ret;
}

void add_work(work_pool* pool, void* (*func)(void**), void** args, int qtd_args, int id){
    pthread_mutex_lock(&pool->mutex);
    work* new_work = malloc(sizeof(work));
    new_work->func = func;
    new_work->args = args;
    new_work->qtd_args = qtd_args;
    new_work->id = id;
    push_queue(pool->q, (void*)new_work);
    pthread_mutex_unlock(&pool->mutex);
}

void* worker(void** args){
    work_pool* pool = args[0];
    int* current_id = args[1];
    int* current_status = args[2];

    while(1){
        pthread_mutex_lock(&pool->mutex);
        
        if(!size_queue(pool->q)){
            pthread_mutex_unlock(&pool->mutex);
            free(args);
            return 0;
        }

        work* cur_work = (work*)pop_queue(pool->q);
        *current_id = cur_work->id;
        *current_status = 1;
        pthread_mutex_unlock(&pool->mutex);
        
        // Inserindo variável de status da thread para a função modificar
        void** new_args = malloc((cur_work->qtd_args+1)*sizeof(void*));
        for(int i = 0; i < cur_work->qtd_args; i++){
            new_args[i] = cur_work->args[i];
        }
        new_args[cur_work->qtd_args] = current_status;

        cur_work->func(new_args);

        *current_id = -1;
        *current_status = 0;
        free(cur_work->args);
        free(cur_work);
    }
    free(args);
    return 0;
}

void setup_workers(void** args){
    work_pool* pool = (work_pool*)args[0];
    // gambiarra para enviar inteiro sem warning
    int thread_count = (int)(uintptr_t)args[1];
    int* thread_work = (int*)args[2];
    int* thread_status = (int*)args[3];
    int* still_working = (int*)args[4];

    pthread_t* threads = malloc(thread_count*sizeof(pthread_t));
    
    for(int i = 0; i < thread_count; i++){
        void** args = malloc(sizeof(void*)*3);
        args[0] = pool;
        args[1] = thread_work+i;
        args[2] = thread_status+i;
        pthread_create(threads+i,NULL,(void*)worker,args);
    }
    for(int i = 0; i < thread_count; i++){
        pthread_join(threads[i],NULL);
    }
    free(threads);
    *still_working = 0;
    free(args);
}

pthread_t* start_working(work_pool* pool, int thread_count, int thread_work[], int thread_status[], int* still_working){
    pthread_t* manager = malloc(sizeof(pthread_t));

    void** args = malloc(sizeof(void*)*5);
    args[0] = pool;
    // Gambiarra para enviar inteiro sem warning
    args[1] = (void*)(long long)thread_count;
    args[2] = thread_work;
    args[3] = thread_status;
    args[4] = still_working;

    pthread_create(manager,NULL,(void*)setup_workers,args);

    return manager;
}

void destruct_work_pool(work_pool* pool){
    destruct_queue(pool->q);
    free(pool);
}