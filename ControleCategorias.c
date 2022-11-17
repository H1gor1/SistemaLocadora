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
#include <ctype.h>
#include <locale.h>
#include <windows.h>
#define categoriaNaoExiste '*'
#define ERROMEM "ERRO: Memoria indisponivel!\n"

int pegaCategoria(categoria *ptr, int quantidade){

    int cat;

    while(1){
        verificaNumero(&cat, "%d");
        if(cat>=quantidade){
            printf("Nao existe esta categoria! Por favor digite novamente!\n");
            continue;
        }
        if(!ptr[cat].flag){
            printf("Nao existe esta categoria! Por favor digite novamente!\n");
            continue;
        }
        break;
    }
    return cat;


}
int verificaExisteCategoria(categoria *ptr, int quantidade){
    
    if(quantidade == 0){
        return 0;
    }
    
    for(int i = 0; i<quantidade; i++){
        
        if(ptr[i].flag){
           return 1;  
        }
    }
    
    return 0;
}

void limpaDadosCategoriaMemoria(categoria *ptr, int quantidade){
    for(int i = 0; i<quantidade; i++){
        ptr[i].nome = limpaMemoria(ptr[i].nome);
        ptr[i].descricao = limpaMemoria(ptr[i].descricao);
    }
}

int leDadosCategoriaBin(categoria **ptr){
    
    int quantidade;
    int quantidadeLetras;
    FILE *f;  
    f = fopen("categorias.bin", "rb");
    
    if(!f){
        f = fopen("categorias.bin", "wb");
        quantidade = 0;
        fwrite(&quantidade, sizeof(int), 1, f);
        fechaArquivo(&f);
        return 0;
    }
    
    fread(&quantidade, sizeof(int), 1, f);
    ptr[0] = malloc(sizeof(categoria)*quantidade);
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
        
        fread(&ptr[0][i].valorLocacao, sizeof(float), 1, f);

        fread(&ptr[0][i].flag, sizeof(int), 1, f);
    }
    fechaArquivo(&f);
    return quantidade;
    
}

int leDadosCategoria(categoria **ptr){
    int quantidade;
    FILE *f;
    f = fopen("categorias.txt", "r");
    
    if(!f){
        f = fopen("categorias.txt", "w");
        fprintf(f, "0");
        fechaArquivo(&f);
        return 0;
    }
    
    fscanf(f, "%d ", &quantidade);
    ptr[0] = malloc(sizeof(categoria)*quantidade);
    verificaOperacao(ptr[0], ERROMEM, 1);
    
    for(int i = 0; i<quantidade; i++){
        
        fscanf(f, "%d ", &ptr[0][i].codigo);
        digText(&ptr[0][i].nome, f);
        digText(&ptr[0][i].descricao, f);
        fscanf(f, "%f", &ptr[0][i].valorLocacao);
        fscanf(f, "%d ", &ptr[0][i].flag);
    }
    fechaArquivo(&f);
    return quantidade;
}

void reescreveDadosCategoriaBin(categoria *ptr, int quantidade){
    
    FILE *f;
    f = fopen("categoriasRes.bin", "wb");
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
        
        fwrite(&ptr[i].valorLocacao, sizeof(float), 1, f);

        fwrite(&ptr[i].flag, sizeof(int), 1, f);
    }
    fechaArquivo(&f);
    remove("categorias.bin");
    rename("categoriasRes.bin", "categorias.bin");
}

void reescreveDadosCategoria(categoria *ptr, int quantidade){
    FILE *f;
    f = fopen("categoriasRes.txt", "w");
    if(!f){
        printf("erro ao salvar arquivo!Novos dados nao foram salvos!\n");
        printf("%s\n", strerror(errno));
        return;
    }
    fprintf(f, "%d\n", quantidade);
    
    for(int i = 0; i<quantidade; i++){
        
            fprintf(f, "%d\n", ptr[i].codigo);
            fprintf(f, "%s\n", ptr[i].nome);
            fprintf(f, "%s\n", ptr[i].descricao);
            fprintf(f, "%f\n", ptr[i].valorLocacao);
            fprintf(f, "%d\n\n", ptr[i].flag);
    }
    fechaArquivo(&f);
    remove("categorias.txt");
    rename("categoriasRes.txt", "categorias.txt");
}

