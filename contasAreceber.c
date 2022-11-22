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
#include "Caixa.h"
#include "ContasAreceber.h"
#include "Devolucoes.h"
#include <ctype.h>
#include <locale.h>
#include <windows.h>
#define ERROMEM "ERRO: Memoria indisponivel!\n"
#define MAXIMOPARCELAS 3

void reescreveLancamentosAprazo(contaArec *contas, int quantidade, char *nomeArq, char *nomeArqOr, char *modo){
    FILE *f;
    
    f = fopen(nomeArq, modo);
    
    if(!f){
        printf("Erro ao salvar novos dados! Dados recem alterados foram perdidos!\n");
        printf("%s\n", strerror(errno));
        return;
    }
    
    for(int i = 0; i<quantidade; i++){
        fprintf(f, "%d\n", contas[i].codigoCliente);
        
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
void reescreveLancamentosAprazoBin(contaArec *contas, int quantidade, char *nomeArq, char *nomeArqOr, char *modo){
    
    FILE *f;
    
    f = fopen(nomeArq, modo);
    
    if(!f){
        
        printf("Erro ao salvar novos dados! Dados recem alterados foram perdidos!\n");
        printf("%s\n", strerror(errno));
        return;
    }
    
    for(int i = 0; i<quantidade; i++){
        fwrite(&contas[i], sizeof(contaArec), 1, f);
    }
    
    fechaArquivo(&f);
    
    if(strcmp(nomeArq, nomeArqOr) != 0){
        remove(nomeArqOr);
        rename(nomeArq, nomeArqOr);
    }
    return;
}
int leDadosLancamentosAprazo(contaArec **contas){
    FILE *f;
    int indice = 0;
    int temp;
    f = fopen("lancamentosAprazo.txt", "r");
    
    if(!f){
        f = fopen("lancamentosAprazo.txt", "w");
        if(!f){
            printf("Erro ao criar arquivo de lancamentos a prazo!\n");
            printf("%s", strerror(errno));
            return 0;
        }
        return 0;
    }
    
    while(1){
        
        
        fscanf(f, "%d ", &temp);
        
        if(feof(f)){
            break;
        }
        
        contas[0] = (indice == 0)?malloc(sizeof(contaArec)):realloc(contas[0], sizeof(contaArec)*(indice+1));
        verificaOperacao(contas[0], ERROMEM, 1);
        
        
        contas[0][indice].codigoCliente = temp;
        
        fscanf(f, "%ld ", &contas[0][indice].codigoCompra);
        
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

int leDadosLancamentosAprazoBin(contaArec **contas){
    
    FILE *f;
    int indice = 0;
    contaArec temp;
    f = fopen("lancamentosAprazo.bin", "rb");
    
    if(!f){
        f = fopen("lancamentosAprazo.bin", "wb");
        if(!f){
            printf("Erro ao criar o arquivo de lancamento a prazo!\n");
            printf("%s\n", strerror(errno));
            return 0;
        }
        return 0;
    }
    
    while(1){
        
        fread(&temp, sizeof(contaArec), 1, f);
        
        if(feof(f)){
            break;
        }
        contas[0] = (indice == 0)?malloc(sizeof(contaArec)):realloc(contas[0], sizeof(contaArec)*(indice+1));


        verificaOperacao(contas[0], ERROMEM, 1);

        contas[0][indice].dataAluga = (struct tm){0,0,0,0,0,0,0,0,0};
        contas[0][indice] = temp;
        
        indice++;
    }
    
    fechaArquivo(&f);
    return indice;
    
}
void lancaEntradaOuParcela(contaArec *conta, int modoArm, float preco){
    
    time_t seg;
    time(&seg);
    lancamentoCaixa lancamento;
    
    lancamento.codigoCompra = conta->codigoCompra;
    
    lancamento.valor = preco;
    
    lancamento.modoPagamento = conta->modoPagamento;
    
    lancamento.data = *localtime(&seg);
    printf("digite o valor pago pelo cliente:\n");
    
    while(1){
        verificaNumero(&lancamento.valorPago, "%f");
        
        if(lancamento.valorPago < lancamento.valor){
            printf("Quantia paga insuficiente!\n");
            continue;
        }
        printf("troco: %.2f\n", (lancamento.troco = lancamento.valorPago - lancamento.valor));
        break;
    }
    Sleep(2000);
    (modoArm)?reescreveLancamentosCaixaBin(&lancamento, 1, "lancamentos.bin", "lancamentos.bin", "ab"):reescreveLancamentosCaixa(&lancamento, 1, "lancamentos.txt", "lancamentos.txt", "a");
    return;
    
    
}
void realizaCompraAprazo(compras *compra, int modoArm){
    
    contaArec Aprazo;
    
    Aprazo.dataAluga = compra->data;
    Aprazo.codigoCompra = compra->codigo;
    Aprazo.codigoCliente = compra->comprador->codigo;
    
    printf("digite o valor de entrada:\n");
    verificaLimiteNumero(&Aprazo.entrada, compra->preco*0.30, 0, "%f");
    
    printf("digite a quantidade de parcelas o cliente deseja dividir:\n");
    verificaLimiteNumero(&Aprazo.parcelas, MAXIMOPARCELAS, 1, "%d");
    
    Aprazo.valorParc = (compra->preco-Aprazo.entrada) / Aprazo.parcelas;
    
    if(Aprazo.entrada>0){
        compra->modoPagamento = 3;
        Aprazo.modoPagamento = 3;
        lancaEntradaOuParcela(&Aprazo, modoArm, Aprazo.entrada);
        
    }else{
        compra->modoPagamento = 2;
        Aprazo.modoPagamento = 2;
    }
    
    (modoArm == 1)
            ?reescreveLancamentosAprazoBin(&Aprazo, 1, "lancamentosAprazo.bin", "lancamentosAprazo.bin", "ab")
            :reescreveLancamentosAprazo(&Aprazo, 1, "lancamentosAprazo.txt", "lancamentosAprazo.txt", "a");
    
   
    
    return;
    
}
contaArec * encontraContaPeloCodigo(contaArec *contas, int quantidade, long int codigo){
    for(int i = 0; i<quantidade; i++){
        if(contas[i].codigoCompra == codigo && contas[i].parcelas > 0){
            return (contas+i);
        }
    }
    return NULL;
}
void consultaContas(contaArec *contas, int quantidade){
    
    for(int i = 0; i<quantidade; i++){
        
        if(contas[i].parcelas>0){
            printf("_______________________________________________\n");
            printf("|Codigo cliente: %d\n", contas[i].codigoCliente);

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

int daBaixa(int modoArm){
    
    contaArec *contas = NULL;
    contaArec *darBaixa = NULL;
    int quantidade = 0;
    int quantidadesNotinhasPagar = 0;
    long int codigo;
    time_t tempo;

    quantidade = (modoArm)?leDadosLancamentosAprazoBin(&contas):leDadosLancamentosAprazo(&contas);
    printf("digite o codigo da compra que deseja dar baixa:\n");
    consultaContas(contas, quantidade);
    while(1){
        
        verificaNumero(&codigo, "%ld");
        
        darBaixa = encontraContaPeloCodigo(contas, quantidade, codigo);
        
        if(!darBaixa){
            disparaSom("Nao existe nenhuma compra com este codigo ou a compra selecionada ja foi paga!", 1);


            continue;
        }
        break;
    }

    tempo = mktime(&darBaixa->dataAluga)+86400*30;
    darBaixa->dataAluga = *localtime(&tempo);
    printf("digite a quantidade de parcelas que deseja baixar desta compra, ainda existem %d parcelas:", darBaixa->parcelas);
    
    verificaLimiteNumero(&quantidadesNotinhasPagar, darBaixa->parcelas, 1, "%d");
    
    lancaEntradaOuParcela(darBaixa, modoArm, darBaixa->valorParc*quantidadesNotinhasPagar);
    darBaixa->parcelas -= quantidadesNotinhasPagar;
    
    (modoArm)?reescreveLancamentosAprazoBin(contas, quantidade, "lancamentosAprazoRes.bin", "lancamentosAprazo.bin", "wb"):reescreveLancamentosAprazo(contas, quantidade, "lancamentosAprazoRes.txt", "lancamentosAprazo.txt", "w");

    return quantidadesNotinhasPagar;
}
int filtraContasVencimentoMes(contaArec *contas, int quantidadeContas, contaArec **atrasadas, int mesVenc){
    struct tm data = {0,0,0,0,0,0,0,0,0};
    time_t tempAt;
    time(&tempAt);
    time_t segundosMesEsc;
    data.tm_mon = escolheMenu("Qual mes deseja filtrar?", 12, 0, "Janeiro", "Fevereiro", "Marco", "Abril", "Maio" ,"Junho", "Julho", "Agosto", "Setembro", "Outubro", "Novembro", "Dezembro");
    data.tm_mday = 1;
    data.tm_year = localtime(&tempAt)->tm_year;
    if(data.tm_mon<= localtime(&tempAt)->tm_mon){
        data.tm_year++;
    }
    segundosMesEsc = mktime(&data);
    int quantidadeContasVencem = 0;
    for(int i = 0; i<quantidadeContas; i++){
        if(contas[i].parcelas){
            tempAt = 2678400*contas[i].parcelas;
            tempAt += mktime(&contas[i].dataAluga);
            if(tempAt>=segundosMesEsc && mktime(&contas[i].dataAluga)<segundosMesEsc) {
                atrasadas[0] = (!quantidadeContasVencem)
                        ? malloc(sizeof(contaArec))
                        : realloc(atrasadas[0],sizeof(contaArec) *(quantidadeContasVencem +1));

                verificaOperacao(atrasadas[0], "ERRO: Memoria indisponivel!", 1);

                atrasadas[0][quantidadeContasVencem] = contas[i];
                quantidadeContasVencem++;
            }
        }
    }
    return quantidadeContasVencem;
}

int filtraContasAtrasadas(contaArec *contas, int quantidadeContas, contaArec **atrasadas, int modoArm){
    
    int quantidadeContasAtrasadas = 0;
    time_t seg;

    time(&seg);
    seg = mktime(localtime(&seg));
    for(int i = 0; i<quantidadeContas; i++){
        if(difftime(seg, mktime(&contas[i].dataAluga)) >= 2629800){
                
            atrasadas[0] = (!quantidadeContasAtrasadas)?malloc(sizeof(contaArec)):realloc(atrasadas[0], sizeof(contaArec)*(quantidadeContasAtrasadas+1));
            
            atrasadas[0][quantidadeContasAtrasadas] = contas[i];
            quantidadeContasAtrasadas++;
            
        }
    }
    return quantidadeContasAtrasadas;
}
int filtraContasClientes(contaArec *contas, int quantidadeContas, contaArec **contasDeUmCliente, int modoArm){
    
    
    int codigo;
    cliente *todosClientes = NULL;
    int quantidadeClientes = 0;
    int quantidadeContasCliente = 0;
    quantidadeClientes = (modoArm)?leDadosClientesBin(&todosClientes):leDadosClientes(&todosClientes);
    printf("digite o nome, cpf ou codigo do cliente que deseja ver as contas:\n");
    codigo = buscaCliente(todosClientes, quantidadeClientes, "Nao existe nenhum cliente cadastrado com este dado, por favor digite novamente!")->codigo;
    
    limpaDadosClienteMemoria(todosClientes, quantidadeClientes);
    todosClientes = limpaMemoria(todosClientes);
    
    for(int i = 0; i<quantidadeContas; i++){
        if(contas[i].codigoCliente == codigo && contas[i].parcelas){
            
            contasDeUmCliente[0] = (!quantidadeContasCliente)
                    ?malloc(sizeof(contaArec))
                    :realloc(contasDeUmCliente[0], sizeof(contaArec)*(quantidadeContasCliente+1));
            
            contasDeUmCliente[0][quantidadeContasCliente] = contas[i];
            quantidadeContasCliente++;
        }
    }
    return quantidadeContasCliente;
   
}
int filtraContasQuantidadeParcelas(contaArec *contas, int quantidadeContas, contaArec **contasRetornar, int parcelas){

    int quantidadeContasComFiltro = 0;
    parcelas = escolheMenu("Escolha a quantidade de parcelas", 3, 0,"Uma parcela", "Duas parcelas", "Tres parcelas")+1;

    for(int i = 0; i<quantidadeContas; i++){
        if(contas[i].parcelas == parcelas){
            contasRetornar[0] = (!quantidadeContasComFiltro)?malloc(sizeof(contaArec)):realloc(contasRetornar[0], sizeof(contaArec)*(quantidadeContasComFiltro+1));
            verificaOperacao(contasRetornar[0], "ERRO: Memoria indisponivel", 1);

            contasRetornar[0][quantidadeContasComFiltro] = contas[i];
            quantidadeContasComFiltro++;
        }
    }

    return quantidadeContasComFiltro;

}
int filtraPorData(contaArec *contas, int quantidade, contaArec **dest, int modo){
    struct tm data = {0,0,0,0,0,0,0,0,0};
    struct tm dataFim = {0,0,0,0,0,0,0,0,0};

    int quantidadeContasFiltradas = 0;
    time_t segundos1, segundos2;

    data.tm_mon = escolheMenu("escolha o mes da data inicial:", 12, 0, "Janeiro", "Fevereiro", "Marco", "Abril", "Maio" ,"Junho", "Julho", "Agosto", "Setembro", "Outubro", "Novembro", "Dezembro");

    data.tm_mday = escolheMenu("escolha o dia:", (data.tm_mon%2!=0)?30:(data.tm_mon == 2)?28:31, 0,
                               "1","2","3", "4", "5", "6", "7","8","9","10","11","12","13","14","15","16","17","18","19","20",
                               "21","22","23","24","25","26","27","28","29","30","31");


    printf("ano: ");
    verificaNumero(&data.tm_year, "%d");

    dataFim.tm_mon = escolheMenu("escolha o mes da data Final:", 12, 0, "Janeiro", "Fevereiro", "Marco", "Abril", "Maio" ,"Junho", "Julho", "Agosto", "Setembro", "Outubro", "Novembro", "Dezembro");

    dataFim.tm_mday = escolheMenu("escolha o dia:", (dataFim.tm_mon%2!=0)?30:(data.tm_mon == 2)?28:31, 0,
                               "1","2","3", "4", "5", "6", "7","8","9","10","11","12","13","14","15","16","17","18","19","20",
                               "21","22","23","24","25","26","27","28","29","30","31");


    printf("ano: ");
    verificaNumero(&dataFim.tm_year, "%d");


    data.tm_year-=1900;

    dataFim.tm_year-=1900;

    segundos1 = mktime(&data);
    segundos2 = mktime(&dataFim);



    for(int i = 0; i<quantidade; i++){
        if(difftime(mktime(&contas[i].dataAluga), segundos1)  >= 0 && difftime(mktime(&contas[i].dataAluga), segundos2) <=0){


            dest[0] = (!quantidadeContasFiltradas)?malloc(sizeof(contaArec)):realloc(dest[0], sizeof(contaArec)*(quantidadeContasFiltradas+1));
            verificaOperacao(dest[0], "ERRO: Memoria indisponivel!", 1);

            dest[0][quantidadeContasFiltradas] = contas[i];
            quantidadeContasFiltradas++;

        }
    }
    return quantidadeContasFiltradas;


}
void mostraContasFiltradas(contaArec *contas, int quantidadeContas, int modoArm, int (*filtra)(contaArec *, int, contaArec **, int), char *mensagem, char *mensagemCasoNaoExis){
    contaArec *contasAtrasadas = NULL;
    int quantidadeContasAtrasadas = 0;
    quantidadeContasAtrasadas = (*filtra)(contas, quantidadeContas, &contasAtrasadas, modoArm);

    if(!quantidadeContasAtrasadas){

        printf(mensagemCasoNaoExis);
        Sleep(2000);
        return;
    }

    int escolha;

    
    filmes *todosFilmes = NULL;
    cliente *todosClientes = NULL;
    Funcionarios *todosFuncionarios = NULL;
    compras *alocacoes = NULL;
    compras *compraEspecifica = NULL;
    quantidades quants = {0,0,0,0};
    
    quants = (modoArm)?leDadosDevolucoesBin(&alocacoes, &todosFuncionarios, &todosClientes, &todosFilmes):leDadosDevolucoes(&alocacoes, &todosFuncionarios, &todosClientes, &todosFilmes);
    menuGraphics(2, "escolha uma opcao:", "consultar uma compra", "sair");
    printf("%s", mensagem);
    consultaContas(contasAtrasadas, quantidadeContasAtrasadas);
    while(1){




        escolha = escolheOpcao();
        
        switch(escolha){
            case 59:
                printf("digite o codigo da compra:\n");
                compraEspecifica = buscaCompra(alocacoes, quants.quantidadeAlugacoes, 0);
                mostraCompra(compraEspecifica,1);
                break;
            case 60:
                break;
            default:
                disparaSom("escolha uma opcao valida!", 1);
                continue;
                
        }
        break;
    }
    limpaDadosFilmeMemoria(todosFilmes, quants.quantidadesFilmes);
    todosFilmes = limpaMemoria(todosFilmes);
    for(int i = 0; i<quants.quantidadeAlugacoes; i++){
        atribuiNull(alocacoes[i].filmesComprados, alocacoes[i].quantidadeFilmesComprados, sizeof(filmes));
        alocacoes[i].filmesComprados = limpaMemoria(alocacoes[i].filmesComprados);
    }
    limpaDadosClienteMemoria(todosClientes, quants.quantidadeClientes);
    todosClientes = limpaMemoria(todosClientes);
    
    apagaDadosStructFuncionarios(todosFuncionarios, quants.quantidadesFuncionarios);
    todosFuncionarios = limpaMemoria(todosFuncionarios);
    
    atribuiNull(alocacoes, quants.quantidadeAlugacoes, sizeof(compras));
    alocacoes = limpaMemoria(alocacoes);
    compraEspecifica = NULL;
    contasAtrasadas = limpaMemoria(contasAtrasadas);
    
}



void consultaLancamentos(int modoArm){
    
    contaArec *contas = NULL;
    int quantidadeContas; 
    int escolha;
    quantidadeContas = (modoArm)?leDadosLancamentosAprazoBin(&contas):leDadosLancamentosAprazo(&contas);
    
    
    


    escolha = escolheMenu("Escolha o modo de filtragem", 6, 0,"Contas atrasadas", "Contas de um cliente", "parcelas restantes", "Filtrar por data especifica", "Filtrar contas que vencem em algum mes","Voltar");
    switch(escolha){
        case 0:
            mostraContasFiltradas(contas, quantidadeContas, modoArm, filtraContasAtrasadas, "Contas atrasadas:\n", "Nao existem contas atrasadas");
            break;

        case 1:
            mostraContasFiltradas(contas, quantidadeContas, modoArm, filtraContasClientes, "Contas do cliente:\n", "Nao existem contas deste cliente");
            break;

        case 2:
            mostraContasFiltradas(contas, quantidadeContas, modoArm, filtraContasQuantidadeParcelas, "Contas pela quantidade de parcelas\n", "Nao existem contas com essa quantidade de parcelas faltando");
            break;
        case 3:
            mostraContasFiltradas(contas, quantidadeContas, modoArm, filtraPorData, "Contas pela filtragem da data especifica digitada\n", "Nao existe conta neste periodo digitado");
            break;
        case 4:
            mostraContasFiltradas(contas, quantidadeContas, modoArm, filtraContasVencimentoMes, "Contas que vencem no mes escolhido:\n", "Nao existem contas que vencem neste mes");
            break;

    }
    contas = limpaMemoria(contas);
    
}
