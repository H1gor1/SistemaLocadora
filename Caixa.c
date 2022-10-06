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
#include "ControleFilmes.h"
#include "controleFuncionarios.h"
#include "ControleClientes.h"
#include <ctype.h>
#include <locale.h>
#include <windows.h>
#define ERROMEM "ERRO: Memoria indisponivel!\n"

float calculaPreco(filmes *filmesAlocados, int quantidade, categoria *categorias, int quantidadeCat){
    
    float preco = 0;
    
    for(int i = 0; i<quantidade; i++){
        for(int j = 0; j<quantidadeCat; j++){
            if(filmesAlocados[i].codigoCategoria == categorias[j].codigo){
                preco += categorias[j].valorLocacao * filmesAlocados[i].exemplares;
                break;
                
            }
        }
    }
    return preco;
}
void alocaFilmes(filmes **filmesAlocados, int *quantidade, filmes *lista, int quantidadeLista){
    
    
    menuGraphics(2, "Selecione uma opcao:", "adicionar filme", "fechar lista");
    filmes *alocAgr = NULL;
    int escolha;
    while(escolha != 2){
        verificaNumero(&escolha, "%d");
        /*adicionar no if uma funcao para verificar se todos os filmes estao zerados*/
        if(escolha == 1){
            
            (*quantidade)++;
            filmesAlocados[0] = (*quantidade==1)
                    ?malloc(sizeof(filmes)*(*quantidade))
                    :realloc(filmesAlocados[0], sizeof(filmes)*(*quantidade));
            printf("digite o nome ou codigo do filme");
            
            do{
                
                alocAgr = buscaFilme(lista, quantidadeLista, "Nao existe um filme cadastrado com este dado, digite novamente:");
                if(alocAgr->exemplares == 0){
                    printf("Estoque do filme %s esta vazio!\n", alocAgr->nome);
                }
            }while(alocAgr->exemplares == 0);
            
            filmesAlocados[0][(*quantidade)-1] = *alocAgr;
            
            printf("digite a quantidade:\n");
            verificaLimiteNumero(&filmesAlocados[0][(*quantidade)-1].exemplares, filmesAlocados[0][(*quantidade)-1].exemplares, 1, "%d");
            printf("Filme alocado com sucesso!\n");
            alocAgr->exemplares -= filmesAlocados[0][(*quantidade)-1].exemplares;
            Sleep(1000);
            
            continue;
        }
        
        if(escolha!=2){
            printf("voce escolheu uma opcao invalida, escolha novamente!\n");
            Sleep(1000);
        }
        menuGraphics(2, "Selecione uma opcao:", "adicionar filme", "fechar lista");
    }
    
    
}


void realizaVenda(int modoArm){
    
    cliente *clientes;
    cliente *comprador;
    int quantidadeClientes;
    
    Funcionarios *funcionarios;
    Funcionarios *vendedor;
    int quantidadeFuncionarios;
    
    filmes *TodosFilmes;
    filmes *filmesAlocados;
    int quantidadeTotalFilmes;
    int quantidadeFilmesAlocados = 0;
   
    categoria *categorias;
    int quantidadeCategorias;
    
    quantidadeClientes = (modoArm)?leDadosClientesBin(&clientes):leDadosClientes(&clientes);
    quantidadeTotalFilmes = (modoArm)?leDadosFilmesBin(&TodosFilmes):leDadosFilmes(&TodosFilmes);
    quantidadeFuncionarios = (modoArm)?leDadosFuncionariosBin(&funcionarios):leDadosFuncionarios(&funcionarios);
    quantidadeCategorias = (modoArm)?leDadosCategoriaBin(&categorias):leDadosCategoria(&categorias);
    
    printf("Digite o codigo ou nome do Funcionario:\n");
    vendedor = buscaFuncionarios(funcionarios, quantidadeFuncionarios, NULL);
    
    printf("digite o codigo, nome ou cpf do cliente:\n");
    comprador = buscaCliente(clientes, quantidadeClientes, "Nao existe nenhum cliente com o campo digitado registrado, por favor digite novamente:\n");
    
    alocaFilmes(&filmesAlocados, &quantidadeFilmesAlocados, TodosFilmes, quantidadeTotalFilmes);
    
    calculaPreco(filmesAlocados, quantidadeFilmesAlocados, categorias, quantidadeCategorias);
    
}