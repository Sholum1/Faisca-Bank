#include <stdio.h>
#include <stdlib.h>
#include <time.h>
// #include"impressao.h"
// #include"banco.h"
// #include"transacao.h"

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
        cents_to_reais(value, buf);
        fprintf(stderr,"%s $ $ $\n", buf);
    
    } else {

        fprintf(stderr,"\n~~~ Mais sorte na próxima tentativa! ~~~\n");
        fprintf(stderr,"L L L Foi taxado em: ");
        cents_to_reais(value, buf);
        fprintf(stderr,"%s L L L\n", buf);
    }
    
}
