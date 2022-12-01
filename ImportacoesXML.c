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
#define IMPORTASTRING(STRING, STRING2, f) fprintf(f, "          <%s>%s</%s>\n", STRING, STRING2, STRING)
#define IMPORTANUMBER(STRING, NUMBER, f) fprintf(f, "          <%s>%d</%s>\n", STRING, NUMBER, STRING)
#define IMPORTAFLOAT(STRING, FLOAT, f) fprintf(f, "          <%s>%.2f</%s>\n", STRING, FLOAT, STRING)
#define IMPORTALD(STRING, LD, f) fprintf(f, "          <%s>%ld</%s>\n", STRING, LD, STRING)



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

    pegaCaminho("importar os dados da locadora", &diretorio);
    f = fopen(diretorio, "w");

    verificaOperacao(f, "Erro ao criar arquivo de importacao XML", 0);



    fprintf(f, "<dados>\n");
    fprintf(f, "    <locadora>\n");
    IMPORTASTRING("nomeFantasia", importar.nomeFantasia, f);
    IMPORTASTRING("razaoSocial", importar.razaoSocial, f);
    IMPORTASTRING("inscricaoEst", importar.inscricaoEstadual, f);
    IMPORTASTRING("CNPJ", importar.cnpj, f);
    IMPORTASTRING("endereco", importar.endereco, f);
    IMPORTANUMBER("telefone", importar.telefone, f);
    IMPORTASTRING("Email", importar.email, f);
    IMPORTASTRING("NomeResponsavel", importar.nomeResponsavel, f);
    IMPORTANUMBER("telefoneResp", importar.telefoneResp, f);
    IMPORTAFLOAT("Multa", importar.multa, f);
    fprintf(f, "    </locadora>\n");
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

    pegaCaminho("importar os dados do arquivo de sons", &diretorio);


    (modoArm)? leDadosSonsBin(&importacoesSons): leDadosSons(&importacoesSons);


    f = fopen(diretorio, "w");
    
    fprintf(f,"<configuracoes>\n");
    fprintf(f, "    <sonsDeErro>\n" );
    IMPORTANUMBER("Frequencia", importacoesSons.sonsDeErro.frequencia, f);
    IMPORTANUMBER("Duracao", importacoesSons.sonsDeErro.duracao, f);
    fprintf(f, "    </sonsDeErro>\n");
    fprintf(f, "    <sonsDeConfirmacao>\n");
    IMPORTANUMBER("Frequencia", importacoesSons.sonsDeConfirmacao.frequencia, f);
    IMPORTANUMBER("Duracao", importacoesSons.sonsDeConfirmacao.duracao, f);
    fprintf(f, "    </sonsDeConfirmacao>\n");
    IMPORTANUMBER("ModoSilencioso", importacoesSons.modoSilencioso, f);
    fprintf(f, "</configuracoes>\n");

    fechaArquivo(&f);


}