categoria *encontraCategoriaCodigo(categoria *ptr, int quantidade, char *codigo, int ignorar ){
    if(!isdigit(codigo[0]) || strlen(codigo)>8){
        return NULL;
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

categoria *encontraCategoriaNome(categoria *ptr, int quantidade, char *nome){
    for(int i = 0; i<quantidade; i++){
        if(strcmp(ptr[i].nome, nome) == 0){
            return (ptr+i);
        }
    }
    return NULL;
}

categoria *buscaCategoria(categoria *buscar, int quantidade, char *mensagem){
    char *dado;
    categoria *ptr;
    do{
        digText(&dado, stdin);
        ptr = encontraCategoriaNome(buscar, quantidade, dado);
        if(ptr && strcmp(ptr->nome, "*")){
            dado = limpaMemoria(dado);
            return ptr;
        }
        ptr = encontraCategoriaCodigo(buscar, quantidade, dado,1);
        if(ptr){
            dado = limpaMemoria(dado);
            return ptr;
        }

        dado = limpaMemoria(dado);
        printf("%s\n", mensagem);
        
    }while(1);
}

void editaCategoria(int modoAbertura){
    int (*leDados[2])(categoria**) = {
        leDadosCategoria, leDadosCategoriaBin
    };
    void (*reescreveDados[2])(categoria*, int)={
        reescreveDadosCategoria, reescreveDadosCategoriaBin 
    };
    categoria *ptr = NULL;
    categoria *categorias = NULL;
    int quantidadeCategorias;
    
    quantidadeCategorias = (*leDados[modoAbertura])(&categorias);
    if(!verificaExisteCategoria(categorias, quantidadeCategorias)){
        printf("nao existe nenhuma categoria cadastrado no momento\n");
        Sleep(2000);
        return;
    }
    printf("digite o nome ou codigo da categoria que deseja editar\n");
    mostraListaCategoria(categorias, quantidadeCategorias);
    ptr = buscaCategoria(categorias, quantidadeCategorias, "Nenhuma categoria possui o dado digitado, por favor digite nome ou codigo");

    int dadoEditar;

    dadoEditar = escolheMenu("Qual campo deseja editar", 4, 0,"Nome", "Descricao", "Valor locacao", "Voltar");


    switch(dadoEditar){
        case 0:
            printf("Digite o nome:\n");
            ptr->nome = limpaMemoria(ptr->nome);
            digText(&ptr->nome, stdin);
            break;
        case 1:
            printf("Digite a nova descricao de categoria: \n");
            ptr->descricao = limpaMemoria(ptr->descricao);
            digText(&ptr->descricao, stdin);
            break;
        case 2:
            printf("Digite o valor da locacao:\n");
            verificaNumero(&ptr->valorLocacao, "%f");
            break;
        case 3:
            break;



    }
    (*reescreveDados[modoAbertura])(categorias, quantidadeCategorias);
    limpaDadosCategoriaMemoria(categorias, quantidadeCategorias);
    categorias = limpaMemoria(categorias);
    ptr = NULL;
}

void cadastraCategoria(int modoLeitura){
    categoria *categorias;
    int (*leDados[2])(categoria **) = {leDadosCategoria, leDadosCategoriaBin};
    void (*reescreveDados[2])(categoria *, int) = {reescreveDadosCategoria, reescreveDadosCategoriaBin};
    int quantidadeCategorias = leDados[modoLeitura](&categorias) + 1;
    
    categorias = (quantidadeCategorias == 1)?malloc(sizeof(categoria)):realloc(categorias, sizeof(categoria)*quantidadeCategorias);
    verificaOperacao(categorias, ERROMEM, 1);
    
    categorias[quantidadeCategorias-1].codigo = quantidadeCategorias-1;
    
    printf("digite o nome da categoria:\n");
    digText(&categorias[quantidadeCategorias-1].nome, stdin);
    
    printf("digite a descricao da categoria\n");
    digText(&categorias[quantidadeCategorias-1].descricao, stdin);
    
    printf("Digite o valor da locacao\n");
    verificaNumero(&categorias[quantidadeCategorias-1].valorLocacao, "%f");

    categorias[quantidadeCategorias-1].flag = 1;
    
    (*reescreveDados[modoLeitura])(categorias, quantidadeCategorias);
    limpaDadosCategoriaMemoria(categorias, quantidadeCategorias);
    categorias = limpaMemoria(categorias);
}

void apagaCategoria(int modo){
    
    printf("Aviso: A categoria que for apagada tambem por consequencia serao apagados todos os filmes associados a ela!\n");
    Sleep(2000);
    categoria *categoriass;
    filmes *Filmes;
    
    int(*leDadosFilm[2])(filmes **) = {
        leDadosFilmes, leDadosFilmesBin
    };
    void (*reescreveDadosFilm[2])(filmes *, int) = {
        reescreveDadosFilme, reescreveDadosFilmeBin
    };
    int quantidadeFilmes;
    int (*leDados[2])(categoria**) = {
      leDadosCategoria, leDadosCategoriaBin  
    };
    void (*reescreveDados[2])(categoria*, int) = {  
        reescreveDadosCategoria, reescreveDadosCategoriaBin
    };
    int quantidadeCategorias = (*leDados[modo])(&categoriass);
    
    if(!verificaExisteCategoria(categoriass, quantidadeCategorias)){
        printf("ate o momento nao existe nenhuma categoria cadastrado\n");
        Sleep(2000);
        return;
    }
    printf("digite o codigo ou nome da categoira que deseja apagar\n");
    mostraListaCategoria(categoriass, quantidadeCategorias);
    categoria *apagar;
    
    apagar = buscaCategoria(categoriass, quantidadeCategorias, "Nenhum categoria tem o dado digitado, por favor, digite um nome ou codigo");
    
    apagar->flag = 0;
    
    (*reescreveDados[modo])(categoriass, quantidadeCategorias);
    limpaDadosCategoriaMemoria(categoriass, quantidadeCategorias);
    
    
    quantidadeFilmes = (*leDadosFilm[modo])(&Filmes);
    
    for(int i = 0; i<quantidadeFilmes; i++){
        if(Filmes[i].codigoCategoria == apagar->codigo){
            deletaFilme((Filmes+i));
        }
    }
    (*reescreveDadosFilm[modo])(Filmes, quantidadeFilmes);
    categoriass = limpaMemoria(categoriass);
    apagar = NULL;
    limpaDadosFilmeMemoria(Filmes, quantidadeFilmes);
    Filmes = limpaMemoria(Filmes);
    Filmes = NULL;
    
}

void mostraListaCategoria(categoria *ptr, int quantidade){
    for(int i = 0; i<quantidade; i++){
        if(ptr[i].flag == 1){
            printf("_________________\n");
            printf("|Nome: %s\n", ptr[i].nome);
            printf("|Codigo: %d\n", ptr[i].codigo);
            printf("|________________\n");
        }
    }
}

int escolheListaCategorias(categoria *categorias, int quantidadeCategorias, char *mensagem){
    system("clear");
    int quantidadeCategoriasExistentes = contaQuantidadeExistente(sizeof(categoria), categorias, quantidadeCategorias);
    int indiceNomes = 0;
    int contador = 0;
    int escolha = 0;
    char **nomes;
    categoria *procurada = NULL;

    nomes  = malloc(sizeof(char *)*quantidadeCategoriasExistentes);
    verificaOperacao(nomes, "ERRO: Memoria indisponivel!", 1);

    for(int i = 0; i<quantidadeCategorias; i++){
        if(categorias[i].flag){
            nomes[indiceNomes] = categorias[i].nome;
            indiceNomes++;
        }
    }

    while(escolha != 13){
        menuGraphicsComSeta(quantidadeCategoriasExistentes, mensagem, contador, nomes, 0);
        escolha = escolheOpcao();

        switch(escolha){
            case 80:
                if(contador < quantidadeCategoriasExistentes-1){
                    contador++;
                }else{
                    contador = 0;
                }
                break;
            case 72:
                if(contador > 0){
                    contador--;
                }else{
                    contador = quantidadeCategoriasExistentes-1;
                }

        }

    }
    procurada = encontraCategoriaNome(categorias, quantidadeCategorias, nomes[contador]);
    nomes = limpaMemoria(nomes);
    return procurada->codigo;
}