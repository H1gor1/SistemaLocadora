#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include "ControleCategorias.h"
#include "ManipulacoesDeTexto.h"
#include "ControleDeDadosLocadora.h"
#include "ControleDeNumeros.h"
#include "FuncUtilitarias.h"
#include "menus.h"
#include "ControleFilmes.h"
#include "controleFuncionarios.h"
#include "ControleClientes.h"
#include "EntradaDeFilmes.h"
#include <ctype.h>
#include <locale.h>
#include <conio.h>
#include <windows.h>
#include "Caixa.h"
#define ERROMEM "ERRO: Memoria indisponivel!\n"
#define MAXIMOPARCELAS 3
#include "ContasAPagar.h"

void reescreveEntradaAprazo(contaApag *contas, int quantidade, char *nomeArq, char *nomeArqOr, char *modo){
    FILE *f;
    f = fopen(nomeArq, modo);

    if(!f){
        printf("Erro ao salvar novos dados! Dados recem alterados foram perdidos!\n");
        printf("%s\n", strerror(errno));
        return;
    }

    for(int i = 0; i<quantidade; i++){
        fprintf(f, "%ld\n", contas[i].codigoCompra);
        fprintf(f, "%d\n", contas[i].modoPagamento);
        fprintf(f, "%d\n", contas[i].parcelas);
        fprintf(f, "%.2f\n", contas[i].valorParc);
        fprintf(f, "%f\n", contas[i].entrada);
        fprintf(f, "%d\n%d\n%d\n%d\n%d\n\n", contas[i].dataAluga.tm_mday, contas[i].dataAluga.tm_mon, contas[i].dataAluga.tm_year, contas[i].dataAluga.tm_hour, contas[i].dataAluga.tm_min);
    }
    fechaArquivo(&f);

    if(strcmp(nomeArq, nomeArqOr) != 0){
        remove(nomeArqOr);
        rename(nomeArq, nomeArqOr);
    }
}

void reescreveEntradasAprazoBin(contaApag *contas, int quantidade, char *nomeArq, char *nomeArqOr, char *modo){
    FILE *f;
    f = fopen(nomeArq, modo);

    if(!f){
        printf("Erro ao salvar novos dados! Dados recem alterados foram perdidos!\n");
        printf("%s\n", strerror(errno));
        return;
    }

    for(int i = 0; i<quantidade; i++){
        fwrite(&contas[i], sizeof(contaApag), 1, f);
    }

    fechaArquivo(&f);

    if(strcmp(nomeArq, nomeArqOr) != 0){
        remove(nomeArqOr);
        rename(nomeArq, nomeArqOr);
    }
    return;
}

int leDadosEntradasAprazo(contaApag **contas){
    FILE *f;
    int indice = 0;
    long int temp;
    f = fopen("entradaAprazo.txt", "r");

    if(!f){
        f = fopen("entradaAprazo.txt", "w");
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

        contas[0] = (indice == 0)?malloc(sizeof(contaApag)):realloc(contas[0], sizeof(contaApag)*(indice+1));
        verificaOperacao(contas[0], ERROMEM, 1);

        contas[0][indice].codigoCompra = temp;

        fscanf(f, "%d ", &contas[0][indice].modoPagamento);
        fscanf(f, "%d ", &contas[0][indice].parcelas);
        fscanf(f, "%f ", &contas[0][indice].valorParc);
        fscanf(f, "%f ", &contas[0][indice].entrada);
        contas[0][indice].dataAluga = (struct tm){0,0,0,0,0,0,0,0,0};
        fscanf(f, "%d ", &contas[0][indice].dataAluga.tm_mday);
        fscanf(f, "%d ", &contas[0][indice].dataAluga.tm_mon);
        fscanf(f, "%d ", &contas[0][indice].dataAluga.tm_year);
        fscanf(f, "%d ", &contas[0][indice].dataAluga.tm_hour);
        fscanf(f, "%d  ", &contas[0][indice].dataAluga.tm_min);

        indice++;
    }
    fechaArquivo(&f);
    return indice;
}

