#include <time.h>
#include "controleFuncionarios.h"
#include "ControleClientes.h"
#ifndef Devolucoes_h
#define Devolucoes_h
typedef struct compras compras;



typedef struct quantidades{
    
    int quantidadesFilmes;
    int quantidadesFuncionarios;
    int quantidadeClientes;
    int quantidadeAlugacoes;
}quantidades;

quantidades leDadosDevolucoes(compras ** dev, Funcionarios **func, cliente **clients, filmes **movies);

quantidades leDadosDevolucoesBin(compras **dev, Funcionarios **func, cliente **clients, filmes **movies);

void reescreveDadosDevolucoes(compras *dev, int quantidade, char *nomeArq, char *nomeArqOr, char *modo);

void reescreveDadosDevolucoesBin(compras *dev, int quantidade, char *nomeArq, char *nomeArqOr, char *modo);

compras * encontraCompraCodigo(compras *ptr, int quantidade, long int codigo);

compras *buscaCompra(compras *ptr, int quantidade, int ignoraDev);

void atribuiNull(void *endereco, int quantidade, size_t oqueLimpar);

void listaCompra(int modoArm);

void devolveFilme(int modoArm);

void mostraCompra(compras *compra, int resumir);

void limpaDadosDevolucoesMemoria(filmes **todosFilmes, cliente ** todosClientes,Funcionarios **todosFuncionarios,
                                 compras **todasAlocacoes, quantidades quant);
#endif