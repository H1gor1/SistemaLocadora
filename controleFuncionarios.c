#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include "ControleClientes.h"
#include "ManipulacoesDeTexto.h"
#include "ControleDeDadosLocadora.h"
#include "ControleDeNumeros.h"
#include "FuncUtilitarias.h"
#include "controleFuncionarios.h"
#include "menus.h"
#include <ctype.h>
#include <time.h>
#include <windows.h>
#define FuncionarioNaoExiste '*'
#define ERROMEM "ERRO: Memoria indisponivel!\n"

int verificaExistemFuncionarios(Funcionarios *ptr, int quantidade){

    for(int i = 0; i<quantidade; i++){
        if(ptr[i].flag){
            return 1;
        }
    }
    return 0;
}
/*funcao responsavel por apagar os dados da struct funcionarios, recebe um ponteiro para a primeira posicao da struct de cadastro
 de funcionarios na memoria, e a quantidade de funcionarios deveram ser apagados*/
void apagaDadosStructFuncionarios(Funcionarios *ptr, int quantidadeFuncionarios){
    /*enquanto nao limpou todos os campos de texto da memoria, continuar limpando*/
    for(int i = 0; i < quantidadeFuncionarios; i++){
        /*limpa os campos da memoria e retorna null para o ponteiro que apontava para ela*/
        ptr[i].nome = limpaMemoria(ptr[i].nome);
        ptr[i].cargo = limpaMemoria(ptr[i].cargo);
        ptr[i].rua = limpaMemoria(ptr[i].rua);
        ptr[i].bairro = limpaMemoria(ptr[i].bairro);
        ptr[i].email = limpaMemoria(ptr[i].email);
    }
}
/*funcao responsavel por ler os dados do arquivo de cadastros de funcionarios
 recebe um ponteiro para ponteiro de funcionarios*/
