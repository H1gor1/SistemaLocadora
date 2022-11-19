#ifndef SONS_C
#define SONS_C
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include "ControleFilmes.h"
#include "ManipulacoesDeTexto.h"
#include "ControleDeDadosLocadora.h"
#include "ControleDeNumeros.h"
#include "FuncUtilitarias.h"
#include "menus.h"
#include <ctype.h>
#include <locale.h>
#include "Devolucoes.h"
#include "ControleCategorias.h"
#include <windows.h>
#include "ControleClientes.h"
#include <conio.h>
#include "sons.h"




int selecionaValorDeSom(char *stringConf, int valorAt){

    int escolha = 0;
    printf("%s: | %d |\n", stringConf, valorAt);
    while(escolha != 13){
        SetConsoleCursorPosition(GetStdHandle( STD_OUTPUT_HANDLE ), (COORD){strlen(stringConf)+3, valorAt});
        printf("%s: | %d |\n", stringConf, valorAt);
        do {

            escolha = escolheOpcao();
        }while(escolha != 80 && escolha != 72 && escolha != 13);
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

            case 13:
                Beep(ConfiguracoeDeSons.SonsDeConfirmacao.frequencia, ConfiguracoeDeSons.SonsDeConfirmacao.duracao);
                return valorAt;
        }
    }

}
void leDadosSons(sons *ptr){
    FILE *f = NULL;
    f = fopen("sons.txt", "r");

    if(!f){

        *ptr = (sons){0,0,0,0};
        reescreveDadosSons(ptr);
        return;
    }

    fscanf(f, "%d ", &ptr->SonsDeConfirmacao.duracao);
    fscanf(f, "%d ", &ptr->SonsDeConfirmacao.frequencia);
    fscanf(f, "%d ", &ptr->SonsDeErro.duracao);
    fscanf(f, "%d ", &ptr->SonsDeErro.frequencia);

    fechaArquivo(&f);
    return;
}
void leDadosSonsBin(sons *ptr){
    FILE *f = NULL;
    f = fopen("sons.bin", "rb");

    if(!f){
        *ptr = (sons){0,0,0,0};
        reescreveDadosSonsBin(ptr);
        return;
    }

    fread(ptr, sizeof(sons), 1, f);
    return;
}
void reescreveDadosSons(sons *ptr){
    FILE *f;
    f = fopen("sons.txt", "w");

    if(!f){
        printf("Erro ao salvar novos dados de efeitos sonoros!\nNovos dados foram perdidos!\n");
        printf("ERRO: %s\n", strerror(errno));
        return;
    }

    fprintf(f, "%d\n", ptr->SonsDeConfirmacao.duracao);
    fprintf(f, "%d\n", ptr->SonsDeConfirmacao.frequencia);
    fprintf(f, "%d\n", ptr->SonsDeErro.duracao);
    fprintf(f, "%d\n", ptr->SonsDeErro.frequencia);

    fechaArquivo(&f);
    return;
}
void reescreveDadosSonsBin(sons *ptr){
    FILE *f;
    f = fopen("sons.bin", "wb");

    if(!f){
        printf("Erro ao salvar novos dados de efeitos sonoros!\nNovos dados foram perdidos!\n");
        printf("ERRO: %s\n", strerror(errno));
        return;
    }

    fwrite(ptr, sizeof(sons), 1, f);
    fechaArquivo(&f);
    return;
}

void editaDadosSons(int modoArm){

    int escolha = escolheMenu("Escolha qual configuracao de som deseja editar:", 3, 0, "Sons de confirmacao", "Sons de erro", "Voltar");

    switch(escolha){
        case 0:
            escolha = escolheMenu("Escolha uma opcao para editar:", 3, 0, "Duracao dos sons de confirmacao", "Frequencia dos sons de confirmacao", "Voltar");

            switch(escolha){
                case 0:
                    ConfiguracoeDeSons.SonsDeConfirmacao.duracao = selecionaValorDeSom("Som de confirmacao", ConfiguracoeDeSons.SonsDeConfirmacao.duracao);
                    break;
                case 1:
                    ConfiguracoeDeSons.SonsDeConfirmacao.frequencia = selecionaValorDeSom("Frequencia do som de confirmacao", ConfiguracoeDeSons.SonsDeConfirmacao.frequencia);
                    break;
                case 2:
                    return;
            }
            break;
        case 1:
            escolha = escolheMenu("Escolha uma opcao para editar:", 3, 0, "Duracao dos sons de erro", "Frequencia dos sons de erro", "Voltar");

            switch(escolha){
                case 0:
                    ConfiguracoeDeSons.SonsDeErro.duracao = selecionaValorDeSom("Som de erro", ConfiguracoeDeSons.SonsDeErro.duracao);
                    break;
                case 1:
                    ConfiguracoeDeSons.SonsDeErro.frequencia = selecionaValorDeSom("Frequencia do som de erro", ConfiguracoeDeSons.SonsDeErro.frequencia);
                    break;
                case 2:
                    return;
            }
            break;
        case 2:
            return;


    }
    (modoArm)? reescreveDadosSonsBin(&ConfiguracoeDeSons): reescreveDadosSons(&ConfiguracoeDeSons);
    return;
}
#endif