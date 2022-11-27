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
#include "Devolucoes.h"
#include "ControleCategorias.h"
#include <windows.h>
#include "ControleClientes.h"
#include <conio.h>
#define FILMENAOEXISTE 0
#define ERROMEM "ERRO: Memoria indisponivel!\n"

void limpaDadosFilmeMemoria(filmes *ptr, int quantidade){
    for(int i = 0; i<quantidade; i++){
        ptr[i].nome = limpaMemoria(ptr[i].nome);
        ptr[i].descricao = limpaMemoria(ptr[i].descricao);
    }
}


void mostraFilmes(filmes *ptr, int quantidade, int mostrarExcluidos){
    for(int i = 0; i<quantidade; i++){
                
        if(ptr[i].flag != 0 || mostrarExcluidos){
                printf("_________________\n");
                printf("|Nome: %s\n", ptr[i].nome);
                printf("|Codigo: %d\n", ptr[i].codigo);
                printf("|________________\n");
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
        fread(&ptr[0][i].valorLocacao, sizeof(float), 1, f);     
        fread(&ptr[0][i].lingua, sizeof(int), 1, f);
        fread(&ptr[0][i].flag, sizeof(int), 1, f);
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
        digText(&ptr[0][i].nome, f, '\n');
        digText(&ptr[0][i].descricao, f, '\n');
        fscanf(f, "%d ", &ptr[0][i].exemplares);
        fscanf(f,"%d ", &ptr[0][i].codigoCategoria);
        fscanf(f,"%f ", &ptr[0][i].valorLocacao);
        fscanf(f, "%d ", &ptr[0][i].lingua);
        fscanf(f, "%d  ", &ptr[0][i].flag);

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
        
        fwrite(&ptr[i].valorLocacao, sizeof(float), 1, f);
        
        fwrite(&ptr[i].lingua, sizeof(int), 1, f);
        
        fwrite(&ptr[i].flag, sizeof(int), 1, f);
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
        fprintf(f, "%f\n", ptr[i].valorLocacao);
        fprintf(f, "%d\n", ptr[i].lingua); 
        fprintf(f, "%d\n\n", ptr[i].flag);
    }
    fechaArquivo(&f);
    remove("filmes.txt");
    rename("filmesRes.txt", "filmes.txt");
}

filmes *encontraFilmeCodigo(filmes *ptr, int quantidade, char *codigo, int ignorar){
    if(!isdigit(codigo[0]) || strlen(codigo)>8){
        return NULL;
    }
    int cod = atoi(codigo);

    for(int i = 0; i<quantidade; i++){
        if(ptr[i].codigo == cod && (ptr[i].flag || ignorar)){
            return ptr+i;
        }
    }
    return NULL;
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
        digText(&dado, stdin, '\n');
        
        ptr = encontraFilmeCodigo(buscar, quantidade, dado,1);
        if(ptr){
            dado = limpaMemoria(dado);
            return ptr;
        }
        
        ptr = encontraFilmeNome(buscar, quantidade, dado);
        if(ptr && ptr->flag != FILMENAOEXISTE){
            dado = limpaMemoria(dado);
            return ptr;
        }
        if(!strcmp(dado, "open list")){
            dado = limpaMemoria(dado);
            return menuOpcoesFilmesOuClientes(buscar, quantidade, "lista completa de filmes da locadora:", 0, sizeof(filmes));
        }
        dado = limpaMemoria(dado);
        disparaSom(mensagem, 1);

        
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
    filmes *fil = NULL;
    int quantidadeFilmes;
    
    quantidadeFilmes = (*leDados[modoAbertura])(&fil);
    if(!contaQuantidadeExistente( sizeof(filmes), fil, quantidadeFilmes)){

        disparaSom("nao existe nenhum filme cadastrado no momento", 1);
        Sleep(2000);
        return;
    }
    printf("digite o nome ou codigo do filme que deseja editar\n");
    mostraFilmes(fil, quantidadeFilmes, 0);
    ptr = buscaFilme(fil, quantidadeFilmes, "Nenhum filme possui o dado digitado, por favor, digite um nome ou codigo");
    
    int dadoEditar;



    dadoEditar = escolheMenu("Qual campo deseja editar:", 6, 0,"Nome do filme", "Descricao", "Exemplares", "Codigo da categoria", "Lingua", "Voltar");


    switch(dadoEditar){
            
        case 0:
            printf("digite o nome do filme\n");
            ptr->nome = limpaMemoria(ptr->nome);
            digText(&ptr->nome, stdin, '\n');
            break;
        case 1:
            printf("digite a descricao do filme\n");
            ptr->descricao = limpaMemoria(ptr->descricao);
            digText(&ptr->descricao, stdin, '\n');
            break;

        case 2:
            printf("digite a quantidade de exemplares disponiveis\n");
            verificaNumero(&ptr->exemplares, "%d");
            break;
                    
        case 3:
            categoria *categorias;
            int quantidadeCategorias;
            int (*LeDadosCategoria[2])(categoria **) = {leDadosCategoria, leDadosCategoriaBin};
            quantidadeCategorias = (*LeDadosCategoria[modoAbertura])(&categorias);
                

            ptr->codigoCategoria = escolheListaCategorias(categorias, quantidadeCategorias, "Escolha a categoria do filme");
            ptr->valorLocacao = categorias[ptr->codigoCategoria].valorLocacao;
                
            limpaDadosCategoriaMemoria(categorias, quantidadeCategorias);
            categorias = limpaMemoria(categorias);
            break;
            
        case 4:

            ptr->lingua = escolheMenu("O filme é dublado ou legendado?", 2, 0,"Dublado", "Legendado")+1;

            break;
        case 5:
            break;

                                            

    
    }
    (*reescreveDados[modoAbertura])(fil, quantidadeFilmes);
    limpaDadosFilmeMemoria(fil, quantidadeFilmes);
    fil = limpaMemoria(fil);
    ptr = NULL;
}

void cadastraFilmes(int modoLeitura){
    filmes *filme;
    
    categoria *categorias;
    int quantidadeCategorias;
    int (*LeDadosCategoria[2])(categoria **) = {leDadosCategoria, leDadosCategoriaBin};
    quantidadeCategorias = (*LeDadosCategoria[modoLeitura])(&categorias);

    if(!verificaExisteCategoria(categorias, quantidadeCategorias)){
        printf("Nao existem categorias de filmes cadastrados ainda!\nPrimeiro cadastre uma categoria, para depois conseguir cadastrar um filme!\n");
        Sleep(2000);
        limpaDadosCategoriaMemoria(categorias, quantidadeCategorias);
        categorias = limpaMemoria(categorias);
        return;
    }
    int (*leDados[2])(filmes **) = {leDadosFilmes, leDadosFilmesBin};
    void (*reescreveDados[2])(filmes *, int) = {reescreveDadosFilme, reescreveDadosFilmeBin};
    int quantidadeFilmes = (*leDados[modoLeitura])(&filme) + 1;
    
    filme = (quantidadeFilmes == 1)?malloc(sizeof(filmes)):realloc(filme, sizeof(filmes)*quantidadeFilmes);
    verificaOperacao(filme, ERROMEM, 1);
    
    filme[quantidadeFilmes-1].codigo = quantidadeFilmes-1;
    
    printf("Digite o nome do filme\n");
    digText(&filme[quantidadeFilmes-1].nome, stdin, '\n');

    printf("Digite a descricao do filme\n");
    digText(&filme[quantidadeFilmes-1].descricao, stdin, '\n');


    filme[quantidadeFilmes-1].exemplares = 0;
    

    filme[quantidadeFilmes-1].codigoCategoria = escolheListaCategorias(categorias, quantidadeCategorias, "Escolha a categoria");

    filme[quantidadeFilmes-1].valorLocacao = categorias[filme[quantidadeFilmes-1].codigoCategoria].valorLocacao;
    

    filme[quantidadeFilmes-1].lingua = 1+escolheMenu("O filme e dublado ou legendado?", 2, 0,"Dublado", "Legendado");
    filme[quantidadeFilmes-1].flag = 1;

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
    
    if(!contaQuantidadeExistente( sizeof(filmes), filme, quantidadeFilmes)){
        printf("Ate o momento nao existe nenhum filme cadastrado\n");
        Sleep(3000);
        return;
    }

    printf("Digite o codigo ou nome do filme que deseja apagar\n");
    mostraFilmes(filme, quantidadeFilmes,0);
    filmes *apagar;
    apagar = buscaFilme(filme, quantidadeFilmes, "Nenhum filme tem o dado digitado, por favor, digite um nome, cpf ou codigo");
    deletaFilme(apagar);
    
    (*reescreveDados[modo])(filme, quantidadeFilmes);
    limpaDadosFilmeMemoria(filme, quantidadeFilmes);
    filme = limpaMemoria(filme);
    apagar = NULL;
}

void deletaFilme(filmes *apagar){
    apagar->flag = FILMENAOEXISTE;
}
void mostraInformacoesFilmes(filmes *todosFilmes, int quantidadeFilmes){

    for(int i = 0; i<quantidadeFilmes; i++){
        printf("_____________________________________________\n");
        printf("|    Nome: %s\n", todosFilmes[i].nome);
        printf("|    Descricao:\n");
        printf("|       %s\n", todosFilmes[i].descricao);
        printf("|    Codigo Categoria: %d\n", todosFilmes[i].codigoCategoria);
        printf("|    Valor alocacao: %.2f\n", todosFilmes[i].valorLocacao);
        printf("|    Quantidade exemplares: %d\n", todosFilmes[i].exemplares);
        printf("|    %s\n", (todosFilmes[i].lingua == 1)?"dublado":"legendado");
        printf("|    %s\n", (todosFilmes[i].flag)?"Ainda existe na lista de cadastro":"Filme ja foi excluido da lista de cadastro");
        printf("_____________________________________________");
    }

    printf("\ndigite qualquer tecla pra sair\n");
    getch();


}
filmes *filtraFilmPeloCodigo(filmes *ptr, int quantidade, int codigo1, int codigo2, int *quantidadeFiltrados){
    (*quantidadeFiltrados) = 0;
    filmes *filmesFiltrados = NULL;
    for(int i = codigo1; i<quantidade && i<codigo2; i++){
        if(ptr[i].codigo >= codigo1 && ptr[i].codigo <= codigo2 &&ptr[i].flag) {

            filmesFiltrados = (!(*quantidadeFiltrados))
                    ?malloc(sizeof(filmes))
                    :realloc(filmesFiltrados, sizeof(filmes)*((*quantidadeFiltrados)+1));

            filmesFiltrados[(*quantidadeFiltrados)] = ptr[i];
            mostraFilmes(ptr + i, 1, 1);
            (*quantidadeFiltrados)++;
        }

    }
    printf("quantidade de filmes nesta faixa de codigo: %d\n", (*quantidadeFiltrados));
    return filmesFiltrados;
}
/*
filmes *buscaFilmeComFaixaDeCodigo(filmes *ptr, int quantidade, int codigo, int codigo1){
    filmes *busca = NULL;
    char *codigoEsp;
    printf("digite o codigo do filme que deseja olhar na faixa de codigo %d a %d\n", codigo, codigo1);

    while(1) {
        digText(&codigoEsp, stdin);
        busca = encontraFilmeCodigo(ptr, quantidade, codigoEsp, 0);
        if(busca == NULL){
            printf("nao existe nenhum filme com este codigo!\n");
            continue;
        }
        if(busca->codigo >= codigo && busca->codigo <=codigo1){
            return busca;
        }
        printf("codigo digitado nao esta entre a faixa de codigo!\n");


    }
}
 */
void listaFilmesPelaCategoria(int modoArm, filmes *ptr, int quantidadeFilmes){

    categoria *todasCategorias = NULL;
    int quantidadeCategorias = 0;
    filmes *olhar = NULL;
    quantidadeCategorias = (modoArm)? leDadosCategoriaBin(&todasCategorias): leDadosCategoria(&todasCategorias);

    int quantidadeFiltrados = 0;
    int codigoCategoria;
    int escolha;
    filmes *filtrados = NULL;
    codigoCategoria = escolheListaCategorias(todasCategorias, quantidadeCategorias, "Escolha a categoria");
    limpaDadosCategoriaMemoria(todasCategorias, quantidadeCategorias);
    todasCategorias = limpaMemoria(todasCategorias);

    for(int i = 0; i<quantidadeFilmes; i++){

        if(ptr[i].codigoCategoria == codigoCategoria && ptr[i].flag){
            filtrados = (!quantidadeFiltrados)?malloc(sizeof(filmes)):realloc(filtrados, sizeof(filmes)*(quantidadeFiltrados+1));
            verificaOperacao(filtrados, "ERRO: Memoria indisponivel", 1);
            filtrados[quantidadeFiltrados] = ptr[i];

            mostraFilmes(ptr+i, 1, 1);
            quantidadeFiltrados++;
        }
    }
    printf("quantidade de filmes desta categoria: %d\n", quantidadeFiltrados);

    printf("deseja olhar algum filme especifico?\n");
    printf("F1 - sim\nF2 - nao\n");


    while(1){
        escolha = escolheOpcao();
        switch(escolha) {


            case 59:
                if(!quantidadeFiltrados){
                    printf("nao existem filmes pra olhar, pois nao existem filmes nesta categoria!\n");
                    Sleep(2000);
                    return;
                }
                break;
            case 60:
                atribuiNull(filtrados, quantidadeFiltrados, sizeof(filmes));
                filtrados = limpaMemoria(filtrados);
                return;
            default:
                printf("escolha uma opcao valida!\n");
                continue;

        }
        break;


    }
    printf("digite o codigo do filme que deseja olhar:\n");
    while(1) {

        olhar = buscaFilme(filtrados, quantidadeFiltrados, "nao existe nenhum filme com este codigo nesta categoria!\n");
        if(olhar->codigoCategoria == codigoCategoria){
            break;
        }
        printf("nao existe nenhum filme com este codigo nesta categoria!\n");
    }
    mostraInformacoesFilmes(olhar, 1);
    atribuiNull(filtrados, quantidadeFiltrados, sizeof(filmes));
    filtrados = limpaMemoria(filtrados);
}
void listaFilme(int modoArm){

    filmes *todosFilmes = NULL;
    int quantidadeFilmes = 0;

    quantidadeFilmes = (modoArm)?leDadosFilmesBin(&todosFilmes): leDadosFilmes(&todosFilmes);

    int escolha;


    escolha = escolheMenu("Como deseja filtrar os filmes", 3, 0,"Pelo codigo", "Pela categoria", "Voltar");




    switch(escolha) {
        case 0:
            listaPeloCodigo(todosFilmes,
                            quantidadeFilmes,
                            (void (*)(void *, int))mostraInformacoesFilmes,
                            "algum filme",
                            (void *(*)(void *, int, int, int, int*))filtraFilmPeloCodigo,
                            (void *(*)(void *, int, void *)) buscaFilme);
            break;

        case 1:
            listaFilmesPelaCategoria(modoArm, todosFilmes, quantidadeFilmes);
            break;


    }

}
