#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <windows.h>
#include <time.h>
#include "ControleFilmes.h"
#include "Caixa.h"
#include "ControleDeNumeros.h"
#include "FuncUtilitarias.h"
#include "ManipulacoesDeTexto.h"
#include "menus.h"
#include "EntradaDeFilmes.h"
#include "Carrinho.h"
#include "ContasAPagar.h"

#define ERROMEM "ERRO: Memoria indisponivel!\n"


int leDadosLancamentoEntradasFilmes(lancamentoEntradas **contas){
    FILE *f;
    int indice = 0;
    long int temp;
    f = fopen("lancamentosEntrada.txt", "r");

    if(!f){
        f = fopen("lancamentosEntrada.txt", "w");
        if(!f){
            printf("Erro ao criar arquivo de lancamentos a prazo!\n");
            printf("%s", strerror(errno));
            return 0;
        }
        return 0;
    }

    while(1){
        fscanf(f, "%ld ", &temp);

        if(feof(f)){
            break;
        }

        contas[0] = (indice == 0)?malloc(sizeof(lancamentoEntradas)):realloc(contas[0], sizeof(lancamentoEntradas)*(indice+1));
        verificaOperacao(contas[0], ERROMEM, 1);

        contas[0][indice].codigoCompra = temp;
        fscanf(f, "%d ", &contas[0][indice].modoPagamento);
        fscanf(f, "%f ", &contas[0][indice].valor);
        fscanf(f, "%f ", &contas[0][indice].valorPago);
        fscanf(f, "%f ", &contas[0][indice].troco);
        fscanf(f, "%d ", &contas[0][indice].data.tm_mday);
        fscanf(f, "%d ", &contas[0][indice].data.tm_mon);
        fscanf(f, "%d ", &contas[0][indice].data.tm_year);
        fscanf(f, "%d ", &contas[0][indice].data.tm_hour);
        fscanf(f, "%d  ", &contas[0][indice].data.tm_min);

        indice++;
    }
    fechaArquivo(&f);
    return indice;
}

int leDadosEntradasFilmesBin(lancamentoEntradas **contas){
    FILE *f;
    int indice = 0;
    lancamentoEntradas temp;
    f = fopen("lancamentosEntrada.bin", "rb");

    if(!f){
        f = fopen("lancamentosEntrada.bin", "wb");
        if(!f){
            printf("Erro ao criar o arquivo de lancamento a prazo!\n");
            printf("%s\n", strerror(errno));
            return 0;
        }
        return 0;
    }

    while(1){
        fread(&temp, sizeof(lancamentoEntradas), 1, f);
        if(feof(f)){
            break;
        }
        contas[0] = (indice == 0)?malloc(sizeof(lancamentoEntradas)):realloc(contas[0], sizeof(lancamentoEntradas)*(indice+1));
        verificaOperacao(contas[0], ERROMEM, 1);
        contas[0][indice] = temp;
        indice++;
    }
    fechaArquivo(&f);
    return indice;
}

void limpaMemoriaRealizaEntrada(fornecedor ** forn, int quantidadeFornecedores, filmes ** todosFilmes, int quantidadeFilmes){
    limpaDadosFilmeMemoria(todosFilmes[0], quantidadeFilmes);
    todosFilmes[0] = limpaMemoria(todosFilmes[0]);
    limpaMemoriaStringsFornecedor(forn[0], quantidadeFornecedores);
    forn[0] = limpaMemoria(forn[0]);
}

float CalcPrecoEntrada(filmes *filmesEntrada, int quantidade, float frete, float imposto){
    float preco = 0;

    for(int i = 0; i<quantidade; i++){
        preco += (filmesEntrada[i].valorLocacao*filmesEntrada[i].exemplares)+frete+imposto;
    }
    return preco;
}