void importaDadosClientes(int modoArm){
    cliente *todosClientes;
    int quantidadeClientes = 0;

    quantidadeClientes = leArquivoOriginal("clientes.txt", (modoArm)?(int (*)(void **))leDadosClientesBin:(int (*)(void **))leDadosClientes, &todosClientes);

    if(!quantidadeClientes){
        return;
    }

    FILE *f;
    char *diretorio;

    pegaCaminho("importar os dados do arquivo clientes", &diretorio);
    f = fopen(diretorio, "w");

    if(!f){
        printf("nao foi possivel criar o arquivo em %s\n", diretorio);
        printf("ERRO: %s\n", strerror(errno));
        return;
    }

    fprintf(f, "<dados>\n");
    for(int i = 0; i<quantidadeClientes; i++){

        fprintf(f, "    <cliente>\n");
        IMPORTANUMBER("codigo", todosClientes[i].codigo, f);
        IMPORTASTRING("nome", todosClientes[i].nomeCompleto,f);
        IMPORTASTRING("rua", todosClientes[i].rua, f);
        IMPORTASTRING("bairro", todosClientes[i].bairro, f);
        IMPORTANUMBER("numero", todosClientes[i].numeroDaCasa, f);
        IMPORTASTRING("CPF", todosClientes[i].cpf, f);
        IMPORTANUMBER("telefone", todosClientes[i].telefone, f);
        IMPORTASTRING("email", todosClientes[i].email, f);
        IMPORTANUMBER("sexo", todosClientes[i].sexo, f);
        IMPORTANUMBER("EstadoCivil", todosClientes[i].estadoCivil, f);
        IMPORTANUMBER("DiaNasc", todosClientes[i].diaNascimento, f);
        IMPORTANUMBER("mes", todosClientes[i].mes, f);
        IMPORTANUMBER("ano", todosClientes[i].ano, f);
        IMPORTANUMBER("flagExclusao", todosClientes[i].flag, f);
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

    pegaCaminho("importar os dados do arquivo de funcionarios", &diretorio);

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
        IMPORTALD("codigo", todosFuncs[i].codigo, f);
        IMPORTASTRING("nome", todosFuncs[i].nome, f);
        IMPORTASTRING("cargo", todosFuncs[i].cargo, f);
        IMPORTASTRING("rua", todosFuncs[i].rua, f);
        IMPORTASTRING("bairro", todosFuncs[i].bairro, f);
        IMPORTANUMBER("numero", todosFuncs[i].numero, f);
        IMPORTANUMBER("telefone", todosFuncs[i].telefone, f);
        IMPORTASTRING("email", todosFuncs[i].email, f);
        IMPORTANUMBER("flagExlusao", todosFuncs[i].flag, f);

        fprintf(f, "    </Funcionario>\n");
    }
    fprintf(f, "</dados>");


    apagaDadosStructFuncionarios(todosFuncs, quantidadeFuncionarios);
    todosFuncs = limpaMemoria(todosFuncs);
    fechaArquivo(&f);
}