int leDadosFuncionarios(Funcionarios **ptr){
    FILE *f;
    /*abre o arquivo de funcionarios em modo de leitura*/
    f = fopen("Funcionarios.txt", "r");
    /*se f for null, entao nao existia arquivo de funcionarios ate o momento*/
    if(!f){
        /*se nao existia, entao cria o arquivo*/
        f = fopen("Funcionarios.txt", "w");
        return 0;//retorna 0 para a quantidade de cliente que existiam
    }
    /*codigo do cliente atual, e necessario guardar em uma var temporaria para evitar jogar memoria fora, mais abaixo sera explicado*/
    long int codigo;
    int i; // i e igual a quantidade de funcionarios
    
    for(i = 0; 1; i++){
        /*le o codigo do cliente atual para dentro de uma variavel temporaria, pois ate o momento nao foi alocada a posicao de memoria para o 
         novo funcionario*/
        fscanf(f, "%ld ", &codigo);
        /*depois de ser feita a leitura, feof testa se o fim do arquivo ja foi marcado pelo ponteiro de stream*/
        if(feof(f)){
            /*se sim, para a operacao de leitura*/
            break;
        }
        /*se nao, continua a leitura*/
        
        /*se i for igual a 0, entao ate o momento nao foi alocado nenhuma posicao de memoria para o vetor de structs, logo e necessario realizar o comando 
         malloc, se nao, entao ja foi alocado, entao sera necessario realizar realloc*/
        ptr[0] = (i == 0)?malloc(sizeof(Funcionarios)):realloc(ptr[0], sizeof(Funcionarios)*(i+1));
        /*verifica se houve memoria o suficiente para realizar a operecao*/
        verificaOperacao(ptr[0], ERROMEM, 1);
        
        /*se nao foi o fim do arquivo, entao sera alocada mais uma posicao, e essa posicao com certeza sera usada, e nao sera disperdicada*/
        /*codigo temporario e alocado em codigo*/
        ptr[0][i].codigo = codigo;
        digText(&ptr[0][i].nome, f);
        digText(&ptr[0][i].cargo, f);
        digText(&ptr[0][i].rua, f);
        digText(&ptr[0][i].bairro, f);
        fscanf(f, "%d ", &ptr[0][i].numero);
        fscanf(f, "%d ", &ptr[0][i].telefone);
        digText(&ptr[0][i].email, f);
        fscanf(f, "%d  ", &ptr[0][i].flag);
        
        
    }
    /*depois de realizar a leitura, fecha o arquivo, e retorna a quantidade*/
    fechaArquivo(&f);
    return i;
}
/*le dados funcionarios de um arquivo bin, recebe um ponteiro para um ponteiro de uma struct de funcionarios*/
int leDadosFuncionariosBin(Funcionarios **ptr){
    
    FILE *f;
    /*abre o arquivo funcionarios em modo de leitura*/
    f = fopen("Funcionarios.bin", "rb");
    //se f recebeu um endereco nulo, entao o arquivo nao existia
    if(!f){
        /*se nao existia, entao ele sera criado*/
        f = fopen("Funcionarios.bin", "wb");
        return 0;//retorna 0 como a quantidade de funcionarios
    }
    /*se f nao for null, entao continua a leitura normalmente*/
    int i; // i e igual a quantidade de funcionarios
    int quantidadeLetras;//quantidade de letras de cada palavra
    long int codigo;//variavel que guarda o codigo do cliente atual, como ja explicado na funcao acima, o motivo de ter uma variavel temporaria
    //pra isso e o mesmo
    
    
    for(i = 0; 1; i++){
        
        
        /*le um tamanho de uma var time_t para o codigo*/
        fread(&codigo, sizeof(long int), 1, f);
        /*feof verifica se ja foi marcado o fim do arquivo*/
        if(feof(f)){
            //se sim, entao para a operacao
            break;
        }
        //se nao, entao aloca mais uma posicao de memoria
        /*se i for 0, entao nao foi feita nenhuma alocacao ate o momento, e sera necesssario executar malloc, se nao, realloc*/
        ptr[0] = (i == 0)?malloc(sizeof(Funcionarios)):realloc(ptr[0], sizeof(Funcionarios)*(i+1));
        /*verifica se houve memoria suficiente para a operacao*/
        verificaOperacao(ptr[0], ERROMEM, 1);
        
        //iguala o codigo temporario ao codigo
        ptr[0][i].codigo = codigo;
        
        /*le a quantidade de letras da proxima palavra*/
        fread(&quantidadeLetras, sizeof(int), 1, f);
        /*aloca o tamanho do nome para dentro do proximo campo de string*/
        ptr[0][i].nome = malloc(sizeof(char)*quantidadeLetras);
        /*verifica se houve memoria o suficiente para a operacao*/
        verificaOperacao(ptr[0][i].nome, ERROMEM, 1);
        /*le a quantidade de bits necessarios para dentro da posicao de memoria apontada por nome*/
        fread(ptr[0][i].nome, sizeof(char), quantidadeLetras, f);
        
        fread(&quantidadeLetras, sizeof(int), 1, f);
        ptr[0][i].cargo = malloc(sizeof(char)*quantidadeLetras);
        verificaOperacao(ptr[0][i].cargo, "ERRO: Memoria indisponivel!\n", 1);
        fread(ptr[0][i].cargo, sizeof(char), quantidadeLetras, f);
        
        fread(&quantidadeLetras, sizeof(int), 1, f);
        ptr[0][i].rua = malloc(sizeof(char)*quantidadeLetras);
        verificaOperacao(ptr[0][i].rua, "ERRO: Memoria indisponivel!\n", 1);
        fread(ptr[0][i].rua, sizeof(char), quantidadeLetras, f);
        
        fread(&quantidadeLetras, sizeof(int), 1, f);
        ptr[0][i].bairro = malloc(sizeof(char)*quantidadeLetras);
        verificaOperacao(ptr[0][i].bairro, "ERRO: Memoria indisponivel!\n", 1);
        fread(ptr[0][i].bairro, sizeof(char), quantidadeLetras, f);
        
        fread(&ptr[0][i].numero, sizeof(int), 1, f);
        
        fread(&ptr[0][i].telefone, sizeof(int), 1, f);
        
        fread(&quantidadeLetras, sizeof(int), 1, f);
        ptr[0][i].email = malloc(sizeof(char)*quantidadeLetras);
        verificaOperacao(ptr[0][i].email, "ERRO: Memoria indisponivel!\n", 1);
        fread(ptr[0][i].email, sizeof(char), quantidadeLetras, f);
        fread(&ptr[0][i].flag, sizeof(int), 1, f);
        
        
    }
    /*depois de terminar a leitura, fecha o arquivo*/
    fechaArquivo(&f);
    /*retorna a quantidade de clientes*/
    return i;
}
/*funcao responsavel por reescreve os dados dos clientes em arquivo binario
 recebe como argumento um ponteiro para uma struct de funcionarios, a quantidade de funcionarios cadastrados
 o modo de abertura que ela deve usar, pois a funcao tambem pode ser usada para salvar mais um cadastro no fim do arquivo
 o nome do arquivo temporario e o nome do arquivo original, se necessario fazer essa operacao de seguranca*/
