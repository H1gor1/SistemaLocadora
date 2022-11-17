#ifndef FUNCUTILITARIAS_H
#define FUNCUTILITARIAS_H

int verificaNull(void *ptr);
void fechaArquivo(FILE **f);

int verificaOperacao(void *ptr, char *mensagem, int sairOuNao);

int contaQuantidadeExistente(size_t tamanho, void *ptr, int quantidade);
void verificaModoArmazenamento(int *var);

void trocaModoArmazenamento(int *modo);


void removeArquivos(int quantidadeRemov, ...);

void *limpaMemoria(void *ptr);

int escolheOpcao();

int retornaNumeroConformeF(int quantidadeOpcoes, int ignorarEsc);

void* menuOpcoesFilmesOuClientes(void *todos, int quantidade, char *mensagem, int ignoraApagados, size_t tamanho);

void listaPeloCodigo(void *ptr, int quantidade, void (*funcaoDeMostrar)(void *, int), char *mensagem, void *(*funcaoDeFiltrar)(void *, int, int, int, int*), void *(*funcaoDeBuscar)(void *, int, void *));
#endif
