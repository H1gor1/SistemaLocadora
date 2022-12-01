#ifndef MENUS_C
#define MENUC_C
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
#include"ImportacoesXML.h"
#include "exportacoesXML.h"
#define ESC 27
#define MENUPRINC  menuGraphics(8, "Menu principal, escolha uma opcao:", "Administrativo", "Clientes", "Filmes", "Categorias", "Trocar modo arm BIN/TXT", "Caixa", "Contas", "Sair")


sons configuracoesSons;

void disparaSom(char *frase, int erro){
    if(configuracoesSons.modoSilencioso){
        printf("%s\n", frase);
        return;
    }
    if(erro) {
        printf("%s\n", frase);
        Beep(configuracoesSons.sonsDeErro.frequencia, configuracoesSons.sonsDeErro.duracao);
    }else{
        Beep(configuracoesSons.sonsDeConfirmacao.frequencia, configuracoesSons.sonsDeConfirmacao.duracao);
    }
}

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
void menuGraphicsComSeta(int quantidadeArgumentos, char *frasePrincipal , int linha, int linhaAnterior, int chamadas, char **frases, int espacamentoMaior){

    COORD cordenadaIn;

    if(!chamadas){
        cordenadaIn.X = 0;
        cordenadaIn.Y = 0;

        SetConsoleCursorPosition(GetStdHandle( STD_OUTPUT_HANDLE ), cordenadaIn);

        printf("\n%55s \n", "\0");

        imprimeEspacamentoMaior(75+espacamentoMaior, ' ', '\n');


        printf("%55s               %-50s\n", "\0", frasePrincipal);
        imprimeEspacamentoMaior(10+espacamentoMaior, ' ', '\n');
        printf("%55s \n", "\0", "\0", frasePrincipal, "\0");
        imprimeEspacamentoMaior(75+espacamentoMaior, ' ', '\n');
        printf("%55s  -->    %-40s \n", "\0", frases[0]);
        for(int i = 1; i<quantidadeArgumentos; i++){

            printf("%55s        %-40s \n", "\0", frases[i]);

        }
        return;
    }
    cordenadaIn.X = 56;
    cordenadaIn.Y = 7+linhaAnterior;
    SetConsoleCursorPosition(GetStdHandle( STD_OUTPUT_HANDLE ), cordenadaIn);
    printf("       %s   ", frases[linhaAnterior]);
    cordenadaIn.X = 56;
    cordenadaIn.Y = 7+linha;
    SetConsoleCursorPosition(GetStdHandle( STD_OUTPUT_HANDLE ), cordenadaIn);
    printf(" -->    %s", frases[linha]);
    SetConsoleCursorPosition(GetStdHandle( STD_OUTPUT_HANDLE ), (COORD){0,0});
    return;
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
    int chamadas = 0;
    int linhaAnterior = 0;
    int contador = 0;
    int escolha = 0;
    system("clear");
    while(escolha != 13){
        menuGraphicsComSeta(quantidade, mensagem, contador, linhaAnterior, chamadas, frases, espacamentoMaior);

        do {

            escolha = escolheOpcao();
        }while(escolha != 80 && escolha != 72 && escolha != 13);

        linhaAnterior = contador;
        chamadas++;
        switch(escolha){
            case 80:

                if(contador <quantidade-1){
                    contador++;
                }else{
                    contador = 0;
                }

                break;

            case 72:
                if(contador>0){
                    contador--;
                }else{
                    contador = quantidade-1;
                }

                break;

            case 13:
                frases = limpaMemoria(frases);
                system("clear");
                va_end(parametros);
                disparaSom("\0", 0);
                return contador;

        }
    }


}
int menuPrincipal(int *modo){

    int escolha;
    printf("\n");
    escolha = escolheMenu("Seja bem vindo! Escolha uma opcao", 11, 0,"Administrativo", "Clientes", "Filmes", "Categorias", "Configuracoes", "Caixa", "Contas a receber","Contas a Pagar","importar dados em um XML" , "exportar dados de um XML", "Sair");
    while(escolha != 10){

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

                menuConfiguracoes(modo);
                
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
                menuImportacoes(*modo);
                break;
            case 9:
                menuExportacoes(*modo);
                break;


            default:
                printf("Opcao invalida!\n");
        }
        if(escolha != 10){
            escolha = escolha = escolheMenu("Menu principal. Escolha uma opcao", 11, 0,"Administrativo", "Clientes", "Filmes", "Categorias", "Configuracoes", "Caixa", "Contas a receber","Contas a Pagar","importar dados em um XML" , "exportar dados de um XML", "Sair");
        }
    }
}
/*correcao feita em sala de aula apos a apresentacao do trabalho, pelo motivo de nao ser possivel o usuario saber o modo atual de armazenamento*/
void menuTroca(int *modo){
    int escolha;
    if(*modo){

        escolha = escolheMenu("Atualmente estamos trabalhando com binario.", 2,0,"Trocar para texto", VLT);
    }else{
        escolha = escolheMenu("Atualmente estamos trabalhando com texto.", 2,0,"Trocar para binario", VLT);
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
    
    while(escolha !=6){
        
        escolha = escolheMenu("Menu do caixa", 7, 0,"Realizar venda", "Devolver filme", "Contar caixa", "Filtrar movimentacoes por periodo", "Filtra compras", "Adicionar valor ao caixa", "Voltar");
        
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
                filtraMovimentacoesPorPeriodoData(modoArm);
                break;
            case 4:
                listaCompra(modoArm);
                break;
            case 5:
                lancaValorCaixa(modoArm);
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

    }
}
void menuConfiguracoes(int *modoArm){

    int escolha;

    while(escolha != 2) {
        escolha = escolheMenu("Menu configuracoes:", 3, 0, "Configuracoes de sons", "Trocar modo de armazenamento", "Voltar");

        switch(escolha){

            case 0:
                editaDadosSons(*modoArm);
                break;
            case 1:
                menuTroca(modoArm);
                break;
            case 2:
                break;
        }
    }
}
void menuImportacoes(int modoArm){
    int escolha;
    void (*f[12])(int) = {importaDadosLocadora, importaDadosFuncs, importaDadosFornecedores, importaCategoria,
                          importaDadosClientes, importaDadosFilmes, importaDadosCompras, importaDadosLancamentos,
                          importaLancamentosAprazo, importaDadosSons, importaLancamentosEntrada, importaEntradaAprazo};

    while(escolha != 10) {
        escolha = escolheMenu("Escolha um arquivo para importar os dados:", 13, 0,
                              "locadora",
                              "funcionarios",
                              "fornecedores",
                              "categorias",
                              "clientes",
                              "filmes",
                              "devolucoes",
                              "lancamentos",
                              "lancamentosAprazo",
                              "sons",
                              "lancamentosEntrada",
                              "entradaAprazo",
                              "voltar");

        if(escolha == 12){
            return;
        }
        f[escolha](modoArm);

    }


}
void menuExportacoes(int modoArm){
    int escolha;
    void (*f[12])(int) = {exportaLocadora, exportaFuncionarios, exportaFornecedores, exportaCategoria, exportaClientes, exportaFilmes};

    while(escolha != 12){
        escolha = escolheMenu("Escolha o destino que deseja exportar dados de outra aplicacao:", 13, 0,
                              "locadora",
                              "funcionarios",
                              "fornecedores",
                              "categorias",
                              "clientes",
                              "filmes",
                              "devolucoes",
                              "lancamentos",
                              "lancamentosAprazo",
                              "sons",
                              "lancamentosEntrada",
                              "entradaAprazo",
                              "voltar");

        if(escolha == 12){
            return;
        }
        f[escolha](modoArm);
    }

}
//configuracoes de sons dos menus

int selecionaValorDeSom(char *stringConf, int valorAt){

    int escolha = 0;
    printf("%s: |%-4d|\n", stringConf, valorAt);
    printf("para testar a configuracao atual, pressione F1:\n");
    while(escolha != 13){
        SetConsoleCursorPosition(GetStdHandle( STD_OUTPUT_HANDLE ), (COORD){strlen(stringConf)+2, 0});
        printf("|%-4d|\n", valorAt);
        do {

            escolha = escolheOpcao();
        }while(escolha != 80 && escolha != 72 && escolha != 13 && escolha != 59);
        switch(escolha){
            case 80:
                if(valorAt > 0){
                    valorAt--;
                }
                break;
            case 72:
                if(valorAt < 20000){
                    valorAt++;
                }
                break;
            case 59:
                if(strstr(stringConf, "Frequencia")){
                    Beep(valorAt, (strstr(stringConf, "confirmacao")?configuracoesSons.sonsDeConfirmacao.duracao:configuracoesSons.sonsDeErro.duracao));
                }else{
                    Beep((strstr(stringConf, "confirmacao"))?configuracoesSons.sonsDeConfirmacao.frequencia:configuracoesSons.sonsDeErro.frequencia, valorAt);
                }
                break;


            case 13:
                Beep(configuracoesSons.sonsDeConfirmacao.frequencia, configuracoesSons.sonsDeConfirmacao.duracao);
                return valorAt;
        }
    }

}
void leDadosSons(sons *ptr){
    FILE *f;

    f = fopen("sons.txt", "r");

    if(!f){
        ptr->sonsDeErro = (AlturaSons){0,0};
        ptr->sonsDeConfirmacao = (AlturaSons){0,0};
        reescreveDadosSons(ptr);
        return;
    }

    fscanf(f, "%d ", &ptr->sonsDeConfirmacao.frequencia);
    fscanf(f, "%d ", &ptr->sonsDeConfirmacao.duracao );
    fscanf(f, "%d ", &ptr->sonsDeErro.frequencia);
    fscanf(f, "%d ", &ptr->sonsDeErro.duracao);
    fscanf(f, "%d ", &ptr->modoSilencioso);



    fechaArquivo(&f);
}
void leDadosSonsBin(sons *ptr){
    FILE *f;

    f = fopen("sons.bin", "rb");

    if(!f){
        ptr->sonsDeConfirmacao = (AlturaSons){0,0};
        ptr->sonsDeErro = (AlturaSons){0,0};
        reescreveDadosSonsBin(ptr);
        return;
    }

    fread(ptr, sizeof(sons), 1, f);
    fechaArquivo(&f);
    return;
}
void reescreveDadosSons(sons *ptr){
    FILE *f;

    f = fopen("sons.txt", "w");

    if(!f){
        printf("Erro ao salvar novos dados de configuracoes de sons, ultimas alteracoes foram perdidas!\n");
        printf("ERRO: %s\n", strerror(errno));
        return;
    }

    fprintf(f, "%d\n", ptr->sonsDeConfirmacao.frequencia);
    fprintf(f, "%d\n", ptr->sonsDeConfirmacao.duracao);
    fprintf(f, "%d\n", ptr->sonsDeErro.frequencia);
    fprintf(f, "%d\n", ptr->sonsDeErro.duracao);
    fprintf(f, "%d\n", ptr->modoSilencioso);

    fechaArquivo(&f);
}
void reescreveDadosSonsBin(sons *ptr){
    FILE *f;

    f = fopen("sons.bin", "wb");

    if(!f){
        printf("Erro ao salvar novos dados de configuracoes de sons, ultimas alteracoes foram perdidas!\n");
        printf("ERRO: %s\n", strerror(errno));
        return;
    }

    fwrite(ptr, sizeof(sons), 1, f);

    fechaArquivo(&f);
}
void editaDadosSons(int modoArm){

    int escolha;


    escolha = escolheMenu("Qual som deseja editar:", 4, 0, "Sons de confirmacao", "Sons de erro", (configuracoesSons.modoSilencioso)?"Desativar modo silencioso":"Ativar modo silencioso","Voltar");

    switch(escolha){
        case 0:
            if(configuracoesSons.modoSilencioso){
                printf("primeiro desative o modo silencioso!\n");
                Sleep(2000);
                return;
            }
            escolha = escolheMenu("Deseja editar a frequencia ou duracao dos sons de confirmacao?", 3, 0, "Frequencia", "Duracao", "Voltar");
            switch (escolha) {
                case 0:
                    configuracoesSons.sonsDeConfirmacao.frequencia = selecionaValorDeSom("Frequencia de sons de confirmacao", configuracoesSons.sonsDeConfirmacao.frequencia);
                    break;

                case 1:
                    configuracoesSons.sonsDeConfirmacao.duracao = selecionaValorDeSom("Duracao de sons de confirmacao", configuracoesSons.sonsDeConfirmacao.duracao);
                    break;

                case 2:
                    return;


            }
            break;
        case 1:
            if(configuracoesSons.modoSilencioso){
                printf("primeiro desative o modo silencioso!\n");
                Sleep(2000);
                return;
            }
            escolha = escolheMenu("Deseja editar a frequencia ou duracao dos sons de erro?", 3, 0, "Frequencia", "Duracao", "Voltar");
            switch (escolha) {
                case 0:
                    configuracoesSons.sonsDeErro.frequencia = selecionaValorDeSom("Frequencia de sons de erro", configuracoesSons.sonsDeErro.frequencia);
                    break;

                case 1:
                    configuracoesSons.sonsDeErro.duracao = selecionaValorDeSom("Duracao de sons de erro", configuracoesSons.sonsDeErro.duracao);
                    break;

                case 2:
                    return;


            }
            break;
        case 2:
            configuracoesSons.modoSilencioso = !configuracoesSons.modoSilencioso;
            if(configuracoesSons.modoSilencioso) {


                printf("alterado para modo silencioso!\n");
                Sleep(1000);
                break;
            }else {

                printf("modo silencioso desativado!\n");
                disparaSom("0",0);
                Sleep(1000);
            }

            break;

    }

    (modoArm)? reescreveDadosSonsBin(&configuracoesSons): reescreveDadosSons(&configuracoesSons);
}
void leDadosDeSons(){
    FILE *f;

    f = fopen("sons.txt", "r");

    if(f){
        fechaArquivo(&f);
        leDadosSons(&configuracoesSons);
        return;
    }
    f = fopen("sons.bin", "rb");

    if(f){
        fechaArquivo(&f);
        leDadosSonsBin(&configuracoesSons);

        return;
    }
    configuracoesSons.sonsDeConfirmacao = (AlturaSons){0,0};
    configuracoesSons.sonsDeErro = (AlturaSons){0,0};
    reescreveDadosSons(&configuracoesSons);
}
#endif