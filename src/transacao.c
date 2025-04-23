#include "transacao.h"
#define print_log(...) fprintf(stderr, __VA_ARGS__)

int calc_taxa(int valor){
    int taxa = valor*TAXA;
    if(taxa == 0)
        taxa = 10; // taxa mínima de 10 centavos
    return taxa;
}

// Calcula quanto deve ser adicionado da reserva do banco
// dado taxa e jackpot
int calc_delta(int valor, unsigned int* seed){
    int taxado = calc_taxa(valor);
    if(rand_r(seed) == 0){
        return -taxado*JACKPOT_MULT;
    }
    return taxado;
}

int valor_necessario(int valor){
    return valor + calc_taxa(valor);
}

/**
 * Realiza transação entre duas contas de forma thread-safe.
 * @param args[0]
 * Ponteiro para o banco
 * @param args[1]
 * Ponteiro para o struct de transação
 * @param args[2]
 * Se for não-nulo, ponteiro para inteiro que será settado para 2 quando
 * a função terminar de esperar o mutex
 *
 * @return
 * Retorna NULL se a transação tem sucesso e qualquer outra coisa se não
 * há saldo suficiente na conta do pagante.
 */
void* realiza_transacao(void** args){
    banco* b = args[0];
    transacao* t = args[1];
    int* status = args[2];
    
    conta* conta_from = b->contas[t->id_from], *conta_to = b->contas[t->id_to];

    char buf_saldo[20];
    cents_to_reais(t->valor, buf_saldo);
    print_log(
        "Transação de %d para %d no valor de %s está pronta para começar.\n",
        t->id_from, t->id_to, buf_saldo);
    
    // If para fazer o valgrind parar de reclamar da ordem de lock
    if(t->id_from < t->id_to){
        pthread_mutex_lock(&conta_from->mutex);
        pthread_mutex_lock(&conta_to->mutex);
    } else {
        pthread_mutex_lock(&conta_to->mutex);
        pthread_mutex_lock(&conta_from->mutex);
    }

    if(status)
        *status = 2;
    
    // Simulando processamento pesado (por exemplo, query a API)
    usleep(rand_r(&conta_from->seed)%DELAY_TRANSACAO);

    cents_to_reais(t->valor, buf_saldo);
    print_log("Transação de %d para %d no valor de %s está sendo processada.\n",
              t->id_from, t->id_to, buf_saldo);
    cents_to_reais(conta_from->saldo, buf_saldo);
    print_log("Saldo de %s (id = %d): %s\n",
              conta_from->nome, t->id_from, buf_saldo);
    cents_to_reais(conta_to->saldo, buf_saldo);
    print_log("Saldo de %s (id = %d): %s\n",
              conta_to->nome, t->id_to, buf_saldo);
    
    if (conta_from->saldo < valor_necessario(t->valor)) {
        print_log("Erro: Conta %d não tem saldo suficiente.\n", t->id_from);
        assert(!pthread_mutex_unlock(&conta_from->mutex));
        assert(!pthread_mutex_unlock(&conta_to->mutex));

        free(args);
        return (void*)-1;
    }

    int delta = calc_delta(t->valor, &conta_from->seed);
    conta_from->saldo -= delta+t->valor;
    conta_to->saldo += t->valor;

    void** arg = malloc(3*sizeof(void*));
    arg[0] = b;
    arg[1] = &delta;
    arg[2] = NULL;
        
    increase_reserva(arg);
    
    cents_to_reais(t->valor, buf_saldo);
    print_log("Transação de %d para %d no valor de %s feita com sucesso.\n",
              t->id_from, t->id_to, buf_saldo);

    assert(!pthread_mutex_unlock(&conta_from->mutex));
    assert(!pthread_mutex_unlock(&conta_to->mutex));

    free(args);

    return NULL;
}