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
#include "Carrinho.h"
#define PEDIRCAMINHO(CAMINHO) printf("Digite o diretorio/nomeDoArquivo que deseja guardar a importacao dos dados %s: \n", CAMINHO)

void pegaCaminho(char *discricaoCaminho, char **ptrGuardarCaminho){
    PEDIRCAMINHO(discricaoCaminho);

    digText(ptrGuardarCaminho, stdin);

    ptrGuardarCaminho[0] = realloc(ptrGuardarCaminho[0], sizeof(char)*(strlen(ptrGuardarCaminho[0])+5));
    strcat(ptrGuardarCaminho[0], ".XML");

}
int leArquivoOriginal(char *nomeArq, int (*f)(void **), void *ptr){


    int quantidade = f(ptr);

    if(!quantidade){
        printf("nao existem cadastros em %s!\n", nomeArq);
        return 0;
    }

    return quantidade;



};

void importaDadosLocadora(int modoArm){
    locadora importar;
    FILE *f;

    f = (modoArm)?fopen("locadora.bin", "rb"):fopen("locadora.txt", "r");

    if(!f){
        printf("Nao existem dados cadastrados da locadora para serem importados!\n");
        Sleep(2000);
        return;
    }

    (modoArm)? leDadosLocadoraBin(&importar, "locadora.bin"): leDadosLocadora(&importar, "locadora.txt");



    char *diretorio;

    pegaCaminho("da locadora", &diretorio);
    f = fopen(diretorio, "w");

    verificaOperacao(f, "Erro ao criar arquivo de importacao XML", 0);

    fprintf(f,"<dados>\n");
    fprintf(f, "   <NomeFantasia>%s</NomeFantasia>\n", importar.nomeFantasia);
    fprintf(f, "    <RazaoSocial>%s</RazaoSocial>\n", importar.razaoSocial);
    fprintf(f, "    <InscricaoEstadual>%s</InscricaoEstadual>\n", importar.inscricaoEstadual);
    fprintf(f, "    <CNPJ>%s</CNPJ>\n", importar.cnpj);
    fprintf(f, "    <Endereco>%s</Endereco>\n", importar.endereco);
    fprintf(f, "    <Telefone>%d</Telefone>\n", importar.telefone);
    fprintf(f, "    <Email>%s</Email>\n", importar.email);
    fprintf(f, "    <NomeResponsavel>%s</NomeResponsavel>\n", importar.nomeResponsavel);
    fprintf(f, "    <TelefoneResp>%d</TelefoneResp>\n", importar.telefoneResp);
    fprintf(f, "    <Multa>%.2f</Multa>\n", importar.multa);
    fprintf(f, "</dados>");

    fechaArquivo(&f);

}
void importaDadosSons(int modoArm){

    FILE *f;
    char *diretorio;
    sons importacoesSons;
    f = (modoArm)?fopen("sons.bin", "rb"):fopen("sons.txt", "r");

    if(!f){
        printf("nao existem cadastros no arquivo de sons!\n");
        return;
    }

    pegaCaminho("do arquivo de sons", &diretorio);


    (modoArm)? leDadosSonsBin(&importacoesSons): leDadosSons(&importacoesSons);


    f = fopen(diretorio, "w");


    fprintf(f,"<configuracoes>\n");
    fprintf(f, "    <sonsDeErro>\n" );
    fprintf(f, "        <Frequencia>%d</Frequencia>\n", importacoesSons.sonsDeErro.frequencia);
    fprintf(f, "        <Duracao>%d</Duracao>\n", importacoesSons.sonsDeErro.duracao);
    fprintf(f, "    </sonsDeErro>\n");
    fprintf(f, "    <sonsDeConfirmacao>\n");
    fprintf(f, "        <Frequencia>%d</Frequencia>\n", importacoesSons.sonsDeConfirmacao.frequencia);
    fprintf(f, "        <Duracao>%d</Duracao>\n", importacoesSons.sonsDeConfirmacao.duracao);
    fprintf(f, "    </sonsDeConfirmacao>\n");
    fprintf(f, "    <modoSilencioso>%d</modoSilencioso>\n", importacoesSons.modoSilencioso);
    fprintf(f, "</configuracaos>\n");


}

