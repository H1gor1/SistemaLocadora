#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include <ctype.h>
#include "ControleDeDadosLocadora.h"
#include "ControleFilmes.h"
#include "FuncUtilitarias.h"
#include "ControleClientes.h"
#include "ControleDeNumeros.h"
#define ERROMEM "ERRO: memoria indiponivel!\n"

int digText(char **text, FILE *f){
    /*recebe um ponteiro para o ponteiro que guarda a primeira posicao da string, pois se necessario, precisa realocar a memoria da string, 
     e devolver o endereco para o ponteiro original, logo nao necessarios dois niveis de desreferenciacao
     Tambem recebe um ponteiro para o fluxo de dados onde deve pegar o texto, se for do teclado sera stdin, se nao sera um ponteiro para um arquivo qualquer*/
    
    
    /*malloc aloca uma posicao de char para a string inicialmente*/
    text[0] = malloc(sizeof(char));
   
    //chamada da funcao para verificar se a alocao foi feita com sucesso
    verificaOperacao(text[0], ERROMEM, 1);
    int indice = 0;//indice das letras do buffer
    
    do{
        /*faz a desreferenciacao do primeiro nivel do ponteiro, chegando ate o ponteiro original de onde a funcao foi chamada, logo apos, faz
         a 2 desreferenciacao, chegando ate o indice atual da string que deve receber a letra */
        text[0][indice] = fgetc(f);//f e passado indicando o fluxo de dados onde deve pegar os caracteres
        indice++;
        //se a proxima letra for enter ou caracter nulo, entao a string ja terminou, e nao e necessario alocar mais memoriaa 
        if(text[0][indice-1] != '\n' && text[0][indice-1] != '\0'){
            //caso nao seja, entao
            
            //funcao realloc aloca a memoria com o novo tamanho, mantendo o valor apontado antes
            text[0] = realloc(text[0], sizeof(char)*(indice+1));
            //chamada da funcao para verificar se a operacao foi bem sucedida
            verificaOperacao(text[0], ERROMEM, 1);
            
        }
        
    }while(text[0][indice-1] != '\n' && text[0][indice-1]!= '\0');//se texto no indice anterior for igual a enter, entao abandona o loop
    
    text[0][indice-1] = '\0';//text na posicao anterior recebe o caracter de finzalicao
    return indice-1;//retorna a quantidade de letras pega do buffer
}

/*funcao responsavel por verificar a ocorrencia de todos os caracteres passado no vetor de strings
 do primeiro argumento, no 2 argumento, ou seja, se no primeiro argumento houver "@." e no segundo
 "fulano@gmail.com" a funcao retornara 1, caso nao houver o @ ou . retornaria 0*/
int verificaOcorrencia(char *verificacoes, char *string){
    
    int i = 0; //indice inicial de verificacoes
    
    while(verificacoes[i] != '\0'){//enquanto nao chegar ao fim do vetor de verificacoes, continuar verificando
        /*strchr retorna um ponteiro nao nulo caso achar na primeira string passada como argumento uma ocorrencia
         do segundo argumento, que deve ser um char, caso nao achar retorna NULL, sendo assim, 
         se o retorno dela for NULL, o if é verdadeiro, pois a condicao esta negada, caso nao, o if e falso*/
        if(!strrchr(string, verificacoes[i])){
            return 0;//se a posicao retornada foi null, entao nao houve ocorrencia de algum caracter na string, logo
            //retorna falso
        }
        i++;//incrementa o indice
    }
    return 1;//caso o while foi abandonado e pra nenhum teste do if foi retornado NULL pela funcao strrchr, entao 
    //houve ocorrencia de todos os caracteres de verificacoes na string, logo retorna 1
}
//funcao responsavel por verificar se um texto do usuario tem todos os caracteres obrigatorios passados como primeiro argumento
// recebe um ponteiro para um ponteiro no 2 argumento pois ela precisa passar para digText tb um ponteiro de ponteiros
void verificaText(char *verificacoes, char **string, char *mensagem){
    //chamada da funcao para receber do buffer a digitacao de uma string
    digText(string, stdin);
    
    //enquanto a funcao verifica ocorrencia dentro do while retornar 0, que significa que a 
    //digitacao nao tinha todos os caracteres obrigatorios, o looping continuara executando
    while(!verificaOcorrencia(verificacoes, string[0])){
        printf("%s", mensagem);//caso o usuario digitou errado, exibe a mensagem de advertencia passada como 3 argumento
        string[0] = limpaMemoria(string[0]);//atravez do ponteiro em que **string aponta, a memoria onde o usuario digitou
        //o texto errado e limpada
        //chamada da funcao para o usuario digitar novamente
        digText(string, stdin);
    }

}
void converteTodasEmMinusculas(char *text){
    int i = 0;
    while(text[i]!='\0'){
        text[i] = tolower(text[i]);
        i++;
    }
}



int verificaStringRepetiu(cliente *ptr, int quantidade, cliente *posicao, char *string){
    
    for(int i = 0; i<quantidade; i++){
        if((ptr+i) != posicao){
            if(!strcmp(ptr[i].nomeCompleto, string) || !strcmp(ptr[i].cpf, string)){
                 return 1;
            }
        }
    }
    return 0;
}
void pegaDadoTextEverificaRepeticao(cliente *ptr, int quantidade, cliente *posicao, char **string, char *mensagem){
    
    while(1){
        if(strstr(mensagem, "cpf")){
            validaCnpjouCpf(string, 11, "987654321", "0123456789", 9, 9, "CPF invalido!", 1, 2);
        }else{
            digText(string, stdin);
        }
        if(verificaStringRepetiu(ptr, quantidade, posicao, string[0])){
            printf("%s\n", mensagem);
            string[0] = limpaMemoria(string[0]);
        }else{
            return;
        }
    }
}

