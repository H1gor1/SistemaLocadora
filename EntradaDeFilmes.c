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
#include <conio.h>
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
        fechaArquivo(&f);
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
        fechaArquivo(&f);
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
    if(quantidadeFilmes) {
        todosFilmes[0] = limpaMemoria(todosFilmes[0]);
    }
    limpaMemoriaStringsFornecedor(forn[0], quantidadeFornecedores);
    if(quantidadeFornecedores) {
        forn[0] = limpaMemoria(forn[0]);
    }
}

void imprimeNotinha(FILE *f, entrada *filmesEntrada, int quantidade, int quantidadeExemplares, float frete, float imposto, float precoTotal){
    fprintf(f, "Nome do fornecedor: %s\n", filmesEntrada->compraAtual.nomeFantasia);
    fprintf(f, "CNPJ do fornecedor: %s\n", filmesEntrada->compraAtual.CNPJ);
    fprintf(f, "Frete total: %f\n", frete);
    fprintf(f, "Frete por produto: %f\n", frete/quantidadeExemplares);
    fprintf(f, "Imposto total: %f\n", imposto);
    fprintf(f, "Imposto por produto: %f\n\n", imposto/quantidadeExemplares);
    for(int i=0; i < quantidade; i++){
        fprintf(f, "Filme: %s\n", filmesEntrada->lista[i].nome);
        fprintf(f, "     Preco de Custo: %f\n", filmesEntrada->lista[i].valorLocacao);
        fprintf(f, "     Quantidade: %d\n", filmesEntrada->lista[i].exemplares);
        fprintf(f, "     Preco Total pago no filme: %f\n", filmesEntrada->lista[i].valorLocacao+(frete/quantidadeExemplares)+(imposto/quantidadeExemplares));
    }
    fprintf(f, "\nPreco total da notinha: %f\n", precoTotal);
}

