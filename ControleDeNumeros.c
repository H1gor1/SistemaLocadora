#include<stdio.h>
#include<stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include "ControleDeDadosLocadora.h"
#include "FuncUtilitarias.h"
#include <math.h>
#define ENTER '\n'

/*funcao responsavel por validar um cpf ou cnpj, basta mudar os parametros
 o primeiro parametro, refere-se a um ponteiro para um cpf, ou cnpj que deseja verificar a validade,
 o segundo parametro refere-se a uma string de numeros de verificacoes
 o 3 parametro e a quantidade de numeros deve entrar no somatorio, se cpf entao 9, se cnpj entao 12
 o 4 e o numero que o somatorio sera dividido
 o 5 e a posicao que sera verificada se o digito esta correto
 e o 6 refere-se se cada numero de verificacoes deve ser somado ou nao, pois como no cpf sao necessarios chegar
 ao numero 10 ou 11, e nao existe um digito para 10 ou 11, entao e necessario conseguilos com 8+2*/
//o site de referencia do algoritimo esta no fim do arquivo
int valida(char *ptr, char *vetorVerificacoes, int quantidade, int divisao, int posicao, int somar){
    
    //somatorio dos digitos do cpf ou cpnj multiplicado pelo vetor de verificacoes
    int somatorio = 0;
    //ciclo for faz o somatorio ate a quantidade pedida
    for(int i = 0; i<quantidade; i++){
        /*por padrao os caracteres de 0 a 9 na tabela ascii estao em ordem crescente, 
         entao se subtrai-los de 0, chegamos ao valor convertido*/
        somatorio+=(ptr[i]-'0')*(vetorVerificacoes[i]-'0'+somar);
    }
    //se o resto da divisao de somatorio pela var divisao for < que 2
    if(somatorio%divisao <2){
        //entao verifica se o numero na posicao desejada e igual a 0
        if((ptr[posicao]-'0')== 0){
            return 1;//se for o cnpj ou cpf e valido
        }else{//se falso, invalido
            return 0;
        }
    }else{//caso falso que seja menor que 2, entao
        //11 - o resto dessa divisao deve ser igual a posicao pedida
        if(11-(somatorio%divisao) == (ptr[posicao]-'0')){
            return 1;//caso verdadeiro, o cnpj ou cpf e valido
        }else{//caso falso, invalido
            return 0;
        }
    }
    
}

/*funcao responsavel por verificar a digitacao de um numero de qualquer tipo, recebe um ponteiro generico, ou seja
 um ponteiro float, int, double, etc, e uma string de controle referente ao tipo*/
void verificaNumero(void *ptr, char *controle){
    
    int digCert;//var que representa se o usuario digitou certo ou nao
    
    do{
        digCert = 0;//inicializa a variavel com 0
        scanf(controle, ptr);//e passado para o scanf a string de controle, e o ponteiro
        while(getchar()!= '\n'){//se no buffer existir alguma letra alem de enter, o codigo do while e executado
            digCert++;//se existir, entao o usuario digitou algo alem de apenas numeros, entao a var digCert e incrementada
        }
        
        if(digCert){//se dig cert for qualquer valor maior ou igual a 1
            printf("Por favor, digite somente numeros!\n");//entao essa mensagem e mostrada
        }
    }while(digCert != 0);//se dig cert for qualquer valor alem de 0, entao o usuario digitou errado, e deve digitar novamente
    
}
//funcao responsavel por verificar os digitos de um vetor de char que so pode ter numeros, recebe uma quantidade que significa a quantidade de digitos
//ela deve verificar
int verificaDigitos(char **ptr, int quant){
    
    ptr[0] = malloc(sizeof(char)*(quant+1));//aloca a quantidade de caracteres serao verificados a quantidade +1 e devolve o endereco desse
    //vetor para o ponteiro apontado por ptr
    
    //chamada da funcao para verificar se a operacao de alocacao foi feita com sucesso
    verificaOperacao(ptr[0], "ERRO: Memoria indisponivel!\n", 1);
    int i = 0;//indice do numero atual pego do buffer
    int numeroAnterior;
    //loop que ocorre ate chegar na quantidade de letras passada para a funcao verificar
    for(i = 0; i<quant; i++){
        ptr[0][i] = getchar();//pega o proximo caractere do buffer para a posicao atual do vetor
        
        //se a letra pega nao for um digito numerico, a funcao is digit retornara 0, e a expressao negada no if sera verdadeira
        if(!isdigit(ptr[0][i])){
            if(ptr[0][i]!='\n'){
                while(getchar()!='\n');
            }  
            return 0;
        }
        //caso nao aconteca, entao is digit retornara 1, e a expressao sera falsa, e o codigo do if nao sera executado
    }
    ptr[0][i] = '\0';
    //apos o fim do loop, e testado se a proxima letra do buffer e um enter, se for, significa que a digitacao foi feita com sucesso
    if(getchar()!='\n'){
        
        while(getchar()!='\n');
        return 0;//entao retorna 0
    }
    //a proxima verificacao e necessaria somente para cpf, pois o algoritimo aceita o cpf 11111111111,
    //mas por regra, cpfs com numeros iguais sao descartados como invalidos
    numeroAnterior = ptr[0][0];//numero anterior recebe o primeiro numero do cpf
    for(int i = 1; i<quant; i++){
        /*se em algum momento o numero anterior nao foi igual ao atuall*/
        if(numeroAnterior != ptr[0][i]){
            return 1;//entao  o  cpf e valido
        }
    }
    return 0;//se o loop for testou igual em todos os ciclos, entao o cpf e falso
   
    
    
}
/*funcao responsavel por validar um cpf ou cnpj, recebe um ponteiro para a string onde ficara guardado o codigo
 o 2 argumento refere-se a quantidade de digitos deverao ser verificados
 o 3 e a primeira string de verificacoes
 o 4 e a segunda string de verificoes
 o 5 e a quantidade de digitos devem entrar no somatorio para a vericacao da do cpf ou cnpj
 o 6 e a posicao que deve ser verificada
 o 7 e a mensagem caso houver erro
 o 8 e o 9 e a soma de cada digito, se necessario, para o caso de testar cpf*/

