#ifndef FUNCUTILITARIAS_H
#define FUNCUTILITARIAS_H

int verificaNull(void *ptr);
void fechaArquivo(FILE **f);

int verificaOperacao(void *ptr, char *mensagem, int sairOuNao);;


void verificaModoArmazenamento(int *var);

void trocaModoArmazenamento(int *modo);


void removeArquivos(int quantidadeRemov, ...);

void *limpaMemoria(void *ptr);

#endif