void reescreveLancamentosEntrada(lancamentoEntradas *lancamentos, int quantidade, char *nomeArq, char *nomeArqOr, char *modo){

    FILE *f;
    f = fopen(nomeArq, modo);

    if(!f){
        printf("Erro ao salvar lancamentos no caixa! Novos dados foram perdidos!\n");
        printf("%s\n", strerror(errno));
        return;
    }

    for(int i = 0; i<quantidade; i++){

        fprintf(f, "%ld\n", lancamentos[i].codigoCompra);
        fprintf(f, "%d\n", lancamentos[i].modoPagamento);
        fprintf(f, "%.2f\n", lancamentos[i].valor);
        fprintf(f, "%.2f\n", lancamentos[i].valorPago);
        fprintf(f, "%.2f\n", lancamentos[i].troco);
        fprintf(f, "%d\n%d\n%d\n%d\n%d\n\n", lancamentos[i].data.tm_mday, lancamentos[i].data.tm_mon, lancamentos[i].data.tm_year, lancamentos[i].data.tm_hour, lancamentos[i].data.tm_min);


    }

    fechaArquivo(&f);

    if(strcmp(nomeArq, nomeArqOr)){
        remove(nomeArqOr);
        rename(nomeArq, nomeArqOr);
    }

    return;
}
void reescreveLancamentosEntradaBin(lancamentoEntradas *lancamentos, int quantidade, char *nomeArq, char *nomeArqOr, char *modo){

    FILE *f;
    f = fopen(nomeArq, modo);

    if(!f){
        printf("Erro ao salvar novos lancamentos no caixa! Novos dados foram perdidos!\n");
        printf("%s\n", strerror(errno));
        return;
    }

    for(int i = 0; i<quantidade; i++){

        fwrite(&lancamentos[i], sizeof(lancamentoEntradas), 1, f);


    }
    fechaArquivo(&f);

    if(strcmp(nomeArq, nomeArqOr)){
        remove(nomeArqOr);
        rename(nomeArq, nomeArqOr);
    }
    return;
}
entrada *encontraEntradaAvistaCodigo(entrada *ptr, int quantidade, long int codigo){
    entrada *busca = NULL;

    for(int i = 0; i<quantidade; i++){
        if(ptr[i].codigo == codigo){
            return ptr+i;
        }
    }
    return NULL;
}
entrada *buscaEntradaAvista(entrada *ptr, int quantidade, int ignoraDev){
    entrada *busca = NULL;
    long int codigo;

    while(1){
        verificaNumero(&codigo, "%ld");

        busca = encontraEntradaAvistaCodigo(ptr, quantidade, codigo);
        if(busca){

            return busca;

        }
        printf("Compra inexistente, digite um codigo valido!\n");
    }
}


void compraAvistaEntrada(entrada *Entradas, int modoAbertura){
    int escolha;
    lancamentoCaixa valorRetirar = {0,0,0,0,0,0};
    Entradas->modoPagamento = 1;
    lancamentoEntradas lancar;



    printf("Valor: %.2f\nDigite o valor pago pelo cliente:\n", Entradas->precoTotal);
    do{
        verificaNumero(&lancar.valorPago, "%f");

        if(lancar.valorPago < Entradas->precoTotal){
            printf("valor pago insuficiente!\n");
        }
    }while(lancar.valorPago< Entradas->precoTotal);

    (lancar.valorPago-Entradas->precoTotal > 0)
    ?printf("compra realizada com  sucesso!\ntroco: %.2f\n", lancar.valorPago-Entradas->precoTotal)
    :printf("compra realizada com sucesso!\n");

    Sleep(2000);

    lancar.codigoCompra = Entradas->codigo;
    lancar.modoPagamento = 1;
    lancar.data = Entradas->data;
    lancar.valor = Entradas->precoTotal;
    lancar.troco = lancar.valorPago - lancar.valor;

    (modoAbertura)?reescreveLancamentosEntradaBin(&lancar, 1, "lancamentosEntrada.bin", "lancamentosEntrada.bin", "ab"):reescreveLancamentosEntrada(&lancar, 1, "lancamentosEntrada.txt", "lancamentosEntrada.txt", "a");
    valorRetirar.codigoCompra = lancar.codigoCompra;
    valorRetirar.data = lancar.data;
    valorRetirar.modoPagamento = lancar.modoPagamento;
    valorRetirar.valor -= lancar.valor;

    (modoAbertura)
        ? reescreveLancamentosCaixaBin(&valorRetirar, 1, "lancamentos.bin", "lancamentos.bin", "ab")
        : reescreveLancamentosCaixa(&valorRetirar, 1, "lancamentos.txt", "lancamentos.txt", "a");

    return;
}

