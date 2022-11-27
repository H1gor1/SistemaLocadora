//
// Created by vitor on 27/11/2022.
//

#include "exportacoesXML.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <conio.h>
#include<errno.h>
#include "ControleCategorias.h"
#include "ManipulacoesDeTexto.h"
#include "ControleDeDadosLocadora.h"
#include "ControleDeNumeros.h"
#include "FuncUtilitarias.h"
#include "Devolucoes.h"
#include "menus.h"
#include "ControleFilmes.h"
#include "controleFuncionarios.h"
#include "ControleClientes.h"
#include "Caixa.h"
#include <ctype.h>
#include <locale.h>
#include <windows.h>
#include "ContasAPagar.h"
#include "EntradaDeFilmes.h"
#include "ContasAreceber.h"
#include "Carrinho.h"
#include "ImportacoesXML.h"


void avancaAtePalavra(char *palavra, FILE *f){
    int indice = 0;
    char letra;
    while(palavra[indice] != '\0'){
        letra = fgetc(f);
        if(feof(f)){
            return;
        }
        indice = (letra == palavra[indice])?indice+1:0;

    }

}
FILE *digitaCaminhoExport(char *discricaoCaminhoo, char **diretorio){
    FILE *f;
    pegaCaminho(discricaoCaminhoo, diretorio);

    f = fopen(diretorio[0], "r");

    while(!f){
        f = fopen(diretorio[0], "r");
        if(f){
            break;
        }
        printf("nao foi encontrado o arquivo em: %s\nDigite novamente: ", diretorio);
        diretorio[0] = limpaMemoria(diretorio[0]);
        digText(diretorio, stdin, '\n');
    }
    diretorio[0] = limpaMemoria(diretorio[0]);
    return f;
}


void exportaLocadora(int modoArm){

    locadora export;
    char *diretorio;
    FILE *f;

    f = digitaCaminhoExport("exportar os dados para o arquivo locadora", &diretorio);

    avancaAtePalavra("<nomeFantasia>", f);
    digText(&export.nomeFantasia, f, '<');

    avancaAtePalavra("<razaoSocial>", f);
    digText(&export.razaoSocial, f, '<');

    avancaAtePalavra("<inscricaoEst>", f);
    digText(&export.inscricaoEstadual, f, '<');

    avancaAtePalavra("<CNPJ>", f);
    digText(&export.cnpj, f, '<');

    avancaAtePalavra("<endereco>", f);
    digText(&export.endereco, f, '<');

    avancaAtePalavra("<telefone>", f);
    fscanf(f, "%d", &export.telefone);

    avancaAtePalavra("<Email>", f);
    digText(&export.email, f, '<');

    avancaAtePalavra("<NomeResponsavel>", f);
    digText(&export.nomeResponsavel, f, '<');

    avancaAtePalavra("<telefoneResp>", f);
    fscanf(f, "%d", &export.telefoneResp);

    avancaAtePalavra("<Multa>", f);
    fscanf(f, "%f", &export.multa);

    fechaArquivo(&f);

    if(modoArm){
        f = fopen("locadora.bin", "wb");

    }else{
        f = fopen("locadora.txt", "w");
    }

    if(!f){
        printf("nao foi possivel atualizar os dados da locadora com os arquivos exportados!\n");
        printf("ERRO: %s\n", strerror(errno));
        Sleep(2000);
        limpaCamposLocadoraMemoria(&export);
        return;

    }
    (modoArm)? reescreveDadosLocadoraBin(&export, f): reescreveDadosLocadora(&export, f);
    fechaArquivo(&f);
    limpaCamposLocadoraMemoria(&export);



}
void exportaConfiguracoesSons(int modoArm){
    sons configuracoesSons;

    char *diretorio;
    FILE *f;
    f = digitaCaminhoExport("exportar arquivo de configuracoes de sons: ", &diretorio);

    avancaAtePalavra("<Frequencia>", f);
    fscanf(f, "%d", &configuracoesSons.sonsDeErro.frequencia);

    avancaAtePalavra("<Duracao>", f);
    fscanf(f, "%d", &configuracoesSons.sonsDeErro.duracao);

    avancaAtePalavra("<Frequencia>", f);
    fscanf(f, "%d", &configuracoesSons.sonsDeConfirmacao.frequencia);

    avancaAtePalavra("<Duracao>", f);
    fscanf(f, "%d", &configuracoesSons.sonsDeConfirmacao.duracao);

    avancaAtePalavra("<ModoSilencioso>", f);
    fscanf(f, "%d", &configuracoesSons.modoSilencioso);

    fechaArquivo(&f);

    (modoArm)? reescreveDadosSonsBin(&configuracoesSons): reescreveDadosSons(&configuracoesSons);

}
void exportaFuncionarios(int modoArm){

    Funcionarios *todosFuncs = NULL;
    int quantidadeFuncs = 0;

    char *diretorio;
    FILE *f;

    f = digitaCaminhoExport("exportar dados para o arquivo de funcionarios:", &diretorio);

    while(!feof(f)){
        todosFuncs = (!quantidadeFuncs)? malloc(sizeof(Funcionarios)):realloc(todosFuncs, sizeof(Funcionarios)*(quantidadeFuncs+1));
        verificaOperacao(todosFuncs, "ERRO: Memoria indisponivel!\n", 1);

        avancaAtePalavra("<codigo>", f);
        fscanf(f, "%ld", &todosFuncs[quantidadeFuncs].codigo);

        avancaAtePalavra("<nome>", f);
        digText(&todosFuncs[quantidadeFuncs].nome, f, '<');

        avancaAtePalavra("<cargo>", f);
        digText(&todosFuncs[quantidadeFuncs].cargo, f, '<');

        avancaAtePalavra("<rua>", f);
        digText(&todosFuncs[quantidadeFuncs].rua, f, '<');

        avancaAtePalavra("<bairro>", f);
        digText(&todosFuncs[quantidadeFuncs].bairro, f, '<');

        avancaAtePalavra("<numero>", f);
        fscanf(f, "%d", &todosFuncs[quantidadeFuncs].numero);

        avancaAtePalavra("<telefone>", f);
        fscanf(f, "%d", &todosFuncs[quantidadeFuncs].telefone);

        avancaAtePalavra("<email>", f);
        digText(&todosFuncs[quantidadeFuncs].email, f, '<');

        avancaAtePalavra("<flagExlusao>", f);
        fscanf(f, "%d", &todosFuncs[quantidadeFuncs].flag);

        avancaAtePalavra("<Funcionario>", f);
        quantidadeFuncs++;
    }
    fechaArquivo(&f);
    (modoArm)
        ? reescreveDadosFuncionariosBin(todosFuncs, quantidadeFuncs, "wb", "FuncionariosRes.bin", "Funcionarios.bin")
        : reescreveDadosFuncionarios(todosFuncs, quantidadeFuncs, "w", "FuncionariosRes.txt", "Funcionarios.txt");
    apagaDadosStructFuncionarios(todosFuncs, quantidadeFuncs);
    todosFuncs = limpaMemoria(todosFuncs);
}