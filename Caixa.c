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
#define ERROMEM "ERRO: Memoria indisponivel!\n"
#define MAXIMOPARCELAS 3


void reescreveLancamentosCaixa(lancamentoCaixa *lancamentos, int quantidade, char *nomeArq, char *nomeArqOr, char *modo){
    
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
void reescreveLancamentosCaixaBin(lancamentoCaixa *lancamentos, int quantidade, char *nomeArq, char *nomeArqOr, char *modo){
    
    FILE *f;
    f = fopen(nomeArq, modo);
    
    if(!f){
        printf("Erro ao salvar novos lancamentos no caixa! Novos dados foram perdidos!\n");
        printf("%s\n", strerror(errno));
        return;
    }
    
    for(int i = 0; i<quantidade; i++){
        
        fwrite(&lancamentos[i], sizeof(lancamentoCaixa), 1, f);
        
        
    }
    fechaArquivo(&f);
    
    if(strcmp(nomeArq, nomeArqOr)){
        remove(nomeArqOr);
        rename(nomeArq, nomeArqOr);
    }
    return;
}
int leDadosLancamentosBin(lancamentoCaixa **lancamentos){
    
    FILE *f;
    f = fopen("lancamentos.bin", "rb");
    int indice = 0;
    lancamentoCaixa temp;
    
    if(!f){
        f = fopen("lancamentos.bin", "wb");
        if(!f){
            printf("Erro ao criar arquivo de lancamentos do caixa!\n");
            printf("%s\n", strerror(errno));
            return 0;
        }
        return 0;
    }
    
    while(1){
        fread(&temp, sizeof(lancamentoCaixa), 1, f);
        
        if(feof(f)){
            break;
        }
        lancamentos[0] = (indice == 0)
                            ?malloc(sizeof(lancamentoCaixa))
                            :realloc(lancamentos[0], sizeof(lancamentoCaixa)*(indice+1));

        verificaOperacao(lancamentos[0], "ERRO: Memoria indisponivel!", 1);
        lancamentos[0][indice].data = (struct tm){0,0,0,0,0,0,0,0,0};
        lancamentos[0][indice] = temp;
        
        indice++;
        
    }
    fechaArquivo(&f);
    
    return indice;
    
}
int leDadosLancamentos(lancamentoCaixa **lancamentos){
    
    FILE *f;
    f = fopen("lancamentos.txt", "r");
    int indice = 0;
    long int seg;
    if(!f){
        f = fopen("lancamentos.txt", "w");
        if(!f){
            printf("Erro ao criar arquivo de lancamentos do caixa!\n");
            printf("%s\n", strerror(errno));
            return 0;
        }
        return 0;
    }
    
    while(1){
        
        fscanf(f, "%ld ", &seg);
        
        if(feof(f)){
            break;
        }
        lancamentos[0] = (!indice)
                            ?malloc(sizeof(lancamentoCaixa)*(indice+1))
                            :realloc(lancamentos[0], sizeof(lancamentoCaixa)*(indice+1));
                    
        lancamentos[0][indice].codigoCompra = seg;
        
        fscanf(f, "%d ", &lancamentos[0][indice].modoPagamento);
        
        fscanf(f, "%f ", &lancamentos[0][indice].valor);
        
        fscanf(f, "%f ", &lancamentos[0][indice].valorPago);
        
        fscanf(f, "%f ", &lancamentos[0][indice].troco);

        lancamentos[0][indice].data = (struct tm){0,0,0,0,0,0,0,0,0};
        fscanf(f, "%d ", &lancamentos[0][indice].data.tm_mday);
        
        fscanf(f, "%d ", &lancamentos[0][indice].data.tm_mon);
        
        fscanf(f, "%d ", &lancamentos[0][indice].data.tm_year);
        
        fscanf(f, "%d ", &lancamentos[0][indice].data.tm_hour);
        
        fscanf(f, "%d  ", &lancamentos[0][indice].data.tm_min);
        
        indice++;
    }
    fechaArquivo(&f);
    return indice;
    
    
}
/*void geraNotinha(compras *ptr, float valorPago, FILE *f, int modoArm){
    
    locadora dadosLocadora;
    
    (modoArm)?leDadosLocadoraBin(&dadosLocadora, "locadora.bin"):leDadosLocadora(&dadosLocadora, "locadora.txt");
    
    fprintf(f, "Nome fantasia: %s\n", dadosLocadora.nomeFantasia);
    fprintf(f, "Razao social: %s\n", dadosLocadora.razaoSocial);
    fprintf(f, "CNPJ: %s\n", dadosLocadora.cnpj);
    fprintf(f, "Tel: %d\n", dadosLocadora.telefone);
    fprintf(f, "E-mail: %s\n", dadosLocadora.email);
    fprintf(f, "Endereco completo: %s\n", dadosLocadora.endereco);
    
    fprintf(f, "Dados de compras:\n");
    fprintf(f, "Vendedor: %s\n", ptr->vendedor->nome);
    fprintf(f, "Codigo Vendedor: %d\n", ptr->vendedor->codigo);
    fprintf(f, "Comprador: %s\n", ptr->comprador->nomeCompleto);
    fprintf(f, "codigo Comprador: %d\n", ptr->comprador->codigo);
    fprintf(f, "Quantidade: %d alocacoes\n", ptr->quantidadeFilmesComprados);
    for(int i = 0; i<ptr->quantidadeFilmesComprados; i++){
        fprintf(f, "%dº filme:\n", i+1);
        fprintf(f, "    codigo: %d\n", ptr->filmesComprados[i].codigo);
        fprintf(f, "    nome:   %s\n", ptr->filmesComprados[i].nome);
        fprintf(f, "    valor unitario: %.2f\n", ptr->filmesComprados[i].valorLocacao);
        fprintf(f, "    quantidade: %d\n", ptr->filmesComprados[i].exemplares);
    }
    fprintf(f, "Valor total: %.2f\n", ptr->preco);
    fprintf(f, "Valor pago: %.2f\n", valorPago);
    if(valorPago - ptr->preco > 0){
        fprintf(f, "Troco: %.2f\n", valorPago-ptr->preco);
    }
    fprintf(f, "Data da compra: %d/%d/%d\n", ptr->data.tm_mday, ptr->data.tm_mon+1, ptr->data.tm_year+1900);
    fprintf(f, "Horario: %d:%d\n", ptr->data.tm_hour, ptr->data.tm_min);
    fprintf(f, "Agradecemos a compra, volte sempre!\n");
    
    limpaCamposLocadoraMemoria(&dadosLocadora);
    
}
*/
void compraAvista(compras *compras, int modoArm){
    int escolha;
    
    compras->modoPagamento = 1;
    lancamentoCaixa lancar;
   
    
   
    printf("Valor: %.2f\nDigite o valor pago pelo cliente:\n", compras->preco);
    do{
        verificaNumero(&lancar.valorPago, "%f");
        
        if(lancar.valorPago < compras->preco){

            disparaSom("valor pago insuficiente!", 1);
        }
    }while(lancar.valorPago< compras->preco);
    
    (lancar.valorPago-compras->preco > 0)
            ?printf("compra realizada com  sucesso!\ntroco: %.2f\n", lancar.valorPago-compras->preco)
            :printf("compra realizada com sucesso!\n");
    
    
    
    Sleep(2000);


    lancar.codigoCompra = compras->codigo;
    lancar.modoPagamento = 1;
    lancar.data = compras->data;
    lancar.valor = compras->preco;
    lancar.troco = lancar.valorPago - lancar.valor;
    
    (modoArm)?reescreveLancamentosCaixaBin(&lancar, 1, "lancamentos.bin", "lancamentos.bin", "ab"):reescreveLancamentosCaixa(&lancar, 1, "lancamentos.txt", "lancamentos.txt", "a");
    
   
    return;
}

float calculaPreco(filmes *filmesAlocados, int quantidade){
    
    float preco = 0;
    
    for(int i = 0; i<quantidade; i++){
        preco += filmesAlocados[i].valorLocacao*filmesAlocados[i].exemplares;
    }
    return preco;
}
float calculaTroco(float valorPagar, float *valorPago){
    
    while(1){
        verificaNumero(valorPago, "%f");
        if(*valorPago > valorPagar){
            return *valorPago-valorPagar;
        }
        printf("valor pago insuficiente!\n");
    }
}
void mostraLancamentos(lancamentoCaixa *todosLancamentos, int quantidade, struct tm* data){
    
    for(int i = 0; i<quantidade; i++){
        if((todosLancamentos[i].data.tm_mday == data->tm_mday && todosLancamentos[i].data.tm_mon == data->tm_mon && todosLancamentos[i].data.tm_year == data->tm_year)
                || !(data->tm_mday || data->tm_mon || data->tm_year)){
            if(todosLancamentos[i].valor > 0) {
                printf("_________________________________________________________\n");
                printf("|Codigo compra: %ld\n", todosLancamentos[i].codigoCompra);
                printf("|Modo pagamento: %s\n", (todosLancamentos[i].modoPagamento == 1)?"A vista":(todosLancamentos[i].modoPagamento == 2)?"A prazo":(todosLancamentos[i].modoPagamento == 3)?"A prazo com entrada":"valor lancado por administrador");
                printf("|Valor: %.2f\n", todosLancamentos[i].valor);
                printf("|Valor pago: %.2f\n", todosLancamentos[i].valorPago);
                printf("|Troco: %.2f\n", todosLancamentos[i].troco);
                printf("|Data: %d/%d/%d\n", todosLancamentos[i].data.tm_mday, todosLancamentos[i].data.tm_mon + 1,
                       todosLancamentos[i].data.tm_year + 1900);
                printf("|Horario: %d:%d\n", todosLancamentos[i].data.tm_hour, todosLancamentos[i].data.tm_min);
                printf("_________________________________________________________");
            }else{
                printf("_________________________________________________________\n");
                printf("|Esta é uma retirada do caixa para uma entrada de filme\n");
                printf("|Codigo compra: %ld\n", todosLancamentos[i].codigoCompra);
                printf("|Modo pagamento: %s\n", (todosLancamentos[i].modoPagamento == 1)?"A vista":(todosLancamentos[i].modoPagamento == 2)?"A prazo":"A prazo com entrada");
                printf("|Valor: %.2f\n", -1*todosLancamentos[i].valor);
                printf("|Valor pago: %.2f\n", todosLancamentos[i].valorPago);
                printf("|Troco: %.2f\n", todosLancamentos[i].troco);
                printf("|Data: %d/%d/%d\n", todosLancamentos[i].data.tm_mday, todosLancamentos[i].data.tm_mon + 1,
                       todosLancamentos[i].data.tm_year + 1900);
                printf("|Horario: %d:%d\n", todosLancamentos[i].data.tm_hour, todosLancamentos[i].data.tm_min);
                printf("_________________________________________________________");
            }
        }
    }
}

ContagemCaixa contabilizaCaixa(lancamentoCaixa *todosLancamentos, int quantidade, struct tm* data){
    
    ContagemCaixa cont = {0,0,0,0};
    
    
    for(int i = 0; i<quantidade; i++){
        
        if((todosLancamentos[i].data.tm_mday == data->tm_mday && todosLancamentos[i].data.tm_mon == data->tm_mon && todosLancamentos[i].data.tm_year == data->tm_year)
                || !(data->tm_mday || data->tm_mon || data->tm_year)){
        cont.dinheiroBruto += todosLancamentos[i].valorPago;
        cont.dinheiroLiquido += todosLancamentos[i].valor;
        cont.troco += todosLancamentos[i].troco;

        if(todosLancamentos[i].valor <0){
            cont.ValoresRetirados += -1*todosLancamentos[i].valor;
        }
        
        }
    }
    
    return cont;
}
lancamentoCaixa *procuraLancamentoCaixa(lancamentoCaixa *ptr, int quantidade, long int codigo){
    for(int i = 0; i<quantidade; i++){
        if(ptr[i].codigoCompra == codigo){
            if(ptr[i].modoPagamento) {
                return ptr + i;
            }else{
                printf("este lancamento e somente")
            }
        }
    }
    return NULL;
}
void contaCaixa(int modoArm){
    
    lancamentoCaixa *todosLancamentos = NULL;
    int quantidadeLancamentos;
    ContagemCaixa valores;
    quantidadeLancamentos = (modoArm)?leDadosLancamentosBin(&todosLancamentos):leDadosLancamentos(&todosLancamentos);
    struct tm data;
    printf("digite a data completa em que deseja conferir a contagem do caixa.\nSe desejar somar todos os valores do caixa, digite 0/0/0\n");
    printf("digite o mes em que deseja conferir a contagem do caixa:\n");
    verificaLimiteNumero(&data.tm_mon, 12, 0, "%d");
    printf("digite o dia:\n");
    verificaLimiteNumero(&data.tm_mday, (data.tm_mon == 2)?28:31, 0, "%d");
    printf("digite o ano:\n");
    verificaNumero(&data.tm_year, "%d");
    if(data.tm_mday && data.tm_mon && data.tm_year){
        data.tm_mon--;
        data.tm_year -= 1900;
    }
    valores = contabilizaCaixa(todosLancamentos, quantidadeLancamentos, &data);
    mostraLancamentos(todosLancamentos, quantidadeLancamentos, &data);
    
    printf("\n\nContagem total:\n   valor liquido: %.2f\n   valor bruto: %.2f\n     trocos totais: %.2f\nValores retirados: %.2f\n", valores.dinheiroLiquido, valores.dinheiroBruto, valores.troco, valores.ValoresRetirados);

    printf("digite qualquer tecla para sair.\n");
    getch();
    
    todosLancamentos = limpaMemoria(todosLancamentos);
}

void filtraMovimentacoesPorPeriodoData(int modoArm){

    lancamentoCaixa *todosLancamentos = NULL;
    compras *todasAsCompras = NULL;
    Funcionarios *funcs = NULL;
    filmes *todosFilmes = NULL;
    cliente *clientes = NULL;
    lancamentoEntradas *todasEntradas = NULL;
    contaApag *todasAsEntradasAprazo = NULL;
    lancamentoCaixa *lancamentosFiltrados = NULL;

    int quantidadeLancamentosFiltrados = 0;
    int quantidadeEntradasAprazo = 0;
    int quantidadeEntradas = 0;
    quantidades quant = {0,0,0,0};

    int quantidadeLancamentos = 0;
    quant = (modoArm)? leDadosDevolucoesBin(&todasAsCompras, &funcs, &clientes, &todosFilmes):leDadosDevolucoes(&todasAsCompras, &funcs, &clientes, &todosFilmes);

    quantidadeEntradas = (modoArm)? leDadosEntradasFilmesBin(&todasEntradas): leDadosLancamentoEntradasFilmes(&todasEntradas);
    quantidadeLancamentos = (modoArm)? leDadosLancamentosBin(&todosLancamentos): leDadosLancamentos(&todosLancamentos);
    quantidadeEntradasAprazo = (modoArm)?leDadosEntradasAprazoBin(&todasAsEntradasAprazo): leDadosEntradasAprazo(&todasAsEntradasAprazo);

    struct tm data1 = {0,0,0,0,0,0,0,0,0};
    struct tm data2 = {0,0,0,0,0,0,0,0,0};
    time_t tempoSegData1;
    time_t tempoSegData2;
    data1.tm_mon = escolheMenu("Qual mes da data inicial?", 12, 0, "Janeiro", "Fevereiro", "Marco", "Abril", "Maio" ,"Junho", "Julho", "Agosto", "Setembro", "Outubro", "Novembro", "Dezembro");

    if(data1.tm_mon%2!= 0){
        data1.tm_mday = escolheMenu("Qual o dia?",
                                    30,0,
                                    "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "30");


    }else
        if(data1.tm_mon == 2){
            data1.tm_mday = escolheMenu("Qual o dia?",
                                        28,0,
                                        "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27", "28");


        }else{
            data1.tm_mday  = escolheMenu("Qual o dia?",
                                                         30,0,
                                                         "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "30", "31");


        };

        printf("e qual o ano?\n");
    verificaNumero(&data1.tm_year, "%d");
    data2.tm_mon =  escolheMenu("qual mes da data final?", 12, 0, "Janeiro", "Fevereiro", "Marco", "Abril", "Maio" ,"Junho", "Julho", "Agosto", "Setembro", "Outubro", "Novembro", "Dezembro");

    if(data2.tm_mon %2!= 0){
        data2.tm_mday = escolheMenu("Qual o dia?",
                                                    30,0,
                                                    "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "30");

    }else
        if(data2.tm_mon == 2){
            data2.tm_mday = escolheMenu("Qual o dia?",
                                        28,0,
                                        "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27", "28");

        }else{
            data2.tm_mday = escolheMenu("Qual o dia?",
                                        31,0,
                                        "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "30", "31");

        }

        printf("e qual o ano?\n");
    verificaNumero(&data2.tm_year, "%d");
    data1.tm_year -=1900;
    data2.tm_year -=1900;

    tempoSegData1 = mktime(&data1);
    tempoSegData2 = mktime(&data2);

    for(int i = 0; i<quantidadeLancamentos; i++){

        if(mktime(&todosLancamentos[i].data) >= tempoSegData1 && mktime(&todosLancamentos[i].data) <= tempoSegData2){

            lancamentosFiltrados = (!quantidadeLancamentosFiltrados)
                    ?malloc(sizeof(lancamentoCaixa))
                    :realloc(lancamentosFiltrados, sizeof(lancamentoCaixa)*(quantidadeLancamentosFiltrados+1));


            lancamentosFiltrados[quantidadeLancamentosFiltrados] = todosLancamentos[i];
            mostraLancamentos(lancamentosFiltrados+quantidadeLancamentosFiltrados, 1, &(struct tm){0,0,0,0,0,0,0,0,0});
            quantidadeLancamentosFiltrados++;
        }
    }
    printf("\nquantidade de lancamentos neste periodo: %d\n", quantidadeLancamentosFiltrados);

    printf("Para consultar algumas destes lancamentos, digite o codigo da compra, caso nao queira, digite um codigo invalido:\n");
    long int codigo;
    lancamentoCaixa  *lancametoProcurado = NULL;
    verificaNumero(&codigo, "%ld");
    lancametoProcurado = procuraLancamentoCaixa(lancamentosFiltrados, quantidadeLancamentosFiltrados, codigo);

    if(lancametoProcurado->valor > 0){

        compras compraProcurada = *encontraCompraCodigo(todasAsCompras, quant.quantidadeAlugacoes, lancametoProcurado->codigoCompra);
        mostraCompra(&compraProcurada, 1);
    }else
        if(lancametoProcurado->modoPagamento == 1){

            lancamentoEntradas entradaProc = *encontraEntradaAvistaCodigo(todasEntradas, quantidadeEntradas, lancametoProcurado->codigoCompra);
            mostraEntradaAvista(&entradaProc, 1);
        }else{

            contaApag entAprazo = *encontraEntradaCodigo(todasAsEntradasAprazo, quantidadeEntradasAprazo, lancametoProcurado->codigoCompra);
            mostraEntradaAtrasada(&entAprazo, 1);
        }



        limpaDadosDevolucoesMemoria(&todosFilmes, &clientes, &funcs, &todasAsCompras, quant);
        todosLancamentos = limpaMemoria(todosLancamentos);
        todasAsEntradasAprazo = limpaMemoria(todasAsEntradasAprazo);
        todasEntradas = limpaMemoria(todasEntradas);
        lancamentosFiltrados = limpaMemoria(lancamentosFiltrados);
        lancametoProcurado = NULL;


}
void lancaValorCaixa(int modoArm){

    printf("ATENCAO: o valor informado será adicionado no caixa, certfique-se que digitou o valor correto!\n");
    disparaSom("\0", 1);
    Sleep(1000);

    lancamentoCaixa adicionar = (lancamentoCaixa){0,0,0,0,0,(struct tm){0,0,0,0,0,0,0,0,0}};
    time_t seg;
    time(&seg);
    adicionar.data = *localtime(&seg);

    printf("digite o valor que deseja adicionar ao caixa:\n");
    verificaNumero(&adicionar.valor, "%f");

    adicionar.modoPagamento = 0;
    adicionar.codigoCompra = time(NULL);

    (modoArm)? reescreveLancamentosCaixaBin(&adicionar, 1, "lancamentos.bin", "lancamentos.bin", "ab"): reescreveLancamentosCaixa(&adicionar, 1, "lancamentos.txt", "lancamentos.txt", "a");



}