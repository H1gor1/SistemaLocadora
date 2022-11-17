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
#include "Caixa.h"
#include "ContasAreceber.h"
#include "Devolucoes.h"
#include <conio.h>
#include "menus.h"
#include "EntradaDeFilmes.h"
#include "ContasAPagar.h"
#define ESC 27
#define VAZIO NULL


int contaQuantidadeExistente(size_t tamanho, void *ptr, int quantidade){
    int quantidadeExistente = 0;
    switch(tamanho){

        case sizeof(cliente):
            for(int i = 0; i<quantidade; i++){
                if(((cliente *)ptr)[i].flag){
                    quantidadeExistente++;
                }
            }
            return quantidadeExistente;

        case sizeof(categoria):
            for(int i = 0; i<quantidade; i++){
                if(((categoria *)ptr)[i].flag){
                    quantidadeExistente++;
                }

            }
            return quantidadeExistente;
        case sizeof(Funcionarios):
                for(int i = 0; i<quantidade; i++){
                    if(((Funcionarios *)ptr)[i].flag){
                        quantidadeExistente++;
                    }

                }
                return quantidadeExistente;

        case sizeof(filmes):
            for(int i = 0; i<quantidade; i++){
                if(((filmes *)ptr)[i].flag){
                    quantidadeExistente++;
                }
            }
            return quantidadeExistente;



    }
}
int escolheOpcao(){
    int opcao = getch();
    if(opcao == 0 || opcao == 224){
        opcao = getch();
    }

    return opcao;
}

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
    free(ptr);
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
    lancamentoCaixa *lancamentos = NULL;
    contaArec *lancAprazo = NULL;
    compras *devolucoes = NULL;
    contaApag *contasApagar = NULL;
    lancamentoEntradas *lancamentosDeEntradas = NULL;

    
    int quantidadeFornecedores;
    quantidades quant;
    int quantidadeCategorias;
    
    int quantidadeLancamentosCaixa;
    int quantidadeLancamentosAprazo;
    int quantidadeLancamentosEntradas;
    int quantidadeContaApag;
    FILE *f;//ponteiro para o arquivo de onde os dados serao lidos ou guardados
    
    /*se modo for 1, entao trocar de binario pra texto*/
    if(*modo){
        /*chamada das funcoes para ler os dados do sistema se estiver em modo binario*/
        leDadosLocadoraBin(&loc,"locadora.bin");
       
        quantidadeCategorias = leDadosCategoriaBin(&categorias);
        quant = leDadosDevolucoesBin(&devolucoes, &funcionario, &clientes, &Filmes);
        quantidadeFornecedores = leDadosFornecedoresBin(&fornecedores);
        quantidadeLancamentosCaixa = leDadosLancamentosBin(&lancamentos);
        quantidadeLancamentosAprazo = leDadosLancamentosAprazoBin(&lancAprazo);
        quantidadeContaApag = leDadosEntradasAprazoBin(&contasApagar);
        quantidadeLancamentosEntradas = leDadosEntradasFilmesBin(&lancamentosDeEntradas);
        
        
        
        
        /*cria o arquivo locadora txt em modo escrita*/
        f = fopen("locadora.txt", "w");
        /*escreve os dados no novo arquivo*/
        reescreveDadosLocadora(&loc, f);
        fechaArquivo(&f);//fecha o arquivo
        //escreve os dados de clientes em um novo arquivo de texto
        reescreveDadosCliente(clientes, quant.quantidadeClientes);
        //escreve os dados dos funcionarios em um novo arquivo de texto
        reescreveDadosFuncionarios(funcionario, quant.quantidadesFuncionarios, "w", "Funcionarios.txt", "Funcionarios.txt");
        /*alterna o modo pra 0*/
        reescreveDadosCategoria(categorias, quantidadeCategorias); 
        reescreveDadosFilme(Filmes, quant.quantidadesFilmes);
        reescreveDadosFornecedores(fornecedores, quantidadeFornecedores);
        reescreveLancamentosCaixa(lancamentos, quantidadeLancamentosCaixa, "lancamentos.txt", "lancamentos.txt", "w");
        reescreveLancamentosAprazo(lancAprazo, quantidadeLancamentosAprazo, "lancamentosAprazo.txt", "lancamentosAprazo.txt", "w");
        reescreveDadosDevolucoes(devolucoes, quant.quantidadeAlugacoes, "devolucoes.txt", "devolucoes.txt", "w");
        reescreveEntradaAprazo(contasApagar, quantidadeContaApag, "entradaAprazo.txt", "entradaAprazo.txt", "w");
        reescreveLancamentosEntrada(lancamentosDeEntradas, quantidadeLancamentosEntradas, "lancamentosEntrada.txt", "lancamentosEntrada.txt", "w");
        //correcao feita apos a apresentacao, problema apontado pelo manoel
        removeArquivos(11, "lancamentosEntrada.bin","entradaAprazo.bin","locadora.bin", "clientes.bin", "Funcionarios.bin", "categorias.bin", "filmes.bin", "fornecedores.bin", "lancamentos.bin", "lancamentosAprazo.bin", "devolucoes.bin");
        *modo = 0;
        
        
    }else{
        /*se falso, entao trocar de texto pra binario*/
        
        /**le os dados dos arquivos de texto*/
        leDadosLocadora(&loc, "locadora.txt");
       
        quantidadeCategorias = leDadosCategoria(&categorias);
        quant = leDadosDevolucoes(&devolucoes, &funcionario, &clientes, &Filmes);
        quantidadeFornecedores = leDadosFornecedores(&fornecedores);
        quantidadeLancamentosCaixa = leDadosLancamentos(&lancamentos);
        quantidadeLancamentosAprazo = leDadosLancamentosAprazo(&lancAprazo);
        quantidadeContaApag = leDadosEntradasAprazo(&contasApagar);
        quantidadeLancamentosEntradas = leDadosLancamentoEntradasFilmes(&lancamentosDeEntradas);
        /*apaga os arquivos de texto*/
       
        
        /*cria um arquivo bin para locadora*/
        f = fopen("locadora.bin", "wb");
        /*reescreve os dados no novo arquivo*/
        reescreveDadosLocadoraBin(&loc, f);
        fechaArquivo(&f);//fecha o arquivo
        
        /*reescreve os dados dos clientes no arquivo binario novo*/
        reescreveDadosClienteBin(clientes, quant.quantidadeClientes);
        
        /*reescreve os dados dos funcionarios em um novo arquivo binario*/
        reescreveDadosFuncionariosBin(funcionario, quant.quantidadesFuncionarios, "wb", "Funcionarios.bin", "Funcionarios.bin");
        
        reescreveDadosCategoriaBin(categorias, quantidadeCategorias);
        reescreveDadosFilmeBin(Filmes, quant.quantidadesFilmes);
        reescreveDadosFornecedoresBin(fornecedores, quantidadeFornecedores);
        reescreveLancamentosCaixaBin(lancamentos, quantidadeLancamentosCaixa, "lancamentos.bin", "lancamentos.bin", "wb");
        reescreveLancamentosAprazoBin(lancAprazo, quantidadeLancamentosAprazo, "lancamentosAprazo.bin", "lancamentosAprazo.bin", "wb");
        reescreveDadosDevolucoesBin(devolucoes, quant.quantidadeAlugacoes, "devolucoes.bin", "devolucoes.bin", "wb");
        reescreveEntradasAprazoBin(contasApagar, quantidadeContaApag, "entradaAprazo.bin", "entradaAprazo.bin", "wb");
        reescreveLancamentosEntradaBin(lancamentosDeEntradas, quantidadeLancamentosEntradas, "lancamentosEntrada.bin", "lancamentosEntrada.bin", "wb");
        removeArquivos(11, "lancamentosEntrada.txt","entradaAprazo.txt","locadora.txt", "clientes.txt", "Funcionarios.txt", "categorias.txt", "filmes.txt", "fornecedores.txt", "lancamentos.txt", "lancamentosAprazo.txt", "devolucoes.txt");
        *modo = 1;//muda o modo de abertura pra 1, que significa binario
        
    }
    
    limpaCamposLocadoraMemoria(&loc);//limpa os campos de text da locadora da memoria
    
    //limpa os campos de text de cada cliente da memoria
    limpaDadosClienteMemoria(clientes, quant.quantidadeClientes);
    /*se cliente tem um valor diferente de null, entao foi feita uma alocacao de memoria para de structs clientes, logo e necessario limpar a memoria*/
    if(clientes){
        //entao limpa as poscioes de memoria que o ponteiro de cliente aponta
        clientes = limpaMemoria(clientes);
    }
    /*se clientes tem um valor null, entao nao sera necessario limpa-lo*/
    /*apaga os campos de text dos funcionarios da memoria*/
    apagaDadosStructFuncionarios(funcionario, quant.quantidadesFuncionarios);
    /*se funcionario tem um valor diferente de null, entao sera necessario limpar a memoria da struct de funcionarios*/
    if(funcionario){
        /*limpa a memoria da struct de funcionarios*/
        funcionario = limpaMemoria(funcionario);
    }
    
    limpaDadosFilmeMemoria(Filmes, quant.quantidadesFilmes);
    
    for(int i = 0; i<quant.quantidadeAlugacoes; i++){
        atribuiNull(devolucoes[i].filmesComprados, devolucoes[i].quantidadeFilmesComprados, sizeof(filmes));
        devolucoes[i].filmesComprados = limpaMemoria(devolucoes[i].filmesComprados);
    }
    atribuiNull(devolucoes, quant.quantidadeAlugacoes, sizeof(compras));
    devolucoes = limpaMemoria(devolucoes);
    
    if(Filmes){
        Filmes = limpaMemoria(Filmes);
    }
    
    limpaDadosCategoriaMemoria(categorias, quantidadeCategorias);
    
    if(categorias){
        categorias = limpaMemoria(categorias);
    }
    if(contasApagar){
        contasApagar = limpaMemoria(contasApagar);
    }
    if(lancamentosDeEntradas){
        lancamentosDeEntradas = limpaMemoria(lancamentosDeEntradas);
    }
    
    
    
}
int retornaNumeroConformeF(int quantidadeOpcoes, int ignorarEsc){
    int F;
    int escolha;

    while(1) {
        F = 59;
        escolha = escolheOpcao();
        if(escolha == 27 && ignorarEsc){
            return escolha;
        }
        for (int i = 1; i <= quantidadeOpcoes; i++) {
            if(escolha == F){
                return i;
            }
            F++;
            if(F == 69){
                F = 133;
            }
        }
        printf("voce escolheu uma opcao invalida! Por favor digite novamente!\n");
    }
}

