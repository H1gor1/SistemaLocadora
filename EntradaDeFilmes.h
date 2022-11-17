#ifndef ENTRADAFILMES_H
#define ENTRADAFILMES_H
#include "fornecedores.h"
#include <time.h>

typedef struct entrada{
    long int codigo;
    fornecedor compraAtual;
    filmes *lista;
    int quantidade;
    float frete;
    float imposto;
    float precoTotal;
    int modoPagamento;
    struct tm data;
}entrada;

typedef struct lancamentoEntradas{

    time_t codigoCompra;
    int modoPagamento;
    float valor;
    float valorPago;
    float troco;
    struct tm data;

}lancamentoEntradas;

int leDadosEntradasFilmesBin(lancamentoEntradas **contas);
int leDadosLancamentoEntradasFilmes(lancamentoEntradas **contas);
entrada *buscaEntradaAvista(entrada *ptr, int quantidade, int ignoraDev);
entrada *encontraEntradaAvistaCodigo(entrada *ptr, int quantidade, long int codigo);
void realizaEntrada(int modoAbertura);
void limpaMemoriaRealizaEntrada(fornecedor ** forn, int quantidadeFornecedores, filmes ** todosFilmes, int quantidadeFilmes);
float CalcPrecoEntrada(filmes *filmesEntrada, int quantidade, float frete, float imposto);
void compraAvistaEntrada(entrada *Entradas, int modoAbertura);
void reescreveLancamentosEntrada(lancamentoEntradas *lancamentos, int quantidade, char *nomeArq, char *nomeArqOr, char *modo);
void reescreveLancamentosEntradaBin(lancamentoEntradas *lancamentos, int quantidade, char *nomeArq, char *nomeArqOr, char *modo);
#endif 