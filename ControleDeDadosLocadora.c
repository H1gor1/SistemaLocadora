#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include "ManipulacoesDeTexto.h"
#include "ControleDeDadosLocadora.h"
#include "ControleDeNumeros.h"
#include "FuncUtilitarias.h"
#include "menus.h"
#define VAZIO NULL
#define ERROMEM "ERRO: Memoria indisponivel!\n"
#define ESC 27


/*funcao responsavel por limpar os campos de texto da struct da locadora da memoria*/
void limpaCamposLocadoraMemoria(locadora *ptr){
    /*cada campo e desalocado da memoria e seu ponteiro recebe um endereco null*/    
    ptr->nomeFantasia = limpaMemoria(ptr->nomeFantasia);
    ptr->razaoSocial = limpaMemoria(ptr->razaoSocial);
    ptr->inscricaoEstadual = limpaMemoria(ptr->inscricaoEstadual);
    ptr->cnpj = limpaMemoria(ptr->cnpj);
    ptr->endereco = limpaMemoria(ptr->endereco);
    ptr->email = limpaMemoria(ptr->email);
    ptr->nomeResponsavel = limpaMemoria(ptr->nomeResponsavel);
}
/*funcao responsavel por cadastrar as informacoes sobre a locadora, recebe um ponteiro para a struct de locadora*/
void cadastraLocadora(locadora *ptr){
    
    printf("digite o nome fantasia da locadora\n");
    /*chamada da funcao para o usuario digitar nome fantasia da locadora*/
    digText(&(ptr->nomeFantasia), stdin, '\n');
   
    printf("digite a razao social da locadora\n");
    /*chamada da funcao para o usuario digitar a razao social da locadora*/
    digText(&(ptr->razaoSocial), stdin, '\n');
    
    printf("digite a inscricao estadual da locadora\n");
    /*chamada da funcao para o usuario digitar a inscricao estadual da locadora */
    validaInsEstadual(&(ptr->inscricaoEstadual));
    
    printf("digite o cnpj da locadora\n");
    /*chamada da funcao para o usuario digitar o cnpj da locadora*/
    validaCnpjouCpf(&(ptr->cnpj), 14, "543298765432", "6543298765432", 12, 12, "CNPJ digitado invalido!", 0, 0);
    
    printf("digite o endereco completo da locadora\n");
    /*chamada da funcao para o usuario digitar o endereco da locadora*/
    digText(&(ptr->endereco), stdin, '\n');
    
    printf("digite o telefone da locadora\n"); 
    /*chamada da funcao para o usuario digitar o numero de telefone da locadora*/
    verificaNumero(&(ptr->telefone),"%d");
    
    printf("digite o email da locadora\n");
    /*chamada da funcao para o usuario digitar o email da locadora*/
    verificaText("@.", &(ptr->email), "Por favor, digite um email valido!\n");
    
    printf("digite o nome do responsavel da locadora\n");
    /*chamada da funcao para o usuario digitar o nome do responsavel da locadora*/
    digText(&(ptr->nomeResponsavel), stdin, '\n');
    
    printf("digite o telefone do responsavel\n");
    /*chamada da funcao para o usuario digitar o numero do telefone do responsavel da locadora*/
    verificaNumero(&(ptr->telefoneResp),"%d");
    
    printf("digite o valor da multa caso algum cliente atrase na devolucao:\n");
    verificaNumero(&(ptr->multa), "%f");
    
}
/*funcao responsavel por rescrever os dados da locadora novamente no arquivo de texto, ou escrever pela primeira vez,
 recebe como argumento um ponteiro da struct locadora, e o ponteiro do arquivo em que deve escrever as informacoes
 que estao dentro da struct*/