int leDadosEntradasAprazoBin(contaApag **contas){
    FILE *f;
    int indice = 0;
    contaApag temp;
    f = fopen("entradaAprazo.bin", "rb");

    if(!f){
        f = fopen("entradaAprazo.bin", "wb");
        if(!f){
            printf("Erro ao criar o arquivo de lancamento a prazo!\n");
            printf("%s\n", strerror(errno));
            return 0;
        }
        fechaArquivo(&f);
        return 0;
    }

    while(1){
        fread(&temp, sizeof(contaApag), 1, f);
        if(feof(f)){
            break;
        }
        contas[0] = (indice == 0)?malloc(sizeof(contaApag)):realloc(contas[0], sizeof(contaApag)*(indice+1));
        verificaOperacao(contas[0], ERROMEM, 1);
        contas[0][indice].dataAluga = (struct tm){0,0,0,0,0,0,0,0,0};
        contas[0][indice] = temp;
        indice++;
    }
    fechaArquivo(&f);
    return indice;
}

void LancaEntradaOuParcela_EntradaFilmes(contaApag *contaP, int modoArm, float preco){

    time_t seg;
    time(&seg);
    lancamentoEntradas lancamento;

    lancamento.codigoCompra = contaP->codigoCompra;
    lancamento.valor = preco;
    lancamento.modoPagamento = contaP->modoPagamento;
    lancamento.data = *localtime(&seg);



    (modoArm)
        ?reescreveLancamentosEntradaBin(&lancamento, 1, "lancamentosEntrada.bin", "lancamentosEntrada.bin", "ab")
        :reescreveLancamentosEntrada(&lancamento, 1, "lancamentosEntrada.txt", "lancamentosEntrada.txt", "a");
    return;
}

void realizaEntradaAprazo(entrada *entradas, int modoArm, float valorCaixa){
    contaApag Aprazo;

    Aprazo.dataAluga = entradas->data;
    Aprazo.codigoCompra = entradas->codigo;
    double porcentagem = 0.30 * entradas->precoTotal;


    if(porcentagem > valorCaixa){
        porcentagem = (valorCaixa/entradas->precoTotal)*entradas->precoTotal;
    }

    if(porcentagem > 0){
        printf("Digite o valor de entrada:\n");
        verificaLimiteNumero(&Aprazo.entrada, porcentagem, 0, "%f");
    }

    printf("Digite a quantidade de parcelas deseja dividir:\n");
    verificaLimiteNumero(&Aprazo.parcelas, MAXIMOPARCELAS, 1, "%d");

    Aprazo.valorParc = (entradas->precoTotal-Aprazo.entrada) / Aprazo.parcelas;

    if(Aprazo.entrada>0){
        entradas->modoPagamento = 3;
        Aprazo.modoPagamento = 3;
        LancaEntradaOuParcela_EntradaFilmes(&Aprazo, modoArm, Aprazo.entrada);

    }else{
        entradas->modoPagamento = 2;
        Aprazo.modoPagamento = 2;
    }

    (modoArm == 1)
    ?reescreveEntradasAprazoBin(&Aprazo, 1, "entradaAprazo.bin", "entradaAprazo.bin", "ab")
    :reescreveEntradaAprazo(&Aprazo, 1, "entradaAprazo.txt", "entradaAprazo.txt", "a");

    if(entradas->modoPagamento == 3){
        lancamentoCaixa caixa = {0,0,0,0,0,0,0,0,0,0, 0, 0, 0,0};
        caixa.modoPagamento = entradas->modoPagamento;
        caixa.codigoCompra = entradas->codigo;
        caixa.data = entradas->data;
        caixa.valor -= Aprazo.entrada;

        (modoArm)
        ? reescreveLancamentosCaixaBin(&caixa, 1, "lancamentos.bin", "lancamentos.bin", "ab")
        : reescreveLancamentosCaixa(&caixa, 1, "lancamentos.txt", "lancamentos.txt", "a");
    }
    return;
}

