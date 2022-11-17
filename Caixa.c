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
#include "menus.h"
#include "ControleFilmes.h"
#include "controleFuncionarios.h"
#include "ControleClientes.h"
#include "Caixa.h"
#include <ctype.h>
#include <locale.h>
#include <windows.h>
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
    time_t seg;
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
            printf("valor pago insuficiente!\n");
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
                printf("|Modo pagamento: %s\n", (todosLancamentos[i].modoPagamento == 1)?"A vista":(todosLancamentos[i].modoPagamento == 2)?"A prazo":"A prazo com entrada");
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