void reescreveDadosFuncionariosBin(Funcionarios *ptr, int quantidadeFuncionarios, char *modo, char *nomeArqTemporario, char *nomeArqOr){
    FILE *f;
    /*abre o arquivo no modo passado como parametro*/
    f = fopen(nomeArqTemporario, modo);
    int quantidadeLetras;//var que guarda a quantidade de letras de cada palavra
    
    //se f for null, entao houve um erro na abertura do arquivo
    if(!f){
        //se houve um erro, entao avisa o usuario sobre o erro
        printf("Erro ao salvar dados! Novos dados foram perdidos!\n");
        printf("%s", strerror(errno));
        return;//cancela a operacao
    }
    /*se f nao for null, entao continua a operacao normalmente*/
    
    //enquanto nao reescreveu toda a quantidade precisa, entao continuar escrevendo
    for(int i = 0; i<quantidadeFuncionarios; i++){
        //escreve o codig ono arquivo
        fwrite(&ptr[i].codigo, sizeof(long int), 1, f);
        
        //faz a leitura de quantos caracteres o campo de texto atual tem e acrescenta 1
        quantidadeLetras = strlen(ptr[i].nome)+1;
        //escreve a quantidade de letras no arquivo
        fwrite(&quantidadeLetras, sizeof(int), 1, f);
        //escreve no arquivo um tamanho de um char vezes a quantidade de letras da palavra pegando bit por bit da posicao de memoria
        //apontada por nome
        fwrite(ptr[i].nome,  sizeof(char), quantidadeLetras, f);
        
        quantidadeLetras = strlen(ptr[i].cargo)+1;
        fwrite(&quantidadeLetras, sizeof(int), 1, f);
        fwrite(ptr[i].cargo,  sizeof(char), quantidadeLetras, f);
        
        quantidadeLetras = strlen(ptr[i].rua)+1;
        fwrite(&quantidadeLetras, sizeof(int), 1, f);
        fwrite(ptr[i].rua,  sizeof(char), quantidadeLetras, f);
        
        quantidadeLetras = strlen(ptr[i].bairro)+1;
        fwrite(&quantidadeLetras, sizeof(int), 1, f);
        fwrite(ptr[i].bairro,  sizeof(char), quantidadeLetras, f);
        
        fwrite(&ptr[i].numero, sizeof(int), 1, f);
        
        fwrite(&ptr[i].telefone, sizeof(int), 1, f);
        
        quantidadeLetras = strlen(ptr[i].email)+1;
        fwrite(&quantidadeLetras, sizeof(int), 1, f);
        fwrite(ptr[i].email,  sizeof(char), quantidadeLetras, f);
        
        fwrite(&ptr[i].flag, sizeof(int), 1, f);
    }
    /*depois de terminar de reescrever o arquivo, fecha o arquivo*/
    fechaArquivo(&f);
    /*se o nome passado como nome original, e nome temporario forem diferentes, entao foi pedido pra reescrever todo o arquivo, e quando
     e pedido pra reescrever todo o arquivo e necessario tomar esse cuidado para nao perder todos os dados
     se forem iguais, entao a funcao fez o trabalho de adicionar no fim do arquivo o novo campo, e nao e necessario trocar os nomes*/
    if(strcmp(nomeArqOr, nomeArqTemporario)){    
        remove(nomeArqOr);
        rename(nomeArqTemporario, nomeArqOr);
    }
}
/*funcao responsavel por reescrever os dados dos funcionarios em um arquivo text
 recebe como argumento um ponteiro para a primeira posicao da struct que deve escreve no arquivo
 a quantidade de funcionarios cadastrados, uma string do modo de abertura especificando se deve reescrever todo o arquivo ou somente adicionar 
 um novo dado no fim do arquivo, e o nome temporario e original*/
