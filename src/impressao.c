#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include"impressao.h"
// Tire o comentário se você quiser que imprima o estado mutex
#define SIMBOLOS 5
#define LINHAS 7
#define CHECK_MUTEX


const char *slot_simbolos[SIMBOLOS][LINHAS] = {
    {// Cherries
     "╔══════════╗",
     "║  ╔═══╗   ║",
     "║  ║ O ║   ║",
     "║  ╚═@═╝   ║",
     "║   \\|/    ║",
     "║    |     ║",
     "╚══════════╝"
    },
    {// 7
     "╔══════════╗",
     "║ ┌──────┐ ║",
     "║ │ 7777 │ ║",
     "║ │   7  │ ║",
     "║ │  7   │ ║",
     "║ └──────┘ ║",
     "╚══════════╝"
    },
    {// OURO
     "╔══════════╗",
     "║          ║",
     "║ ┌──────┐ ║",
     "║ │ OURO │ ║",
     "║ └──────┘ ║",
     "║          ║",
     "╚══════════╝"
    },
    {// LIMÃO
     "╔══════════╗",
     "║   _()_   ║",
     "║  / ´¨`\\  ║",
     "║ /.´ .. \\ ║",
     "║ \\ ` .  / ║",
     "║  \\____/  ║",
     "╚══════════╝"
    },
    {// SINO
     "╔══════════╗",
     "║    /\\    ║",
     "║   /  \\   ║",
     "║  / /  \\  ║",
     "║ / O    \\ ║",
     "║ \\______/ ║",
     "╚══════════╝"
    }
};

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
	        printf("A conta está sendo acessada e portanto está");
	        printf(" inacessível a outras threads!\n");
            }
        #endif
    }

    printf("\n=====================================\n");
}

void print_simbolos(const int simbolos[3]) {
    for (int i = 0; i < LINHAS; i++) {
	printf("  %s  %s  %s\n",
	       slot_simbolos[simbolos[0]][i],
	       slot_simbolos[simbolos[1]][i],
	       slot_simbolos[simbolos[2]][i]);
    }
}

void print_jackpot(int value) {
    
    char buf[20];
    srand(time(NULL));
    int simbolos[3];
    
    if (value > 0) { // Ganhou
	int win_symbol = rand() % SIMBOLOS;
	simbolos[0] = simbolos[1] = simbolos[2] = win_symbol;
    } else {        // Perdeu
	do {
	    simbolos[0] = rand() % SIMBOLOS;
	    simbolos[1] = rand() % SIMBOLOS;
	    simbolos[2] = rand() % SIMBOLOS;
	} while (simbolos[0] == simbolos[1] && simbolos[1] == simbolos[2]);
    }
    
    printf("\n");
    print_simbolos(simbolos);
    
    if (value > 0) {
	printf("\n!!! VENCEDOR !!!\n");
	printf("$ $ $ RECEBEU: ");
	cents_to_reais(value, buf);
	printf(" $ $ $\n");
    } else {
	printf("\n~~~ Mais sorte na próxima tentativa! ~~~\n");
	printf("L L L Foi taxado em: ");
	cents_to_reais(value, buf);
	printf(" $ $ $\n");
    }
}
