#include"fila.h"

// Avança o valor de *x em 1 e tira módulo mod desse valor
// Importante para conseguir andar na queue visto que é um vetor circular.
void mod_advance(int* x, int mod){
    *x = (*x+1)%mod;
}

queue* construct_queue(int max_size){
    queue* ret = malloc(sizeof(queue));
    if(!ret)
        exit(1);
    ret->data = malloc(sizeof(void*)*(max_size + 1)); 
    // preciso dar um espaço a mais para diferenciar o caso de fila de tamanho 0 e filha de tamanho max_size
    if(!ret->data)
        exit(1);
    
    ret->max_size = max_size;
    ret->id_front = 0;
    ret->id_back = 0;

    return ret;
}

void* front_queue(queue* q){
    return q->data[q->id_front];
}

void* pop_queue(queue* q){
    int old_id = q->id_front;
    mod_advance(&q->id_front, q->max_size + 1);
    return q->data[old_id];
}

void push_queue(queue* q, void* x){
    q->data[q->id_back] = x;
    mod_advance(&q->id_back, q->max_size + 1);
}

int size_queue(queue* q){
    int ret = q->id_back-q->id_front;
    if(ret < 0)
        ret+=q->max_size+1;
    
    return ret;
}

void destruct_queue(queue* q){
    free(q->data);
    free(q);
}