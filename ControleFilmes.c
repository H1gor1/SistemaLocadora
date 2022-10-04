#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include "ControleFilmes.h"
#include "ManipulacoesDeTexto.h"
#include "ControleDeDadosLocadora.h"
#include "ControleDeNumeros.h"
#include "FuncUtilitarias.h"
#include "menus.h"
#include <ctype.h>
#include <locale.h>
#include "ControleCategorias.h"
#include <windows.h>
#define filmeNaoExiste '*'
#define ERROMEM "ERRO: Memoria indisponivel!\n"

void limpaDadosFilmeMemoria(filmes *ptr, int quantidade){
    for(int i = 0; i<quantidade; i++){
        ptr[i].nome = limpaMemoria(ptr[i].nome);
        ptr[i].descricao = limpaMemoria(ptr[i].descricao);
    }
}

int verificaExisteFilme(filmes *ptr, int quantidade){
    if(quantidade == 0){
        return 0;
    }
    for(int i = 0; i<quantidade; i++){
        if(strcmp(ptr[i].nome, "*")){
           return 1;  
        }
    }
    return 0;
}
void mostraFilmes(filmes *ptr, int quantidade){
    for(int i = 0; i<quantidade; i++){
                
        if(ptr[i].nome[0] != '*'){
                printf("╔════════════════\n");
                printf("║Nome: %s\n", ptr[i].nome);
                printf("║Codigo: %d\n", ptr[i].codigo);
                printf("╚════════════════\n");
        }
    }
}

int leDadosFilmesBin(filmes **ptr){
    int quantidade;
    int quantidadeLetras;
    FILE *f;
    f = fopen("filmes.bin", "rb");
    if(!f){
        f = fopen("filmes.bin", "wb");
        quantidade = 0;
        fwrite(&quantidade, sizeof(int), 1, f);
        fechaArquivo(&f);
        return 0;
    }
    fread(&quantidade, sizeof(int), 1, f);
    ptr[0] = malloc(sizeof(filmes)*quantidade);
    verificaOperacao(ptr[0], ERROMEM, 1);
    
    for(int i = 0; i<quantidade; i++){
        fread(&ptr[0][i].codigo, sizeof(int), 1, f);
        
        fread(&quantidadeLetras, sizeof(int), 1, f);
        ptr[0][i].nome = malloc(sizeof(char)*quantidadeLetras);
        verificaOperacao(ptr[0][i].nome, ERROMEM, 1);
        fread(ptr[0][i].nome, sizeof(char), quantidadeLetras, f);
        
        fread(&quantidadeLetras, sizeof(int), 1, f);
        ptr[0][i].descricao = malloc(sizeof(char)*quantidadeLetras);
        verificaOperacao(ptr[0][i].descricao, ERROMEM, 1);
        fread(ptr[0][i].descricao, sizeof(char), quantidadeLetras, f);
        
        
        fread(&ptr[0][i].exemplares, sizeof(int), 1, f);
        
        
        fread(&ptr[0][i].codigoCategoria, sizeof(int), 1, f);
        
        
        fread(&ptr[0][i].lingua, sizeof(int), 1, f);
    }
    
    fechaArquivo(&f);
    return quantidade;
    
}

int leDadosFilmes(filmes **ptr){
    int quantidade;
    FILE *f;
    f = fopen("filmes.txt", "r");
    if(!f){
        f = fopen("filmes.txt", "w");
        fprintf(f, "0");
        fechaArquivo(&f);
        return 0;
    }
    fscanf(f, "%d", &quantidade);
    ptr[0] = malloc(sizeof(filmes)*quantidade);
    verificaOperacao(ptr[0], ERROMEM, 1);
    
    for(int i = 0; i<quantidade; i++){
        fscanf(f, "%d ", &ptr[0][i].codigo);
        digText(&ptr[0][i].nome, f);
        digText(&ptr[0][i].descricao, f);
        fscanf(f, "%d ", &ptr[0][i].exemplares);
        fscanf(f,"%d ", &ptr[0][i].codigoCategoria);
        fscanf(f, "%d ", &ptr[0][i].lingua);

    }
    fechaArquivo(&f);
    return quantidade;
}

