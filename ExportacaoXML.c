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
#define PEDIRCAMINHO(CAMINHO) printf("Digite o diretorio/nomeDoArquivo que deseja %s: \n", CAMINHO)
#define EXPORTASTRING(STRING, STRING2, f) fprintf(f, "          <%s>%s</%s>\n", STRING, STRING2, STRING)
#define EXPORTANUMBER(STRING, NUMBER, f) fprintf(f, "          <%s>%d</%s>\n", STRING, NUMBER, STRING)
#define EXPORTAFLOAT(STRING, FLOAT, f) fprintf(f, "          <%s>%.2f</%s>\n", STRING, FLOAT, STRING)
#define EXPORTALD(STRING, LD, f) fprintf(f, "          <%s>%ld</%s>\n", STRING, LD, STRING)



void pegaCaminho(char *discricaoCaminho, char **ptrGuardarCaminho){
    PEDIRCAMINHO(discricaoCaminho);

    digText(ptrGuardarCaminho, stdin, '\n');

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

void exportaLocadora(int modoArm){
    locadora importar;
    FILE *f;

    f = (modoArm)?fopen("locadora.bin", "rb"):fopen("locadora.txt", "r");

    if(!f){
        printf("Nao existem dados cadastrados da locadora para serem exportadas!\n");
        Sleep(2000);
        return;
    }

    (modoArm)? leDadosLocadoraBin(&importar, "locadora.bin"): leDadosLocadora(&importar, "locadora.txt");



    char *diretorio;

    pegaCaminho("exportar os dados da locadora", &diretorio);
    f = fopen(diretorio, "w");

    verificaOperacao(f, "Erro ao criar arquivo de exportacoes.XML", 0);



    fprintf(f, "<dados>\n");
    fprintf(f, "    <locadora>\n");
    EXPORTASTRING("nomeFantasia", importar.nomeFantasia, f);
    EXPORTASTRING("razaoSocial", importar.razaoSocial, f);
    EXPORTASTRING("inscricaoEst", importar.inscricaoEstadual, f);
    EXPORTASTRING("CNPJ", importar.cnpj, f);
    EXPORTASTRING("endereco", importar.endereco, f);
    EXPORTANUMBER("telefone", importar.telefone, f);
    EXPORTASTRING("Email", importar.email, f);
    EXPORTASTRING("NomeResponsavel", importar.nomeResponsavel, f);
    EXPORTANUMBER("telefoneResp", importar.telefoneResp, f);
    EXPORTAFLOAT("Multa", importar.multa, f);
    fprintf(f, "    </locadora>\n");
    fprintf(f, "</dados>");


    fechaArquivo(&f);

}
void exportaDadosSons(int modoArm){

    FILE *f;
    char *diretorio;
    sons importacoesSons;
    f = (modoArm)?fopen("sons.bin", "rb"):fopen("sons.txt", "r");

    if(!f){
        printf("nao existem cadastros no arquivo de sons!\n");
        return;
    }

    pegaCaminho("exportar os dados do arquivo de sons", &diretorio);


    (modoArm)? leDadosSonsBin(&importacoesSons): leDadosSons(&importacoesSons);


    f = fopen(diretorio, "w");
    
    fprintf(f,"<configuracoes>\n");
    fprintf(f, "    <sonsDeErro>\n" );
    EXPORTANUMBER("Frequencia", importacoesSons.sonsDeErro.frequencia, f);
    EXPORTANUMBER("Duracao", importacoesSons.sonsDeErro.duracao, f);
    fprintf(f, "    </sonsDeErro>\n");
    fprintf(f, "    <sonsDeConfirmacao>\n");
    EXPORTANUMBER("Frequencia", importacoesSons.sonsDeConfirmacao.frequencia, f);
    EXPORTANUMBER("Duracao", importacoesSons.sonsDeConfirmacao.duracao, f);
    fprintf(f, "    </sonsDeConfirmacao>\n");
    EXPORTANUMBER("ModoSilencioso", importacoesSons.modoSilencioso, f);
    fprintf(f, "</configuracoes>\n");

    fechaArquivo(&f);


}

void exportaDadosClientes(int modoArm){
    cliente *todosClientes;
    int quantidadeClientes = 0;

    quantidadeClientes = leArquivoOriginal("clientes.txt", (modoArm)?(int (*)(void **))leDadosClientesBin:(int (*)(void **))leDadosClientes, &todosClientes);

    if(!quantidadeClientes){
        return;
    }

    FILE *f;
    char *diretorio;

    pegaCaminho("exportar os dados do arquivo clientes", &diretorio);
    f = fopen(diretorio, "w");


    if(!f){
        printf("nao foi possivel criar o arquivo em %s\n", diretorio);
        printf("ERRO: %s\n", strerror(errno));
        return;
    }

    fprintf(f, "<dados>\n");
    for(int i = 0; i<quantidadeClientes; i++){

        fprintf(f, "    <cliente>\n");
        EXPORTANUMBER("codigo", todosClientes[i].codigo, f);
        EXPORTASTRING("nome", todosClientes[i].nomeCompleto,f);
        EXPORTASTRING("rua", todosClientes[i].rua, f);
        EXPORTASTRING("bairro", todosClientes[i].bairro, f);
        EXPORTANUMBER("numero", todosClientes[i].numeroDaCasa, f);
        EXPORTASTRING("CPF", todosClientes[i].cpf, f);
        EXPORTANUMBER("telefone", todosClientes[i].telefone, f);
        EXPORTASTRING("email", todosClientes[i].email, f);
        EXPORTANUMBER("sexo", todosClientes[i].sexo, f);
        EXPORTANUMBER("EstadoCivil", todosClientes[i].estadoCivil, f);
        EXPORTANUMBER("DiaNasc", todosClientes[i].diaNascimento, f);
        EXPORTANUMBER("mes", todosClientes[i].mes, f);
        EXPORTANUMBER("ano", todosClientes[i].ano, f);
        EXPORTANUMBER("flagExclusao", todosClientes[i].flag, f);
        fprintf(f, "    </cliente>\n");

    }
    fprintf(f, "</dados>");

    fechaArquivo(&f);


    limpaDadosClienteMemoria(todosClientes, quantidadeClientes);
    todosClientes = limpaMemoria(todosClientes);

}
void exportaDadosFuncs(int modoArm){

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

    pegaCaminho("exportar os dados do arquivo de funcionarios", &diretorio);

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
        EXPORTALD("codigo", todosFuncs[i].codigo, f);
        EXPORTASTRING("nome", todosFuncs[i].nome, f);
        EXPORTASTRING("cargo", todosFuncs[i].cargo, f);
        EXPORTASTRING("rua", todosFuncs[i].rua, f);
        EXPORTASTRING("bairro", todosFuncs[i].bairro, f);
        EXPORTANUMBER("numero", todosFuncs[i].numero, f);
        EXPORTANUMBER("telefone", todosFuncs[i].telefone, f);
        EXPORTASTRING("email", todosFuncs[i].email, f);
        EXPORTANUMBER("flagExlusao", todosFuncs[i].flag, f);

        fprintf(f, "    </Funcionario>\n");
    }
    fprintf(f, "</dados>");


    apagaDadosStructFuncionarios(todosFuncs, quantidadeFuncionarios);
    todosFuncs = limpaMemoria(todosFuncs);
    fechaArquivo(&f);
}