contaApag *encontraEntradaCodigo(contaApag *conta, int quantidade, long int codigo){
    for(int i=0; i<quantidade; i++){
        if(conta[i].codigoCompra == codigo && conta[i].parcelas > 0){
            return (conta+i);
        }
    }
    return NULL;
}



void consultaEntradas(contaApag *contas, int quantidade){
    for(int i = 0; i<quantidade; i++){
        if(contas[i].parcelas>0){
            printf("_______________________________________________\n");
            printf("|Codigo Compra: %ld\n", contas[i].codigoCompra);
            printf("|Modo de pagamento: %s\n", (contas[i].modoPagamento == 2)?"A prazo":"A prazo com entrada");
            printf("|Quantidade parcelas: %d\n", contas[i].parcelas);
            printf("|Valor Cada Parcela: %.2f\n", contas[i].valorParc);
            printf("|Valor da entrada: %.2f\n", contas[i].entrada);
            printf("|Data: %2d/%2d/%4d\n", contas[i].dataAluga.tm_mday, contas[i].dataAluga.tm_mon+1, contas[i].dataAluga.tm_year+1900);
            printf("|Horario: %d:%d\n\n", contas[i].dataAluga.tm_hour, contas[i].dataAluga.tm_min);
            printf("__________________________________________________\n");
        }
    }
}

int BaixaEntradasFilmes(int modoArm){
    contaApag *contas = NULL;
    contaApag *darBaixa = NULL;
    lancamentoCaixa *todosLancamentosCaixa = NULL;
    int quantidadeLancamentosCaixa = 0;
    int quantidade = 0;
    int quantidadesNotinhasPagar = 0;
    long int codigo;
    time_t tempo;
    quantidadeLancamentosCaixa = (modoArm)? leDadosLancamentosBin(&todosLancamentosCaixa): leDadosLancamentos(&todosLancamentosCaixa);
    quantidade = (modoArm)?leDadosEntradasAprazoBin(&contas):leDadosEntradasAprazo(&contas);
    float valorCaixa = contabilizaCaixa(todosLancamentosCaixa, quantidadeLancamentosCaixa, &(struct tm){0,0,0,0,0,0,0,0,0}).dinheiroLiquido;
    printf("Digite o codigo da compra que deseja dar baixa:\n");
    consultaEntradas(contas, quantidade);
    while(1){
        verificaNumero(&codigo, "%ld");
        darBaixa = encontraEntradaCodigo(contas, quantidade, codigo);
        if(!darBaixa){
            printf("Nao existe nenhuma compra com este codigo ou a compra selecionada ja foi paga!\n");
            continue;
        }
        if(valorCaixa < darBaixa->valorParc){
            printf("Nao é possivel pagar essa conta com o valor atual do caixa!\n");
            Sleep(2000);
            continue;
        }
        break;
    }


    printf("Digite a quantidade de parcelas que deseja baixar desta compra, voce pode pagar no maximo %.0f parcelas:", (valorCaixa/darBaixa->valorParc>4)?3:valorCaixa/darBaixa->valorParc);
    verificaLimiteNumero(&quantidadesNotinhasPagar, valorCaixa/darBaixa->valorParc, 1, "%d");
    tempo = mktime(&darBaixa->dataAluga)+86400*30*quantidadesNotinhasPagar;
    darBaixa->dataAluga = *localtime(&tempo);
    LancaEntradaOuParcela_EntradaFilmes(darBaixa, modoArm, darBaixa->valorParc*quantidadesNotinhasPagar);
    darBaixa->parcelas -= quantidadesNotinhasPagar;

    lancamentoCaixa parcelaAbatida = (lancamentoCaixa){0,0,0,0,0,(struct tm){0,0,0,0,0,0,0,0,0}};
    parcelaAbatida.data = darBaixa->dataAluga;
    parcelaAbatida.codigoCompra = darBaixa->codigoCompra;
    parcelaAbatida.modoPagamento = darBaixa->modoPagamento;
    parcelaAbatida.valor = darBaixa->valorParc*-1*quantidadesNotinhasPagar;


    if(modoArm) {
        reescreveEntradasAprazoBin(contas, quantidade, "entradaAprazoRes.bin", "entradaAprazo.bin", "wb");
        reescreveLancamentosCaixaBin(&parcelaAbatida, 1, "lancamnetos.bin", "lancamentos.bin", "ab");
    }else{
        reescreveEntradaAprazo(contas, quantidade, "entradaAprazoRes.txt", "entradaAprazo.txt", "w");
        reescreveLancamentosCaixa(&parcelaAbatida, 1, "lancamentos.txt", "lancamentos.txt", "a");
    }
    todosLancamentosCaixa = limpaMemoria(todosLancamentosCaixa);
    return quantidadesNotinhasPagar;
}

