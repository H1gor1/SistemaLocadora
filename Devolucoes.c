#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include <time.h>
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


void atribuiNull(void *endereco, int quantidade, int oqueLimpar){
    switch(oqueLimpar){
        case 1:
            for(int i = 0; i<quantidade; i++){
                ((filmes *)endereco)[i].nome = NULL;
                ((filmes *)endereco)[i].descricao = NULL;
            }
            return;
        case 2:
            for(int i = 0; i<quantidade; i++){
                ((compras *)endereco)[i].comprador = NULL;
                ((compras *)endereco)[i].vendedor = NULL;
            }
            return;
            
    }
}
quantidades leDadosDevolucoes(compras ** dev, Funcionarios **func, cliente **clients, filmes **movies){
    
    time_t codigo;
    quantidades quants = {0,0,0,0};

    
    quants.quantidadesFilmes = leDadosFilmes(movies);
    quants.quantidadeClientes = leDadosClientes(clients);
    quants.quantidadesFuncionarios = leDadosFuncionarios(func);
    FILE *f;
    
    
    f = fopen("devolucoes.txt", "r");
    
    if(!f){
        f = fopen("devolucoes.txt", "w");
        if(!f){
            printf("Erro ao tentar criar arquivo de devolucoes!\n");
            printf("%s\n", strerror(errno));
            return quants;
        }
        return quants;
    }
    
    while(1){
        
        
        fscanf(f, "%ld ", &codigo);
        
        if(feof(f)){
            break;
        }
        
        dev[0] = (!quants.quantidadeAlugacoes)?malloc(sizeof(compras)):realloc(dev[0], sizeof(compras)*(quants.quantidadeAlugacoes+1));
        verificaOperacao(dev[0], ERROMEM, 1);
        
        dev[0][quants.quantidadeAlugacoes].codigo = codigo;
        
        fscanf(f, "%d ", &dev[0][quants.quantidadeAlugacoes].modoPagamento);
        
        fscanf(f, "%d %d %d ", &dev[0][quants.quantidadeAlugacoes].data.tm_mday, &dev[0][quants.quantidadeAlugacoes].data.tm_mon, &dev[0][quants.quantidadeAlugacoes].data.tm_year);
        
        fscanf(f, "%d %d ", &dev[0][quants.quantidadeAlugacoes].data.tm_hour, &dev[0][quants.quantidadeAlugacoes].data.tm_min);
        
        fscanf(f, "%ld ", &codigo);
        
        dev[0][quants.quantidadeAlugacoes].vendedor = encontraFuncionarioCodigo(func[0], codigo, quants.quantidadesFuncionarios, NULL);
        
        fscanf(f, "%d ", &codigo);
        
        dev[0][quants.quantidadeAlugacoes].comprador = encontraClienteCodigo(clients[0], quants.quantidadeClientes, converteIntEmString(codigo), 0);
        
        fscanf(f, "%f ", &dev[0][quants.quantidadeAlugacoes].preco);
        
        fscanf(f, "%d ", &dev[0][quants.quantidadeAlugacoes].quantidadeFilmesComprados);
        dev[0][quants.quantidadeAlugacoes].filmesComprados = malloc(sizeof(filmes)*dev[0][quants.quantidadeAlugacoes].quantidadeFilmesComprados);
        
        for(int j = 0; j<dev[0][quants.quantidadeAlugacoes].quantidadeFilmesComprados; j++){
            fscanf(f, "%d ", &codigo);
            dev[0][quants.quantidadeAlugacoes].filmesComprados[j] = *encontraFilmeCodigo(movies[0], quants.quantidadesFilmes, converteIntEmString(codigo), 0);
            fscanf(f, "%d ", &dev[0][quants.quantidadeAlugacoes].filmesComprados[j].exemplares);
        }
        fscanf(f, "%d  ", &dev[0][quants.quantidadeAlugacoes].devolvido);
        
        quants.quantidadeAlugacoes++;
    }
    
    fechaArquivo(&f);
    
    return quants;
}

