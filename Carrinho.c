#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include "ControleCategorias.h"
#include "ManipulacoesDeTexto.h"
#include "ControleDeDadosLocadora.h"
#include "ControleDeNumeros.h"
#include "FuncUtilitarias.h"
#include "menus.h"
#include "Devolucoes.h"
#include "ControleFilmes.h"
#include "controleFuncionarios.h"
#include "ControleClientes.h"
#include "Caixa.h"
#include "Devolucoes.h"
#include "ContasAreceber.h"
#include <ctype.h>
#include <locale.h>
#include <windows.h>
#define ERROMEM "ERRO: Memoria indisponivel!\n"
#define MAXIMOPARCELAS 3



void mostraListaFilmesAlocados(filmes *listaAlocacao, int quantidade){
    for(int i = 0; i<quantidade; i++){
        printf("%d-nome:%s\n"
                "  Valor:%.2f\n"
                "  codigo:%d\n"
                "  quantidade:%d\n",i ,listaAlocacao[i].nome, listaAlocacao[i].valorLocacao, listaAlocacao[i].codigo, listaAlocacao[i].exemplares);
    }
    if(quantidade){
        printf("valor: %.2f\n", calculaPreco(listaAlocacao, quantidade));
    }
}
filmes *deletaFilmeCarrinho(filmes *lista, int *quantidade, filmes *listaOriginal, int quantidadeListOriginal){
    char *codigo;
    filmes *apagar = NULL;
    filmes *novaLista = NULL;
    printf("digite o indice do carrinho ou nome do filme que deseja apagar:\n");
    //comentar sobre funcao estar buscando pelo indice e isso causar erros no carrinho
    apagar = buscaFilme(lista, *quantidade, "Nao existe esse filme no carrinho!");
    int indiceLista = 0;
    for(int i =0; i<*quantidade; i++){
        if(apagar != lista+i){
            novaLista = (indiceLista == 0)?malloc(sizeof(filmes)):realloc(novaLista, sizeof(filmes)*(indiceLista+1));
            
            novaLista[indiceLista] = lista[i];
            indiceLista++;
            
        }else{
            codigo = converteIntEmString(apagar->codigo);
            encontraFilmeCodigo(listaOriginal, quantidadeListOriginal, codigo, 1)->exemplares +=apagar->exemplares;
            codigo = limpaMemoria(codigo);
        }
    }
    lista = limpaMemoria(lista);
    *quantidade = indiceLista;
    return novaLista;
    
    
}
void alocaFilmes(filmes **filmesAlocados, int *quantidade, filmes *lista, int quantidadeLista, int ignora, char *frase){
    
    (*quantidade) = 0;
    filmes *alocAgr = NULL;
    int escolha;
    while(escolha != 61){
        menuGraphics(3, "Selecione uma opcao:", "adicionar filme", "Deletar filme", "fechar lista");
        mostraListaFilmesAlocados(filmesAlocados[0], *quantidade);
        escolha = escolheOpcao();
        /*adicionar no if uma funcao para verificar se todos os filmes estao zerados*/
        switch(escolha){
            case 59:
            
                (*quantidade)++;
                filmesAlocados[0] = (*quantidade==1)
                        ?malloc(sizeof(filmes)*(*quantidade))
                        :realloc(filmesAlocados[0], sizeof(filmes)*(*quantidade));
                verificaOperacao(filmesAlocados[0], ERROMEM, 1);
                printf("digite o nome ou codigo do filme\n");

                do{

                    alocAgr = buscaFilme(lista, quantidadeLista, "Nao existe um filme cadastrado com este dado, digite novamente:");
                    if(alocAgr->exemplares == 0 && ignora){
                        printf("Estoque do filme %s esta vazio!\n", alocAgr->nome);
                    }
                }while(alocAgr->exemplares == 0);

                filmesAlocados[0][(*quantidade)-1] = *alocAgr;

                printf("digite a quantidade:\n");
                verificaLimiteNumero(&filmesAlocados[0][(*quantidade)-1].exemplares, filmesAlocados[0][(*quantidade)-1].exemplares, 1, "%d");
                printf("%s\n", frase);
                alocAgr->exemplares -= filmesAlocados[0][(*quantidade)-1].exemplares;
                Sleep(1000);
            
                continue;
        
            case 60:
                filmesAlocados[0] = deletaFilmeCarrinho(filmesAlocados[0], quantidade, lista, quantidadeLista);
                continue;
            case 61:
                break;
            default:
                printf("voce escolheu uma opcao invalida, escolha novamente!\n");
                Sleep(1000);
                break;
        }
        menuGraphics(2, "Selecione uma opcao:", "adicionar filme", "fechar lista");
    }
    
    
}
void limpaMemoriaRealizaVenda(cliente** clients, int quantidadeClientes, Funcionarios ** func, int quantidadeFuncionarios, filmes ** todosFilmes, int quantidadeFilmes){
    limpaDadosClienteMemoria(clients[0], quantidadeClientes);
    clients[0] = limpaMemoria(clients[0]);
    limpaDadosFilmeMemoria(todosFilmes[0], quantidadeFilmes);
    todosFilmes[0] = limpaMemoria(todosFilmes[0]);
    apagaDadosStructFuncionarios(func[0], quantidadeFuncionarios);
    func[0] = limpaMemoria(func[0]);
}