int filtraEntradasAtrasadas(contaApag *contas, int quantidadeContas, contaApag **atrasadas, int modoArm){

    int quantidadeContasAtrasadas = 0;
    time_t seg;

    time(&seg);

    for(int i = 0; i<quantidadeContas; i++){
        if(difftime(seg, mktime(&contas[i].dataAluga)) >= 2629800){

            atrasadas[0] = (!quantidadeContasAtrasadas)?malloc(sizeof(contaApag)):realloc(atrasadas[0], sizeof(contaApag)*(quantidadeContasAtrasadas+1));

            atrasadas[0][quantidadeContasAtrasadas] = contas[i];
            quantidadeContasAtrasadas++;

        }
    }
    return quantidadeContasAtrasadas;
}

int filtraEntradasQuantidadeParcelas(contaApag *contas, int quantidadeContas, contaApag **contasRetornar, int parcelas){
    int quantidadeContasComFiltro = 0;
    parcelas = escolheMenu("Escolha a quantidade de parcelas", 3, 0,"Uma parcela", "Duas parcelas", "Tres parcelas")+1;

    for(int i = 0; i<quantidadeContas; i++){
        if(contas[i].parcelas == parcelas){
            contasRetornar[0] = (!quantidadeContasComFiltro)?malloc(sizeof(contaApag)):realloc(contasRetornar[0], sizeof(contaApag)*(quantidadeContasComFiltro+1));
            verificaOperacao(contasRetornar[0], "ERRO: Memoria indisponivel", 1);

            contasRetornar[0][quantidadeContasComFiltro] = contas[i];
            quantidadeContasComFiltro++;
        }
    }
    return quantidadeContasComFiltro;
}

contaApag *buscaEntrada(contaApag *ptr, int quantidade, int ignoraDev){
    contaApag *busca = NULL;
    long int codigo;

    while(1){
        verificaNumero(&codigo, "%d");

        busca = encontraEntradaCodigo(ptr, quantidade, codigo);
        if(busca){

            return busca;

        }
        printf("Compra inexistente, digite um codigo valido!\n");
    }
}

void mostraEntrada(entrada *compra){
    printf("Codigo compra: %ld\n", compra->codigo);
    printf("Modo pagamento: %s\n", (compra->modoPagamento == 1)?"a vista":(compra->modoPagamento == 2)?"a prazo":"a prazo com entrada");
    printf("Data: %d/%d/%d\n", compra->data.tm_mday, compra->data.tm_mon+1, compra->data.tm_year+1900);
    printf("Horario: %d:%d\n", compra->data.tm_hour, compra->data.tm_min);


    printf("Preco: %.2f\n", compra->precoTotal);
    printf("Filmes Comprados:\n");
    for(int i = 0; i<compra->quantidade; i++){
        printf("    %d filme:\n", i+1);
        printf("        Nome: %s\n", compra->lista[i].nome);
        printf("        Codigo: %d\n", compra->lista[i].codigo);
        printf("        Preco: %.2f\n", compra->lista[i].valorLocacao);
        printf("        Quantidade: %d\n", compra->lista[i].exemplares);
    }

}

