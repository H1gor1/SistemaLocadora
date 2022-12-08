#ifndef CONTASARECEBER_H
#define CONTASARECEBER_H

#include <time.h>


typedef struct compras compras;

typedef struct contaArec{
    int codigoCliente;
    long int codigoCompra;
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
contaArec * encontraContaPeloCodigo(contaArec *contas, int quantidade, long int codigo);
int daBaixa(int modoArm);
int filtraContasAtrasadas(contaArec *contas, int quantidadeContas, contaArec **atrasadas, int modoArm);
int filtraContasClientes(contaArec *contas, int quantidadeContas, contaArec **contasDeUmCliente, int modoArm);
void mostraContasFiltradas(contaArec *contas, int quantidadeContas, int modoArm,
                           int (*filtra)(contaArec *, int, contaArec **, int), char *mensagem, char *mensagemCasoNaoExista);
void consultaLancamentos(int modoArm);
int filtraPorData(contaArec *contas, int quantidade, contaArec **dest, int modoArm);
int filtraContasVencimentoMes(contaArec *contas, int quantidadeContas, contaArec **atrasadas, int mesVenc);

#endif