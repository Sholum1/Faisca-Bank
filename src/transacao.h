typedef struct {
    int id_from;
    int id_to;
    int valor; // Lembra de dividir por 100;   
} transacao;

/**
 * Dado um banco e um struct de transação, realiza transação entre duas contas
 * 
 * Retorna NULL se a transação tem sucesso e qualquer outra coisa se não há saldo suficiente na conta do pagante.
 */
void* realiza_transacao(void** args);


