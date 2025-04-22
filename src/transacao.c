#include "transacao.h"
// #define printf(...)


/**
 * Dado um banco e um struct de transação, realiza transação entre duas contas
 * 
 * Retorna NULL se a transação tem sucesso e qualquer outra coisa se não há saldo suficiente na conta do pagante.
 */
void* realiza_transacao(void** args){
    banco* b = args[0];
    transacao* t = args[1];
    conta* conta_from = b->contas[t->id_from], *conta_to = b->contas[t->id_to];
    printf("Transação de %d para %d no valor de %s está pronta para começar.\n", t->id_from, t->id_to, cents_to_reais(t->valor));
    
    // If para fazer o valgrind parar de reclamar da ordem de lock
    if(t->id_from < t->id_to){
        pthread_mutex_lock(&conta_from->mutex);
        pthread_mutex_lock(&conta_to->mutex);
    } else {
        pthread_mutex_lock(&conta_to->mutex);
        pthread_mutex_lock(&conta_from->mutex);
    }
    

    printf("Transação de %d para %d no valor de %s está sendo processada.\n", t->id_from, t->id_to, cents_to_reais(t->valor));
    printf("Saldo de %s (id = %d): %s\n", conta_from->nome, t->id_from, cents_to_reais(conta_from->saldo));
    printf("Saldo de %s (id = %d): %s\n", conta_to->nome, t->id_to, cents_to_reais(conta_to->saldo));

    // Simulando processamento pesado
    sleep(rand_r(&conta_from->seed)%3);

    if (conta_from->saldo < t->valor){
        printf("Erro: Conta %d não tem saldo suficiente.\n", t->id_from);
        assert(!pthread_mutex_unlock(&conta_from->mutex));
        assert(!pthread_mutex_unlock(&conta_to->mutex));  
        return (void*)-1;
    }
    
    conta_from->saldo -= t->valor;
    conta_to->saldo += t->valor;
    printf("Transação de %d para %d no valor de %s feita com sucesso.\n", t->id_from, t->id_to, cents_to_reais(t->valor));

    assert(!pthread_mutex_unlock(&conta_from->mutex));
    assert(!pthread_mutex_unlock(&conta_to->mutex));                                                                  

    return NULL;
}
