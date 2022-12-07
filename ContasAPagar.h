#ifndef UNTITLED_CONTASAPAGAR_H
#define UNTITLED_CONTASAPAGAR_H

#include <time.h>

typedef struct entrada entrada;

typedef struct contaApag{
    long int codigoCompra;
    int modoPagamento;
    int parcelas;
    float valorParc;
    float entrada;
    struct tm dataAluga;
}contaApag;

void reescreveEntradaAprazo(contaApag *contas, int quantidade, char *nomeArq, char *nomeArqOr, char *modo);
void reescreveEntradasAprazoBin(contaApag *contas, int quantidade, char *nomeArq, char *nomeArqOr, char *modo);
int leDadosEntradasAprazo(contaApag **contas);
int leDadosEntradasAprazoBin(contaApag **contas);
void LancaEntradaOuParcela_EntradaFilmes(contaApag *contaP, int modoArm, float preco);
void realizaEntradaAprazo(entrada *entradas, int modoArm, float valorCaixa);
contaApag *encontraEntradaCodigo(contaApag *conta, int quantidade, long int codigo);
void consultaEntradas(contaApag *contas, int quantidade);
int BaixaEntradasFilmes(int modoArm);
int filtraEntradasAtrasadas(contaApag *contas, int quantidadeContas, contaApag **atrasadas, int modoArm);
int filtraEntradasQuantidadeParcelas(contaApag *contas, int quantidadeContas, contaApag **contasRetornar, int parcelas);
contaApag *buscaEntrada(contaApag *ptr, int quantidade, int ignoraDev);
void mostraEntrada(entrada *compra);
void mostraEntradasFiltradas(contaApag *contas, int quantidadeContas, int modoArm, int (*filtra)(contaApag *, int, contaApag **, int), char *mensagem);
void consultaLancamentosEntrada(int modoArm);
void mostraEntradaAtrasada(contaApag *compra, int quantidade);

#endif //UNTITLED_CONTASAPAGAR_H