void realizaEntrada(int modoAbertura){
    filmes *filme;
    fornecedor *fornecedores;
    lancamentoCaixa *valoresCaixa = NULL;
    time_t seg;
    int escolha;
    entrada EntradaAtual;
    
    int quantidadeFilmes;
    int quantidadeFornecedores;
    int quantidadeLancamentosCaixa;
    float valorCaixa;

    quantidadeLancamentosCaixa = (modoAbertura)? leDadosLancamentosBin(&valoresCaixa): leDadosLancamentos(&valoresCaixa);
    quantidadeFornecedores = (modoAbertura)? leDadosFornecedoresBin(&fornecedores): leDadosFornecedores(&fornecedores);
    quantidadeFilmes = (modoAbertura)?leDadosFilmesBin(&filme):leDadosFilmes(&filme);

    valorCaixa = contabilizaCaixa(valoresCaixa, quantidadeLancamentosCaixa, &(struct tm){0,0,0,0,0,0,0,0,0}).dinheiroLiquido;
    valoresCaixa = limpaMemoria(valoresCaixa);

     if(!verificaExisteFornecedores(fornecedores, quantidadeFornecedores)){
        printf("Nao existem fornecedores cadastrados ate o momento!\n");
        limpaMemoriaRealizaEntrada(&fornecedores, quantidadeFornecedores, &filme, quantidadeFilmes);
        Sleep(2000);
        return;
    }

    if(!contaQuantidadeExistente(sizeof(filmes), filme, quantidadeFilmes)){
        printf("Nao existem filmes cadastrados, primeiro cadastre um filme pra depois realizar a venda!\n");
        limpaMemoriaRealizaEntrada(&fornecedores, quantidadeFornecedores, &filme, quantidadeFilmes);
        Sleep(2000);
        return;
    }
     
     mostraDadosFornecedores(fornecedores, quantidadeFornecedores);
     printf("Escolha o fornecedor: \n");
     
     EntradaAtual.compraAtual = *buscaFornecedor(fornecedores, quantidadeFornecedores);

     alocaFilmes(&EntradaAtual.lista, &EntradaAtual.quantidade, filme, quantidadeFilmes, 0, "Filme adicionado com sucesso!");

     printf("Digite o valor do frete: \n");
     verificaNumero(&EntradaAtual.frete, "%f");
     printf("Digite o valor do imposto: \n");
     verificaNumero(&EntradaAtual.imposto, "%f");

     EntradaAtual.precoTotal = CalcPrecoEntrada(EntradaAtual.lista, EntradaAtual.quantidade, EntradaAtual.frete, EntradaAtual.imposto);

    time(&seg);
    EntradaAtual.data = *localtime(&seg);
    EntradaAtual.codigo = time(NULL);


    if (valorCaixa-EntradaAtual.precoTotal > 0){
        EntradaAtual.modoPagamento = escolheMenu("Escolha o modo de pagamento", 2, 0,"A vista", "A prazo")+1;
        switch(EntradaAtual.modoPagamento){

            case 1:
                compraAvistaEntrada(&EntradaAtual, modoAbertura);
                break;
            case 2:
                realizaEntradaAprazo(&EntradaAtual, modoAbertura, valorCaixa);
                break;

        }
    } else {
        EntradaAtual.modoPagamento = escolheMenu("Escolha o modo de pagamento", 2, 0,"A prazo", "Sair")+1;
        switch(EntradaAtual.modoPagamento){
            case 1:
                realizaEntradaAprazo(&EntradaAtual, modoAbertura, valorCaixa);
                break;
            case 2:
                break;
        }
    }


    (modoAbertura)?reescreveDadosFilmeBin(filme, quantidadeFilmes):reescreveDadosFilme(filme, quantidadeFilmes);
    limpaMemoriaRealizaEntrada(&fornecedores, quantidadeFornecedores, &filme, quantidadeFilmes);
    return;
}