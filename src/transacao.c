#include "transacao.h"
#include "pthread.h"
#include "banco.h"
#include "impressao.h"
#include <stdio.h>
#include <stdlib.h> 

/**
 * Dado um banco e um struct de transação, realiza transação entre duas contas
 * 
 * Retorna NULL se a transação tem sucesso e qualquer outra coisa se não há saldo suficiente na conta do pagante.
 */
void* realiza_transacao(void** args){
    banco* b = args[0];
    transacao* t = args[1];
    printf("pointers: %p %p\n", b, t);
    printf("Transação de %d para %d no valor de %d\n", t->id_from, t->id_to, t->valor);

    conta* conta_from = b->contas[t->id_from], *conta_to = b->contas[t->id_to];

    pthread_mutex_lock(&conta_from->mutex);
    pthread_mutex_lock(&conta_to->mutex);

    
    if (conta_from->saldo < t->valor){
        printf("Conta %d não tem saldo suficiente\n", t->id_from);
        return (void*)1;
    }
    
    conta_from->saldo -= t->valor;
    conta_to->saldo += t->valor;
    printf("Transação de %d para %d no valor de %d feita com sucesso\n", t->id_from, t->id_to, t->valor);

    pthread_mutex_unlock(&conta_from->mutex);
    pthread_mutex_unlock(&conta_to->mutex);

    return NULL;
}
