#include"impressao.h"

char* cents_to_reais(int valor){
    int cents = valor%100;
    int reais = valor/100;
    char* ret = malloc(20);

    snprintf(ret,20,"R$%d.%d",reais,cents);

    return ret;
}

void situacoes_conta(banco* faisca){
    for(int i = 0; i < faisca->qtd_contas; i++){
        conta* cur_conta = faisca->contas[i];
        printf("Nome da conta: %s\n", cur_conta->nome);
        printf("Saldo: %s\n", cents_to_reais(cur_conta->saldo));
        // int is_free = !pthread_mutex_trylock(&cur_conta->mutex);
        // if(is_free)
        //     pthread_mutex_unlock(&cur_conta->mutex);
        // printf("Mutex: %d", is_free);
        printf("\n");
    }
}