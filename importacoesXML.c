//
// Created by vitor on 27/11/2022.
//
#include <time.h>
#include "importacoesXML.h"
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
#include "ExportacaoXML.h"



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
FILE *digitaCaminhoImport(char *discricaoCaminhoo, char **diretorio){
    FILE *f;
    pegaCaminho(discricaoCaminhoo, diretorio);

    f = fopen(diretorio[0], "r");

    while(!f){
        f = fopen(diretorio[0], "r");
        if(f){
            break;
        }
        printf("nao foi encontrado o arquivo em: %s\n ", diretorio[0]);
        diretorio[0] = limpaMemoria(diretorio[0]);
        pegaCaminho("\0", diretorio);
    }
    diretorio[0] = limpaMemoria(diretorio[0]);
    return f;
}


void importaLocadora(int modoArm){

    locadora export;
    char *diretorio;
    FILE *f;

    f = digitaCaminhoImport("importar os dados para o arquivo locadora", &diretorio);

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
        printf("nao foi possivel atualizar os dados da locadora com os arquivos importados!\n");
        printf("ERRO: %s\n", strerror(errno));
        Sleep(2000);
        limpaCamposLocadoraMemoria(&export);
        return;

    }
    (modoArm)? reescreveDadosLocadoraBin(&export, f): reescreveDadosLocadora(&export, f);
    fechaArquivo(&f);
    limpaCamposLocadoraMemoria(&export);



}
void importaConfiguracoesSons(int modoArm){
    sons configuracoesSons;

    char *diretorio;
    FILE *f;
    f = digitaCaminhoImport("importar arquivo de configuracoes de sons: ", &diretorio);

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
void importaFuncionarios(int modoArm){

    Funcionarios *todosFuncs = NULL;
    int quantidadeFuncs = 0;

    char *diretorio;
    FILE *f;

    f = digitaCaminhoImport("importar dados para o arquivo de funcionarios", &diretorio);

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
void importaFornecedores(int modoArm){

    fornecedor *todosFornecedores = NULL;
    int quantidadeFornecedores = 0;

    char *diretorio;
    FILE *f;

    f = digitaCaminhoImport("importar dados para o arquivo de fornecedores", &diretorio);
    avancaAtePalavra("<codigo>", f);
    while(!feof(f)){
        todosFornecedores = (!quantidadeFornecedores)?malloc(sizeof(fornecedor)): realloc(todosFornecedores, sizeof(fornecedor)*(quantidadeFornecedores+1));
        verificaOperacao(todosFornecedores, "ERRO: Memoria indisponivel!\n", 1);


        fscanf(f, "%d", &todosFornecedores[quantidadeFornecedores].codigo);

        avancaAtePalavra("<nomeFantasia>", f);
        digText(&todosFornecedores[quantidadeFornecedores].nomeFantasia, f, '<');

        avancaAtePalavra("<razaoSocial>", f);
        digText(&todosFornecedores[quantidadeFornecedores].razaoSocial, f, '<');

        avancaAtePalavra("<inscEstadual>", f);
        digText(&todosFornecedores[quantidadeFornecedores].inscricaoEstadual, f, '<');

        avancaAtePalavra("<CNPJ>", f);
        digText(&todosFornecedores[quantidadeFornecedores].CNPJ, f, '<');

        avancaAtePalavra("<bairro>", f);
        digText(&todosFornecedores[quantidadeFornecedores].bairro, f, '<');

        avancaAtePalavra("<rua>", f);
        digText(&todosFornecedores[quantidadeFornecedores].rua, f, '<');

        avancaAtePalavra("<numero>", f);
        fscanf(f, "%d", &todosFornecedores[quantidadeFornecedores].numero);

        avancaAtePalavra("<telefone>", f);
        fscanf(f, "%d", &todosFornecedores[quantidadeFornecedores].telefone);

        avancaAtePalavra("<email>", f);
        digText(&todosFornecedores[quantidadeFornecedores].email, f, '<');

        avancaAtePalavra("<flag>", f);
        fscanf(f, "%d", &todosFornecedores[quantidadeFornecedores].flag);


        avancaAtePalavra("<codigo>", f);
        quantidadeFornecedores++;
    }
    fechaArquivo(&f);
    (modoArm)
        ? reescreveDadosFornecedoresBin(todosFornecedores, quantidadeFornecedores)
        : reescreveDadosFornecedores(todosFornecedores, quantidadeFornecedores);
    limpaMemoriaStringsFornecedor(todosFornecedores, quantidadeFornecedores);
    todosFornecedores = limpaMemoria(todosFornecedores);

}
void importaCategoria(int modoArm){

    categoria *todasCats = NULL;
    int quantidadeCategoria = 0;

    char *diretorio;
    FILE *f;

    f = digitaCaminhoImport("importar dados para o arquivo de categorias", &diretorio);
    avancaAtePalavra("<codigo>", f);
    while(!feof(f)){
        todasCats = (!quantidadeCategoria)?malloc(sizeof(categoria)):realloc(todasCats, sizeof(categoria)*(quantidadeCategoria+1));
        verificaOperacao(todasCats, "ERRO: Memoria indisponivel!", 1);


        fscanf(f, "%d", &todasCats[quantidadeCategoria].codigo);

        avancaAtePalavra("<nome>", f);
        digText(&todasCats[quantidadeCategoria].nome, f, '<');

        avancaAtePalavra("<descricao>", f);
        digText(&todasCats[quantidadeCategoria].descricao, f, '<');

        avancaAtePalavra("<valorLoc>", f);
        fscanf(f, "%f", &todasCats[quantidadeCategoria].valorLocacao);

        avancaAtePalavra("<flag>", f);
        fscanf(f, "%d", &todasCats[quantidadeCategoria].flag);

        avancaAtePalavra("<codigo>", f);
        quantidadeCategoria++;
    }
    fechaArquivo(&f);
    (modoArm)
        ? reescreveDadosCategoriaBin(todasCats, quantidadeCategoria)
        : reescreveDadosCategoria(todasCats, quantidadeCategoria);
    limpaDadosCategoriaMemoria(todasCats, quantidadeCategoria);
    todasCats = limpaMemoria(todasCats);
}
void importaClientes(int modoArm){
    cliente *todosClientes = NULL;
    int quantidadeClientes = 0;

    char *discricao;
    FILE *f;

    f = digitaCaminhoImport("importar dados para o arquivo de clientes", &discricao);
    avancaAtePalavra("<codigo>", f);
    while(!feof(f)){

        todosClientes = (!quantidadeClientes)
                            ? malloc(sizeof(cliente))
                            : realloc(todosClientes, sizeof(cliente)*(quantidadeClientes+1));

        verificaOperacao(todosClientes, "ERRO: Memoria indisponivel!", 1);

        fscanf(f, "%d", &todosClientes[quantidadeClientes].codigo);

        avancaAtePalavra("<nome>", f);
        digText(&todosClientes[quantidadeClientes].nomeCompleto, f,'<');

        avancaAtePalavra("<rua>", f);
        digText(&todosClientes[quantidadeClientes].rua, f, '<');

        avancaAtePalavra("<bairro>", f);
        digText(&todosClientes[quantidadeClientes].bairro, f, '<');

        avancaAtePalavra("<numero>", f);
        fscanf(f, "%d", &todosClientes[quantidadeClientes].numeroDaCasa);

        avancaAtePalavra("<CPF>", f);
        digText(&todosClientes[quantidadeClientes].cpf, f, '<');

        avancaAtePalavra("<telefone>", f);
        fscanf(f, "%d", &todosClientes[quantidadeClientes].telefone);

        avancaAtePalavra("<email>", f);
        digText(&todosClientes[quantidadeClientes].email, f, '<');

        avancaAtePalavra("<sexo>", f);
        fscanf(f, "%d", &todosClientes[quantidadeClientes].sexo);

        avancaAtePalavra("<EstadoCivil>", f);
        fscanf(f, "%d", &todosClientes[quantidadeClientes].estadoCivil);

        avancaAtePalavra("<DiaNasc>", f);
        fscanf(f, "%d", &todosClientes[quantidadeClientes].diaNascimento);

        avancaAtePalavra("<mes>", f);
        fscanf(f, "%d", &todosClientes[quantidadeClientes].mes);

        avancaAtePalavra("<ano>", f);
        fscanf(f, "%d", &todosClientes[quantidadeClientes].ano);

        avancaAtePalavra("<flagExclusao>", f);
        fscanf(f, "%d", &todosClientes[quantidadeClientes].flag);

        avancaAtePalavra("<codigo>", f);
        quantidadeClientes++;
    }
    fechaArquivo(&f);
    (modoArm)
        ? reescreveDadosClienteBin(todosClientes, quantidadeClientes)
        : reescreveDadosCliente(todosClientes, quantidadeClientes);
    limpaDadosClienteMemoria(todosClientes, quantidadeClientes);
    todosClientes = limpaMemoria(todosClientes);
}
void importaFilmes(int modoArm){

    filmes *todosFilmes = NULL;
    int quantidadeFilmes = 0;

    char *descricao;
    FILE *f;

    f = digitaCaminhoImport("importar dados para o arquivo de filmes", &descricao);
    avancaAtePalavra("<codigo>", f);

    while(!feof(f)){

        todosFilmes = (!quantidadeFilmes)?malloc(sizeof(filmes)):realloc(todosFilmes, sizeof(filmes)*(quantidadeFilmes+1));
        verificaOperacao(todosFilmes, "ERRO: Memoria indisponivel!", 1);

        fscanf(f, "%d", &todosFilmes[quantidadeFilmes].codigo);

        avancaAtePalavra("<nome>", f);
        digText(&todosFilmes[quantidadeFilmes].nome, f, '<');

        avancaAtePalavra("<descricao>", f);
        digText(&todosFilmes[quantidadeFilmes].descricao, f, '<');

        avancaAtePalavra("<CodigoCat>", f);
        fscanf(f, "%d", &todosFilmes[quantidadeFilmes].codigoCategoria);

        avancaAtePalavra("<ValorLoc>", f);
        fscanf(f, "%f", &todosFilmes[quantidadeFilmes].valorLocacao);

        avancaAtePalavra("<exemplares>", f);
        fscanf(f, "%d", &todosFilmes[quantidadeFilmes].exemplares);

        avancaAtePalavra("<lingua>", f);
        fscanf(f, "%d", &todosFilmes[quantidadeFilmes].lingua);

        avancaAtePalavra("<flag>", f);
        fscanf(f, "%d", &todosFilmes[quantidadeFilmes].flag);

        avancaAtePalavra("<codigo>", f);
        quantidadeFilmes++;
    }
    fechaArquivo(&f);
    (modoArm)
        ? reescreveDadosFilmeBin(todosFilmes, quantidadeFilmes)
        : reescreveDadosFilme(todosFilmes, quantidadeFilmes);
    limpaDadosFilmeMemoria(todosFilmes, quantidadeFilmes);
    todosFilmes = limpaMemoria(todosFilmes);



}
void importaDevolucoes(int modoArm){


    compras *todasAsCompras = NULL;
    Funcionarios *todosFuncionarios = NULL;
    cliente *todosClientes = NULL;
    filmes *todosFilmes = NULL;
    quantidades quant = {0,0,0,0};
    time_t temp = 0;
    int temp2;
    char *descricao;
    FILE *f;

    printf("Para importar as compras de outro sistema, será necessario tambem importar os clientes, filmes, funcionarios e categorias!\n");

    importaCategoria(modoArm);
    importaClientes(modoArm);
    importaFilmes(modoArm);
    importaFuncionarios(modoArm);

    quant.quantidadeClientes = (modoArm)? leDadosClientesBin(&todosClientes): leDadosClientes(&todosClientes);
    quant.quantidadesFuncionarios = (modoArm)? leDadosFuncionariosBin(&todosFuncionarios): leDadosFuncionarios(&todosFuncionarios);
    quant.quantidadesFilmes = (modoArm)? leDadosFilmesBin(&todosFilmes): leDadosFilmes(&todosFilmes);



    f = digitaCaminhoImport("importar dados para o arquivo de devolucoes", &descricao);
    avancaAtePalavra("<codigoCompra>", f);

    while(!feof(f)){

        todasAsCompras = (!quant.quantidadeAlugacoes)? malloc(sizeof(compras)): realloc(todasAsCompras, sizeof(compras)*(quant.quantidadeAlugacoes+1));
        verificaOperacao(todasAsCompras, "ERRO: Memoria indisponivel!", 1);

        fscanf(f, "%ld", &todasAsCompras[quant.quantidadeAlugacoes].codigo);

        avancaAtePalavra("<modoPagamento>", f);
        fscanf(f, "%d", &todasAsCompras[quant.quantidadeAlugacoes].modoPagamento);

        avancaAtePalavra("<data>",f);
        fscanf(f, "%ld", &temp);
        todasAsCompras[quant.quantidadeAlugacoes].data = *localtime(&temp);

        avancaAtePalavra("<vendedor>", f);
        fscanf(f, "%ld", &temp);
        todasAsCompras[quant.quantidadeAlugacoes].vendedor = encontraFuncionarioCodigo(todosFuncionarios, temp, quant.quantidadesFuncionarios, NULL);

        avancaAtePalavra("<comprador>", f);
        fscanf(f, "%d", &temp2);
        descricao = converteIntEmString(temp2);
        todasAsCompras[quant.quantidadeAlugacoes].comprador = encontraClienteCodigo(todosClientes, quant.quantidadeClientes, descricao, 0);
        descricao = limpaMemoria(descricao);

        avancaAtePalavra("<preco>", f);
        fscanf(f, "%f", &todasAsCompras[quant.quantidadeAlugacoes].preco);


        avancaAtePalavra("<quantidadeFilmesComprados>", f);
        fscanf(f, "%d", &todasAsCompras[quant.quantidadeAlugacoes].quantidadeFilmesComprados);

        todasAsCompras[quant.quantidadeAlugacoes].filmesComprados =
                malloc(sizeof(filmes)*(todasAsCompras[quant.quantidadeAlugacoes].quantidadeFilmesComprados));

        verificaOperacao(todasAsCompras[quant.quantidadeAlugacoes].filmesComprados, "ERRO: Memoria indisponivel!", 1);

        for(int i = 0; i<todasAsCompras[quant.quantidadeAlugacoes].quantidadeFilmesComprados; i++){

            avancaAtePalavra("<codigoFilme>", f);
            fscanf(f, "%d", &temp2);
            descricao = converteIntEmString(temp2);
            todasAsCompras[quant.quantidadeAlugacoes].filmesComprados[i] = *encontraFilmeCodigo(todosFilmes, quant.quantidadesFilmes, descricao, 0);

            avancaAtePalavra("<exemplares>",f);
            fscanf(f, "%d", &todasAsCompras[quant.quantidadeAlugacoes].filmesComprados[i].exemplares);
        }
        avancaAtePalavra("<devolvido>", f);
        fscanf(f, "%d", &todasAsCompras[quant.quantidadeAlugacoes].devolvido);

        avancaAtePalavra("<codigoCompra>", f);
        quant.quantidadeAlugacoes++;

    }

    fechaArquivo(&f);
    (modoArm)
        ? reescreveDadosDevolucoesBin(todasAsCompras, quant.quantidadeAlugacoes, "devolucoesRes.bin", "devolucoes.bin", "wb")
        : reescreveDadosDevolucoes(todasAsCompras, quant.quantidadeAlugacoes, "devolucoesRes.txt", "devolucoes.txt", "w");
    limpaDadosDevolucoesMemoria(&todosFilmes, &todosClientes, &todosFuncionarios, &todasAsCompras, quant);

}
void importaLancamentosCaixa(int modoArm){
    lancamentoCaixa *todosLanc = NULL;
    int quantidade = 0;
    time_t temp = 0;
    char *descricao;
    FILE *f;

    f = digitaCaminhoImport("importar os lancamentos do caixa", &descricao);

    avancaAtePalavra("<codigoCompra>", f);

    while(!feof(f)){

        todosLanc = (!quantidade)? malloc(sizeof(lancamentoCaixa)): realloc(todosLanc, sizeof(lancamentoCaixa)*(quantidade+1));
        verificaOperacao(todosLanc, "ERRO: Memoria indisponivel!\n", 1);

        fscanf(f, "%ld", &todosLanc[quantidade].codigoCompra);

        avancaAtePalavra("<modoPagamento>", f);
        fscanf(f, "%d", &todosLanc[quantidade].modoPagamento);

        avancaAtePalavra("<valor>", f);
        fscanf(f, "%f", &todosLanc[quantidade].valor);

        avancaAtePalavra("<valorPago>", f);
        fscanf(f, "%f", &todosLanc[quantidade].valorPago);

        avancaAtePalavra("<troco>", f);
        fscanf(f, "%f", &todosLanc[quantidade].troco);

        avancaAtePalavra("<data>", f);
        fscanf(f, "%ld", &temp);
        todosLanc[quantidade].data = *localtime(&temp);

        avancaAtePalavra("<codigoCompra>", f);
        quantidade++;

    }

    fechaArquivo(&f);

    (modoArm)
        ? reescreveLancamentosCaixaBin(todosLanc, quantidade, "lancamentosRes.bin", "lancamentos.bin", "wb")
        : reescreveLancamentosCaixa(todosLanc, quantidade, "lancamentosRes.txt", "lancamentos.txt", "w");

    todosLanc = limpaMemoria(todosLanc);



}
void importaCompraAprazo(int modoArm){

    contaArec *todasAscontas = NULL;
    int quantidade = 0;
    time_t temp=0;

    char *descricao;
    FILE *f;

    f = digitaCaminhoImport("importar dados do arquivo de contas a receber", &descricao);

    avancaAtePalavra("<codigoCliente>", f);

    while(!feof(f)){
        todasAscontas = (!quantidade)?malloc(sizeof(contaArec)): realloc(todasAscontas, sizeof(contaArec)*(quantidade+1));
        verificaOperacao(todasAscontas, "ERRO: Memoria indisponivel!", 1);
        todasAscontas[quantidade] = (contaArec){0,0,0,0,0,0,(struct tm){0,0,0,0,0,0,0,0,0}};

        fscanf(f, "%d", &todasAscontas[quantidade].codigoCliente);

        avancaAtePalavra("<codigoCompra>", f);
        fscanf(f, "%ld", &todasAscontas[quantidade].codigoCompra);

        avancaAtePalavra("<modoPagamento>", f);
        fscanf(f, "%d", &todasAscontas[quantidade].modoPagamento);

        avancaAtePalavra("<parcelas>", f);
        fscanf(f, "%d", &todasAscontas[quantidade].parcelas);

        avancaAtePalavra("<valorParcela>", f);
        fscanf(f, "%f", &todasAscontas[quantidade].valorParc);

        avancaAtePalavra("<ValorEntrada>", f);
        fscanf(f, "%f", &todasAscontas[quantidade].entrada);

        avancaAtePalavra("<data>", f);
        fscanf(f, "%ld", &temp);
        todasAscontas[quantidade].dataAluga = *localtime(&temp);

        avancaAtePalavra("<codigoCliente>", f);
        quantidade++;
    }
    fechaArquivo(&f);

    (modoArm)
        ? reescreveLancamentosAprazoBin(todasAscontas, quantidade, "lancamentosAprazoRes.bin", "lancamentosAprazo.bin", "wb")
        : reescreveLancamentosAprazo(todasAscontas, quantidade, "lancamentosAprazoRes.txt", "lancamentosAprazo.txt", "wb");

    todasAscontas = limpaMemoria(todasAscontas);


}
void importaDadosLancamentoentradas(int modoArm){

    lancamentoEntradas *todasAsEntradas = NULL;
    int quantidadeContas = 0;
    time_t temp = 0;

    char *descricao;
    FILE *f;

    f = digitaCaminhoImport("importar dados de lancamentos de entrada", &descricao);

    avancaAtePalavra("<codigoCompra>", f);

    while(!feof(f)){

        todasAsEntradas = (!quantidadeContas)
                ?malloc(sizeof(lancamentoEntradas))
                :realloc(todasAsEntradas, sizeof(lancamentoEntradas)*(quantidadeContas+1));

        verificaOperacao(todasAsEntradas, "ERRO: Memoria indisponivel!", 1);
        todasAsEntradas[quantidadeContas] = (lancamentoEntradas){0,0,0,0,0,(struct tm){0,0,0,0,0,0,0,0,0}};
        fscanf(f, "%ld", &todasAsEntradas[quantidadeContas].codigoCompra);

        avancaAtePalavra("<modoPagamento>", f);
        fscanf(f, "%d", &todasAsEntradas[quantidadeContas].modoPagamento);

        avancaAtePalavra("<valor>", f);
        fscanf(f, "%f", &todasAsEntradas[quantidadeContas].valor);

        avancaAtePalavra("<valorPago>", f);
        fscanf(f, "%f", &todasAsEntradas[quantidadeContas].valorPago);

        avancaAtePalavra("<troco>", f);
        fscanf(f, "%f", &todasAsEntradas[quantidadeContas].troco);

        avancaAtePalavra("<data>", f);
        fscanf(f, "%ld", &temp);
        todasAsEntradas[quantidadeContas].data = *localtime(&temp);

        avancaAtePalavra("<codigoCompra>", f);

        quantidadeContas++;

    }
    fechaArquivo(&f);

    (modoArm)
        ? reescreveLancamentosEntradaBin(todasAsEntradas, quantidadeContas, "lancamentosEntradaRes.bin", "lancamentosEntrada.bin", "wb")
        : reescreveLancamentosEntrada(todasAsEntradas, quantidadeContas, "lancamentosEntradasRes.txt", "lancamentosEntrada.txt", "w");

    todasAsEntradas = limpaMemoria(todasAsEntradas);




}
void importaEntradasAprazo(int modoArm){
    contaApag *todasAsEntradasAprazo = NULL;
    int quantidadeEntradasAprazo = 0;

    FILE *f;
    char *descricao;
    time_t temp = 0;

    f = digitaCaminhoImport("importar dados de entradas a prazo", &descricao);

    avancaAtePalavra("<codigoCompra>", f);

    while(!feof(f)){

        todasAsEntradasAprazo = (!quantidadeEntradasAprazo)? malloc(sizeof(contaApag)): realloc(todasAsEntradasAprazo, sizeof(contaApag)*(quantidadeEntradasAprazo+1));
        verificaOperacao(todasAsEntradasAprazo, "ERRO: Memoria indisponivel!", 1);
        todasAsEntradasAprazo[quantidadeEntradasAprazo] = (contaApag){0,0,0,0,0,(struct tm){0,0,0,0,0,0,0,0,0}};

        fscanf(f, "%ld", &todasAsEntradasAprazo[quantidadeEntradasAprazo].codigoCompra);

        avancaAtePalavra("<modoPagamento>", f);
        fscanf(f, "%d", &todasAsEntradasAprazo[quantidadeEntradasAprazo].modoPagamento);

        avancaAtePalavra("<parcelas>", f);
        fscanf(f, "%d", &todasAsEntradasAprazo[quantidadeEntradasAprazo].parcelas);

        avancaAtePalavra("<valorParc>", f);
        fscanf(f, "%f", &todasAsEntradasAprazo[quantidadeEntradasAprazo].valorParc);

        avancaAtePalavra("<valorEntrada>", f);
        fscanf(f, "%f", &todasAsEntradasAprazo[quantidadeEntradasAprazo].entrada);

        avancaAtePalavra("<data>", f);
        fscanf(f, "%ld", &temp);
        todasAsEntradasAprazo[quantidadeEntradasAprazo].dataAluga = *localtime(&temp);

        avancaAtePalavra("<codigoCompra>", f);

        quantidadeEntradasAprazo++;

    }
    fechaArquivo(&f);

    (modoArm)
        ? reescreveEntradasAprazoBin(todasAsEntradasAprazo, quantidadeEntradasAprazo, "entradaAprazoRes.bin", "entradaAprazo.bin", "wb")
        : reescreveEntradaAprazo(todasAsEntradasAprazo, quantidadeEntradasAprazo, "entradaAprazoRes.txt", "entradaAprazo.txt", "w");

    todasAsEntradasAprazo = limpaMemoria(todasAsEntradasAprazo);
}