void reescreveDadosFuncionarios(Funcionarios *ptr, int quantidadeFuncionarios, char *modo, char *nomeArquivoTemporario, char *nomeArquivoOr){
    
    FILE *f;
    /*abre o arquivo no modo pedido*/
    f = fopen(nomeArquivoTemporario, modo);
    /*se f for null, entao houve um problema ao tentar salvar os dados*/
    if(!f){
        /*entao mostra a mensagem na tela para o usuario*/
        printf("Erro ao salvar dados! Novos dados foram perdidos!\n");
        printf("%s", strerror(errno));
        return;
    }
    /*enquanto nao reescrever todos os campos pedidos na chamada da funcao, continuar escrevendo*/
    for(int i = 0; i<quantidadeFuncionarios; i++){
        
        fprintf(f, "%ld\n", ptr[i].codigo);
        
        fprintf(f, "%s\n", ptr[i].nome);
        
        fprintf(f, "%s\n", ptr[i].cargo);
        
        fprintf(f, "%s\n", ptr[i].rua);
    
        fprintf(f, "%s\n", ptr[i].bairro);
        
        fprintf(f, "%d\n", ptr[i].numero);
        
        fprintf(f, "%d\n", ptr[i].telefone);
        
        fprintf(f, "%s\n", ptr[i].email);
        
        fprintf(f, "%d\n\n", ptr[i].flag);
        
        
    }
    /*fecha o arquivo f*/
    fechaArquivo(&f);
    //mesma explicacao da funcao de reescrita em binario
    if(strcmp(nomeArquivoOr, nomeArquivoTemporario)){
        remove(nomeArquivoOr);
        rename(nomeArquivoTemporario, nomeArquivoOr);
    }
}
/*funcao responsavel por cadastrar um novo funcionario, recebe como argumento o modo de armazenamento que deve usar*/
void cadastraFuncionario(int modoAbertura){
    //struct que guardara os dados do novo funcionario, nao precisa ser um ponteiro que guarda todos os dados iguais nas funcoes de cliente,
    //pois desta vez nao e necessario fazer verificacoes de repeticoes de nomes ou cpfs, logo podera somente escrever no final do arquivo as 
    //novas informacoes
    Funcionarios novo;
    /*vetor de funcoes de reescrita de dados*/
    void (*reescrita[2])(Funcionarios *, int, char*, char *, char *)={
        reescreveDadosFuncionarios, reescreveDadosFuncionariosBin
    };
    /*modos de escrita que poderao ser usados*/
    char modoEscrita[2][3] = {
        "a", "ab"
    };
    char nomeArqOr[2][17] = {
        "Funcionarios.txt", "Funcionarios.bin"
    };
   
    
    novo.codigo = time(NULL);
    
    printf("digite o nome do funcionario:\n");
    digText(&novo.nome, stdin);
    
    printf("digite o cargo do funcionario:\n");
    digText(&novo.cargo, stdin);
    
    printf("digite a rua do funcionario:\n");
    digText(&novo.rua, stdin);
    
    printf("digite o bairro do funcionario:\n");
    digText(&novo.bairro, stdin);
    
    printf("digite o numero da casa do funcionario:\n");
    verificaNumero(&novo.numero, "%d");
    
    printf("digite o numero do telefone do funcionario:\n");
    verificaNumero(&novo.telefone, "%d");
    
    printf("digite o email do funcionario:\n");
    verificaText("@.", &novo.email, "email invalido, por favor, digite um email valido!\n");
    
    novo.flag = 1;
    
    (*reescrita[modoAbertura])(&novo, 1, modoEscrita[modoAbertura], nomeArqOr[modoAbertura], nomeArqOr[modoAbertura]);
    apagaDadosStructFuncionarios(&novo, 1);


    
   
    
}

