#ifndef CAIXA_H
#define CAIXA_H
#include <time.h>
typedef struct Funcionarios Funcionarios;
typedef struct cliente cliente;
typedef struct filmes filmes;

typedef struct lancamentoCaixa{
    
    time_t codigoCompra;
    int modoPagamento;
    float valor;
    float valorPago;
    float troco;
    struct tm data;
    
}lancamentoCaixa;

typedef struct ContagemCaixa{
    
    float dinheiroLiquido;//valor discontado do troco
    float troco;
    float dinheiroBruto;//valor sem discontar do troco
    float ValoresRetirados;
}ContagemCaixa;

typedef struct compras{
    long int codigo;
    int modoPagamento;
    struct tm data;
    Funcionarios *vendedor;
    cliente *comprador;
    float preco;
    int quantidadeFilmesComprados;
    filmes *filmesComprados;
    int devolvido;

    
    
}compras;


void reescreveLancamentosCaixa(lancamentoCaixa *lancamentos, int quantidade, char *nomeArq, char *nomeArqOr, char *modo);
void reescreveLancamentosCaixaBin(lancamentoCaixa *lancamentos, int quantidade, char *nomeArq, char *nomeArqOr, char *modo);
int leDadosLancamentosBin(lancamentoCaixa **lancamentos);
float calculaPreco(filmes *filmesAlocados, int quantidade);
int leDadosLancamentos(lancamentoCaixa **lancamentos);
//void geraNotinha(compras *ptr, float valorPago, FILE *f, int modoArm);
void compraAvista(compras *compras, int modoArm);
float calculaTroco(float valorPagar, float *valorPago);
void compraAvista(compras *compras, int modoArm);
void mostraLancamentos(lancamentoCaixa *todosLancamentos, int quantidade, struct tm *data);
ContagemCaixa contabilizaCaixa(lancamentoCaixa *todosLancamentos, int quantidade, struct tm *data);
void contaCaixa(int modoArm);

#endif

