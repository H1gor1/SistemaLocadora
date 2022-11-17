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
filmes *deletaFilmeCarrinho(filmes *lista, int *quantidade, filmes *listaOriginal, int quantidadeListOriginal, int vendaOuEntrada){
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
            encontraFilmeCodigo(listaOriginal, quantidadeListOriginal, codigo, 1)->exemplares +=(vendaOuEntrada)?apagar->exemplares:-apagar->exemplares;
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
    filmes *casoRepetir = NULL;
    int escolha;
    while(escolha != 61){
        menuGraphics(3, "Selecione uma opcao:", "adicionar filme", "Deletar filme", "fechar lista");
        mostraListaFilmesAlocados(filmesAlocados[0], *quantidade);
        escolha = escolheOpcao();

        switch(escolha){
            case 59:
                printf("digite o nome ou codigo do filme:\n");
                do{

                    alocAgr = buscaFilme(lista, quantidadeLista, "Nao existe um filme cadastrado com este dado, digite novamente:");
                    if(alocAgr->exemplares == 0 && ignora){
                        printf("Estoque do filme %s esta vazio!\n", alocAgr->nome);
                    }
                }while(alocAgr->exemplares == ((ignora)?0:-1));
                casoRepetir = encontraFilmeCodigo(filmesAlocados[0], *quantidade, converteIntEmString(alocAgr->codigo), 1);
                if(casoRepetir && *quantidade){
                    int quantidadeTemporaria = 0;
                    printf("digite a quantidade:\n");
                    if(ignora) {
                        verificaLimiteNumero(&quantidadeTemporaria, alocAgr->exemplares, 1, "%d");
                        casoRepetir->exemplares += quantidadeTemporaria;
                        alocAgr->exemplares-=quantidadeTemporaria;
                    }else{
                        verificaNumero(&quantidadeTemporaria, "%d");
                        casoRepetir->exemplares+=quantidadeTemporaria;
                        alocAgr->exemplares+=quantidadeTemporaria;
                    }
                    printf("%s\n", frase);

                break;
                }
                (*quantidade)++;
                filmesAlocados[0] = (*quantidade==1)
                                    ?malloc(sizeof(filmes)*(*quantidade))
                                    :realloc(filmesAlocados[0], sizeof(filmes)*(*quantidade));
                verificaOperacao(filmesAlocados[0], ERROMEM, 1);




                filmesAlocados[0][(*quantidade)-1] = *alocAgr;


                printf("digite a quantidade:\n");
                if(ignora == 1) {
                    verificaLimiteNumero(&filmesAlocados[0][(*quantidade) - 1].exemplares,
                                         alocAgr->exemplares, 1, "%d");

                } else {
                    verificaNumero(&filmesAlocados[0][(*quantidade - 1)].exemplares, "%d");

                }
                printf("%s\n", frase);

                if(ignora == 1){
                    alocAgr->exemplares -= filmesAlocados[0][(*quantidade)-1].exemplares;
                } else {
                    alocAgr->exemplares += filmesAlocados[0][(*quantidade)-1].exemplares;
                }

                Sleep(1000);

                continue;

            case 60:
                filmesAlocados[0] = deletaFilmeCarrinho(filmesAlocados[0], quantidade, lista, quantidadeLista, ignora);
                continue;
            case 61:
                break;
            default:
                printf("voce escolheu uma opcao invalida, escolha novamente!\n");
                Sleep(1000);
                break;
        }

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
    if(!contaQuantidadeExistente(sizeof(cliente), funcionarios, quantidadeClientes)){

        printf("Nao existem clientes cadastrados, primeiro cadastre um clientes pra depois realizar a venda!\n");
        limpaMemoriaRealizaVenda(&clientes, quantidadeClientes, &funcionarios, quantidadeFuncionarios, &TodosFilmes, quantidadeTotalFilmes);
        Sleep(4000);
        return;
    }
    if(!contaQuantidadeExistente(sizeof(filmes), funcionarios, quantidadeTotalFilmes)){

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
    

    compraAtual.modoPagamento = escolheMenu("Escolha o modo de pagamento", 2, 0,"A vista", "A prazo")+1;
    switch(compraAtual.modoPagamento){
            
        case 1:
            compraAvista(&compraAtual, modoArm);
            break;
        case 2:
            realizaCompraAprazo(&compraAtual, modoArm);
            break;

    }
    (modoArm)?reescreveDadosDevolucoesBin(&compraAtual, 1, "devolucoes.bin", "devolucoes.bin", "ab"):reescreveDadosDevolucoes(&compraAtual, 1, "devolucoes.txt", "devolucoes.txt", "a");
    (modoArm)?reescreveDadosFilmeBin(TodosFilmes, quantidadeTotalFilmes):reescreveDadosFilme(TodosFilmes, quantidadeTotalFilmes);
    limpaMemoriaRealizaVenda(&clientes, quantidadeClientes, &funcionarios, quantidadeFuncionarios, &TodosFilmes, quantidadeTotalFilmes);
    return;
    
}