void listaPeloCodigo(void *ptr, int quantidade, void (*funcaoDeMostrar)(void *, int), char *mensagem, void *(*funcaoDeFiltrar)(void *, int, int, int, int*), void *(funcaoDeBuscar)(void *, int, void *)){
    int codigo, codigo1;
    int escolha;
    void *endOlhar = NULL;
    void *novaListaFiltrada = NULL;
    printf("digite os numeros da faixa de codigo que deseja filtrar:\n");
    printf("digite o primeiro numero\n");
    verificaNumero(&codigo, "%d");
    printf("digite o segundo numero\n");
    verificaNumero(&codigo1, "%d");
    int quantidadeFiltrada = 0;
    novaListaFiltrada =  funcaoDeFiltrar(ptr, quantidade, (codigo > codigo1)?codigo1:codigo, (codigo > codigo1)?codigo:codigo1, &quantidadeFiltrada);

    printf("Deseja visualizar %s nesta faixa de codigos?\nF1-sim\nF2-nao\n", mensagem);
    while(1){
        escolha = escolheOpcao();
        switch (escolha) {
            case 59:
                break;
            case 60:
                return;
            default:
            printf("digite uma opcao valida!\n");
            continue;
        }
        break;
    }
    printf("digite o codigo ou nome de algum dos dados filtrados:\n");
    endOlhar = funcaoDeBuscar(novaListaFiltrada, quantidadeFiltrada, "Nenhum dos dados filtrados tem esta informacao no cadastro!");

    funcaoDeMostrar(endOlhar, 1);




}