quantidades leDadosDevolucoesBin(compras **dev, Funcionarios **func, cliente **clients, filmes **movies){
    
    time_t codigo;
    int codigo1;
    quantidades quants = {0,0,0,0};
    
    quants.quantidadesFuncionarios = leDadosFuncionariosBin(func);;
    quants.quantidadeClientes = leDadosClientesBin(clients);
    quants.quantidadesFilmes = leDadosFilmesBin(movies);
    quants.quantidadeAlugacoes = 0;
    FILE *f;
    
    f = fopen("devolucoes.bin", "rb");
    
    if(!f){
        f = fopen("devolucoes.bin", "wb");
        
        if(!f){
            printf("Erro ao tentar criar arquivo de devolucoes!\n");
            printf("%s\n", strerror(errno));
            return quants;
        }
        return quants;
    }
    
    while(1){
        
        fread(&codigo, sizeof(time_t), 1, f);
        
        if(feof(f)){
            break;
        }
        
        dev[0] = (!quants.quantidadeAlugacoes)?malloc(sizeof(compras)):realloc(dev[0], sizeof(compras)*(quants.quantidadeAlugacoes+1));
        verificaOperacao(dev[0], ERROMEM, 1);
        
        dev[0][quants.quantidadeAlugacoes].codigo = codigo;
        
        fread(&dev[0][quants.quantidadeAlugacoes].modoPagamento, sizeof(int), 1, f);
        
        fread(&dev[0][quants.quantidadeAlugacoes].data, sizeof(struct tm), 1, f);
        
        fread(&codigo, sizeof(time_t), 1, f);
        dev[0][quants.quantidadeAlugacoes].vendedor = encontraFuncionarioCodigo(func[0], codigo, quants.quantidadesFuncionarios, NULL);
        
        fread(&codigo1, sizeof(int), 1, f);
        dev[0][quants.quantidadeAlugacoes].comprador = encontraClienteCodigo(clients[0], quants.quantidadeClientes, converteIntEmString(codigo1), 0);
        
        fread(&dev[0][quants.quantidadeAlugacoes].preco, sizeof(float), 1, f);
        
        fread(&dev[0][quants.quantidadeAlugacoes].quantidadeFilmesComprados, sizeof(int), 1, f);
        dev[0][quants.quantidadeAlugacoes].filmesComprados = malloc(sizeof(filmes)*(dev[0][quants.quantidadeAlugacoes].quantidadeFilmesComprados));
        for(int j = 0; j<dev[0][quants.quantidadeAlugacoes].quantidadeFilmesComprados; j++){
            fread(&codigo1, sizeof(int), 1, f);
            dev[0][quants.quantidadeAlugacoes].filmesComprados[j] = *encontraFilmeCodigo(movies[0], quants.quantidadesFilmes, converteIntEmString(codigo1), 0);
            fread(&dev[0][quants.quantidadeAlugacoes].filmesComprados[j].exemplares, sizeof(int), 1, f);
        }
        fread(&dev[0][quants.quantidadeAlugacoes].devolvido, sizeof(int), 1, f);
        quants.quantidadeAlugacoes++;
    }
    
    fechaArquivo(&f);
    return quants;
}
void reescreveDadosDevolucoes(compras *dev, int quantidade, char *nomeArq, char *nomeArqOr, char *modo){
    
    FILE *f;
    
    f = fopen(nomeArq, modo);
    
    if(!f){
        printf("Erro ao tentar salvar novos dados, novos dados foram perdidos!\n");
        printf("%s\n", strerror(errno));
        return;
    }
    
    for(int i = 0; i<quantidade; i++){
        fprintf(f, "%ld\n", dev[i].codigo);
        fprintf(f, "%d\n", dev[i].modoPagamento);
        fprintf(f, "%d\n%d\n%d\n", dev[i].data.tm_mday, dev[i].data.tm_mon, dev[i].data.tm_year);
        fprintf(f, "%d\n%d\n", dev[i].data.tm_hour, dev[i].data.tm_min);
        fprintf(f, "%ld\n", dev[i].vendedor->codigo);
        fprintf(f, "%d\n", dev[i].comprador->codigo);
        fprintf(f, "%.2f\n", dev[i].preco);
        fprintf(f, "%d\n", dev[i].quantidadeFilmesComprados);
        for(int j = 0; j<dev[i].quantidadeFilmesComprados; j++){
            fprintf(f, "%d\n", dev[i].filmesComprados[j].codigo);
            fprintf(f, "%d\n", dev[i].filmesComprados[j].exemplares);
        }
        fprintf(f, "%d\n\n", dev[i].devolvido);
        
    }
    fechaArquivo(&f);
    if(strcmp(nomeArq, nomeArqOr)){
        remove(nomeArqOr);
        rename(nomeArq, nomeArqOr);
    }
    return;
}