void exportaDadosFornecedores(int modoArm){
    fornecedor *forn = NULL;
    int quantidadeForn = 0;

    quantidadeForn = leArquivoOriginal((modoArm)?"fornecedores.bin":"fornecedores.txt", (modoArm)?(int (*)(void **))leDadosFornecedoresBin:(int (*)(void **)) leDadosFornecedores, &forn);

    if(!quantidadeForn){
        return;
    }
    char *diretorio;

    pegaCaminho("exportar os dados do arquivo fornecedores", &diretorio);

    FILE *f;

    f = fopen(diretorio, "w");

    if(!f){
        printf("nao foi possivel criar o arquivo %s\n", diretorio);
        printf("ERRO: %s\n", strerror(errno));
        Sleep(2000);
    }
    fprintf(f, "<dado>\n");
    for(int i = 0; i<quantidadeForn; i++){
        fprintf(f,"    <Fornecedor>\n");
        EXPORTANUMBER("codigo", forn[i].codigo, f);
        EXPORTASTRING("nomeFantasia", forn[i].nomeFantasia, f);
        EXPORTASTRING("razaoSocial", forn[i].razaoSocial, f);
        EXPORTASTRING("inscEstadual", forn[i].inscricaoEstadual, f);
        EXPORTASTRING("CNPJ", forn[i].CNPJ, f);
        EXPORTASTRING("bairro", forn[i].bairro, f);
        EXPORTASTRING("rua", forn[i].rua, f);
        EXPORTANUMBER("numero", forn[i].numero, f);
        EXPORTANUMBER("telefone", forn[i].telefone, f);
        EXPORTASTRING("email", forn[i].email, f);
        EXPORTANUMBER("flag", forn[i].flag, f);
        fprintf(f,"    </Fornecedor>\n");
    }
    fprintf(f, "</dado>");

    fechaArquivo(&f);
    limpaMemoriaStringsFornecedor(forn, quantidadeForn);
    forn = limpaMemoria(forn);
}
void exportaCategoria(int modoArm){
    categoria *todasCat = NULL;
    int quantidadeCat = 0;

    quantidadeCat = leArquivoOriginal((modoArm)?"categorias.bin":"categorias.txt", (modoArm)?(int(*)(void **))leDadosCategoriaBin:(int (*)(void **))leDadosCategoria, &todasCat);

    if(!quantidadeCat){
        return;
    }
    char *diretorio;
    FILE *f;

    pegaCaminho("exportar os dados do arquivo categoria", &diretorio);

    f = fopen(diretorio, "w");
    if(!f){
        printf("Nao foi possivel criar o arquivo de importacoes no local indicado!\n");
        printf("ERRO: %s\n", strerror(errno));
        Sleep(2000);
        limpaDadosCategoriaMemoria(todasCat, quantidadeCat);
        todasCat = limpaMemoria(todasCat);
        return;
    }
    fprintf(f, "<dado>\n");
    for(int i = 0; i<quantidadeCat; i++){

        fprintf(f, "   <categoria>\n");
        EXPORTANUMBER("codigo", todasCat[i].codigo, f);
        EXPORTASTRING("nome", todasCat[i].nome, f);
        EXPORTASTRING("descricao", todasCat[i].descricao, f);
        EXPORTAFLOAT("valorLoc", todasCat[i].valorLocacao, f);
        EXPORTANUMBER("flag", todasCat[i].flag, f);
        fprintf(f, "   </categoria>\n");

    }
    fprintf(f , "</dado>");

    fechaArquivo(&f);
    limpaDadosCategoriaMemoria(todasCat, quantidadeCat);
    todasCat = limpaMemoria(todasCat);



}
void exportaDadosFilmes(int modoArm){

    filmes *todosFilmes = NULL;
    int quantidadeFilmes = 0;

    quantidadeFilmes = leArquivoOriginal((modoArm)?"filmes.bin":"filmes.txt", (modoArm)?(int (*)(void **))leDadosFilmesBin:(int (*)(void **))leDadosFilmes, &todosFilmes);

    if(!quantidadeFilmes){
        return;
    }
    FILE *f;
    char *diretorio;

    pegaCaminho("exportar os dados do arquivo filmes", &diretorio);

    f = fopen(diretorio, "w");
    fprintf(f, "<dados>\n");

    for(int i = 0; i<quantidadeFilmes; i++){


        fprintf(f, "    <Filme>\n");
        EXPORTANUMBER("codigo", todosFilmes[i].codigo, f);
        EXPORTASTRING("nome", todosFilmes[i].nome, f);
        EXPORTASTRING("descricao", todosFilmes[i].descricao,f);
        EXPORTANUMBER("CodigoCat", todosFilmes[i].codigoCategoria, f);
        EXPORTAFLOAT("ValorLoc", todosFilmes[i].valorLocacao, f);
        EXPORTANUMBER("exemplares", todosFilmes[i].exemplares, f);
        EXPORTANUMBER("lingua", todosFilmes[i].lingua, f);
        EXPORTANUMBER("flag", todosFilmes[i].flag, f);
        fprintf(f, "    </Filme>\n");

    }
    fprintf(f, "</dados>");

    fechaArquivo(&f);
    limpaDadosFilmeMemoria(todosFilmes, quantidadeFilmes);
    todosFilmes = limpaMemoria(todosFilmes);

}
void exportaDadosCompras(int modoArm){

    compras *todasAsCompras = NULL;
    filmes *todosFilmes = NULL;
    Funcionarios  *todosFuncs = NULL;
    cliente *todosClientes = NULL;
    quantidades quant = {0,0,0,0};

    quant = (modoArm)?leDadosDevolucoesBin(&todasAsCompras, &todosFuncs, &todosClientes, &todosFilmes): leDadosDevolucoes(&todasAsCompras, &todosFuncs, &todosClientes, &todosFilmes);

    if(!quant.quantidadeAlugacoes){
        printf("nao existem compras feitas ate o momento!\n");
        limpaDadosDevolucoesMemoria(&todosFilmes, &todosClientes, &todosFuncs, &todasAsCompras, quant);
        return;
    }

    FILE *f;
    char *diretorio;

    pegaCaminho("exportar os dados do arquivo de compras", &diretorio);

    f = fopen(diretorio, "w");

    fprintf(f, "<dado>\n");

    for(int i = 0; i<quant.quantidadeAlugacoes; i++){

        fprintf(f, "    <compra>\n");
        EXPORTALD("codigoCompra", todasAsCompras[i].codigo, f);
        EXPORTANUMBER("modoPagamento", todasAsCompras[i].modoPagamento, f);
        EXPORTALD("data", mktime(&todasAsCompras[i].data), f);
        EXPORTALD("vendedor", todasAsCompras[i].vendedor->codigo, f);
        EXPORTANUMBER("comprador", todasAsCompras[i].comprador->codigo, f);
        EXPORTAFLOAT("preco", todasAsCompras[i].preco, f);
        EXPORTANUMBER("quantidadeFilmesComprados", todasAsCompras[i].quantidadeFilmesComprados, f);
        for(int j = 0; j<todasAsCompras[i].quantidadeFilmesComprados; j++){
            fprintf(f, "          <filme%d>\n", j+1);
            fprintf(f, "                <codigoFilme>%d</codigoFilme>\n", todasAsCompras[i].filmesComprados[j].codigo);
            fprintf(f, "                <exemplares>%d</exemplares>\n", todasAsCompras[i].filmesComprados[j].exemplares);
            fprintf(f, "          </filme%d>\n", j+1);
        }
        EXPORTANUMBER("devolvido", todasAsCompras[i].devolvido, f);
        fprintf(f, "    </compra>\n");
    }
    fprintf(f, "</dado>\n");

    fechaArquivo(&f);
    limpaDadosDevolucoesMemoria(&todosFilmes, &todosClientes, &todosFuncs, &todasAsCompras, quant);
}
void exportaDadosLancamentos(int modoArm){
    lancamentoCaixa *todosLancamentos = NULL;
    int quantidadeLancamentos = 0;

    quantidadeLancamentos = leArquivoOriginal((modoArm)?"lancamentos.bin":"lancamentos.txt",
                                              (modoArm)? (int (*)(void **))leDadosLancamentosBin: (int (*)(void **))leDadosLancamentos,
                                              &todosLancamentos);

    if(!quantidadeLancamentos){
        return;
    }

    FILE *f;
    char *diretorio;

    pegaCaminho("exportar os dados do arquivo lancamento caixa", &diretorio);

    f = fopen(diretorio, "w");

    if(!f){
        printf("Nao foi possivel criar o arquivo XML no diretorio digitado!\n");
        printf("ERRO: %s\n", strerror(errno));
        todosLancamentos = limpaMemoria(todosLancamentos);
        Sleep(2000);
        return;
    }

    fprintf(f, "<dados>\n");
    for(int i = 0; i<quantidadeLancamentos; i++){
        fprintf(f, "    <lancamento>\n");
        EXPORTALD("codigoCompra", todosLancamentos[i].codigoCompra, f);
        EXPORTANUMBER("modoPagamento", todosLancamentos[i].modoPagamento, f);
        EXPORTAFLOAT("valor", todosLancamentos[i].valor, f);
        EXPORTAFLOAT("valorPago", todosLancamentos[i].valorPago,f);
        EXPORTAFLOAT("troco", todosLancamentos[i].troco, f);
        EXPORTALD("data", mktime(&todosLancamentos[i].data), f);
        fprintf(f, "    </lancamento>\n");


    }
    fprintf(f, "</dados>\n");

    fechaArquivo(&f);
    todosLancamentos = limpaMemoria(todosLancamentos);


}
void exportaLancamentosAprazo(int modoArm){

    contaArec *todasContas = NULL;
    int quantidadeContas = 0;

    quantidadeContas = leArquivoOriginal((modoArm)?"lancamentosAprazo.bin":"lancamentosAprazo.txt",
                                         (modoArm)?(int (*)(void **))leDadosLancamentosAprazoBin: (int (*)(void **))leDadosLancamentosAprazo,
                                         &todasContas);

    if(!quantidadeContas){
        return;
    }

    char *diretorio;
    FILE *f;

    pegaCaminho("exportar os dados do arquivo de contas a receber", &diretorio);

    f = fopen(diretorio, "w");

    if(!f){
        printf("Nao foi possivel criar o arquivo XML no caminho indicado!\n");
        printf("ERRO: %s\n", strerror(errno));
        todasContas = limpaMemoria(todasContas);
        Sleep(2000);
        return;
    }

    fprintf(f, "<dados>\n");

    for(int i = 0; i<quantidadeContas; i++){
        fprintf(f, "    <conta>\n");
        EXPORTANUMBER("codigoCliente", todasContas[i].codigoCliente, f);
        EXPORTALD("codigoCompra", todasContas[i].codigoCompra, f);
        EXPORTANUMBER("modoPagamento", todasContas[i].modoPagamento, f);
        EXPORTANUMBER("parcelas", todasContas[i].parcelas, f);
        EXPORTAFLOAT("valorParcela", todasContas[i].valorParc, f);
        EXPORTAFLOAT("ValorEntrada", todasContas[i].entrada, f);
        EXPORTALD("data", mktime(&todasContas[i].dataAluga), f);
        fprintf(f, "    </conta>\n");
    }
    fprintf(f, "</dados>");

    fechaArquivo(&f);
    todasContas = limpaMemoria(todasContas);

}