void* menuOpcoesFilmesOuClientes(void *todos, int quantidade, char *mensagem, int ignoraApagados, size_t tamanho){
    system("clear");
    int quantidadeExistente = (!ignoraApagados)?contaQuantidadeExistente(tamanho, todos, quantidade):quantidade;
    int indice = 0;
    int contador = 0;
    int escolha = 0;
    char **informacoesEmText = NULL;
    char *nomeConcat = NULL;
    cliente *ptrClient = NULL;
    filmes *ptrMovies = NULL;
    Funcionarios  *ptrFuncs = NULL;

    void *itemProcurado =NULL;


    informacoesEmText = malloc(sizeof(char *)*quantidadeExistente);

    switch (tamanho) {

        case sizeof(cliente):
            ptrClient = todos;

            for(int i = 0; i<quantidade; i++){
                if(ptrClient[i].flag){
                    escolha = strlen(ptrClient[i].nomeCompleto)+1;
                    informacoesEmText[indice] = malloc(sizeof(char)*escolha);
                    strcpy(informacoesEmText[indice], ptrClient[i].nomeCompleto);

                    escolha+=strlen("; CPF: ")+1;
                    informacoesEmText[indice] = realloc(informacoesEmText[indice], sizeof(char)*escolha);
                    strcat(informacoesEmText[indice], "; CPF: ");

                    escolha += strlen(ptrClient[i].cpf)+1;
                    informacoesEmText[indice] = realloc(informacoesEmText[indice], sizeof(char)*escolha);
                    strcat(informacoesEmText[indice], ptrClient[i].cpf);

                    indice++;

                }
            }
            break;

        case sizeof(filmes):
            ptrMovies = todos;

            for(int i = 0; i<quantidade; i++){
                if(ptrMovies[i].flag){
                    escolha += strlen(ptrMovies[i].nome)+1;
                    informacoesEmText[indice] = malloc(sizeof(char)*escolha);
                    strcpy(informacoesEmText[indice], ptrMovies[i].nome);

                    escolha += strlen("; Quant: ")+1;
                    informacoesEmText[indice] = realloc(informacoesEmText[indice], sizeof(char)*escolha);
                    strcat(informacoesEmText[indice], "; Quant: ");

                    nomeConcat = converteIntEmString(ptrMovies[i].exemplares);
                    escolha += strlen(nomeConcat)+1;
                    informacoesEmText[indice] = realloc(informacoesEmText[indice], sizeof(char)*escolha);
                    strcat(informacoesEmText[indice], nomeConcat);
                    nomeConcat = limpaMemoria(nomeConcat);

                    escolha += strlen("; Val: ")+1;
                    informacoesEmText[indice] = realloc(informacoesEmText[indice], sizeof(char)*escolha);
                    strcat(informacoesEmText[indice], "; Val: ");

                    nomeConcat = converteFloatemString(ptrMovies[i].valorLocacao, 2);
                    escolha += strlen(nomeConcat)+1;
                    informacoesEmText[indice] = realloc(informacoesEmText[indice], sizeof(char)*escolha);
                    strcat(informacoesEmText[indice], nomeConcat);
                    nomeConcat = limpaMemoria(nomeConcat);

                    indice++;

                }
            }
            break;

        case sizeof(Funcionarios):
            ptrFuncs = todos;

            for(int i = 0; i<quantidade; i++){
                if(ptrFuncs[i].flag){
                    escolha = strlen(ptrFuncs[i].nome)+1;
                    informacoesEmText[indice] = malloc(sizeof(char)*escolha);
                    strcpy(informacoesEmText[indice], ptrFuncs[i].nome);

                    escolha += strlen("; Cod: ")+1;
                    informacoesEmText[indice] = realloc(informacoesEmText[indice], sizeof(char)*escolha);
                    strcat(informacoesEmText[indice], "; Cod: ");

                    nomeConcat = converteIntEmString(ptrFuncs[i].codigo);
                    escolha += strlen(nomeConcat)+1;
                    informacoesEmText[indice] = realloc(informacoesEmText[indice], sizeof(char)*escolha);
                    strcat(informacoesEmText[indice], nomeConcat);
                    nomeConcat = limpaMemoria(nomeConcat);

                    escolha += strlen(ptrFuncs[i].cargo)+4;
                    informacoesEmText[indice] = realloc(informacoesEmText[indice], sizeof(char)*escolha);
                    strcat(informacoesEmText[indice], "; ");
                    strcat(informacoesEmText[indice], ptrFuncs[i].cargo);

                    indice++;



                }
            }
            break;




    }
    while(escolha != 13){
        menuGraphicsComSeta(quantidadeExistente, mensagem, contador, informacoesEmText, 60);
        escolha = escolheOpcao();

        switch (escolha) {

            case 80:
                if(contador <quantidadeExistente-1){
                    contador++;
                }else{
                    contador = 0;
                }
                break;
            case 72:
                if(contador > 0){
                    contador--;
                }else{
                    contador = quantidadeExistente-1;
                }
                break;
        }
    }
    switch(tamanho){
        case sizeof(cliente):
            itemProcurado = (void *)encontraClienteNome(ptrClient, quantidade, strtok(informacoesEmText[contador], ";"));
            break;
        case sizeof(Funcionarios):
            itemProcurado = (void *) encontraFuncionarioNome(ptrFuncs, strtok(informacoesEmText[contador], ";"), quantidade, NULL);
            break;

        case sizeof(filmes):
            itemProcurado = (void *) encontraFilmeNome(ptrMovies, quantidade, strtok(informacoesEmText[contador], ";"));
            break;
    }

    for(int i = 0; i<quantidadeExistente; i++){
        informacoesEmText[i] = limpaMemoria(informacoesEmText[i]);
    }
    informacoesEmText = limpaMemoria(informacoesEmText);


    return itemProcurado;


}

