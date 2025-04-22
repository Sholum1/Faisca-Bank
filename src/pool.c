#include"pool.h"

work_pool* construct_work_pool(int max_proc){
    work_pool* ret = malloc(sizeof(work_pool));
    ret->q = construct_queue(max_proc);
    pthread_mutex_init(&ret->mutex,NULL);
    return ret;
}

void add_work(work_pool* pool, void* (*func)(void**), void** args, int id){
    pthread_mutex_lock(&pool->mutex);
    work* new_work = malloc(sizeof(work));
    new_work->func = func;
    new_work->args = args;
    new_work->id = id;
    push_queue(pool->q, (void*)new_work);
    pthread_mutex_unlock(&pool->mutex);
}

void* worker(void** args){
    work_pool* pool = args[0];
    int* current_work = args[1];
    while(1){
        pthread_mutex_lock(&pool->mutex);
        
        if(!size_queue(pool->q)){
            pthread_mutex_unlock(&pool->mutex);
            free(args);
            return 0;
        }

        work* cur_work = (work*)pop_queue(pool->q);
        *current_work = cur_work->id;
        pthread_mutex_unlock(&pool->mutex);

        cur_work->func(cur_work->args);
    }
    free(args);
    return 0;
}

void start_working(work_pool* pool, int thread_count, int current_work[]){
    pthread_t* threads = malloc(thread_count*sizeof(pthread_t));
    
    for(int i = 0; i < thread_count; i++){
        void** args = malloc(sizeof(void*)*2);
        args[0] = pool;
        args[1] = current_work+i;
        pthread_create(threads+i,NULL,(void*)worker,args);
    }
    for(int i = 0; i < thread_count; i++){
        pthread_join(threads[i],NULL);
    }
}