void exportaLancamentosEntrada(int modoArm){

    lancamentoEntradas *todasEntradas;
    int quantidadeEntrada;

    quantidadeEntrada = leArquivoOriginal((modoArm)?"lancamentosEntrada.bin": "lancamentosEntrada.txt",
                                          (modoArm)?(int (*)(void **))leDadosEntradasFilmesBin:(int (*)(void **))leDadosLancamentoEntradasFilmes, &todasEntradas);

    if(!quantidadeEntrada){
        return;
    }

    FILE *f;
    char *diretorio;

    pegaCaminho("exportar os dados de entradas a vista", &diretorio);

    f = fopen(diretorio, "w");

    if(!f){
        printf("Nao foi possivel criar o arquivo no diretorio indicado!\n");
        printf("ERRO: %s\n", strerror(errno));
        todasEntradas = limpaMemoria(todasEntradas);
        Sleep(2000);
        return;
    }

    fprintf(f, "<dados>\n");

    for(int i = 0; i<quantidadeEntrada; i++){
        fprintf(f, "    <lancamento>\n");
        EXPORTALD("codigoCompra", todasEntradas[i].codigoCompra, f);
        EXPORTANUMBER("modoPagamento", todasEntradas[i].modoPagamento, f);
        EXPORTAFLOAT("valor", todasEntradas[i].valor, f);
        EXPORTAFLOAT("valorPago", todasEntradas[i].valorPago, f);
        EXPORTAFLOAT("troco", todasEntradas[i].troco, f);
        EXPORTALD("data", mktime(&todasEntradas[i].data), f);
        fprintf(f, "    </lancamento>\n");
    }
    fprintf(f, "</dados>\n");
    fechaArquivo(&f);
    todasEntradas = limpaMemoria(todasEntradas);


}
void exportaEntradaAprazo(int modoArm){

    contaApag *todasContasApag;
    int quantidadeContasApagar = 0;

    quantidadeContasApagar = leArquivoOriginal((modoArm)?"entradaAprazo.bin": "entradaAprazo.txt",
                                               (modoArm)?(int (*)(void **))leDadosEntradasAprazoBin:(int (*)(void **))leDadosEntradasAprazo,
                                               &todasContasApag);

    if(!quantidadeContasApagar){
        return;
    }

    FILE *f;
    char *diretorio;

    pegaCaminho("exportar os dados de entradas a prazo", &diretorio);

    f = fopen(diretorio, "w");



    if(!f){
        printf("nao foi possivel criar o arquivo no diretorio digitado!\n");
        printf("ERRO: %s\n", strerror(errno));
        todasContasApag = limpaMemoria(todasContasApag);
        Sleep(2000);
        return;
    }

    fprintf(f, "<dados>\n");

    for(int i =0; i<quantidadeContasApagar; i++){
        fprintf(f, "    <lancamento>\n");
        EXPORTALD("codigoCompra", todasContasApag[i].codigoCompra, f);
        EXPORTANUMBER("modoPagamento", todasContasApag[i].modoPagamento, f);
        EXPORTANUMBER("parcelas", todasContasApag[i].parcelas, f);
        EXPORTAFLOAT("valorParc", todasContasApag[i].valorParc, f);
        EXPORTAFLOAT("valorEntrada", todasContasApag[i].entrada, f);
        EXPORTALD("data", mktime(&todasContasApag[i].dataAluga), f);
        fprintf(f, "    </lancamento>\n");
    }
    fprintf(f, "</dados>\n");

    fechaArquivo(&f);
    todasContasApag = limpaMemoria(todasContasApag);

}