void realizaVenda(int modoArm){
   
    compras compraAtual;
    cliente *clientes;
    int quantidadeClientes;
    time_t seg;
    Funcionarios *funcionarios;
    int quantidadeFuncionarios;
    
    filmes *TodosFilmes;
    
    int quantidadeTotalFilmes;
    
    quantidadeClientes = (modoArm)?leDadosClientesBin(&clientes):leDadosClientes(&clientes);
    quantidadeTotalFilmes = (modoArm)?leDadosFilmesBin(&TodosFilmes):leDadosFilmes(&TodosFilmes);
    quantidadeFuncionarios = (modoArm)?leDadosFuncionariosBin(&funcionarios):leDadosFuncionarios(&funcionarios);

    if(!contaQuantidadeExistente(sizeof(Funcionarios), funcionarios, quantidadeFuncionarios)){

        printf("Nao existem funcionarios cadastrados, primeiro cadastre um funcionario pra depois realizar a venda!\n");
        limpaMemoriaRealizaVenda(&clientes, quantidadeClientes, &funcionarios, quantidadeFuncionarios, &TodosFilmes, quantidadeTotalFilmes);
        Sleep(4000);
        return;
    }
    if(!contaQuantidadeExistente(sizeof(cliente), funcionarios, quantidadeFuncionarios)){

        printf("Nao existem clientes cadastrados, primeiro cadastre um clientes pra depois realizar a venda!\n");
        limpaMemoriaRealizaVenda(&clientes, quantidadeClientes, &funcionarios, quantidadeFuncionarios, &TodosFilmes, quantidadeTotalFilmes);
        Sleep(4000);
        return;
    }
    if(!contaQuantidadeExistente(sizeof(filmes), funcionarios, quantidadeFuncionarios)){

        printf("Nao existem filmes cadastrados, primeiro cadastre um filme pra depois realizar a venda!\n");
        limpaMemoriaRealizaVenda(&clientes, quantidadeClientes, &funcionarios, quantidadeFuncionarios, &TodosFilmes, quantidadeTotalFilmes);
        Sleep(4000);
        return;
    }
    printf("Digite o codigo ou nome do Funcionario:\n");
    compraAtual.vendedor = buscaFuncionarios(funcionarios, quantidadeFuncionarios, NULL);
    compraAtual.devolvido = 0;
    
    printf("digite o codigo, nome ou cpf do cliente:\n");
    compraAtual.comprador = buscaCliente(clientes, quantidadeClientes, "Nao existe nenhum cliente com o campo digitado registrado, por favor digite novamente:\n");
    
    alocaFilmes(&compraAtual.filmesComprados, &compraAtual.quantidadeFilmesComprados, TodosFilmes, quantidadeTotalFilmes, 1, "Filme alocado com sucesso!");
    
    compraAtual.preco = calculaPreco(compraAtual.filmesComprados, compraAtual.quantidadeFilmesComprados);
    
    time(&seg);
    compraAtual.data = *localtime(&seg);
    compraAtual.codigo = time(NULL);
    
    while(1){
        menuGraphics(2, "Escolha o modo de pagamento:", "A vista", "A prazo");
        compraAtual.modoPagamento = retornaNumeroConformeF(2, 0);
        switch(compraAtual.modoPagamento){
            
            case 1:
                compraAvista(&compraAtual, modoArm);
                break;
            case 2:
                realizaCompraAprazo(&compraAtual, modoArm);
                break;
            default:
                printf("voce digitou uma opcao invalida! Por favor, digite novamente:\n");
                Sleep(1500);
                continue;
        }
        break;
    }
    (modoArm)?reescreveDadosDevolucoesBin(&compraAtual, 1, "devolucoes.bin", "devolucoes.bin", "ab"):reescreveDadosDevolucoes(&compraAtual, 1, "devolucoes.txt", "devolucoes.txt", "a");
    (modoArm)?reescreveDadosFilmeBin(TodosFilmes, quantidadeTotalFilmes):reescreveDadosFilme(TodosFilmes, quantidadeTotalFilmes);
    limpaMemoriaRealizaVenda(&clientes, quantidadeClientes, &funcionarios, quantidadeFuncionarios, &TodosFilmes, quantidadeTotalFilmes);
    return;
    
}