void importaDadosFornecedores(int modoArm){
    fornecedor *forn = NULL;
    int quantidadeForn = 0;

    quantidadeForn = leArquivoOriginal((modoArm)?"fornecedores.bin":"fornecedores.txt", (modoArm)?(int (*)(void **))leDadosFornecedoresBin:(int (*)(void **)) leDadosFornecedores, &forn);

    if(!quantidadeForn){
        return;
    }
    char *diretorio;

    pegaCaminho("importar os dados do arquivo fornecedores", &diretorio);

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
        IMPORTANUMBER("codigo", forn[i].codigo, f);
        IMPORTASTRING("nomeFantasia", forn[i].nomeFantasia, f);
        IMPORTASTRING("razaoSocial", forn[i].razaoSocial, f);
        IMPORTASTRING("inscEstadual", forn[i].inscricaoEstadual, f);
        IMPORTASTRING("CNPJ", forn[i].CNPJ, f);
        IMPORTASTRING("bairro", forn[i].bairro, f);
        IMPORTASTRING("rua", forn[i].rua, f);
        IMPORTANUMBER("numero", forn[i].numero, f);
        IMPORTANUMBER("telefone", forn[i].telefone, f);
        IMPORTASTRING("email", forn[i].email, f);
        IMPORTANUMBER("flag", forn[i].flag, f);
        fprintf(f,"    </Fornecedor>\n");
    }
    fprintf(f, "</dado>");

    fechaArquivo(&f);
    limpaMemoriaStringsFornecedor(forn, quantidadeForn);
    forn = limpaMemoria(forn);
}
void importaCategoria(int modoArm){
    categoria *todasCat = NULL;
    int quantidadeCat = 0;

    quantidadeCat = leArquivoOriginal((modoArm)?"categorias.bin":"categorias.txt", (modoArm)?(int(*)(void **))leDadosCategoriaBin:(int (*)(void **))leDadosCategoria, &todasCat);

    if(!quantidadeCat){
        return;
    }
    char *diretorio;
    FILE *f;

    pegaCaminho("importar os dados do arquivo categoria", &diretorio);

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
        IMPORTANUMBER("codigo", todasCat[i].codigo, f);
        IMPORTASTRING("nome", todasCat[i].nome, f);
        IMPORTASTRING("descricao", todasCat[i].descricao, f);
        IMPORTAFLOAT("valorLoc", todasCat[i].valorLocacao, f);
        IMPORTANUMBER("flag", todasCat[i].flag, f);
        fprintf(f, "   </categoria>\n");

    }
    fprintf(f , "</dado>");

    fechaArquivo(&f);
    limpaDadosCategoriaMemoria(todasCat, quantidadeCat);
    todasCat = limpaMemoria(todasCat);



}
void importaDadosFilmes(int modoArm){

    filmes *todosFilmes = NULL;
    int quantidadeFilmes = 0;

    quantidadeFilmes = leArquivoOriginal((modoArm)?"filmes.bin":"filmes.txt", (modoArm)?(int (*)(void **))leDadosFilmesBin:(int (*)(void **))leDadosFilmes, &todosFilmes);

    if(!quantidadeFilmes){
        return;
    }
    FILE *f;
    char *diretorio;

    pegaCaminho("importar os dados do arquivo filmes", &diretorio);

    f = fopen(diretorio, "w");
    fprintf(f, "<dados>\n");

    for(int i = 0; i<quantidadeFilmes; i++){


        fprintf(f, "    <Filme>\n");
        IMPORTANUMBER("codigo", todosFilmes[i].codigo, f);
        IMPORTASTRING("nome", todosFilmes[i].nome, f);
        IMPORTASTRING("descricao", todosFilmes[i].descricao,f);
        IMPORTANUMBER("CodigoCat", todosFilmes[i].codigoCategoria, f);
        IMPORTAFLOAT("ValorLoc", todosFilmes[i].valorLocacao, f);
        IMPORTANUMBER("exemplares", todosFilmes[i].exemplares, f);
        IMPORTANUMBER("lingua", todosFilmes[i].lingua, f);
        IMPORTANUMBER("flag", todosFilmes[i].flag, f);
        fprintf(f, "    </Filme>\n");

    }
    fprintf(f, "</dados>");

    fechaArquivo(&f);
    limpaDadosFilmeMemoria(todosFilmes, quantidadeFilmes);
    todosFilmes = limpaMemoria(todosFilmes);

}
void importaDadosCompras(int modoArm){

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

    pegaCaminho("importar os dados do arquivo de compras", &diretorio);

    f = fopen(diretorio, "w");

    fprintf(f, "<dado>\n");

    for(int i = 0; i<quant.quantidadeAlugacoes; i++){

        fprintf(f, "    <compra>\n");
        IMPORTALD("codigoCompra", todasAsCompras[i].codigo, f);
        IMPORTANUMBER("modoPagamento", todasAsCompras[i].modoPagamento, f);
        IMPORTALD("data", mktime(&todasAsCompras[i].data), f);
        IMPORTALD("vendedor", todasAsCompras[i].vendedor->codigo, f);
        IMPORTANUMBER("comprador", todasAsCompras[i].comprador->codigo, f);
        IMPORTAFLOAT("preco", todasAsCompras[i].preco, f);
        IMPORTANUMBER("quantidadeFilmesComprados", todasAsCompras[i].quantidadeFilmesComprados, f);
        for(int j = 0; j<todasAsCompras[i].quantidadeFilmesComprados; j++){
            fprintf(f, "          <filme%d>\n", j+1);
            fprintf(f, "            <codigoFilme>%d</codigoFilme>\n", todasAsCompras[i].filmesComprados[j].codigo);
            fprintf(f, "            <exemplares>%d</exemplares>\n", todasAsCompras[i].filmesComprados[j].exemplares);
            fprintf(f, "          </filme%d>\n", j+1);
        }
        IMPORTANUMBER("devolvido", todasAsCompras[i].devolvido, f);
        fprintf(f, "    </compra>\n");
    }
    fprintf(f, "</dado>\n");

    fechaArquivo(&f);
    limpaDadosDevolucoesMemoria(&todosFilmes, &todosClientes, &todosFuncs, &todasAsCompras, quant);
}
void importaDadosLancamentos(int modoArm){
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

    pegaCaminho("importar os dados do arquivo lancamento caixa", &diretorio);

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
        IMPORTALD("codigoCompra", todosLancamentos[i].codigoCompra, f);
        IMPORTANUMBER("modoPagamento", todosLancamentos[i].modoPagamento, f);
        IMPORTAFLOAT("valor", todosLancamentos[i].valor, f);
        IMPORTAFLOAT("valorPago", todosLancamentos[i].valorPago,f);
        IMPORTAFLOAT("troco", todosLancamentos[i].troco, f);
        IMPORTALD("data", mktime(&todosLancamentos[i].data), f);
        fprintf(f, "    </lancamento>\n");


    }
    fprintf(f, "</dados>\n");

    fechaArquivo(&f);
    todosLancamentos = limpaMemoria(todosLancamentos);


}
void importaLancamentosAprazo(int modoArm){

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

    pegaCaminho("importar os dados do arquivo de contas a receber", &diretorio);

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
        IMPORTANUMBER("codigoCliente", todasContas[i].codigoCliente, f);
        IMPORTALD("codigoCompra", todasContas[i].codigoCompra, f);
        IMPORTANUMBER("modoPagamento", todasContas[i].modoPagamento, f);
        IMPORTANUMBER("parcelas", todasContas[i].parcelas, f);
        IMPORTAFLOAT("valorParcela", todasContas[i].valorParc, f);
        IMPORTAFLOAT("ValorEntrada", todasContas[i].entrada, f);
        IMPORTALD("data", mktime(&todasContas[i].dataAluga), f);
        fprintf(f, "    </conta>\n");
    }
    fprintf(f, "</dados>");

    fechaArquivo(&f);
    todasContas = limpaMemoria(todasContas);

}

