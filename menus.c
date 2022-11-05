#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <windows.h>
#include "ManipulacoesDeTexto.h"
#include "ControleDeDadosLocadora.h"
#include "ControleDeNumeros.h"
#include "FuncUtilitarias.h"
#include "ControleClientes.h"
#include "controleFuncionarios.h"
#include "ControleFilmes.h"
#include "ControleCategorias.h"
#include "menus.h"
#include "Caixa.h"
#include "fornecedores.h"
#include "Carrinho.h"
#include "ContasAreceber.h"
#include "Devolucoes.h"
#define OPCAO "Escolha a opcao desejada:"
#define CAD "Cadastrar"
#define ED "Editar"
#define APG "Apagar"
#define VLT "Voltar"


void menuGraphics(int quantidadeArgumentos, char *frasePrincipal , ...){
    system("clear");
    char *string;
    va_list parametros;
    
    va_start(parametros, quantidadeArgumentos);
    
    printf("\n%50s ___________________________________________________________\n"
            "%50s|               %-35s         |\n"
            "%50s|                                                           |\n", "\0", "\0", frasePrincipal, "\0");
    for(int i = 0; i<quantidadeArgumentos; i++){
        
        string = va_arg(parametros, char *);
        
        printf("%50s|   %2d- %-25s                           |\n","\0", i+1, string);
        
    }
    printf("%50s|___________________________________________________________|\n", "\0");
    va_end(parametros);
    
}
int menuPrincipal(int *modo){
    menuGraphics(8, "Seja bem Vindo! Escolha uma opcao:", "Administrativo", "Clientes", "Filmes", "Categorias", "Trocar modo arm BIN/TXT", "Caixa", "Contas", "Sair");
    int escolha;
    printf("\n");
    
    while(escolha != 8){
        verificaNumero(&escolha, "%d");
        switch(escolha){
            case 1:
                system("clear");
                MenuAdm(*modo);
                break;
            case 2:
                system("clear");
                MenuClientes(*modo);
                break;
            case 3:
                system("clear");
                MenuFilmes(*modo);
                break;
            case 4:
                system("clear");
                MenuCategoria(*modo);
                break;
            case 5:
                
                menuTroca(modo);
                
                break;
            case 6:
                menuCaixa(*modo);
                menuGraphics(8, "Menu principal, escolha uma opcao:", "Administrativo", "Clientes", "Filmes", "Categorias", "Trocar modo arm BIN/TXT", "Caixa", "Contas", "Sair");
                break;
            case 7:
                menuContas(*modo);
                menuGraphics(8, "Menu principal, escolha uma opcao:", "Administrativo", "Clientes", "Filmes", "Categorias", "Trocar modo arm BIN/TXT", "Caixa", "Contas", "Sair");
                break;
            case 8:
                break;
            default:
                printf("Opcao invalida!\n");
        }
    }
}
/*correcao feita em sala de aula apos a apresentacao do trabalho, pelo motivo de nao ser possivel o usuario saber o modo atual de armazenamento*/
void menuTroca(int *modo){
    if(*modo){
        menuGraphics(2, "Atualmente estamos trabalhando com binario.\n", "trocar para texto", VLT);
    }else{
        menuGraphics(2, "Atualmente estamos trabalhando com texto.\n", "trocar para binario", VLT);
    }
    int escolha;
    
    while(1){
        verificaNumero(&escolha, "%d");
        switch(escolha){
            
            case 1:
                trocaModoArmazenamento(modo);
                printf("Modo de armazenamento trocado com sucesso!\n");
                Sleep(2000);
                break;
                        
            case 2:
                 
                 break;
                 
            default:
                printf("Voce digitou uma opcao invalida, por favor digite novamente:\n");
                continue;
        }
        break;
    }
    menuGraphics(9, "Menu Principal! Escolha uma opcao:", "Administrativo", "Clientes", "Filmes", "Categorias", "Trocar modo arm BIN/TXT", "Alugar um filme", "Contas","Devolver filme", "Sair");
}
int MenuAdm(int modo){
    menuGraphics(4, OPCAO, "Editar dados Loc.", "Funcionarios", "Fornecedores", VLT);    
    int escolha;
    while(escolha != 4){
        verificaNumero(&escolha, "%d");
        switch(escolha){
            case 1:
                system("clear");
                editaDadosLocadora(modo);
                menuGraphics(4, OPCAO, "Editar dados Loc.", "Funcionarios", "Fornecedores", VLT);    
                break;
            case 2:
                system("clear");
                MenuFuncionarios(modo);
                break;
            case 3:
                system("clear");
                menuFornecedores(modo);
                //Abrir menu fornecedores
                break;
            case 4:
                break;
            default:
                printf("Opcao invalida");
        }
    }
    menuGraphics(9, "Menu Principal! Escolha uma opcao:", "Administrativo", "Clientes", "Filmes", "Categorias", "Trocar modo arm BIN/TXT", "Alugar um filme", "Contas","Devolver filme", "Sair");
    
}

int MenuClientes(int modo){
    menuGraphics(4, OPCAO, CAD, ED, APG, VLT);    
    int escolha;
    while(escolha != 4){
        verificaNumero(&escolha, "%d");
        switch(escolha){
            case 1:
                system("clear");
                cadastraCliente(modo);
                menuGraphics(4, OPCAO, CAD, ED, APG, VLT);  
                break;
            case 2:
                system("clear");
                editaCliente(modo);
                menuGraphics(4, OPCAO, CAD, ED, APG, VLT);  
                break;
            case 3:
                system("clear");
                apagaCliente(modo);
                menuGraphics(4, OPCAO, CAD, ED, APG, VLT);  
                break;
            case 4:
                break;
            default:
                printf("Opcao invalida");
        }
    }
    menuGraphics(9, "Menu Principal! Escolha uma opcao:", "Administrativo", "Clientes", "Filmes", "Categorias", "Trocar modo arm BIN/TXT", "Alugar um filme", "Contas","Devolver filme", "Sair");
}

