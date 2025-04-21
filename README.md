# TODO
- [ ] Pool de threads
- [ ] Structs das contas
- [ ] Funções de transações
- [ ] Semaforos


# Projeto 1 de MC504 (Multi-threads)
O projeto consiste em um sistema "bancário" que suporta transações entre diferentes contas.
Visamos implementar o multi-threading ao processar diferentes transações. Com isso em mente, teremos de considerar diversos problemas que são acarretados por este meio de implementação, como transações duplicadas, erro de contabilidade do valor total da conta, dentre outros erros que decorrem. Assim, implementaremos também, métodos que evitem esses possíveis erros de sincronização como a utilização de semáforos ou mutex locks, além de variaveis de condição que garantirão a sincronização das transações dentro do sistema.


https://github.com/defnotmee/faisca-bet