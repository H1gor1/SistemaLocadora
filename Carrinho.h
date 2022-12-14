#ifndef CARRINHO_H
#define CARRINHO_H

void mostraListaFilmesAlocados(filmes *listaAlocacao, int quantidade);

void realizaVenda(int modoArm);

void alocaFilmes(filmes **filmesAlocados, int *quantidade, filmes *lista, int quantidadeLista, int ignora, char *frase);

filmes *deletaFilmeCarrinho(filmes *lista, int *quantidade, filmes *listaOriginal, int quantidadeListaOriginal, int vendaOuEntrada);

void limpaMemoriaRealizaVenda(cliente** clients, int quantidadeClientes,
                              Funcionarios ** func, int quantidadeFuncionarios, filmes ** todosFilmes, int quantidadeFilmes);
#endif