int MenuFuncionarios(int modo){
    menuGraphics(4, OPCAO, CAD, ED, APG, VLT);  
    int escolha;
    while(escolha != 4){
        verificaNumero(&escolha, "%d");
        switch(escolha){
            case 1:
                system("clear");
                cadastraFuncionario(modo);
                menuGraphics(4, OPCAO, CAD, ED, APG, VLT);  
                break;
            case 2:
                system("clear");
                editaFuncionario(modo);
                menuGraphics(4, OPCAO, CAD, ED, APG, VLT); 
                break;
            case 3:
                system("clear");
                apagaFuncionario(modo);
                menuGraphics(4, OPCAO, CAD, ED, APG, VLT); 
                break;
            case 4:
                break;
            default:
                printf("Opcao Invalida");
        }
    }
    menuGraphics(4, OPCAO, "Editar dados Loc.", "Funcionarios", "Fornecedores", VLT);
}

int MenuFilmes(int modo){
    menuGraphics(4, OPCAO, CAD, ED, APG, VLT);  
    int escolha;
    while(escolha != 4){
        verificaNumero(&escolha, "%d");
        switch(escolha){
            case 1:
                system("clear");
                cadastraFilmes(modo);
                menuGraphics(4, OPCAO, CAD, ED, APG, VLT);  
                break;
            case 2:
                system("clear");
                editaFilme(modo);
                menuGraphics(4, OPCAO, CAD, ED, APG, VLT);  
                break;
            case 3:
                system("clear");
                apagaFilme(modo);
                menuGraphics(4, OPCAO, CAD, ED, APG, VLT);  
                break;
            case 4:
                break;
            default:
                printf("Opcao invalida");
        }
    }
    menuGraphics(9, "Menu Principal! Escolha uma opcao:", "Administrativo", "Clientes", "Filmes", "Categorias", "Trocar modo arm BIN/TXT", "Alugar um filme", "Contas","Devolver filme", "Sair");
}

int MenuCategoria(int modo){
    menuGraphics(4, OPCAO, CAD, ED, APG, VLT);     
    int escolha;
    while(escolha != 4){
        verificaNumero(&escolha, "%d");
        switch(escolha){
            case 1:
                system("clear");
                cadastraCategoria(modo);
                menuGraphics(4, OPCAO, CAD, ED, APG, VLT);  
                break;
            case 2:
                system("clear");
                editaCategoria(modo);
                menuGraphics(4, OPCAO, CAD, ED, APG, VLT);  
                break;
            case 3:
                system("clear");
                apagaCategoria(modo);
                menuGraphics(4, OPCAO, CAD, ED, APG, VLT);  
                break;
            case 4:
                break;
            default:
                printf("Opcao invalida");
        }
    }
    menuGraphics(9, "Menu Principal! Escolha uma opcao:", "Administrativo", "Clientes", "Filmes", "Categorias", "Trocar modo arm BIN/TXT", "Alugar um filme", "Contas","Devolver filme", "Sair");
}
int menuFornecedores(int modo){
    menuGraphics(4, OPCAO, CAD, ED, APG, VLT); 
    int escolha;
    while(escolha != 4){
        verificaNumero(&escolha, "%d");
        switch(escolha){
            case 1:
                system("clear");
                cadastraFornecedor(modo);
                menuGraphics(4, OPCAO, CAD, ED, APG, VLT);  
                break;
            case 2:
                system("clear");
                editaFornecedor(modo);
                menuGraphics(4, OPCAO, CAD, ED, APG, VLT);  
                break;
            case 3:
                system("clear");
                apagaFornecedor(modo);
                menuGraphics(4, OPCAO, CAD, ED, APG, VLT);  
                break;
            case 4:
                break;
            default:
                printf("Opcao invalida");
        }

        
    }
    menuGraphics(4, OPCAO, "Editar dados Loc.", "Funcionarios", "Fornecedores", VLT);
}
void menuContas(int modoArm){
    
    menuGraphics(3, "Menu de contas", "Dar baixa", "Consultar Conta", "Sair");
    
    int escolha;
    while(escolha!=3){
        verificaNumero(&escolha, "%d");
        
        switch(escolha){
            
            case 1:
                daBaixa(modoArm);
                menuGraphics(3, "Menu de contas", "Dar baixa", "Consultar Conta", "Sair");
                continue;
                
            case 2:
                 consultaLancamentos(modoArm);
                 menuGraphics(3, "Menu de contas", "Dar baixa", "Consultar Conta", "Sair");
                //....
                continue;
                
            case 3:
                break;
        }
        break;
    }
    
}
void menuCaixa(int modoArm){
    menuGraphics(4, "Menu do caixa", "Realizar venda", "devolver filme", "Contar caixa","sair");
    int escolha;
    
    while(escolha !=4){
        
        verificaNumero(&escolha, "%d");
        
        switch(escolha){
            
            case 1:
                realizaVenda(modoArm);
                menuGraphics(4, "Menu do caixa", "Realizar venda", "devolver filme","contarCaixa", "sair");
                break;
                
            case 2:
                devolveFilme(modoArm);
                menuGraphics(4, "Menu do caixa", "Realizar venda", "devolver filme", "contarCaixa", "sair");
                break;
                
            case 3:
                contaCaixa(modoArm);
                menuGraphics(4, "Menu do caixa", "Realizar venda", "devolver filme", "contarCaixa", "sair");
                break;
                
            case 4:
                break;
            default:
                printf("escolha uma opcao valida!\n");
                break;
                
        }
    }
}