void reescreveDadosLocadora(locadora *ptr, FILE *f){
    /*escreve cada campo da struct dentro do arquivo e em seguida uma quebra de linha*/
    fprintf(f, "%s\n", ptr->nomeFantasia);
    fprintf(f, "%s\n", ptr->razaoSocial);
    fprintf(f, "%s\n", ptr->inscricaoEstadual);
    fprintf(f, "%s\n", ptr->cnpj);
    fprintf(f, "%s\n", ptr->endereco);
    fprintf(f, "%d\n", ptr->telefone);
    fprintf(f, "%s\n", ptr->email);
    fprintf(f, "%s\n", ptr->nomeResponsavel);
    fprintf(f, "%d\n", ptr->telefoneResp);
    fprintf(f, "%f\n", ptr->multa);
}
/*funcao responsavel por reescrever os dados da locadora em binario, recebe um ponteiro para a struct de locadora, e o arquivo onde deve salvar*/
void reescreveDadosLocadoraBin(locadora *ptr, FILE *f){
    /*pucha a quantidade de letras o campo de texto atual gastara e soma 1 para guardar tb o caracter nulo*/
    int quantidadeLetras = strlen(ptr->nomeFantasia)+1;
    /*escreve a quantidade de letras a palavra gastara*/
    fwrite(&quantidadeLetras, sizeof(int), 1, f);
    /*escreve o tamanho de um char vezes a quantidade de letras pegando como referencia os bits da memoria apontado pelo ponteiro nome fantasia*/
    fwrite(ptr->nomeFantasia, sizeof(char), quantidadeLetras, f);
    
    quantidadeLetras = strlen(ptr->razaoSocial)+1;
    fwrite(&quantidadeLetras, sizeof(int), 1, f);
    fwrite(ptr->razaoSocial, sizeof(char), quantidadeLetras, f);
    
    quantidadeLetras = strlen(ptr->inscricaoEstadual)+1;
    fwrite(&quantidadeLetras, sizeof(int), 1, f);
    fwrite(ptr->inscricaoEstadual, sizeof(char),quantidadeLetras, f);
    
    quantidadeLetras = strlen(ptr->cnpj)+1;
    fwrite(&quantidadeLetras, sizeof(int), 1, f);
    fwrite(ptr->cnpj, sizeof(char), quantidadeLetras, f);
    
    quantidadeLetras = strlen(ptr->endereco)+1;
    fwrite(&quantidadeLetras, sizeof(int), 1, f);
    fwrite(ptr->endereco, sizeof(char), quantidadeLetras, f);
    /*escreve no arquivo o tamanho de um inteiro pegando como referenciao os bits da posicao de memoria de telefone*/
    fwrite(&ptr->telefone, sizeof(int), 1, f);
    
    quantidadeLetras = strlen(ptr->email)+1;
    fwrite(&quantidadeLetras, sizeof(int), 1, f);
    fwrite(ptr->email, sizeof(char), quantidadeLetras, f);
    
    quantidadeLetras = strlen(ptr->nomeResponsavel)+1;
    fwrite(&quantidadeLetras, sizeof(int), 1, f);
    fwrite(ptr->nomeResponsavel, sizeof(char), quantidadeLetras, f);
    
    fwrite(&ptr->telefoneResp, sizeof(int), 1, f);
    
    fwrite(&ptr->multa, sizeof(float), 1, f);
}
/*funcao responsavel por ler os dados da locadora em binario, recebe como argumento um ponteiro para a struct de locadora, e o nome
 do arquivo de onde deve ler as informacoes*/
void leDadosLocadoraBin(locadora *ptr, char *nomeArq){
    FILE *f;
    //abre o arquivo em modo de leitura
    f = fopen(nomeArq, "rb");
    /*quantidade de letras cada campo de letras ocupara*/
    int quantidadeLetras;
    /*le a quantidade de letras sera necessaria para alocar a proxima palavra*/
    fread(&quantidadeLetras, sizeof(int), 1, f);
    /*aloca a quantidade de letras necessarias para a palavra*/
    ptr->nomeFantasia = malloc(sizeof(char)*quantidadeLetras);
    /*verifica se houve memoria o suficiente para a operacao*/
    verificaOperacao(ptr->nomeFantasia, ERROMEM, 1);
    /*copia para a posicao de memoria apontada por nome fantasia o tamanho de um char vezes a quantidade de letras do arquivo f bit a bit*/
    fread(ptr->nomeFantasia, sizeof(char), quantidadeLetras, f);
    
    
    /*as logicas dos campos abaixo sao iguais quando os campos forem strings*/
    fread(&quantidadeLetras, sizeof(int), 1, f);
    ptr->razaoSocial = malloc(sizeof(char)*quantidadeLetras);
    verificaOperacao(ptr->razaoSocial, ERROMEM, 1);
    fread(ptr->razaoSocial, sizeof(char), quantidadeLetras, f);
    
    fread(&quantidadeLetras, sizeof(int), 1, f);
    ptr->inscricaoEstadual = malloc(sizeof(char)*quantidadeLetras);
    verificaOperacao(ptr->inscricaoEstadual, ERROMEM, 1);
    fread(ptr->inscricaoEstadual, sizeof(char), quantidadeLetras, f);
    
    fread(&quantidadeLetras, sizeof(int), 1, f);
    ptr->cnpj = malloc(sizeof(char)*quantidadeLetras);
    verificaOperacao(ptr->cnpj, ERROMEM, 1);
    fread(ptr->cnpj, sizeof(char), quantidadeLetras, f);
    
    fread(&quantidadeLetras, sizeof(int), 1, f);
    ptr->endereco = malloc(sizeof(char)*quantidadeLetras);
    verificaOperacao(ptr->endereco, ERROMEM, 1);
    fread(ptr->endereco, sizeof(char), quantidadeLetras, f);
    
    /*le o tamanho de um inteiro para o a posicao de memoria de telefone, do arquivo f*/
    fread(&ptr->telefone, sizeof(int), 1, f);
    
    fread(&quantidadeLetras, sizeof(int), 1, f);
    ptr->email = malloc(sizeof(char)*quantidadeLetras);
    verificaOperacao(ptr->email, ERROMEM, 1);
    fread(ptr->email, sizeof(char), quantidadeLetras, f);
    
    fread(&quantidadeLetras, sizeof(int), 1, f);
    ptr->nomeResponsavel = malloc(sizeof(char)*quantidadeLetras);
    verificaOperacao(ptr->nomeResponsavel, ERROMEM, 1);
    fread(ptr->nomeResponsavel, sizeof(char), quantidadeLetras, f);
    
    fread(&ptr->telefoneResp, sizeof(int), 1, f);
    
    fread(&ptr->multa, sizeof(float), 1, f);
    /*depois de terminar de fazer a leitura, fecha o arquivo f*/
    fechaArquivo(&f);
    
}


