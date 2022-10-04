#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include <stdarg.h>
#include "ManipulacoesDeTexto.h"
#include "ControleDeDadosLocadora.h"
#include "ControleDeNumeros.h"
#include "ControleClientes.h"
#include "ControleFilmes.h"
#include "controleFuncionarios.h"
#include "ControleCategorias.h"
#include "fornecedores.h"
#define VAZIO NULL

void removeArquivos(int quantidadeRemov, ...){
    
    char *remover;
    
    va_list parametros;
    va_start(parametros, quantidadeRemov);
    
    for(int i = 0; i<quantidadeRemov; i++){
        remover = va_arg(parametros, char *);
        
        remove(remover);
    }
    va_end(parametros);
}


/*funcao responsavel por fechar um arquivo evitando ponteiros soltos, desaloca o buffer apontado
 pelo ponteiro do arquivo da memoria e tambem atribui NULL ao ponteiro*/
void fechaArquivo(FILE **f){
    /*desrefencia um nivel de ponteiro para passar para fclose o valor apotando pelo ponteiro apontado por f*/
    fclose(f[0]);
    /*desrefencia um nivel de ponteiro para que o ponteiro da funcao chamadora receba null*/
    f[0] = VAZIO;
}
int verificaNull(void *ptr){
    //se endereco for igual a 0
    if(!ptr){
        return 0;//return falso
    }
    return 1;//se igual a 1, return verdadeiro
}
/*funcao responsavel por verificar uma operacao de alocacao de memoria ou abertura de arquivo, seu processo sera definido
 pelo 2 e 3 argumento, o 2 e a mensagem pra caso o processo tenha falhado, e o 3 fala se a funcao deve encerrar o programa ou nao*/
int verificaOperacao(void *ptr, char *mensagem, int sairOuNao){
    //verifica se o o ptr e null, caso for
    if(!verificaNull(ptr)){
        printf("%s", mensagem);//exibe a mensagem de erro passada como argumento
        printf("%s\n", strerror(errno));//chamada da funcao para mostrar para o usuario o motivo especifico do erro
        if(sairOuNao){//se o 3 argumento foi pedido pra fechar o programa
            exit(1);//fecha o programa
        }
        //caso nao foi pedido pra fechar o programa, a funcao retorna 1
        return 1;
    }
    //retorna 0 significando que o ponteiro nao foi nulo, e qualquer acao que tenha sido feita em cima desse 
    //ponteiro antes foi bem sucedida
    return 0;
}
/*funcao responsavel por consultar o arquivo de configuracoes de abertura do sistema e pegar o valor refente a
 qual modo de armazenamento esta sendo usado, se 0, texto, se 1, binario*/

void verificaModoArmazenamento(int *var){
    
    FILE *f;
    /*tenta abrir o arquivo de cadastro de locadora para verificar o modo em que os arquivos estao sendo salvos atualmente*/
    f = fopen("locadora.txt", "r");
    /*se f recebeu um endereco null, entao o modo de armazenamento nao esta em txt, se nao esta em txt*/
    if(!f){
        /*entao esta em binario, logo o modo de armazenamento recebe 1*/
        *var = 1;
    }else{
        /*se falso que f recebeu um endereco null, entao os dados estao sendo armazenados em texto*/
        *var = 0;//se estao em texto, entao a variavel recebe 0
        fechaArquivo(&f);//fecha o arquivo f
    }
    
    
}
/*funcao responsavel por limpar a memoria e evitar ponteiros soltos
 recebe um ponteiro do tipo void, podendo assim ser usada para desalocar qualquer tipo de memoria "int, char, struct"*/
void *limpaMemoria(void *ptr){
    /*repassa o ponteiro generico recebido como argumento para fclose*/
     (ptr);
    return NULL;//retorna o endereco nulo
    /*na chamada desta funcao e obrigatorio igualar o ponteiro desalocado ao retorno dela, se nao o ponteiro
     continuara sem receber null, assim apontando pra lixo de memoria*/
}

