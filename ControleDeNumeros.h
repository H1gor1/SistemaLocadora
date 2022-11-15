#ifndef CONTROLEDENUMEROS_H
#define CONTROLEDENUMEROS_H

/*funcao responsavel por verificar se a digitacao de um numero foi correta, recebe por parametro o ponteiro para a variavel, um inteiro em que 0 representa
 inteiros, e 1 representa flutuantes, e a string de controle do tipo*/
void* verificaNumero(void *ptr, char *controle);
/*funcao responsavel por verificar a digitacao de numeros que precisam estar em um vetor, como cpf ou cnpj, recebe a primeira posicao de memoria
 do vetor, e a quantidade de numeros ela deve pegar do buffer*/
int verificaDigitos(int *ptr, int quant);
/*funcao responsavel por verificar se um cnpj digitado tem as condicoes minimas suficientes para ser valido*/
int validaCnpjouCpf(char **ptr, int quantidadedigitos, char *verificacoes, char *verificacoes2, int quantidadeMult, int posicao, char *mensagemeERRO, int somar, int somar2);
    
/*funcao responsavel por verificar a digitacao da inscricao estadual do usuario*/
int validaInsEstadual(char **ptr);
/*funcao usada para fazer a verificacao dos numeros de um cnpj ou cpf, o link a qual ela foi baseada e esse
https://www.macoratti.net/alg_cnpj.htm#:~:text=Algoritmo%20para%20valida%C3%A7%C3%A3o%20do%20CNPJ&text=O%20n%C3%BAmero%20que%20comp%C3%B5e%20o,que%20s%C3%A3o%20os%20d%C3%ADgitos%20verificadores.*/
int valida(char *ptr, char *vetorVerificacoes, int quantidade, int divisao, int posicao);

int verificaLimiteNumero(void *ptr, float limiteAlto, float limiteBaixo, char *controle);

char *converteIntEmString(int valorInt);

char *converteFloatemString(float valorFloat, int quantidadeCasasDecimais);

#endif

