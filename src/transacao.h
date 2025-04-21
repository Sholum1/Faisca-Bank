typedef struct {
    char nome[50];
    int id;
    int valor; // Lembra de dividir por 100;
    int being_used;
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

// Função que realiza a transição, levando em consideração se as contas envolvidas estão sendo usadas
void* realiza_transacao(void** args);