void reescreveDadosFilmeBin(filmes *ptr, int quantidade){
    
    FILE *f;
    f = fopen("filmesRes.bin", "wb");
    if(!f){
        printf("erro na abertura do arquivo! Novos dados nao foram salvos!\n");
        printf("%s\n", strerror(errno));
        return;
    }
    fwrite(&quantidade, sizeof(int), 1, f);
    int quantidadeLetras;
    
    for(int i = 0; i<quantidade; i++){
        fwrite(&ptr[i].codigo, sizeof(int), 1, f);
        
        quantidadeLetras = strlen(ptr[i].nome)+1;
        fwrite(&quantidadeLetras, sizeof(int), 1, f);
        fwrite(ptr[i].nome, sizeof(char), quantidadeLetras, f);
        
        quantidadeLetras = strlen(ptr[i].descricao)+1;
        fwrite(&quantidadeLetras, sizeof(int), 1, f);
        fwrite(ptr[i].descricao, sizeof(char), quantidadeLetras, f);
        
        fwrite(&ptr[i].exemplares, sizeof(int), 1, f);
        
        fwrite(&ptr[i].codigoCategoria, sizeof(int), 1, f);
        
        fwrite(&ptr[i].lingua, sizeof(int), 1, f);
    }
    fechaArquivo(&f);
    remove("filmes.bin");
    rename("filmesRes.bin", "filmes.bin");
}

void reescreveDadosFilme(filmes *ptr, int quantidade){
    FILE *f;
    f = fopen("filmesRes.txt", "w");
    if(!f){
        printf("erro ao salvar arquivo!Novos dados nao foram salvos!\n");
        printf("%s\n", strerror(errno));
        return;//cancela a operacao
    }
    fprintf(f, "%d\n", quantidade);
    
    for(int i = 0; i<quantidade; i++){
        fprintf(f, "%d\n", ptr[i].codigo);
        fprintf(f, "%s\n", ptr[i].nome);
        fprintf(f, "%s\n", ptr[i].descricao);
        fprintf(f, "%d\n", ptr[i].exemplares);
        fprintf(f, "%d\n", ptr[i].codigoCategoria);
        fprintf(f, "%d\n\n", ptr[i].lingua); 
    }
    fechaArquivo(&f);
    remove("filmes.txt");
    rename("filmesRes.txt", "filmes.txt");
}

filmes *encontraFilmeCodigo(filmes *ptr, int quantidade, char *codigo, int ignorar ){
    if(!isdigit(codigo[0]) || strlen(codigo)>8){
    }
    int cod = atoi(codigo);
    if(cod>=quantidade){
        return NULL;
    }
    if(strcmp(ptr[cod].nome, "*") == 0 && ignorar){
        return NULL;
    }
    return (ptr+cod);
}

filmes *encontraFilmeNome(filmes *ptr, int quantidade, char *nome){
    for(int i = 0; i<quantidade; i++){
        if(strcmp(ptr[i].nome, nome) == 0){
            return (ptr+i);
        }
    }
    return NULL;
}

filmes *buscaFilme(filmes *buscar, int quantidade, char *mensagem){
    char *dado;
    filmes *ptr;
    do{
        digText(&dado, stdin);
        
        ptr = encontraFilmeCodigo(buscar, quantidade, dado,1);
        if(ptr){
            dado = limpaMemoria(dado);
            return ptr;
        }
        
        ptr = encontraFilmeNome(buscar, quantidade, dado);
        if(ptr && strcmp(ptr->nome, "*")){
            dado = limpaMemoria(dado);
            return ptr;
        }
        
        dado = limpaMemoria(dado);
        printf("%s\n", mensagem);
        
    }while(1);
}

void editaFilme(int modoAbertura){
    int (*leDados[2])(filmes**) = {
        leDadosFilmes, leDadosFilmesBin
    };
    void (*reescreveDados[2])(filmes*, int)={
        reescreveDadosFilme, reescreveDadosFilmeBin 
    };
    filmes *ptr = NULL;
    filmes *filmes = NULL;
    int quantidadeFilmes;
    
    quantidadeFilmes = (*leDados[modoAbertura])(&filmes);
    if(!verificaExisteFilme(filmes, quantidadeFilmes)){
        printf("nao existe nenhum filme cadastrado no momento\n");
        Sleep(2000);
        return;
    }
    printf("digite o nome ou codigo do filme que deseja editar\n");
    mostraFilmes(filmes, quantidadeFilmes);
    ptr = buscaFilme(filmes, quantidadeFilmes, "Nenhum filme possui o dado digitado, por favor, digite um nome ou codigo");
    
    int dadoEditar;
    printf("digite o campo do filme que deseja editar:\n");
    printf("1-Nome do filme\n2-Descricao\n3-Exemplares\n4-Codigo da Categoria\n5-Lingua\n");
    verificaNumero(&dadoEditar, "%d");
    

    while(1){
        switch(dadoEditar){
            
            case 1:
                printf("digite o nome do filme\n");
                ptr->nome = limpaMemoria(ptr->nome);
                digText(&ptr->nome, stdin);
                break;
            case 2:
                printf("digite a descricao do filme\n");
                ptr->descricao = limpaMemoria(ptr->descricao);
                digText(&ptr->descricao, stdin);
                break;

            case 3:
                printf("digite a quantidade de exemplares disponiveis\n");
                verificaNumero(&ptr->exemplares, "%d");
                break;
                    
            case 4:
                printf("digite codigo da categoria do filme\n");
                verificaNumero(&ptr->codigoCategoria, "%d");
                break;
            
            case 5:
                printf("O filme é dublado ou legendado?:\n");
                printf("1-Dublado\n2-Legendado\n");
                verificaLimiteNumero(&ptr->lingua, 2, 1, "%d");
                break;      
            default:
                printf("Voce digitou uma opcao invalida, por favor, digite novamente!\n");
                verificaNumero(&dadoEditar, "%d");
                
                continue;
                                            
        }
        break;
    
    }
    (*reescreveDados[modoAbertura])(filmes, quantidadeFilmes);
    limpaDadosFilmeMemoria(filmes, quantidadeFilmes);
    filmes = limpaMemoria(filmes);
    ptr = NULL;
}

