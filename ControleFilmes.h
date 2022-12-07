#ifndef CONTROLEFILMES_H
#define CONTROLEFILMES_H

typedef struct filmes{
    int codigo;
    char *nome;
    char *descricao;
    int codigoCategoria;
    float valorLocacao;
    int exemplares;
    int lingua;
    int flag;
    float totalInvestido;
    float totalPago;
}filmes;

void apagaFilme(int modo);
void mostraFilmes(filmes *ptr, int quantidade, int mostrarExcluidos);
int leDadosFilmesBin(filmes **ptr);

int leDadosFilmes(filmes **ptr);

void cadastraFilmes(int modoLeitura);

void limpaDadosFilmeMemoria(filmes *ptr, int quantidade);

void reescreveDadosFilmeBin(filmes *ptr, int quantidade);

void reescreveDadosFilme(filmes *ptr, int quantidade);

filmes *encontraFilmeCodigo(filmes *ptr, int quantidade, char *codigo, int ignorar);

filmes *encontraFilmeNome(filmes *ptr, int quantidade, char *nome);

filmes *buscaFilme(filmes *buscar, int quantidade, char *mensagem);

void editaFilme(int modoAbertura);


void deletaFilme(filmes *ptr);

void listaFilme(int modoArm);

void listaFilmesPelaCategoria(int modoArm, filmes *ptr, int quantidadeFilmes);


filmes *filtraFilmPeloCodigo(filmes *ptr, int quantidade, int codigo1, int codigo2, int *quantidadeFiltrados);

void mostraInformacoesFilmes(filmes *todosFilmes, int quantidadeFilmes);


#endif