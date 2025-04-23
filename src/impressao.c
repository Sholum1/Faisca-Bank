#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include"impressao.h"
#include"banco.h"
#include"transacao.h"

#define SIMBOLOS 5
#define LINHAS 7

#define JACKPOT_VAL 5000

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

void situacoes_conta(banco* faisca) {
    printf("╔═════════════════════════════════════════════════════════════════════╗\n");

    printf("║%*s%*s║\n", 47, " SITUAÇÃO DAS CONTAS ", 24, "");

    printf("╠══════════════════════════════════════╦════════════════╦═════════════╣\n");

    printf("║ %-36s ║ %14s ║ %-11s ║\n", "CONTA", "SALDO", "STATUS");
    printf("╠══════════════════════════════════════╬════════════════╬═════════════╣\n");

    for(int i = 0; i < faisca->qtd_contas; i++) {
        conta* cur_conta = faisca->contas[i];
        char buf[20];
        cents_to_reais(cur_conta->saldo, buf);

        int livrep = !pthread_mutex_trylock(&cur_conta->mutex);
        char status[19];
        const char* icon = livrep ? "🟢" : "🔴";

        snprintf(status, 19, "%-8.8s %s",
                livrep ? "LIVRE" : "EM USO",
                icon);

        if(livrep) pthread_mutex_unlock(&cur_conta->mutex);

        printf("║ %-36.34s ║ %14s ║ %-13.18s ║\n",
                cur_conta->nome,
                buf,
                status);
    }

    printf("╚══════════════════════════════════════╩════════════════╩═════════════╝\n");
}

void print_simbolos(const int simbolos[3]) {
    printf("╔══════════════════════════════════════════╗\n");
    for (int i = 0; i < LINHAS; i++) {
	printf("║ %s  %s  %s ║\n",
	       slot_simbolos[simbolos[0]][i],
	       slot_simbolos[simbolos[1]][i],
	       slot_simbolos[simbolos[2]][i]);
    }
    printf("╚══════════════════════════════════════════╝\n");
}

void print_jackpot(int value) {
    
    char buf[20];
    srand(time(NULL));
    int simbolos[3];

    if (value == -1) { // Ganhou
    
        int win_symbol = rand() % SIMBOLOS;
	    simbolos[0] = simbolos[1] = simbolos[2] = win_symbol;
    
    } else { // Perdeu

        do { // Gera 3 símbolos diferentes
        
            simbolos[0] = rand() % SIMBOLOS;
            simbolos[1] = rand() % SIMBOLOS;
            simbolos[2] = rand() % SIMBOLOS;
        
        } while (simbolos[0] == simbolos[1] && simbolos[1] == simbolos[2]);
    
    }
    
    printf("\n");
    print_simbolos(simbolos);
    
    if (value == -1) {
        
        printf("\n!!! VENCEDOR !!!\n");
        printf("$ $ $ RECEBEU: ");
        cents_to_reais(JACKPOT_VAL, buf);
        printf("%s $ $ $\n", buf);
    
    } else {

        printf("\n~~~ Mais sorte na próxima tentativa! ~~~\n");
        printf("L L L Foi taxado em: ");
        cents_to_reais(value, buf);
        printf("%s L L L\n", buf);
    }
}