void reescreveDadosDevolucoesBin(compras *dev, int quantidade, char *nomeArq, char *nomeArqOr, char *modo){
    
    FILE *f;
    
    f = fopen(nomeArq, modo);
    
    if(!f){
        printf("Erro ao salvar novos dados, novos dados foram perdidos!\n");
        printf("%s\n", strerror(errno));
        return;
    }
    
    for(int i = 0; i<quantidade; i++){
        fwrite(&dev[i].codigo, sizeof(time_t), 1, f);
        
        fwrite(&dev[i].modoPagamento, sizeof(int), 1, f);
        
        fwrite(&dev[i].data, sizeof(struct tm), 1, f);
        
        fwrite(&dev[i].vendedor->codigo, sizeof(time_t), 1, f);
        
        fwrite(&dev[i].comprador->codigo, sizeof(int), 1, f);
        
        fwrite(&dev[i].preco, sizeof(float), 1, f);
        
        fwrite(&dev[i].quantidadeFilmesComprados, sizeof(int), 1, f);
        
        for(int j = 0; j<dev[i].quantidadeFilmesComprados; j++){
            fwrite(&dev[i].filmesComprados[j].codigo, sizeof(int), 1, f);
            fwrite(&dev[i].filmesComprados[j].exemplares, sizeof(int), 1, f);
        }
        
        fwrite(&dev[i].devolvido, sizeof(int), 1, f);
        
        
    }
    fechaArquivo(&f);
    
    if(strcmp(nomeArq, nomeArqOr)){
        remove(nomeArqOr);
        rename(nomeArq, nomeArqOr);
    }
    return;
}
compras * encontraCompraCodigo(compras *ptr, int quantidade, time_t codigo){
    
    for(int i = 0; i<quantidade; i++){
        if(ptr[i].codigo == codigo){
            return (ptr+i);
        }
    }
    return NULL;
}
compras *buscaCompra(compras *ptr, int quantidade, int ignoraDev){
    compras *busca = NULL;

    long int codigo;
    
    while(1){
        verificaNumero(&codigo, "%d");

        busca = encontraCompraCodigo(ptr, quantidade, codigo);
        if(busca){
            if(busca->devolvido && ignoraDev){
                printf("Compra inexistente, digite um codigo valido!\n");
                
            }else{
                return busca;
            }
        }
        printf("Compra inexistente, digite um codigo valido!\n");
    }
    
    
    
}
void devolveFilme(int modoArm){
    cliente *todosClientes = NULL;
    Funcionarios *funcs = NULL;
    filmes *todosFilmes = NULL;
    compras *devolvida = NULL;
    compras *alocacoes = NULL;
    quantidades quant = {0,0,0,0};
    time_t codigoCompraDevolvida = 0;
    struct tm dataDev;
    int escolha;
    quant = (modoArm)?leDadosDevolucoesBin(&alocacoes, &funcs, &todosClientes, &todosFilmes):leDadosDevolucoes(&alocacoes, &funcs, &todosClientes, &todosFilmes);
    
    time(&codigoCompraDevolvida);
    
    dataDev = *localtime(&codigoCompraDevolvida);
    printf("digite o codigo da alocacao que sera devolvida:\n");
    devolvida = buscaCompra(alocacoes, quant.quantidadeAlugacoes, 0);
    
    menuGraphics(2, "Confirme a devolucao:", "sim", "nao");
    printf("informacoes da compra:\n");
    mostraCompra(devolvida,0);
    
    while(1){
        verificaNumero(&escolha, "%d");
        if(escolha == 2){
            limpaDadosFilmeMemoria(todosFilmes, quant.quantidadesFilmes);
            for(int i = 0; i<quant.quantidadeAlugacoes; i++){
                atribuiNull(alocacoes[i].filmesComprados, alocacoes[i].quantidadeFilmesComprados, 1);
                alocacoes[i].filmesComprados = limpaMemoria(alocacoes[i].filmesComprados);
            }
            todosFilmes = limpaMemoria(todosFilmes);

            limpaDadosClienteMemoria(todosClientes, quant.quantidadeClientes);
            todosClientes = limpaMemoria(todosClientes);

            apagaDadosStructFuncionarios(funcs, quant.quantidadesFuncionarios);
            funcs = limpaMemoria(funcs);

            atribuiNull(alocacoes, quant.quantidadeAlugacoes, 2);

            alocacoes = limpaMemoria(alocacoes);
    
            return;
        }else if(escolha != 1){
            printf("escolha uma opcao valida!\n");
        }
        break;
    }
    
    
    devolvida->devolvido = 1;
    
    for(int i = 0; i<devolvida->quantidadeFilmesComprados; i++){
        
       encontraFilmeCodigo(todosFilmes, quant.quantidadesFilmes, converteIntEmString(devolvida->filmesComprados[i].codigo), 0)->exemplares+=devolvida->filmesComprados[i].exemplares;
    }
    if((devolvida->data.tm_mon < dataDev.tm_mon && devolvida->data.tm_mday <= dataDev.tm_mday) || 
            (devolvida->data.tm_year < dataDev.tm_year && devolvida->data.tm_mday <=dataDev.tm_mday)){
        locadora multa;
        lancamentoCaixa pagMulta;
        (modoArm)?leDadosLocadoraBin(&multa, "locadora.bin"):leDadosLocadora(&multa, "locadora.txt");
        pagMulta.valor = multa.multa;
        pagMulta.codigoCompra = devolvida->codigo;
        pagMulta.data = dataDev;
        pagMulta.modoPagamento = 1;
        printf("devolucao atrasada, sera cobrado um valor de multa no valor de %.2f!\n", multa.multa);
        printf("digite o valor pago:\n");
        pagMulta.troco = calculaTroco(pagMulta.valor, &pagMulta.valorPago);
        if(pagMulta.troco){
            printf("troco: %.2f\n", pagMulta.troco);
            Sleep(2000);
        }
        (modoArm)?reescreveLancamentosCaixaBin(&pagMulta, 1, "lancamentos.bin", "lancamentos.bin", "ab"):reescreveLancamentosCaixa(&pagMulta, 1, "lancamentos.txt", "lancamentos.txt", "a");
        
        limpaCamposLocadoraMemoria(&multa);
    }
    
    printf("filme devolvido com sucesso!\n");
    
    Sleep(1000);
    (modoArm)?reescreveDadosFilmeBin(todosFilmes, quant.quantidadesFilmes):reescreveDadosFilme(todosFilmes, quant.quantidadesFilmes);
    
    (modoArm)?reescreveDadosDevolucoesBin(alocacoes, quant.quantidadeAlugacoes, "devolucoesRes.bin", "devolucoes.bin", "wb"):reescreveDadosDevolucoes(alocacoes, quant.quantidadeAlugacoes, "devolucoesRes.txt", "devolucoes.txt", "w");
    
    limpaDadosFilmeMemoria(todosFilmes, quant.quantidadesFilmes);
    for(int i = 0; i<quant.quantidadeAlugacoes; i++){
        atribuiNull(alocacoes[i].filmesComprados, alocacoes[i].quantidadeFilmesComprados, 1);
        alocacoes[i].filmesComprados = limpaMemoria(alocacoes[i].filmesComprados);
    }
    todosFilmes = limpaMemoria(todosFilmes);
    
    limpaDadosClienteMemoria(todosClientes, quant.quantidadeClientes);
    todosClientes = limpaMemoria(todosClientes);
    
    apagaDadosStructFuncionarios(funcs, quant.quantidadesFuncionarios);
    funcs = limpaMemoria(funcs);
    
    atribuiNull(alocacoes, quant.quantidadeAlugacoes, 2);
    
    alocacoes = limpaMemoria(alocacoes);
    
   
}
void mostraCompra(compras *compra, int resumir){
    printf("Codigo compra: %ld\n", compra->codigo);
    printf("Modo pagamento: %s\n", (compra->modoPagamento == 1)?"a vista":(compra->modoPagamento == 2)?"a prazo":"a prazo com entrada");
    printf("Data: %d/%d/%d\n", compra->data.tm_mday, compra->data.tm_mon+1, compra->data.tm_year+1900);
    printf("Horario: %d:%d\n", compra->data.tm_hour, compra->data.tm_min);
    if(resumir){
        printf("Vendedor:\n");
        printf("    nome: %s\n",compra->vendedor->nome);
        printf("    cargo: %s\n", compra->vendedor->cargo);
        printf("    codigo: %ld\n", compra->vendedor->codigo);
    }
    printf("Comprador:\n");
    printf("    nome: %s\n", compra->comprador->nomeCompleto);
    printf("    CPF: %s\n", compra->comprador->cpf);
    
    if(resumir){
        printf("    codigo: %d\n", compra->comprador->codigo);
        printf("    telefone: %d\n", compra->comprador->telefone);
        printf("    endereco: %s, %s, %d\n", compra->comprador->rua, compra->comprador->bairro, compra->comprador->numeroDaCasa);
    }
    printf("Preco: %.2f\n", compra->preco);
    printf("Filmes Comprados:\n");
    for(int i = 0; i<compra->quantidadeFilmesComprados; i++){
        printf("   %dº filme:\n", i+1);
        printf("        Nome: %s\n", compra->filmesComprados[i].nome);
        printf("        Codigo: %d\n", compra->filmesComprados[i].codigo);
        printf("        Preco: %.2f\n", compra->filmesComprados[i].valorLocacao);
        printf("        Quantidade: %d\n", compra->filmesComprados[i].exemplares);
   
        
    }
    printf("Status: %s\n", (compra->devolvido)?"devolvido":"Nao devolvido");
    if(resumir){
        printf("digite 1 para sair:\n");
        while(getchar()!='1');

    }
}
