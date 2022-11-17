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
#include "ContasAPagar.h"
#include "EntradaDeFilmes.h"
#define ESC 27
#define MENUPRINC  menuGraphics(8, "Menu principal, escolha uma opcao:", "Administrativo", "Clientes", "Filmes", "Categorias", "Trocar modo arm BIN/TXT", "Caixa", "Contas", "Sair")
void imprimeEspacamentoMaior(int espacamento, char letra, char letraFim){
    for(int i = 0; i<espacamento; i++){
        printf("%c", letra);
    }
    if(letraFim != '\n'){
        printf("%c", letraFim);
        printf("\n");
    }else{
        printf("\n");
    }

}

void menuGraphicsComSeta(int quantidadeArgumentos, char *frasePrincipal , int linha, char **frases, int espacamentoMaior){
    COORD cordenadaIn;
    cordenadaIn.X = 0;
    cordenadaIn.Y = 0;

    SetConsoleCursorPosition(GetStdHandle( STD_OUTPUT_HANDLE ), cordenadaIn);
    printf("\n%55s ", "\0");

    imprimeEspacamentoMaior(75+espacamentoMaior, ' ', '\n');


    printf("%55s               %-50s", "\0", frasePrincipal);
    imprimeEspacamentoMaior(10+espacamentoMaior, ' ', '\n');
    printf("%55s ", "\0", "\0", frasePrincipal, "\0");
    imprimeEspacamentoMaior(75+espacamentoMaior, ' ', '\n');
    for(int i = 0; i<quantidadeArgumentos; i++){


        if(!strcmp(frases[i], "Sair")|| !strcmp(frases[i], "Voltar")){
            if(i == linha) {

                printf("%55s  -->   %-40s <--", "\0", frases[i]);
                imprimeEspacamentoMaior(24+espacamentoMaior, ' ', '\n');
            }else{
                printf("%55s    %-40s", "\0", frases[i]);
                imprimeEspacamentoMaior(32+espacamentoMaior, ' ', '\n');
            }
        }else {

            if( i == linha){
                printf("%55s  -->   %-40s <--", "\0", frases[i]);
                imprimeEspacamentoMaior(24+espacamentoMaior, ' ', '\n');

            }else{
                printf("%55s    %-40s", "\0", frases[i]);
                imprimeEspacamentoMaior(32+espacamentoMaior, ' ', '\n');
            }
        }
    }
    printf("%55s ", "\0");
    imprimeEspacamentoMaior(75+espacamentoMaior, ' ', '\n');


}


void menuGraphics(int quantidadeArgumentos, char *frasePrincipal , ...){
    system("clear");
    char *string;
    va_list parametros;

    va_start(parametros, quantidadeArgumentos);

    printf("\n%55s ___________________________________________________________\n"
           "%55s|               %-35s         |\n"
           "%55s|                                                           |\n", "\0", "\0", frasePrincipal, "\0");

    for(int i = 0; i<quantidadeArgumentos; i++){

        string = va_arg(parametros, char *);
        if(!strcmp(string, "Sair")|| !strcmp(string, "Voltar")){

            printf("%55s|   Esc- %-25s                           |\n","\0", string);
        }else {
            printf("%55s|   F%d- %-25s                           |\n", "\0", i + 1, string);



        }
    }

    printf("%55s|___________________________________________________________|\n", "\0");
    va_end(parametros);

}
int escolheMenu(char *mensagem, int quantidade,int espacamentoMaior, ...){

    va_list parametros;
    va_start(parametros, quantidade);
    char **frases;

    frases = malloc(sizeof(char*)*quantidade);

    for(int i = 0; i<quantidade; i++){
        frases[i] = va_arg(parametros, char *);
    }

    int contador = 0;
    int escolha = 0;
    system("clear");
    while(escolha != 13){
        menuGraphicsComSeta(quantidade, mensagem, contador, frases, espacamentoMaior);

        do {

            escolha = escolheOpcao();
        }while(escolha != 80 && escolha != 72 && escolha != 13);


        switch(escolha){
            case 80:
                if(contador <quantidade-1){
                    contador++;
                }else{
                    contador = 0;
                }
                Beep(2000,10);
                break;

            case 72:
                if(contador>0){
                    contador--;
                }else{
                    contador = quantidade-1;
                }
                Beep(3000,10);
                break;

            case 13:
                frases = limpaMemoria(frases);
                system("clear");
                va_end(parametros);
                Beep(10000,10);
                return contador;

        }
    }


}
int menuPrincipal(int *modo){

    int escolha;
    printf("\n");
    escolha = escolheMenu("Seja bem vindo! Escolha uma opcao", 9, 0,"Administrativo", "Clientes", "Filmes", "Categorias", "Trocar modo arm BIN/TXT", "Caixa", "Contas a receber","Contas a Pagar", "Sair");
    while(escolha != 8){

        switch(escolha){
            case 0:

                MenuAdm(*modo);
                break;
            case 1:

                MenuClientes(*modo);
                break;
            case 2:

                MenuFilmes(*modo);
                break;
            case 3:

                MenuCategoria(*modo);
                break;
            case 4:
                
                menuTroca(modo);
                
                break;
            case 5:
                menuCaixa(*modo);
                break;
            case 6:
                menuContas(*modo);
                break;
            case 7:
                menuContasPagar(*modo);
                break;
            case 8:
                break;


            default:
                printf("Opcao invalida!\n");
        }
        if(escolha != 8){
            escolha = escolheMenu("Menu principal. Escolha uma opcao", 9, 0,"Administrativo", "Clientes", "Filmes", "Categorias", "Trocar modo arm BIN/TXT", "Caixa", "Contas a receber", "Contas a pagar" ,"Sair");
        }
    }
}
/*correcao feita em sala de aula apos a apresentacao do trabalho, pelo motivo de nao ser possivel o usuario saber o modo atual de armazenamento*/
void menuTroca(int *modo){
    int escolha;
    if(*modo){

        escolha = escolheMenu("Atualmente estamos trabalhando com binario.\n", 2,0,"Trocar para texto", VLT);
    }else{
        escolha = escolheMenu("Atualmente estamos trabalhando com texto.\n", 2,0,"Trocar para binario", VLT);
    }

    


    switch(escolha){
            
        case 0:
            trocaModoArmazenamento(modo);
            printf("Modo de armazenamento trocado com sucesso!\n");
            Sleep(2000);
            break;

        case 1:
            break;

    }



}
int MenuAdm(int modo){

    int escolha;
    while(escolha != 3){
        escolha = escolheMenu(OPCAO, 4,0,"Editar dados Loc.", "Funcionarios", "Fornecedores", "Voltar");
        switch(escolha){
            case 0:

                editaDadosLocadora(modo);

                break;
            case 1:

                MenuFuncionarios(modo);
                break;
            case 2:

                menuFornecedores(modo);
                //Abrir menu fornecedores
                break;
            case 3:
                break;
            default:
                printf("Opcao invalida");
        }
    }

    
}

