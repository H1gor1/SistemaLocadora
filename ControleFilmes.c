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
        digText(&ptr[0][i].nome, f);
        digText(&ptr[0][i].descricao, f);
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
    if(cod>=quantidade){
        return NULL;
    }
    if(ptr[cod].flag == 0 && ignorar){
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
        if(ptr && ptr->flag != FILMENAOEXISTE){
            dado = limpaMemoria(dado);
            return ptr;
        }
        if(!strcmp(dado, "open list")){
            dado = limpaMemoria(dado);
            return menuEscolhaFilmes(buscar, quantidade, "lista de filmes", 0);
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
    filmes *fil = NULL;
    int quantidadeFilmes;
    
    quantidadeFilmes = (*leDados[modoAbertura])(&fil);
    if(!contaQuantidadeExistente( sizeof(filmes), fil, quantidadeFilmes)){
        printf("nao existe nenhum filme cadastrado no momento\n");
        Sleep(2000);
        return;
    }
    printf("digite o nome ou codigo do filme que deseja editar\n");
    mostraFilmes(fil, quantidadeFilmes, 0);
    ptr = buscaFilme(fil, quantidadeFilmes, "Nenhum filme possui o dado digitado, por favor, digite um nome ou codigo");
    
    int dadoEditar;



    dadoEditar = escolheMenu("Qual campo deseja editar:", 6, "Nome do filme", "Descricao", "Exemplares", "Codigo da categoria", "Lingua", "Voltar");


    switch(dadoEditar){
            
        case 0:
            printf("digite o nome do filme\n");
            ptr->nome = limpaMemoria(ptr->nome);
            digText(&ptr->nome, stdin);
            break;
        case 1:
            printf("digite a descricao do filme\n");
            ptr->descricao = limpaMemoria(ptr->descricao);
            digText(&ptr->descricao, stdin);
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

            ptr->lingua = escolheMenu("O filme é dublado ou legendado?", 2, "Dublado", "Legendado")+1;

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
    digText(&filme[quantidadeFilmes-1].nome, stdin);

    printf("Digite a descricao do filme\n");
    digText(&filme[quantidadeFilmes-1].descricao, stdin);

    printf("digite a quantidade de exemplares para alugar do filme:\n");
    verificaNumero(&filme[quantidadeFilmes-1].exemplares, "%d");
    

    filme[quantidadeFilmes-1].codigoCategoria = escolheListaCategorias(categorias, quantidadeCategorias, "Escolha a categoria");

    filme[quantidadeFilmes-1].valorLocacao = categorias[filme[quantidadeFilmes-1].codigoCategoria].valorLocacao;
    

    filme[quantidadeFilmes-1].lingua = 1+escolheMenu("O filme e dublado ou legendado?", 2, "Dublado", "Legendado");
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
void filtraFilmPeloCodigo(filmes *ptr, int quantidade, int codigo1, int codigo2){
    int quantidadeFilmes = 0;
    for(int i = codigo1; i<quantidade && i<codigo2; i++){

            quantidadeFilmes++;
            mostraFilmes(ptr+i, 1, 1);

    }
    printf("quantidade de filmes nesta faixa de codigo: %d\n", quantidadeFilmes);
}
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
void listaFilmesPelaCategoria(int modoArm, filmes *ptr, int quantidadeFilmes){

    categoria *todasCategorias = NULL;
    int quantidadeCategorias = 0;
    filmes *olhar = NULL;
    quantidadeCategorias = (modoArm)? leDadosCategoriaBin(&todasCategorias): leDadosCategoria(&todasCategorias);

    int quantidadeDeFilmesDestaCategoria = 0;
    int codigoCategoria;
    int escolha;

    codigoCategoria = escolheListaCategorias(todasCategorias, quantidadeCategorias, "Escolha a categoria");
    limpaDadosCategoriaMemoria(todasCategorias, quantidadeCategorias);
    todasCategorias = limpaMemoria(todasCategorias);

    for(int i = 0; i<quantidadeFilmes; i++){

        if(ptr[i].codigoCategoria == codigoCategoria){
            mostraFilmes(ptr+i, 1, 1);
            quantidadeDeFilmesDestaCategoria++;
        }
    }
    printf("quantidade de filmes desta categoria: %d\n", quantidadeDeFilmesDestaCategoria);

    printf("deseja olhar algum filme especifico?\n");
    printf("F1 - sim\nF2 - nao\n");


    while(1){
        escolha = escolheOpcao();
        switch(escolha) {


            case 59:
                if(!quantidadeDeFilmesDestaCategoria){
                    printf("nao existem filmes pra olhar, pois nao existem filmes nesta categoria!\n");
                    Sleep(2000);
                    return;
                }
                break;
            case 60:
                return;
            default:
                printf("escolha uma opcao valida!\n");
                continue;

        }
        break;


    }
    printf("digite o codigo do filme que deseja olhar:\n");
    while(1) {

        olhar = buscaFilme(ptr, quantidadeFilmes, "nao existe nenhum filme com este codigo nesta categoria!\n");
        if(olhar->codigoCategoria == codigoCategoria){
            break;
        }
        printf("nao existe nenhum filme com este codigo nesta categoria!\n");
    }
    mostraInformacoesFilmes(olhar, 1);
}
void listaFilme(int modoArm){

    filmes *todosFilmes = NULL;
    int quantidadeFilmes = 0;

    quantidadeFilmes = (modoArm)?leDadosFilmesBin(&todosFilmes): leDadosFilmes(&todosFilmes);

    int escolha;


    escolha = escolheMenu("Como deseja filtrar os filmes", 3, "Pelo codigo", "Pela categoria", "Voltar");




    switch(escolha) {
        case 0:
            listaPeloCodigo(todosFilmes,
                            quantidadeFilmes,
                            (void (*)(void *, int))mostraInformacoesFilmes,
                            "algum filme",
                            (void (*)(void *, int, int, int))filtraFilmPeloCodigo,
                            (void *(*)(void *, int, int, int))buscaFilmeComFaixaDeCodigo);
            break;

        case 1:
            listaFilmesPelaCategoria(modoArm, todosFilmes, quantidadeFilmes);
            break;


    }

}
filmes* menuEscolhaFilmes(filmes *todosOsfilmes, int quantidade, char *mensagem, int ignoraApagados){
    system("clear");
    int quantidadeExistente = (!ignoraApagados)?contaQuantidadeExistente(sizeof(filmes), todosOsfilmes, quantidade):quantidade;
    int indiceFilme = 0;
    int contador = 0;
    int escolha = 0;
    char **informacoesFilmes = NULL;
    char *nomeConcat = NULL;

    int quantidadeCategorias;
    filmes *filmeProcurado =NULL;


    informacoesFilmes = malloc(sizeof(char*)*quantidadeExistente);
    verificaOperacao(informacoesFilmes, "ERRO: Memoria indisponivel!", 1);

    for(int i = 0; i<quantidade; i++){
        if(todosOsfilmes[i].flag || ignoraApagados){
            informacoesFilmes[indiceFilme] = malloc(sizeof(char)*(strlen(todosOsfilmes[i].nome+60)));
            verificaOperacao(informacoesFilmes[indiceFilme], "ERRO: Memoria indisponivel!", 1);

            strcpy(informacoesFilmes[indiceFilme], todosOsfilmes[i].nome);

            strcat(informacoesFilmes[indiceFilme], "    valor: ");

            nomeConcat = converteFloatemString(todosOsfilmes[i].valorLocacao, 2);
            strcat(informacoesFilmes[indiceFilme],
                   nomeConcat);
            nomeConcat = limpaMemoria(nomeConcat);

            strcat(informacoesFilmes[indiceFilme], "      quantidade: ");

            nomeConcat = converteIntEmString(todosOsfilmes[i].exemplares);
            printf("oi");
            strcat(informacoesFilmes[indiceFilme], nomeConcat);
            nomeConcat = limpaMemoria(nomeConcat);

            indiceFilme++;

        }
    }

    while(escolha != 13){

        menuGraphicsComSeta(quantidadeExistente, "escolha um filme", contador, informacoesFilmes);
        escolha = escolheOpcao();

        switch(escolha){

            case 80:
                if(contador<quantidadeExistente-1){
                    contador++;
                }else{
                    contador = 0;
                }
                break;
            case 72:
                if(contador>0){
                    contador--;
                }else{
                    contador = quantidadeExistente-1;
                }
                break;

        }
    }
    filmeProcurado = encontraFilmeNome(todosOsfilmes, quantidade, informacoesFilmes[contador]);
    for(int i = 0; i<quantidadeExistente; i++){
        informacoesFilmes[i] = limpaMemoria(informacoesFilmes[i]);

    }
    informacoesFilmes = limpaMemoria(informacoesFilmes);
    return filmeProcurado;




}