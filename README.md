# TODO
- [x] Pool de threads
- [x] Structs das contas
- [x] Funções de transações
- [x] Faísca Cashback
- [x] Taxação
- [x] Visualização


# Projeto 1 de MC504 (Multi-threads)
## 🚀 Faísca Bank: O Sistema Bancário que (Não) Aposta na Sorte
<div align="center">
  <a href="https://en.wikipedia.org/wiki/C_(programming_language)">
    <img src="https://img.shields.io/badge/Linguagem-C-%23f34b7d?style=for-the-badge" alt="Linguagem C">
  </a>
</div>

---

### 📌 **Descrição do Projeto**
O **[Faísca Bet](https://github.com/defnotmee/faisca-bet)** trouxe diversão e oportunidades financeiras para todo país por meio de jogos
simples e divertidos. Mas para que só se divertir dentro dos jogos se podemos se divertir fora também? Visando isso, criamos o **Faísca Bank**!!!

Nosso sistema de transações promove uma forma rápida e segura de realizar pagamentos bancários, sem toda a burocracia de bancos convencionais. Além
disso, trazemos uma experiência divertida, pois toda vez que você realiza um pagamento, você tem a chance de receber um **Faísca Cashback** :tm:.

Utilizamos um sistema multithread para realizar todo esse processammento, garantindo que suas transações ocorram de forma rápida e segura. Visto
a alta tecnologia, também coletamos uma pequena taxa de cada transação, mas nada demais!

---

### 🛠️ **Funcionalidades**
- **Transações concorrentes** sem medo de *race conditions* (ou de processos judiciais).
- **Sistema Altamente Parametrizável** a partir de fatores como número de threads e contas.
- **Faísca Cashback** para chance de ganhar dinheiro fácil, provocando uma faísca na sua carteira.
- **Linda Visualização** a partir de tabelas em ASCII via terminal.
---

### 🦧 **Como Usar**

Basta usar o comando `bash run.sh` na pasta principal para executar uma simulação padrão.

Para mudar os parametros da simulação, basta editar os defines na pasta src.

### 🔧 **Parâmetros**

#### **main.c**

- **QTD_CONTAS:** Quantidade de contas da simulação *(default 10)*
- **QTD_TRANSACOES:** Quantidade de transações na simulação *(default 400)*
- **MAX_THREADS:** Quantidade de threads na simulação *(default 5)*

#### **transacao.h**

- **DELAY_TRANSACAO:** Delay máximo (em microssegundos) associado a cada transação *(default 500000)*
- **TAXA:** Proporção da taxa associada a cada transação *(default 0.05, ou seja, 5%)*
- **JACKPOT_CHANCE:** Número esperado de transações para receber um Faísca Cashback *(default 75)*
- **JACKPOT_MULT:** Quantidade de vezes que seu dinheiro é multiplicado ao receber um faísca cashback. Note que o valor multiplicado é a **taxa**, não o valor da transação  *(default 50)*

#### **conta.h**

- **MAX_LEN:** Tamanho máximo do nome das contas do Faísca Bank.