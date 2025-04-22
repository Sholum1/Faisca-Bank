#include"impressao.h"
// Tire o comentário se você quiser que imprima o estado mutex
// #define CHECK_MUTEX


void cents_to_reais(int valor, char* buf){
    int cents = valor%100;
    int reais = valor/100;

    snprintf(buf,20,"R$%d.%02d",reais,cents);
}

void situacoes_conta(banco* faisca){
    for(int i = 0; i < faisca->qtd_contas; i++){
        conta* cur_conta = faisca->contas[i];

        #ifndef CHECK_MUTEX
            pthread_mutex_lock(&cur_conta->mutex);

            char buf[20];
            cents_to_reais(cur_conta->saldo, buf);
            printf("Nome da conta: %s\n", cur_conta->nome);
            printf("Saldo: %s\n", buf);
            
            pthread_mutex_unlock(&cur_conta->mutex);
        #else
            char buf[20];
            // Literalmente uma race condition aqui em baixo!!!
            // Precisa fazer isso pois temos que checar se o mutex
            // está bloqueado, logo não podemos dar lock
            cents_to_reais(cur_conta->saldo, buf);
            printf("Nome da conta: %s\n", cur_conta->nome);
            printf("Saldo: %s\n", buf);
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