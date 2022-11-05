#ifndef CONTASARECEBER_H
#define CONTASARECEBER_H

#include <time.h>


typedef struct compras compras;

typedef struct contaArec{
    int codigoCliente;
    time_t codigoCompra;
    int modoPagamento;
    int parcelas;
    float valorParc;
    float entrada;
    struct tm dataAluga;
}contaArec;

void consultaContas(contaArec *contas, int quantidade);
void lancaEntradaOuParcela(contaArec *conta, int modoArm, float preco);
void reescreveLancamentosAprazo(contaArec *contas, int quantidade, char *nomeArq, char *nomeArqOr, char *modo);
void reescreveLancamentosAprazoBin(contaArec *contas, int quantidade, char *nomeArq, char *nomeArqOr, char *modo);
int leDadosLancamentosAprazo(contaArec **contas);
int leDadosLancamentosAprazoBin(contaArec **contas);
void realizaCompraAprazo(compras *compra, int modoArm);
contaArec * encontraContaPeloCodigo(contaArec *contas, int quantidade, time_t codigo);
int daBaixa(int modoArm);
int filtraContasAtrasadas(contaArec *contas, int quantidadeContas, contaArec **atrasadas, int modoArm);
int filtraContasClientes(contaArec *contas, int quantidadeContas, contaArec **contasDeUmCliente, int modoArm);
void mostraContasFiltradas(contaArec *contas, int quantidadeContas, int modoArm, int (*filtra)(contaArec *, int, contaArec **, int), char *mensagem);
void consultaLancamentos(int modoArm);

#endif