float CalcPrecoEntrada(entrada *filmesEntrada, int quantidade, float frete, float imposto){
    float preco = 0;
    float precoTotal = 0;
    int escolha;
    int quantidadeExemplares = 0;
    for(int i = 0; i<quantidade; i++){
        preco += (filmesEntrada->lista[i].valorLocacao*filmesEntrada->lista[i].exemplares);
        quantidadeExemplares += filmesEntrada->lista[i].exemplares;
    }
    precoTotal = preco+frete+imposto;

    imprimeNotinha(stdout, filmesEntrada, quantidade, quantidadeExemplares, frete, imposto, precoTotal);
    printf("Aperta qualquer botão para continuar: ");
    getch();

    escolha = escolheMenu("Deseja imprimir a notinha em arquivo?", 2, 0, "Sim", "Não");
    switch (escolha) {
        case 0:
            FILE *f;
            char diretorio[26] = "nota";
            char *diretorioTemp = converteIntEmString(filmesEntrada->data.tm_mday);
            strcat(diretorio, diretorioTemp);
            strcat(diretorio, "-");
            diretorioTemp = limpaMemoria(diretorioTemp);

            diretorioTemp = converteIntEmString(filmesEntrada->data.tm_mon+1);
            strcat(diretorio, diretorioTemp);
            strcat(diretorio, "-");
            diretorioTemp = limpaMemoria(diretorioTemp);

            diretorioTemp = converteIntEmString(filmesEntrada->data.tm_year+1900);
            strcat(diretorio, diretorioTemp);
            strcat(diretorio, "_");
            diretorioTemp = limpaMemoria(diretorioTemp);

            diretorioTemp = converteIntEmString(filmesEntrada->data.tm_hour);
            strcat(diretorio, diretorioTemp);
            strcat(diretorio, "-");
            diretorioTemp = limpaMemoria(diretorioTemp);

            diretorioTemp = converteIntEmString(filmesEntrada->data.tm_min);
            strcat(diretorio, diretorioTemp);
            strcat(diretorio, ".txt");
            diretorioTemp = limpaMemoria(diretorioTemp);

            f = fopen(diretorio, "w");

            imprimeNotinha(f, filmesEntrada, quantidade, quantidadeExemplares, frete, imposto, precoTotal);

            fechaArquivo(&f);
            return precoTotal;
    }
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
lancamentoEntradas *encontraEntradaAvistaCodigo(lancamentoEntradas *ptr, int quantidade, long int codigo){
    lancamentoEntradas *busca = NULL;

    for(int i = 0; i<quantidade; i++){
        if(ptr[i].codigoCompra == codigo){
            return ptr+i;
        }
    }
    return NULL;
}
lancamentoEntradas *buscaEntradaAvista(lancamentoEntradas *ptr, int quantidade){
    lancamentoEntradas *busca = NULL;
    long int codigo;

    while(1){
        verificaNumero(&codigo, "%ld");

        busca = encontraEntradaAvistaCodigo(ptr, quantidade, codigo);
        if(busca){

            return busca;

        }
        disparaSom("Compra inexistente, digite um codigo valido!", 1);

    }
}


void compraAvistaEntrada(entrada *Entradas, int modoAbertura){
    int escolha;
    lancamentoCaixa valorRetirar = {0,0,0,0,0,0};
    Entradas->modoPagamento = 1;
    lancamentoEntradas lancar;




    printf("compra realizada com sucesso!\n");

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
        printf("Nao existem filmes cadastrados, primeiro cadastre um filme pra depois realizar a entrada!\n");
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
     time(&seg);
     EntradaAtual.data = *localtime(&seg);
     EntradaAtual.codigo = time(NULL);


     float totalInvestido = 0;
     for(int i = 0; i<EntradaAtual.quantidade; i++){
         totalInvestido = EntradaAtual.lista[i].valorLocacao*EntradaAtual.lista[i].exemplares +
                 EntradaAtual.frete + EntradaAtual.imposto;

         encontraFilmeNome(filme, quantidadeFilmes,EntradaAtual.lista[i].nome)->totalInvestido+= totalInvestido;
     }

     EntradaAtual.precoTotal = CalcPrecoEntrada(&EntradaAtual, EntradaAtual.quantidade, EntradaAtual.frete, EntradaAtual.imposto);







    if (valorCaixa-EntradaAtual.precoTotal > 0){
        EntradaAtual.modoPagamento = escolheMenu("Escolha o modo de pagamento", 3, 0,"A vista", "A prazo", "Sair")+1;
        switch(EntradaAtual.modoPagamento){

            case 1:
                compraAvistaEntrada(&EntradaAtual, modoAbertura);
                break;
            case 2:
                realizaEntradaAprazo(&EntradaAtual, modoAbertura, valorCaixa);
                break;

            case 3:
                limpaMemoriaRealizaEntrada(&fornecedores, quantidadeFornecedores, &filme, quantidadeFilmes);
                return;
        }
    } else {
        EntradaAtual.modoPagamento = escolheMenu("Escolha o modo de pagamento", 2, 0,"A prazo", "Sair")+1;
        switch(EntradaAtual.modoPagamento){
            case 1:
                realizaEntradaAprazo(&EntradaAtual, modoAbertura, valorCaixa);
                break;
            case 2:
                limpaMemoriaRealizaEntrada(&fornecedores, quantidadeFornecedores, &filme, quantidadeFilmes);
                return;
        }
    }


    (modoAbertura)?reescreveDadosFilmeBin(filme, quantidadeFilmes):reescreveDadosFilme(filme, quantidadeFilmes);
    limpaMemoriaRealizaEntrada(&fornecedores, quantidadeFornecedores, &filme, quantidadeFilmes);
    return;
}
void mostraEntradaAvista(lancamentoEntradas *ptr, int quantidade){
    for(int i = 0; i<quantidade;i++){

        printf("Codigo compra: %d\n", ptr[i].codigoCompra);
        printf("Modo de pagamento: %s\n", (ptr[i].modoPagamento == 1)?"a vista":(ptr[i].modoPagamento == 2)?"a prazo":"a prazo com entrada");
        printf("Valor: %.2f\n", ptr[i].valor);
        printf("Valor pago: %.2f\n", ptr[i].valorPago);
        printf("Troco: %.2f\n", ptr[i].troco);
        printf("Data: %d/%d/%d\n", ptr[i].data.tm_mday, ptr[i].data.tm_mon, ptr[i].data.tm_year);
        printf("Horario: %d:%d\n\n\n", ptr[i].data.tm_hour, ptr[i].data.tm_min);

    }
    printf("aperte qualquer tecla para sair:\n");
    getch();

}