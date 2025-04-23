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
    if (cents < 0) {
        cents = -cents;
    }

    snprintf(buf,20,"R$%d.%02d",reais,cents);
}

void situacao_contas(banco* faisca) {
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

    char buf[20];
    printf("╠══════════════════════════════════════╬════════════════╬═════════════╣\n");

    cents_to_reais(faisca->reserva, buf);
    printf("║ %-36s ║ %14s ║ %-11s ║\n",
            "RESERVA",
            buf,
            faisca->reserva >= 0 ? (faisca->reserva > 0 ? "LUCRANDO 🤑" : "         🤔") : "FALINDO  🥺");

    printf("╚══════════════════════════════════════╩════════════════╩═════════════╝\n");
}

void situacao_threads(int qtd_threads, int thread_work[], int thread_status[], void* transacoes, conta* contas[]){
    printf("╔═════════════════════════════════════════════════════════════════════╗\n");
    printf("║%*s%*s║\n", 47, " SITUAÇÃO DAS THREADS ", 24, "");
    printf("╠════╦═══════════════════════╦═══════════════════════╦════════════════╣\n");
    printf("║ %-2s ║ %-21s ║ %-22s ║ %-14s ║\n", "ID", "PAGANTE", "DESTINATÁRIO", "STATUS");
    printf("╠════╬═══════════════════════╬═══════════════════════╬════════════════╣\n");
    for(int i = 0; i < qtd_threads; i++) {
        char pagante[30] = "N/A";
        char destinatario[30] = "N/A";

        if(thread_work[i] != -1){
            transacao* t = &((transacao*)transacoes)[thread_work[i]];
            snprintf(pagante, 30, "%-20.20s", contas[t->id_from]->nome);
            snprintf(destinatario, 30, "%-20.20s", contas[t->id_to]->nome);
        }
        const char* status;
        const char* icon;
        
        if(thread_status[i] == 0) {
            status = "LIVRE";
            icon = "🟢";
        } else if(thread_status[i] == 1){
            status = "ESPERANDO";
            icon = "🔴";
        } else {
            status = "EXECUTANDO";
            icon = "⏳";
        }

        printf("║ %-2d ║ %-21s ║ %-21s ║ %-11s %s ║\n",
               i,
               pagante,
               destinatario,
               status,
               icon);
    }

    printf("╚════╩═══════════════════════╩═══════════════════════╩════════════════╝\n");
}

void print_simbolos(const int simbolos[3]) {
    fprintf(stderr,"╔══════════════════════════════════════════╗\n");
    for (int i = 0; i < LINHAS; i++) {
	fprintf(stderr,"║ %s  %s  %s ║\n",
	       slot_simbolos[simbolos[0]][i],
	       slot_simbolos[simbolos[1]][i],
	       slot_simbolos[simbolos[2]][i]);
    }
    fprintf(stderr,"╚══════════════════════════════════════════╝\n");
}

void print_jackpot(int value) {
    
    char buf[20];
    int simbolos[3];

    if (value < 0 ) { // Ganhou
    
        int win_symbol = rand() % SIMBOLOS;
	    simbolos[0] = simbolos[1] = simbolos[2] = win_symbol;
    
    } else { // Perdeu

        do { // Gera 3 símbolos diferentes
        
            simbolos[0] = rand() % SIMBOLOS;
            simbolos[1] = rand() % SIMBOLOS;
            simbolos[2] = rand() % SIMBOLOS;
        
        } while (simbolos[0] == simbolos[1] && simbolos[1] == simbolos[2]);
    
    }
    
    fprintf(stderr,"\n");
    print_simbolos(simbolos);
    
    if (value < 0) {
        
        fprintf(stderr,"\n!!! VENCEDOR !!!\n");
        fprintf(stderr,"$ $ $ RECEBEU: ");
        cents_to_reais(-value, buf);
        fprintf(stderr,"%s $ $ $\n", buf);
    
    } else {

        fprintf(stderr,"\n~~~ Mais sorte na próxima tentativa! ~~~\n");
        fprintf(stderr,"L L L Foi taxado em: ");
        cents_to_reais(value, buf);
        fprintf(stderr,"%s L L L\n", buf);
    }
    
}