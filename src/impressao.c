#include"impressao.h"
// Tire o comentário se você quiser que imprima o mutex
// #define CHECK_MUTEX


char* cents_to_reais(int valor){
    int cents = valor%100;
    int reais = valor/100;
    char* ret = malloc(20);

    snprintf(ret,20,"R$%d.%d%d",reais,cents/10,cents%10);

    return ret;
}

void situacoes_conta(banco* faisca){
    for(int i = 0; i < faisca->qtd_contas; i++){
        conta* cur_conta = faisca->contas[i];

        #ifndef CHECK_MUTEX
            pthread_mutex_lock(&cur_conta->mutex);

            printf("Nome da conta: %s\n", cur_conta->nome);
            printf("Saldo: %s\n", cents_to_reais(cur_conta->saldo));
            
            pthread_mutex_unlock(&cur_conta->mutex);
        #else
            // Literalmente uma race condition aqui em baixo!!!
            // Precisa fazer isso pois temos que checar se o mutex
            // está bloqueado, logo não podemos dar lock
            printf("Nome da conta: %s\n", cur_conta->nome);
            printf("Saldo: %s\n", cents_to_reais(cur_conta->saldo));
            int is_free = !pthread_mutex_trylock(&cur_conta->mutex);
            if(is_free){
                printf("Mutex está livre!\n");
                pthread_mutex_unlock(&cur_conta->mutex);
            } else {
                printf("Mutex está bloqueado!\n");
            }
        #endif
    }
}