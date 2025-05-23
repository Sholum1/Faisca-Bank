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

/**
 * Dado um valor em centavos, escreve uma string em buffer indicando o valor
 * em reais (por exemplo, se valor = 123, será escrito "R$1,23" em buf).
 */
void cents_to_reais(int valor, char* buf){
    int cents = valor%100;
    int reais = valor/100;
    if (cents < 0) {
        cents = -cents;
    }

    snprintf(buf,20,"R$%d.%02d",reais,cents);
}

/**
 * Imprime uma tabela indicando para conta o nome do titular, saldo e 
 * se ela está sendo acessada nesse momento.
 * 
 * Essa função *causa* uma data race, pois lê de variáveis que estão sendo
 * escritas por outras threads. Entretanto, isso é *esperado*, pois o intuito
 * da função é mostar uma snapshot do sistema, mesmo que ele esteja dessincronizado.
 * 
 * Garantir que essa função é síncrona altera significativamente o funcionamento
 * das threads e, ao meu ver, vai contra o espírito do projeto. Por exemplo, por não
 * ser síncrona, é possível ver que o total de dinheiro no sistema não é constante para
 * todo instante (pois entre os writes e reads pode haver falta de sincronia pontual), 
 * mas que ao final do programa o dinheiro é conservado
 */
void situacao_contas(banco* faisca) {
    printf("╔═════════════════════════════════════════════════════════════════════╗\n");

    printf("║%*s%*s║\n", 47, " SITUAÇÃO DAS CONTAS ", 24, "");

    printf("╠══════════════════════════════════════╦════════════════╦═════════════╣\n");

    printf("║ %-36s ║ %14s ║ %-11s ║\n", "NOME", "SALDO", "STATUS");
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

/**
 * Imprime uma tabela indicando para thread o nome dos envolvidos na transação
 * que ela está processando, e o status da operação (se ela está esperando outra
 * thread, processando a transação ou livre).
 * 
 * Essa função *causa* uma data race, pois lê de variáveis que estão sendo
 * escritas por outras threads. Entretanto, isso é *esperado*, pois o intuito
 * da função é mostar uma snapshot do sistema, mesmo que ele esteja dessincronizado.
 */
void situacao_threads(int qtd_threads, int thread_work[], int thread_status[], transacao* transacoes, conta* contas[]){
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

/**
 * Função auxiliar para imprimir 3 símbolos do caça níquel
 */
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

/**
 * Se value for maior ou igual a 0, imprime na saída de erro um caça níquel
 * mostrando que o usuário perdeu (e portanto pagará taxa). Caso contrário,
 * mostra caça níquel indicando que o usuário ganhou o faísca cashback.
 */
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
    
    fprintf(stderr,"===============================================\n");
    print_simbolos(simbolos);
    
    if (value < 0) {
        
        fprintf(stderr,"\n🔥🔥🔥 FAÍSCA 🔥🔥🔥 FAÍSCA 🔥🔥🔥 FAÍSCA 🔥🔥🔥 FAÍSCA 🔥🔥🔥\n");
        cents_to_reais(-value, buf);
        fprintf(stderr,"💰💰💰 VOCÊ GANHOU INCRÍVEIS: %s !!! PARABÉNS!!! 💰💰💰\n", buf);
    
    } else {

        fprintf(stderr,"\n~~~ Mais sorte na próxima tentativa! ~~~\n");        
        cents_to_reais(value, buf);
        fprintf(stderr,"💸💸💸 Taxa de transação: %s 💸💸💸\n", buf);
    }
    
    fprintf(stderr,"===============================================\n");
}