#pragma once
#include<stdlib.h>
// Implementação de uma fila através de um vetor circular.

typedef struct{
    void** data;
    int max_size;
    int id_front, id_back;
    // id_front -> id da frente da queue
    // id_back -> id 1 após a parte de trás da queue
} queue;

// Constrói uma queue que o tamanho máximo é max_size e retorna um
// ponteiro pra ela.
// O vetor na verdade tem tamanho max_size + 1 para poder diferenciar o caso de 
// size = 0 vs size = max_size.
queue* construct_queue(int max_size);

// Retorna elemento na frente da fila. Undefined behaviour para
// filas de tamanho 0.
void* front_queue(queue* q);

// Retira elemento na frente da fila e retorna seu valor. Undefined behaviour
// para filas de tamanho 0.
void* pop_queue(queue* q);

// Coloca o valor x no final da fila.
void push_queue(queue* q, void* x);

// Retorna o tamanho da fila em função dos ids do começo e final da fila.
int size_queue(queue* q);

// Destrói a fila q
void destruct_queue(queue* q);