int validaCnpjouCpf(char **ptr, int quantidadedigitos, char *verificacoes, char *verificacoes2, int quantidadeMult, int posicao, char *mensagemeERRO, int somar, int somar2){
    
    do{
        
        //enquanto a digitacao nao ter 14 numeros e todos serem numeros, o ciclo while nao e abandonado
        while(!verificaDigitos(ptr, quantidadedigitos)){
            free(ptr[0]);
            printf("%s\n", mensagemeERRO);
            
            
        }
        
        //funcao valida e chamada trocando os parametros para verificar o penultimo e o ultimo digito, se os dois derem verdadeiro, o cnpj e valido
        if(valida(ptr[0], verificacoes, quantidadeMult, 11, posicao, somar) && valida(ptr[0], verificacoes2, quantidadeMult+1, 11, posicao+1, somar2)){
            return 1;//retorna verdadeiro e o ciclo do while e abandonado
        }else{//se falso, entao 
            //limpa a memoria onde estava digitado o antigo valor do cnpj
            ptr[0] = limpaMemoria(ptr[0]);
            //pede a digitacao de um novo cnpj valido
            printf("%s\n", mensagemeERRO);
        }
    }while(1);
    
    
}
//funcao responsavel por validar a inscricao estadual digitada pelo usuario
int validaInsEstadual(char **ptr){
    //enquanto nao forem digitados exatamentes 9 digitos, o ciclo for nao sera abandonado
    while(!verificaDigitos(ptr, 9)){
        printf("Inscricao Estadual invalida!Por favor, digite um valor valido!\n");
    }
    return 1;
}
/*funcao responsavel por verificar a digitacao de um numero, e testar se ele esta dentro do limite pedido*/
int verificaLimiteNumero(void *ptr, float limiteAlto, float limiteBaixo, char *controle){
    /*ponteiros de apoio para desrefenciar o valor digitado e verificar o limite*/
    int *ptrInteiro = NULL;
    float *ptrFloat = NULL;
    
    do{
        //chama a funcao para verificar o numero digitado pelo usuario, verificando se ele nao digitou letras em 
        //vez de numeros
        verificaNumero(ptr, controle);
        
        //se foi pedido um inteiro pela string de controle, entao
        if(strcmp(controle, "%d")==0){
            ptrInteiro = ptr;//ptr do tipo inteiro recebe o mesmo endereco que o ptr void 
            if(*ptrInteiro <= limiteAlto && *ptrInteiro >=limiteBaixo){//se o valor de ponteiro inteiro for menor ou igual ao limite
                return 1;//retorna verdadeiro
            }
            //se nao, entao digitou um valor incorreto
            
        }else{//se falso que foi pedido um valor inteiro, entao
            //ptr float recebe o endereco do ponteiro
            ptrFloat = ptr;
            if(*ptrFloat <= limiteAlto && *ptrFloat >=limiteBaixo){//se o valor apontado por ponteiroFloat for menor ou igual ao limite
                return 1;//retorna 1
            }
            //se nao, entao digitou errado
        }
        //se os ifs de verificaco de valor de limite nao foram executados, entao e pedido para o usuario digitar novamente
        //um valor, respeitando o limite
        printf("voce digitou um valor invalido! O valor maximo e ");
        printf("%.0f e o minimo e %.0f\n", floor(limiteAlto), limiteBaixo);
        printf(" digite novamente:\n");
    }while(1);
}

char *converteIntEmString(int valorInt){
    

    char *valor;
    int indice = 0;
    if(valorInt == 0){
        valor = malloc(sizeof(char)*2);
        strcpy(valor, "0");
        return valor;
    }
    while(valorInt>0){
        
        valor = (indice == 0)?malloc(sizeof(char)):realloc(valor, sizeof(char)*(indice+1));
        
        valor[indice] = (valorInt%10)+'0';
        valorInt /= 10;
        
        indice++;
    }
    
    valor = realloc(valor, sizeof(char)*(indice+1));
    valor[indice] = '\0';
    
    char temporario;
    int indiceContra = strlen(valor)-1;
    indice = 0;
    while(indice<indiceContra){
        temporario = valor[indice];
        valor[indice] = valor[indiceContra];
        valor[indiceContra] = temporario;
        
        indice++;
        indiceContra--;
        
    }
    return valor;
}
char *converteFloatemString(float valorFloat, int quantidadeCasasDecimais){
    char *valor = converteIntEmString(valorFloat);
    valor = realloc(valor, sizeof(char)*(strlen(valor)+quantidadeCasasDecimais+2));
    int valorQuebrado;
    int indice = strlen(valor);
    valor[indice] = ',';
    indice++;
    for(int i = 0; i<quantidadeCasasDecimais;i++){

        valorQuebrado = (valorFloat*=10);

        valor[indice] = (valorQuebrado%10)+'0';
        indice++;
    }
    valor[indice] = '\0';
    return valor;

}