int MenuClientes(int modo){

    int escolha;
    while(escolha != 4){
        escolha = escolheMenu(OPCAO, 5, 0,CAD, ED, APG, "Filtrar clientes", VLT);
        switch(escolha){
            case 0:

                cadastraCliente(modo);
                break;
            case 1:

                editaCliente(modo);
                break;
            case 2:

                apagaCliente(modo);
                break;
            case 3:
                listaClientes(modo);
                break;
            case 4:
                break;
            default:
                printf("Opcao invalida");
        }

    }

}

int MenuFuncionarios(int modo){

    int escolha;
    while(escolha != 3){
        escolha = escolheMenu(OPCAO, 4, 0,CAD, ED, APG, VLT);
        switch(escolha){
            case 0:

                cadastraFuncionario(modo);

                break;
            case 1:

                editaFuncionario(modo);


                break;
            case 2:

                apagaFuncionario(modo);

                break;
            case 3:
                break;
            default:
                printf("Opcao Invalida");
        }
    }

}

int MenuFilmes(int modo){

    int escolha;
    while(escolha != 5){
        escolha = escolheMenu(OPCAO, 6, 0,CAD, ED, APG, "Filtrar filme", "Realizar entrada", VLT);
        switch(escolha){
            case 0:

                cadastraFilmes(modo);

                break;
            case 1:

                editaFilme(modo);

                break;
            case 2:

                apagaFilme(modo);

                break;
            case 3:
                listaFilme(modo);
                break;
            case 4:
                realizaEntrada(modo);
                break;
            case 5:
                break;
            default:
                printf("Opcao invalida");
        }
    }

}

int MenuCategoria(int modo){

    int escolha;
    while(escolha != 3){
        escolha = escolheMenu(OPCAO, 4, 0,CAD, ED, APG, VLT);
        switch(escolha){
            case 0:

                cadastraCategoria(modo);
                break;
            case 1:

                editaCategoria(modo);
                break;
            case 2:

                apagaCategoria(modo);
                break;
            case 3:
                break;
            default:
                printf("Opcao invalida");
        }
    }

}
int menuFornecedores(int modo){

    int escolha;
    while(escolha != 3){
        escolha = escolheMenu(OPCAO, 4, 0,CAD, ED, APG, VLT);
        switch(escolha){
            case 0:

                cadastraFornecedor(modo);

                break;
            case 1:

                editaFornecedor(modo);

                break;
            case 2:

                apagaFornecedor(modo);

                break;
            case 3:
                break;
            default:
                printf("Opcao invalida");
        }

        
    }

}
void menuContas(int modoArm){
    

    
    int escolha;
    while(escolha!=2){
        escolha = escolheMenu("Menu de contas", 3, 0,"Dar baixa", "Consultar conta", "Sair");
        
        switch(escolha){
            
            case 0:
                daBaixa(modoArm);

                continue;
                
            case 1:
                 consultaLancamentos(modoArm);

                //....
                continue;
                
            case 2:
                break;

        }
        break;
    }
    
}
void menuCaixa(int modoArm){

    int escolha;
    
    while(escolha !=4){
        
        escolha = escolheMenu("Menu do caixa", 5, 0,"Realizar venda", "Devolver filme", "Contar caixa", "Filtra compras" , "Voltar");
        
        switch(escolha){
            
            case 0:
                realizaVenda(modoArm);
                break;
                
            case 1:
                devolveFilme(modoArm);
                break;
                
            case 2:
                contaCaixa(modoArm);
                break;
                
            case 3:
                listaCompra(modoArm);
                break;
            case 4:
                break;
            default:
                printf("escolha uma opcao valida!\n");
                break;
                
        }

    }
}
void menuContasPagar(int modoArm){
    int escolha;
    while(escolha!=2){
        escolha = escolheMenu("Menu de contas a pagar", 3, 0,"Dar baixa", "Consultar conta", "Sair");

        switch(escolha){

            case 0:
                BaixaEntradasFilmes(modoArm);
                continue;

            case 1:
                consultaLancamentosEntrada(modoArm);
                continue;

            case 2:
                break;

        }
        break;
    }
}