void mostraEntradaAtrasada(contaApag *compra, int quantidade) {
    system("clear");
    for (int i = 0; i < quantidade; i++) {
        printf("Codigo da compra: %ld\n", compra[i].codigoCompra);
        printf("Modo pagamento: %s\n",
               (compra->modoPagamento == 1) ? "a vista" : (compra[i].modoPagamento == 2) ? "a prazo"
                                                                                       : "a prazo com entrada");
        printf("Quantidade de parcelas: %d\n", compra[i].parcelas);
        printf("Valor cada parcela: %.2f\n", compra[i].valorParc);
        printf("Valor entrada: %.2f\n", compra[i].entrada);
        printf("Data: %d/%d/%d\n", compra[i].dataAluga.tm_mday, compra[i].dataAluga.tm_mon, compra[i].dataAluga.tm_year);
        printf("Hora: %d/%d\n", compra[i].dataAluga.tm_hour, compra[i].dataAluga.tm_min);
    }
    printf("Aperte qualquer tecla para sair:\n");
    getch();
}


void mostraEntradasFiltradas(contaApag *contas, int quantidadeContas, int modoArm, int (*filtra)(contaApag *, int, contaApag **, int), char *mensagem){
    contaApag *entradasAtrasadas = NULL;
    int quantidadeEntradasAtrasadas = 0;
    quantidadeEntradasAtrasadas = (*filtra)(contas, quantidadeContas, &entradasAtrasadas, modoArm);

    if(!quantidadeEntradasAtrasadas){

        if(filtra == filtraEntradasQuantidadeParcelas) {
            printf("nao existem compras que restam essa quantidade de parcelas!\n");
        }else{
            printf("nao existem compras atrasadas!\n");
        }
        Sleep(2000);
        return;
    }

    int escolha;



    contaApag *compraEspecifica = NULL;

    menuGraphics(2, "escolha uma opcao:", "consultar uma compra", "sair");
    printf("%s", mensagem);
    consultaEntradas(entradasAtrasadas, quantidadeEntradasAtrasadas);
    while(1){

        escolha = escolheOpcao();

        switch(escolha){
            case 59:
                printf("digite o codigo da compra:\n");
                compraEspecifica = buscaEntrada(entradasAtrasadas, quantidadeEntradasAtrasadas, 0);
                mostraEntradaAtrasada(compraEspecifica,1);
                break;
            case 60:
                break;
            default:

                disparaSom("escolha uma opcao valida!", 1);
                continue;
        }
        break;
    }
    entradasAtrasadas = limpaMemoria(entradasAtrasadas);
    compraEspecifica = NULL;

}

void consultaLancamentosEntrada(int modoArm){
    contaApag *contas = NULL;
    int quantidadeContas;
    int escolha;
    quantidadeContas = (modoArm)?leDadosEntradasAprazoBin(&contas):leDadosEntradasAprazo(&contas);


    escolha = escolheMenu("Escolha o modo de filtragem", 3, 0,"Contas atrasadas", "parcelas restantes","Voltar");
    switch(escolha){
        case 0:
            mostraEntradasFiltradas(contas, quantidadeContas, modoArm, filtraEntradasAtrasadas, "Contas atrasadas:\n");
            break;
        case 1:
            mostraEntradasFiltradas(contas, quantidadeContas, modoArm, filtraEntradasQuantidadeParcelas, "Contas pela quantidade de parcelas");
            break;
        case 2:
            break;
    }
    contas = limpaMemoria(contas);
}