void importaLancamentosEntrada(int modoArm){

    lancamentoEntradas *todasEntradas;
    int quantidadeEntrada;

    quantidadeEntrada = leArquivoOriginal((modoArm)?"lancamentosEntrada.bin": "lancamentosEntrada.txt",
                                          (modoArm)?(int (*)(void **))leDadosEntradasFilmesBin:(int (*)(void **))leDadosLancamentoEntradasFilmes, &todasEntradas);

    if(!quantidadeEntrada){
        return;
    }

    FILE *f;
    char *diretorio;

    pegaCaminho("importar os dados de entradas a vista", &diretorio);

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
        IMPORTALD("codigoCompra", todasEntradas[i].codigoCompra, f);
        IMPORTANUMBER("modoPagamento", todasEntradas[i].modoPagamento, f);
        IMPORTAFLOAT("valor", todasEntradas[i].valor, f);
        IMPORTAFLOAT("valorPago", todasEntradas[i].valorPago, f);
        IMPORTAFLOAT("troco", todasEntradas[i].troco, f);
        IMPORTALD("data", mktime(&todasEntradas[i].data), f);
        fprintf(f, "    </lancamento>\n");
    }
    fprintf(f, "</dados>\n");
    fechaArquivo(&f);
    todasEntradas = limpaMemoria(todasEntradas);


}
void importaEntradaAprazo(int modoArm){

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

    pegaCaminho("importar os dados de entradas a prazo", &diretorio);

    f = fopen(diretorio, "w");

    if(!f){

    }

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
        IMPORTALD("codigoCompra", todasContasApag[i].codigoCompra, f);
        IMPORTANUMBER("modoPagamento", todasContasApag[i].modoPagamento, f);
        IMPORTANUMBER("parcelas", todasContasApag[i].parcelas, f);
        IMPORTAFLOAT("valorParc", todasContasApag[i].valorParc, f);
        IMPORTAFLOAT("valorEntrada", todasContasApag[i].entrada, f);
        IMPORTALD("data", mktime(&todasContasApag[i].dataAluga), f);
        fprintf(f, "    </lancamento>\n");
    }
    fprintf(f, "</dados>\n");

    fechaArquivo(&f);
    todasContasApag = limpaMemoria(todasContasApag);

}