/*funcao responsavel por ler os dados da locadora de um arquivo de texto, recebe um ponteiro onde deve salvar os dados em memoria
 e o nome do arquivo que deve abrir*/
void leDadosLocadora(locadora *ptr, char *nomeArq){
    FILE *f;
    f = fopen(nomeArq, "r");
    //chama a funcao para ler o primeiro campo do arquivo e guardar no campo nome fantasia da struct
    digText(&(ptr->nomeFantasia), f, '\n');
    //chama a funcao para ler o proximo campo do arquivo e guardar no campo razao social da struct
    digText(&(ptr->razaoSocial), f, '\n');
    //chama a funcao para ler o proximo campo do arquivo e guardar no campo inscricao estadual da struct
    digText(&(ptr->inscricaoEstadual), f, '\n');
    //chama a funcao para ler o proximo campo do arquivo e guardar no campo cnpj da struct
    digText(&(ptr->cnpj), f, '\n');
    //chama a funcao para ler o proximo campo do arquivo e guardar no campo endereco da struct
    digText(&(ptr->endereco), f, '\n');
    //chama a funcao para ler o proximo campo do arquivo e guardar no campo telefone da struct
    fscanf(f, "%d ", &(ptr->telefone));
    /*chama a funcao para ler o proximo campo do arquivo e guardar no campo email da struct*/
    digText(&(ptr->email), f, '\n');
    /*chama a funcao para ler o proximo campo do arquivo e guardar no campo nome responsavel*/
    digText(&(ptr->nomeResponsavel), f, '\n');
    /*chama a funcao para ler o proximi campo do aruqivo e guardar no campo telefone respp da struct*/
    fscanf(f, "%d ", &(ptr->telefoneResp));
    
    fscanf(f, "%f ", &ptr->multa);
    fechaArquivo(&f);//depois de terminar de fazer a leitura dos campos, fecha o arquuivo
}

/*funcao responsavel por verificar se o cadastro das informacoes da locadora ja foi feito, se nao foi feito, pedir pro usuario cadastrar 
 os dados da locadora*/
void VerificaExisteArquivoLocadora(){
    
    locadora verifica;
    
    FILE *f;
    
    f = fopen("locadora.txt", "r");
    
    if(f){
        fechaArquivo(&f);
        return;
    }
    f = fopen("locadora.bin", "rb");
    
    if(f){
        fechaArquivo(&f);
        return;
    }
    
    f = fopen("locadora.txt", "w");
    
    if(!f){
        printf("Erro ao tentar criar arquivo de cadastro da locadora!\n");
        printf("%s\n", strerror(errno));
        return;
    }
    
    printf(" ______________________________________________________________________\n"
           "|                                                                      |\n"
           "|                        Cadastre sua locadora!                        |\n"
           "|______________________________________________________________________|\n");
    
    
    cadastraLocadora(&verifica);
    
    reescreveDadosLocadora(&verifica, f);
    
    limpaCamposLocadoraMemoria(&verifica);
    
    fechaArquivo(&f);
}
/*funcao responavel por editar os dados da locadora, 
 recebe como argumento o modo em que deve ler e salvar os arquivos*/