Funcionarios *encontraFuncionarioNome(Funcionarios *ptr, char *nome, int quantidade, Funcionarios *posicao){
    
    for(int i = 0; i<quantidade; i++){
        if((ptr+i)!=posicao && ptr[i].flag){
            if(!strcmp(ptr[i].nome, nome)){
                return (ptr+i);
            }
        }
    }
    return NULL;
}
Funcionarios *encontraFuncionarioCodigo(Funcionarios *ptr, long int codigo, int quantidade, Funcionarios *posicao){
    
    for(int i = 0; i<quantidade; i++){
        if((ptr+i)!=posicao && ptr[i].flag){
            if(ptr[i].codigo == codigo){
                return (ptr+i);
            }
        }
    }
    return NULL;
}
Funcionarios *buscaFuncionarios(Funcionarios *ptr, int quantidade, Funcionarios *posicao){
    Funcionarios *busca;
    char *itemBusca;
    do{
        digText(&itemBusca, stdin);
        busca = encontraFuncionarioNome(ptr, itemBusca, quantidade, posicao);
        if(busca){
            itemBusca = limpaMemoria(itemBusca);
            return busca;
        }
        long int codigo = atoi(itemBusca);
        busca = encontraFuncionarioCodigo(ptr, codigo, quantidade, posicao);
        if(busca){
            itemBusca = limpaMemoria(itemBusca);
            return busca;
        }
        if(!strcmp(itemBusca, "open list")){
            itemBusca = limpaMemoria(itemBusca);
            return menuOpcoesFilmesOuClientes(ptr, quantidade, "Lista completa de todos os funcionarios", 0, sizeof(Funcionarios));
        }


        printf("nenhum funcionario com esse dado foi encontrado, por favor, verifique se digitou certo!\n");
        itemBusca = limpaMemoria(itemBusca);
    }while(1);
    
   
    
}
int verificaExisteFuncionarios(Funcionarios *ptr, int quantidade){
    if(!quantidade){
        return 0;
    }
    for(int i = 0; i<quantidade; i++){
        if(ptr[i].flag){
            return 1;
        }
    }
    return 0;
}
void editaFuncionario(int modoAbertura){
    Funcionarios *ptr = NULL;
    Funcionarios *editar = NULL;
    int (*leitura[2])(Funcionarios **) = {
        leDadosFuncionarios, leDadosFuncionariosBin
    };
    void (*reescrita[2])(Funcionarios *, int, char*, char *, char *)={
        reescreveDadosFuncionarios, reescreveDadosFuncionariosBin
    };
    char nomeArquivoTemp[2][20] = {
        "FuncionariosRes.txt", "FuncionariosRes.bin"
    };
    char nomeArquivoOr[2][17] = {
        "Funcionarios.txt", "Funcionarios.bin"
    };
    char modoEscrita[2][3] = {
        "w", "wb"
    };
    int quantidadeFuncionarios;
    int  dado;
    
    quantidadeFuncionarios = (*leitura[modoAbertura])(&ptr);
    if(!verificaExisteFuncionarios(ptr, quantidadeFuncionarios)){
        printf("nao existe nenhum funcionario no banco de dados!\n");
        Sleep(2000);
        return;
    }
    printf("digite o codigo ou nome do funcionario que deseja editar\n");
    mostraFuncionarios(modoAbertura);
    editar = buscaFuncionarios(ptr, quantidadeFuncionarios, (ptr-1));
    
    


        
    dado = escolheMenu("Qual informacao deseja editar:", 8,0,"Nome Completo", "Cargo", "Rua", "Bairro", "Numero da casa", "Telefone", "E-mail", "Voltar");
        
    switch(dado){
            
        case 0:
            editar->nome = limpaMemoria(editar->nome);
            printf("digite o nome do funcionario:\n");
            digText(&editar->nome, stdin);
            break;

        case 1:
         
            editar->cargo = limpaMemoria(editar->cargo);
            printf("digite o cargo do funcionario:\n");
            digText(&editar->cargo, stdin);
            break;
                
        case 2:
                
            editar->rua = limpaMemoria(editar->rua);
            printf("digite a rua do funcionario:\n");
            digText(&editar->rua, stdin);
            break;
                
        case 3:
                
            editar->bairro = limpaMemoria(editar->bairro);
            printf("digite o bairro do funcionario:\n");
            digText(&editar->bairro, stdin);
            break;

        case 4:
                
            printf("digite o numero da casa do funcionario:\n");
            verificaNumero(&editar->numero, "%d");
            break;
            
        case 5:
            printf("digite o telefone do funcionario:\n");
            verificaNumero(&editar->telefone, "%d");
            break;
                       
        case 6:
                                                   
            editar->email = limpaMemoria(editar->email);
            printf("digite o email do funcionario:\n");
            verificaText("@.", &editar->email, "email invalido, por favor, digite um email valido!\n");
            break;

        case 7:
            break;

                                


    
    
    }
    (*reescrita[modoAbertura])(ptr, quantidadeFuncionarios, modoEscrita[modoAbertura], nomeArquivoTemp[modoAbertura], nomeArquivoOr[modoAbertura]);
    apagaDadosStructFuncionarios(ptr, quantidadeFuncionarios);
    ptr = limpaMemoria(ptr);
    editar = NULL;

    
    
    
}
void apagaFuncionario(int modoAbertura){
    int (*leitura[2])(Funcionarios **) = {
        leDadosFuncionarios, leDadosFuncionariosBin
    };
    void (*reescrita[2])(Funcionarios *, int, char*, char *, char *)={
        reescreveDadosFuncionarios, reescreveDadosFuncionariosBin
    };
    char modoEscrita[2][3] = {
        "w", "wb"
    };
    char nomeArquivoTemp[2][20] = {
        "FuncionariosRes.txt", "FuncionariosRes.bin"
    };
    char nomeArquivoOr[2][17] = {
        "Funcionarios.txt", "Funcionarios.bin"
    };
    Funcionarios *ptr = NULL;
    Funcionarios *apagar = NULL;
    int quantidadeFuncionarios;
 
    quantidadeFuncionarios = (*leitura[modoAbertura])(&ptr);
    if(!verificaExisteFuncionarios(ptr, quantidadeFuncionarios)){
        printf("nao existe nenhum funcionario cadastro no sistema ainda!\n");
        Sleep(2000);
        return;
    }
    printf("digite o nome ou codigo do funcionario que deseja apagar:\n");
    mostraFuncionarios(modoAbertura);
    apagar = buscaFuncionarios(ptr, quantidadeFuncionarios, (ptr-1));
    
    apagar->flag = 0;
    
    (*reescrita[modoAbertura])(ptr, quantidadeFuncionarios, modoEscrita[modoAbertura], nomeArquivoTemp[modoAbertura], nomeArquivoOr[modoAbertura]);
    apagaDadosStructFuncionarios(ptr, quantidadeFuncionarios);
    ptr = limpaMemoria(ptr);
    apagar = NULL;


    
    
}
void mostraFuncionarios(int modoAbertura){
    int (*leitura[2])(Funcionarios **) = {
        leDadosFuncionarios, leDadosFuncionariosBin
    };
    Funcionarios *ptr;
    int quantidadeFuncionarios;
    
    quantidadeFuncionarios = (*leitura[modoAbertura])(&ptr);
    
    for(int i = 0; i<quantidadeFuncionarios; i++){
        if(ptr[i].flag){
            printf("__________________________\n");
            printf("| codigo: %ld\n", ptr[i].codigo);
            printf("| nome: %s\n_______________________\n", ptr[i].nome);
        }
    }
    apagaDadosStructFuncionarios(ptr, quantidadeFuncionarios);
    ptr = limpaMemoria(ptr);
    
        
}