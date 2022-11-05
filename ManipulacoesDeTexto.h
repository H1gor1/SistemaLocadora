#ifndef MANIPULACOESDETEXTO_H
#define MANIPULACOESDETEXTO_H
typedef struct cliente cliente;
typedef struct filmes filmes;
/*Recebe um endereço de um ponteiro que aponta pra uma string, e conforme necessario aloca mais memoria para 
 o texto que o usuario estiver digitando, pega os caracteres de um arquivo passado como ponteiro ou do ponteiro da
 entrada strdin(entrada do buffer) retorna a quantidade de caracteres foram pegos*/
int digText(char **text, FILE *f);

/*funcao responsavel por verificar se a string passada como 2 parametro tem todos os argumentos da primeira string,
 caso nao ter, exibe a mensagem passada como 3 parametro*/
void verificaText(char *verificacoes, char **string, char *mensagem);
/*funcao responsavel por verificar a ocorrencia de caracteres passados em um vetor de strings no 2 vetor de string passado
 como argumento, caso houver ocorrencia de todos os caracteres, retorna 1, caso nao, retorna 0*/
int verificaOcorrencia(char *verificacoes, char *string);

void converteTodasEmMinusculas(char *text);

int verificaStringRepetiu(cliente *ptr, int quantidade, cliente *posicao, char *string);

void pegaDadoTextEverificaRepeticao(cliente *ptr, int quantidade, cliente *posicao, char **string, char *mensagem);
#endif