void cadastraFilmes(int modoLeitura){
    filmes *filme;
    categoria *categorias;
    int quantidadeCategorias;
    int (*LeDadosCategoria[2])(categoria **) = {leDadosCategoria, leDadosCategoriaBin};
    quantidadeCategorias = (*LeDadosCategoria[modoLeitura])(&categorias);

    int (*leDados[2])(filmes **) = {leDadosFilmes, leDadosFilmesBin};
    void (*reescreveDados[2])(filmes *, int) = {reescreveDadosFilme, reescreveDadosFilmeBin};
    int quantidadeFilmes = (*leDados[modoLeitura])(&filme) + 1;
    
    filme = (quantidadeFilmes == 1)?malloc(sizeof(filmes)):realloc(filme, sizeof(filmes)*quantidadeFilmes);
    verificaOperacao(filme, ERROMEM, 1);
    
    filme[quantidadeFilmes-1].codigo = quantidadeFilmes-1;
    
    printf("Digite o nome do filme\n");
    digText(&filme[quantidadeFilmes-1].nome, stdin);

    printf("Digite a descricao do filme\n");
    digText(&filme[quantidadeFilmes-1].descricao, stdin);

    printf("digite a quantidade de exemplares para alugar do filme:\n");
    verificaNumero(&filme[quantidadeFilmes-1].exemplares, "%d");
    
    printf("Digite o código da categoria do filme:\n");
    mostraListaCategoria(categorias, quantidadeCategorias);
    verificaNumero(&filme[quantidadeFilmes-1].codigoCategoria, "%d");
    
    printf("Escolha se o filme é dublado ou legendado\n");
    printf("1-Dublado\n2-Legendado\n");
    verificaLimiteNumero(&filme[quantidadeFilmes-1].lingua, 2, 1, "%d");
    

    (*reescreveDados[modoLeitura])(filme, quantidadeFilmes);
    limpaDadosFilmeMemoria(filme, quantidadeFilmes);
    filme = limpaMemoria(filme);
    limpaDadosCategoriaMemoria(categorias, quantidadeCategorias);
    categorias = limpaMemoria(categorias);
}

void apagaFilme(int modo){
    filmes *filme;

    int (*leDados[2])(filmes**) = {
      leDadosFilmes, leDadosFilmesBin  
    };

    void (*reescreveDados[2])(filmes*, int) = {  
        reescreveDadosFilme, reescreveDadosFilmeBin
    };

    int quantidadeFilmes = (*leDados[modo])(&filme);
    
    if(!verificaExisteFilme(filme, quantidadeFilmes)){
        printf("Ate o momento nao existe nenhum filme cadastrado\n");
        return;
    }

    printf("Digite o codigo ou nome do filme que deseja apagar\n");
    mostraFilmes(filme, quantidadeFilmes);
    filmes *apagar;
    apagar = buscaFilme(filme, quantidadeFilmes, "Nenhum filme tem o dado digitado, por favor, digite um nome, cpf ou codigo");
    deletaFilme(apagar);
    
    (*reescreveDados[modo])(filme, quantidadeFilmes);
    limpaDadosFilmeMemoria(filme, quantidadeFilmes);
    filme = limpaMemoria(filme);
    apagar = NULL;
}

void deletaFilme(filmes *apagar){
    

    apagar->nome = limpaMemoria(apagar->nome);
    apagar->nome = malloc(sizeof(char)*2);

    apagar->nome[0] = filmeNaoExiste;

    apagar->nome[1] = '\0';
    
    apagar->descricao = limpaMemoria(apagar->descricao);
    apagar->descricao = malloc(sizeof(char)*2);
    apagar->descricao[0] = filmeNaoExiste;
    apagar->descricao[1] = '\0';
    
    

    apagar->exemplares = -1;
    apagar->codigoCategoria = -1;    
    apagar->lingua = -1;
}