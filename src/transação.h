typedef struct {
    char nome[50];
    int id;
    int valor; // Lembra de dividir por 100;   
} conta;

typedef struct {
    int id_from;
    int id_to;
    int valor; // Lembra de dividir por 100;   
} transacao;

typedef struct{
    conta* contas;
    int num;
} banco;


