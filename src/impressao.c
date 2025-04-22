#include"impressao.h"
// Tire o comentário se você quiser que imprima o estado mutex
#define CHECK_MUTEX


void cents_to_reais(int valor, char* buf){
    int cents = valor%100;
    int reais = valor/100;

    snprintf(buf,20,"R$%d.%02d",reais,cents);
}

void situacoes_conta(banco* faisca){
    printf("Situação das Contas:\n\n");
    for(int i = 0; i < faisca->qtd_contas; i++){
        conta* cur_conta = faisca->contas[i];

        #ifndef CHECK_MUTEX
            char buf[20];
            cents_to_reais(cur_conta->saldo, buf);
            printf("Nome da conta: %s\n", cur_conta->nome);
            printf("Saldo: %s\n", buf);
        #else
            char buf[20];
            cents_to_reais(cur_conta->saldo, buf);
            printf("Nome da conta: %s\n", cur_conta->nome);
            printf("Saldo: %s\n", buf);
            int is_free = !pthread_mutex_trylock(&cur_conta->mutex);
            if(is_free){
                printf("A conta está livre para as threads usarem!\n");
                pthread_mutex_unlock(&cur_conta->mutex);
            } else {
                printf("A conta está sendo acessada e portanto está inacessível a outras threads!\n");
            }
        #endif
    }

    printf("\n=====================================\n");
}