void importaDadosClientes(int modoArm){
    cliente *todosClientes;
    int quantidadeClientes = 0;

    quantidadeClientes = leArquivoOriginal("clientes.txt", (modoArm)?(int (*)(void **))leDadosClientesBin:(int (*)(void **))leDadosClientes, &todosClientes);

    if(!quantidadeClientes){
        printf("Nao existem clientes cadastrados no momento!\n");
        Sleep(2000);
        return;
    }

    FILE *f;
    char *diretorio;

    pegaCaminho("arquivo clientes", &diretorio);
    f = fopen(diretorio, "w");

    if(!f){
        printf("nao foi possivel criar o arquivo em %s\n", diretorio);
        printf("ERRO: %s\n", strerror(errno));
        return;
    }

    fprintf(f, "<dados>\n");
    for(int i = 0; i<quantidadeClientes; i++){

        fprintf(f, "    <cliente>\n");
        fprintf(f, "        <codigo>%d</codigo>\n", todosClientes[i].codigo);
        fprintf(f, "        <nome>%s</nome>\n", todosClientes[i].nomeCompleto);
        fprintf(f, "        <rua>%s</rua>\n", todosClientes[i].rua);
        fprintf(f, "        <bairro>%s</bairro>\n", todosClientes[i].bairro);
        fprintf(f, "        <numero>%d</numero>\n", todosClientes[i].numeroDaCasa);
        fprintf(f, "        <cpf>%s</cpf>\n", todosClientes[i].cpf);
        fprintf(f, "        <telefone>%d</telefone>\n", todosClientes[i].telefone);
        fprintf(f, "        <email>%s</email>\n", todosClientes[i].email);
        fprintf(f, "        <sexo>%d</sexo>\n", todosClientes[i].sexo);
        fprintf(f, "        <estadoCivil>%d</estadoCivil>\n", todosClientes[i].estadoCivil);
        fprintf(f, "        <diaNascimento>%d</diaNascimento>\n", todosClientes[i].diaNascimento);
        fprintf(f, "        <mes>%d</mes>\n", todosClientes[i].mes);
        fprintf(f, "        <ano>%d</ano>\n", todosClientes[i].ano);
        fprintf(f, "        <flagExclusao>%d</flagExclusao>\n", todosClientes[i].flag);
        fprintf(f, "    </cliente>\n");

    }
    fprintf(f, "</dados>");

    fechaArquivo(&f);


    limpaDadosClienteMemoria(todosClientes, quantidadeClientes);
    todosClientes = limpaMemoria(todosClientes);

}
void importaDadosFuncs(int modoArm){

    Funcionarios *todosFuncs = NULL;
    int quantidadeFuncionarios;

    quantidadeFuncionarios = leArquivoOriginal(
            (modoArm)?"Funcionarios.bin":"Funcionarios.txt",
            (modoArm)?(int (*)(void **))leDadosFuncionariosBin:(int (*)(void **))leDadosFuncionarios, &todosFuncs);

    if(!quantidadeFuncionarios){
        printf("Nao existe funcionarios cadastros no momento!\n");
        Sleep(2000);
        return;
    }

    FILE *f;
    char *diretorio;

    pegaCaminho("de funcionarios", &diretorio);

    f = fopen(diretorio, "w");

    if(!f){
        printf("Nao foi possivel criar o arquivo de importacoes no local indicado!\n");
        printf("ERRO: %s\n", strerror(errno));
        Sleep(2000);
        return;
    }

    fprintf(f, "<dados>\n");

    for(int i = 0; i<quantidadeFuncionarios; i++){

        fprintf(f, "    <Funcionario>\n");
        fprintf(f, "        <codigo>%ld</codigo>\n", todosFuncs[i].codigo);
        fprintf(f, "        <nome>%s</nome>\n", todosFuncs[i].nome);
        fprintf(f, "        <rua>%s</rua>\n", todosFuncs[i].rua);
        fprintf(f, "        <bairro>%s</bairro>\n", todosFuncs[i].bairro);
        fprintf(f, "        <numero>%d</numero>\n", todosFuncs[i].numero);
        fprintf(f, "        <telefone>%d</telefone>\n", todosFuncs[i].telefone);
        fprintf(f, "        <email>%s</email>\n", todosFuncs[i].email);
        fprintf(f, "        <tagExclusao>%d</tagExclusao>\n", todosFuncs[i].flag);
        fprintf(f, "    </Funcionario>\n");
    }
    fprintf(f, "</dados>");


    apagaDadosStructFuncionarios(todosFuncs, quantidadeFuncionarios);
    todosFuncs = limpaMemoria(todosFuncs);
    fechaArquivo(&f);
}