void editaDadosLocadora(int modo){
    
    locadora editar;
    /*vetor de funcoes de leitura de dados*/
    void (*leDados[2])(locadora *, char *) = {leDadosLocadora, leDadosLocadoraBin};
    /*vetor de funcoes de escrita de dados*/
    void (*reescreveDados[2])(locadora *, FILE *) = {reescreveDadosLocadora, reescreveDadosLocadoraBin};
    /*vetor de nomes temporarios dos arquivos*/
    char nomeArq[2][16] = {"locadoraRes.txt", "locadoraRes.bin"};
    /*vetor de nomes originais*/
    char nomeArqOriginal[2][13] = {"locadora.txt", "locadora.bin"};
    /*modos de abertura possiveis*/
    char modoAbertura[2][3] = {"w", "wb"};
    /*le os dados do arquivo para a struct editar*/
    (*leDados[modo])(&editar, nomeArqOriginal[modo]);
    int escolha;
    FILE *f;
    f = fopen(nomeArq[modo], modoAbertura[modo]);


        

    escolha = escolheMenu("Escolhe uma opcao para editar",11, 0,"Nome fantasia", "Razao social", "Inscricao estadual", "CNPJ",
                              "Endereco", "Telefone", "E-mail", "Nome responsavel", "Telefone responsavel", "Multa de atraso",
                              "Voltar");


    switch(escolha){
        case 0:
            //caso o campo escolha seja igual a nome fantasia
            printf("Digite o novo nome fantasia da locadora:\n");
            editar.nomeFantasia = limpaMemoria(editar.nomeFantasia);
            //pede a digitacao do novo nome para o campo
            digText(&editar.nomeFantasia, stdin, '\n');
            break;
            
        case 1:
               
            //caso seja igual, entao
            printf("Digite o nome nome para razao social:\n");
            editar.razaoSocial = limpaMemoria(editar.razaoSocial);
            //pede a digitacao do novo nome para o campo
            digText(&editar.razaoSocial, stdin, '\n');
            break;
                
        case 2:
                
            printf("Digite o novo numero da Inscricao Estadual\n");
            /*chama a funcao para o usuario digitar a nova inscricao estadual*/
            validaInsEstadual(&editar.inscricaoEstadual);
            break;

        case 3:
            //caso verdadeiro, entao
            printf("Digite o novo numero do CNPJ:\n");
            //chama a funcao para o usuario digitar o novo valor de cnpj
            validaCnpjouCpf(&editar.cnpj, 14, "543298765432", "6543298765432", 12, 12, "CNPJ digitado invalido!", 0, 0);
            break;
                        
        case 4:
                   
                        
            //caso verdadeiro, entao
            printf("Digite o novo endereco:\n");
            //pede a digitacao do novo endereco para o usuario
            digText(&editar.endereco, stdin, '\n');
            break;
        case 5:
                
            //caso verdadeiro, entao
            printf("Digite o novo telefone:\n");
            //pede a digitacao do novo numero de telefone
            verificaNumero(&editar.telefone, "%d");
            break;
                        
        case 6:
                
            printf("Digite o novo email:\n");
            //pede a digitacao do novo email para o usuario
            verificaText("@.", &editar.email, "Por favor, digite um email valido!\n");
            break;

        case 7:
            //caso verdadeiro, entao
            printf("Digite o novo Nome do Responsavel:\n");
            //pede a digitacao do novo nome do responsavel
            digText(&editar.nomeResponsavel, stdin, '\n');
            break;
                        
        case 8:
                
                                    
            //caso verdadeiro, entao
            printf("Digite o novo telefone do responsavel\n");
            //pede a digitacao do novo numero do telefone
            verificaNumero(&editar.telefoneResp, "%d");
            break;
                        
        case 9:
                
            printf("digite o valor da multa caso algum cliente atrase:\n");
            verificaNumero(&editar.multa, "%f");
            break;
        case 10:
            break;

                                        



    }
    /*reescreve os dados editados para o arquivo*/
    (*reescreveDados[modo])(&editar, f);
    fechaArquivo(&f);//depois de reescrito, fecha o arquivo
    /*limpa os campos de texto da struct da memoria*/
    limpaCamposLocadoraMemoria(&editar);
    /*depois de ter certeza que os dados foram salvos corretamente, apaga o arquivo antigo*/
    remove(nomeArqOriginal[modo]);
    /*e renomeia o novo para o nome do original*/
    rename(nomeArq[modo], nomeArqOriginal[modo]);
    
}