/*funcao responsavel por trocar o modo de armazenamento do sistema, de texto pra binario ou binario pra texto*/
void trocaModoArmazenamento(int *modo){
    /*para fazer a trocar ela pucha todos os dados do sistema pra memoria, apagar os arquivos antigos, e cria
     os novos, logo apos guarda os dados nos novos arquivos*/
    
    locadora loc;//struct que recebera os dados da locadora
    cliente *clientes = NULL;//ponteiro para struct que guardara os dados dos clientes
    Funcionarios *funcionario = NULL;//ponteiro para a struct que guardara os dados dos funcionarios
    categoria *categorias = NULL;
    filmes *Filmes = NULL;
    fornecedor *fornecedores = NULL;
    
    int quantidadeFornecedores;
    int quantidadeClientes;//var que guardara as quantidades dos clientes totais
    int quantidadeFuncionarios;//variavel que guardara a quantidade de funcionarios
    int quantidadeCategorias;
    int quantidadeFilmes;
    FILE *f;//ponteiro para o arquivo de onde os dados serao lidos ou guardados
    
    /*se modo for 1, entao trocar de binario pra texto*/
    if(*modo){
        /*chamada das funcoes para ler os dados do sistema se estiver em modo binario*/
        leDadosLocadoraBin(&loc,"locadora.bin");
        quantidadeClientes = leDadosClientesBin(&clientes);
        quantidadeFuncionarios = leDadosFuncionariosBin(&funcionario);
        quantidadeCategorias = leDadosCategoriaBin(&categorias);
        quantidadeFilmes = leDadosFilmesBin(&Filmes);
        quantidadeFornecedores = leDadosFornecedoresBin(&fornecedores);
        /*remocao dos arquivos antigos*/
        
        
        
        /*cria o arquivo locadora txt em modo escrita*/
        f = fopen("locadora.txt", "w");
        /*escreve os dados no novo arquivo*/
        reescreveDadosLocadora(&loc, f);
        fechaArquivo(&f);//fecha o arquivo
        //escreve os dados de clientes em um novo arquivo de texto
        reescreveDadosCliente(clientes, quantidadeClientes);
        //escreve os dados dos funcionarios em um novo arquivo de texto
        reescreveDadosFuncionarios(funcionario, quantidadeFuncionarios, "w", "Funcionarios.txt", "Funcionarios.txt");
        /*alterna o modo pra 0*/
        reescreveDadosCategoria(categorias, quantidadeCategorias); 
        reescreveDadosFilme(Filmes, quantidadeFilmes);
        reescreveDadosFornecedores(fornecedores, quantidadeFornecedores);
        
        //correcao feita apos a apresentacao, problema apontado pelo manoel
        removeArquivos(6, "locadora.bin", "clientes.bin", "Funcionarios.bin", "categorias.bin", "filmes.bin", "fornecedores.bin");
        *modo = 0;
        
        
    }else{
        /*se falso, entao trocar de texto pra binario*/
        
        /**le os dados dos arquivos de texto*/
        leDadosLocadora(&loc, "locadora.txt");
        quantidadeClientes = leDadosClientes(&clientes);
        quantidadeFuncionarios = leDadosFuncionarios(&funcionario);
        quantidadeCategorias = leDadosCategoria(&categorias);
        quantidadeFilmes = leDadosFilmes(&Filmes);
        quantidadeFornecedores = leDadosFornecedores(&fornecedores);
        /*apaga os arquivos de texto*/
       
        
        /*cria um arquivo bin para locadora*/
        f = fopen("locadora.bin", "wb");
        /*reescreve os dados no novo arquivo*/
        reescreveDadosLocadoraBin(&loc, f);
        fechaArquivo(&f);//fecha o arquivo
        
        /*reescreve os dados dos clientes no arquivo binario novo*/
        reescreveDadosClienteBin(clientes, quantidadeClientes);
        
        /*reescreve os dados dos funcionarios em um novo arquivo binario*/
        reescreveDadosFuncionariosBin(funcionario, quantidadeFuncionarios, "wb", "Funcionarios.bin", "Funcionarios.bin");
        
        reescreveDadosCategoriaBin(categorias, quantidadeCategorias);
        reescreveDadosFilmeBin(Filmes, quantidadeFilmes);
        reescreveDadosFornecedoresBin(fornecedores, quantidadeFornecedores);
        
        
        /*correcao feita em sala de aula de um problema apontado pelo manoel*/
        removeArquivos(6, "locadora.txt", "clientes.txt", "Funcionarios.txt", "categorias.txt", "filmes.txt", "fornecedores.txt");
        *modo = 1;//muda o modo de abertura pra 1, que significa binario
        
    }
    
    limpaCamposLocadoraMemoria(&loc);//limpa os campos de text da locadora da memoria
    
    //limpa os campos de text de cada cliente da memoria
    limpaDadosClienteMemoria(clientes, quantidadeClientes);
    /*se cliente tem um valor diferente de null, entao foi feita uma alocacao de memoria para de structs clientes, logo e necessario limpar a memoria*/
    if(clientes){
        //entao limpa as poscioes de memoria que o ponteiro de cliente aponta
        clientes = limpaMemoria(clientes);
    }
    /*se clientes tem um valor null, entao nao sera necessario limpa-lo*/
    /*apaga os campos de text dos funcionarios da memoria*/
    apagaDadosStructFuncionarios(funcionario, quantidadeFuncionarios);
    /*se funcionario tem um valor diferente de null, entao sera necessario limpar a memoria da struct de funcionarios*/
    if(funcionario){
        /*limpa a memoria da struct de funcionarios*/
        funcionario = limpaMemoria(funcionario);
    }
    
    limpaDadosFilmeMemoria(Filmes, quantidadeFilmes);
    
    if(Filmes){
        Filmes = limpaMemoria(Filmes);
    }
    
    limpaDadosCategoriaMemoria(categorias, quantidadeCategorias);
    
    if(categorias){
        categorias = limpaMemoria(categorias);
    }
    
    
    
}

