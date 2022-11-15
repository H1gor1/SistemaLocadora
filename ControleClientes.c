#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include "ControleClientes.h"
#include "ManipulacoesDeTexto.h"
#include "ControleDeDadosLocadora.h"
#include "ControleDeNumeros.h"
#include "FuncUtilitarias.h"
#include "menus.h"
#include <ctype.h>
#include <locale.h>
#include <windows.h>
#define clienteNaoExiste 0
#define ERROMEM "ERRO: Memoria indisponivel!\n"
#define CLIENTEEXISTE 1
/*funcao responsavel por verificar se existem clientes cadastrados, visto que a exclusao e feita por exclusao logica, entao pode ter 5 cadastros
 mas todos estarem com os campos com "*"*/
int verificaExisteClientes(cliente *ptr, int quantidade){
    //se quantidade for 0, entao nao existem clientes cadastrados
    if(quantidade == 0){
        return 0;
    }
    /*loop verifica se existe algum cliente com flag 1*/
    for(int i = 0; i<quantidade; i++){
        //se a flag atual for diferente de 0, entao existe clientes
        if(ptr[i].flag != 0){
           return 1;  
        }
    }
    //se em nenhum caso o if foi verdadeiro, entao nao existem clientes cadastrados
    return 0;
}
/*funcao responsavel por limpar os dados de uma struct de clientes, recebe um ponteiro para o inicio do vetor de struct, e a quantidade
 de clientes estao alocados na memoria*/
void limpaDadosClienteMemoria(cliente *ptr, int quantidade){
    /*enquanto nao terminar de limpar todos os campos da memoria, continua limpando*/
    for(int i = 0; i<quantidade; i++){
        ptr[i].nomeCompleto = limpaMemoria(ptr[i].nomeCompleto);
        ptr[i].rua = limpaMemoria(ptr[i].rua);
        ptr[i].bairro = limpaMemoria(ptr[i].bairro);
        ptr[i].cpf = limpaMemoria(ptr[i].cpf);
        ptr[i].email = limpaMemoria(ptr[i].email);
        
    }
}
/*funcao responsavel por ler os dados do arquivo de clientes binario, recebe um ponteiro para o ponteiro que aponta para o inicio da struct*/
int leDadosClientesBin(cliente **ptr){
    /*quantidade de clientes cadastrados*/
    int quantidade;
    /*quantidade de letras de cada palavra que sera lida*/
    int quantidadeLetras;
    FILE *f;
    /*abre o arquivo de clientes em modo de leitura*/
    f = fopen("clientes.bin", "rb");
    /*se f recebeu null, entao o arquivo nao existe*/
    if(!f){
        /*se nao existe, entao ele sera criado*/
        f = fopen("clientes.bin", "wb");
        /*escreve como primeira informacao no arquivo a quantidade 0, que signifca nenhum cliente, visto que o arquivo acaba de ser criado*/
        quantidade = 0;
        fwrite(&quantidade, sizeof(int), 1, f);
        fechaArquivo(&f);//fecha o arquivo
        return 0;//retorna 0 como quantidade de clientes cadastrados
    }
    /*se f nao recebeu null, entao e lida a quantidade de clientes totais cadastrados*/
    fread(&quantidade, sizeof(int), 1, f);
    /*aloca a quantidade de memoria necessaria para as informacoes de cadastros de clientes, e o endereco da alocacao e guardado no endereco em que 
     o ponteiro aponta, ou seja, no ponteiro de struct que foi lhe passado o endereco*/
    ptr[0] = malloc(sizeof(cliente)*quantidade);
    /*verifica se houve memoria o suficiente para a operacao*/
    verificaOperacao(ptr[0], ERROMEM, 1);
    
    /*enquanto nao foi lida a quantidade total de clientes cadastrados, continuar lendo*/
    for(int i = 0; i<quantidade; i++){
        /*le a quantidade de bytes um int ocupa de dentro do arquivo binario, e esse numero e guardado no codigo do cliente atual*/
        fread(&ptr[0][i].codigo, sizeof(int), 1, f);
        
        /*le a quantidade de letras do proximo campo de string do cliente atual*/
        fread(&quantidadeLetras, sizeof(int), 1, f);
        /*aloca a memoria necessaria para o nome do cliente*/
        ptr[0][i].nomeCompleto = malloc(sizeof(char)*quantidadeLetras);
        /*verifica se houve memoria o suficiente para a operacao*/
        verificaOperacao(ptr[0][i].nomeCompleto, ERROMEM, 1);
        /*le um tamanho de um char vezes a quantidade de letras da palavra para dentro da posicao de memoria do nome*/
        fread(ptr[0][i].nomeCompleto, sizeof(char), quantidadeLetras, f);
        
        
        /*a logica se repete nas linhas abaixo*/
        fread(&quantidadeLetras, sizeof(int), 1, f);
        ptr[0][i].rua = malloc(sizeof(char)*quantidadeLetras);
        verificaOperacao(ptr[0][i].rua, ERROMEM, 1);
        fread(ptr[0][i].rua, sizeof(char), quantidadeLetras, f);
        
        fread(&quantidadeLetras, sizeof(int), 1, f);
        ptr[0][i].bairro = malloc(sizeof(char)*quantidadeLetras);
        verificaOperacao(ptr[0][i].bairro, ERROMEM, 1);
        fread(ptr[0][i].bairro, sizeof(char), quantidadeLetras, f);
        
        fread(&ptr[0][i].numeroDaCasa, sizeof(int), 1, f);
        
        fread(&quantidadeLetras, sizeof(int), 1, f);
        ptr[0][i].cpf = malloc(sizeof(char)*quantidadeLetras);
        verificaOperacao(ptr[0][i].cpf, ERROMEM, 1);
        fread(ptr[0][i].cpf, sizeof(char), quantidadeLetras, f);
        
        fread(&ptr[0][i].telefone, sizeof(int), 1, f);
        
        fread(&quantidadeLetras, sizeof(int), 1, f);
        ptr[0][i].email = malloc(sizeof(char)*quantidadeLetras);
        verificaOperacao(ptr[0][i].email, ERROMEM, 1);
        fread(ptr[0][i].email, sizeof(char), quantidadeLetras, f);
        
        fread(&ptr[0][i].sexo, sizeof(int), 1, f);
        
        fread(&ptr[0][i].estadoCivil, sizeof(int), 1, f);
        
        fread(&ptr[0][i].diaNascimento, sizeof(int), 1, f);
        fread(&ptr[0][i].mes, sizeof(int), 1, f);
        fread(&ptr[0][i].ano, sizeof(int), 1, f);
        fread(&ptr[0][i].flag, sizeof(int), 1, f);
    }
    
    fechaArquivo(&f);//depois de lido todos os dados do aquivo, fecha o arquivo
    return quantidade;//retorna a quantidade de clientes
    
}
/*funcao responsavel por ler os dados dos clientes de um arquivo de texto
 recebe um ponteiro para uma ponteiro de uma struct de clientes*/
int leDadosClientes(cliente **ptr){
    /*quantidade de clientes totais cadastrados*/
    int quantidade;
    FILE *f;
    /*abre o arquivo de cadastros de clientes em modo de leitura*/
    f = fopen("clientes.txt", "r");
    /*se f recebeu null, entao o arquivo nao existe*/
    if(!f){
        /*entao cria o arquivo*/
        f = fopen("clientes.txt", "w");
        //printa como primeira informamcao no arquivo o numero, que significa 0 clientes
        fprintf(f, "0");
        //fecha o arquivo, e retorna  como quantidade
        fechaArquivo(&f);
        return 0;
    }
    /*caso f nao for null, entao le a primeira informacao do arquivo para a quantidade de clientes cadastrados*/
    fscanf(f, "%d", &quantidade);
    /*desreferencia o primeiro nivel do ponteiro para acessar de fato o ponteiro que guarda o endereco da struct, e 
     nesse ponteiro e guardado o endereco da alocacao da struct necessaria para guardar todas as informacoes dos clientes em memoria*/
    ptr[0] = malloc(sizeof(cliente)*quantidade);
    /*verifica se houve memoria o suficiente para a operacao*/
    verificaOperacao(ptr[0], ERROMEM, 1);
    
    /*enquanto nao ler todas as informacoes de todos os clientes, continuar lendo*/
    for(int i = 0; i<quantidade; i++){
        /*cada informacao e pega e guardada dentro do campo certo do cliente atual*/
        fscanf(f, "%d ", &ptr[0][i].codigo);
        
        digText(&ptr[0][i].nomeCompleto, f);
        
        digText(&ptr[0][i].rua, f);
        
        digText(&ptr[0][i].bairro, f);
        
        fscanf(f, "%d ", &ptr[0][i].numeroDaCasa);
        
        digText(&ptr[0][i].cpf, f);
        
        fscanf(f,"%d ", &ptr[0][i].telefone);
        
        digText(&ptr[0][i].email, f);
        
        fscanf(f, "%d ", &ptr[0][i].sexo);
        
       
        fscanf(f, "%d ", &ptr[0][i].estadoCivil);
        
        fscanf(f, "%d ", &ptr[0][i].diaNascimento);
        
        fscanf(f, "%d ", &ptr[0][i].mes);
        
        fscanf(f, "%d ", &ptr[0][i].ano);
        
        fscanf(f, "%d  ", &ptr[0][i].flag);
    }
    /*depois de terminar a leitura dos dados para a struct, fecha o arquivo, e retorna a quantidade*/
    fechaArquivo(&f);
    return quantidade;
}
/*funcao responsavel por reescrever os dados dos clientes para um arquivo binario, recebe um ponteiro que aponta
 para a primeira posicao da struct guardada em memoria, e a quantidade de clientes totais cadastrados*/
void reescreveDadosClienteBin(cliente *ptr, int quantidade){
    
    FILE *f;
    /*cria um arquivo temporario de clientes para escrever os dados*/
    f = fopen("clientesRes.bin", "wb");
    /*se f for null, entao houve um problema ao tentar criar o arquivo*/
    if(!f){
        /*se houve um erro, entao avisa o usuario sobre o erro*/
        printf("erro na abertura do arquivo! Novos dados nao foram salvos!\n");
        printf("%s\n", strerror(errno));
        /*retorna para a execucao da funcao anterior, cancelando a operacao*/
        return;
    }
    /*caso nao houve nenhum erro, le a primeira informacao para a quantidade de clientes cadastrados*/
    fwrite(&quantidade, sizeof(int), 1, f);
    //var que representa a quantidade de letras de cada palavra
    int quantidadeLetras;
    
    /*enquanto nao escreveu os dados de todos os clientes no arquivo, continuar escrevendo*/
    for(int i = 0; i<quantidade; i++){
        /*escreve o codigo do cliente atual no arquivo*/
        fwrite(&ptr[i].codigo, sizeof(int), 1, f);
        
        /*guarda a quantidade de letras da palavra atual dentro da var*/
        quantidadeLetras = strlen(ptr[i].nomeCompleto)+1;
        /*escreve a quantidade de letras a proxima palavra gasta em memoria dentro do arquivo*/
        fwrite(&quantidadeLetras, sizeof(int), 1, f);
        /*escreve a palavra*/
        fwrite(ptr[i].nomeCompleto, sizeof(char), quantidadeLetras, f);
        
        /*os proximos comandos seguem a mesma logica dos anteriores*/
        quantidadeLetras = strlen(ptr[i].rua)+1;
        fwrite(&quantidadeLetras, sizeof(int), 1, f);
        fwrite(ptr[i].rua, sizeof(char), quantidadeLetras, f);
        
        quantidadeLetras = strlen(ptr[i].bairro)+1;
        fwrite(&quantidadeLetras, sizeof(int), 1, f);
        fwrite(ptr[i].bairro, sizeof(char), quantidadeLetras, f);
        
        fwrite(&ptr[i].numeroDaCasa, sizeof(int), 1, f);
        
        quantidadeLetras = strlen(ptr[i].cpf)+1;
        fwrite(&quantidadeLetras, sizeof(int), 1, f);
        fwrite(ptr[i].cpf, sizeof(char), quantidadeLetras, f);
        
        fwrite(&ptr[i].telefone, sizeof(int), 1, f);
        
        quantidadeLetras = strlen(ptr[i].email)+1;
        fwrite(&quantidadeLetras, sizeof(int), 1, f);
        fwrite(ptr[i].email, sizeof(char), quantidadeLetras, f);
        
        fwrite(&ptr[i].sexo, sizeof(int), 1, f);
        
        fwrite(&ptr[i].estadoCivil, sizeof(int), 1, f);
        
        fwrite(&ptr[i].diaNascimento, sizeof(int), 1, f);
        
        fwrite(&ptr[i].mes, sizeof(int), 1, f);
        
        fwrite(&ptr[i].ano, sizeof(int), 1, f);
        
        fwrite(&ptr[i].flag, sizeof(int), 1, f);
    }
    //fecha o arquivo depois de salvar todos os dados no arquivo temporario
    fechaArquivo(&f);
    /*depois de ter certeza que todos os dados foram salvos corretamente, apaga o arquivo de clientes anterior*/
    remove("clientes.bin");
    /*renomeio o arquivo temporario para o nome original*/
    rename("clientesRes.bin", "clientes.bin");
}
/*funcao responsavel por reescrever os dados dos clientes em arquivo texto, recebe um ponteiro para a struct de clientes em memoria, 
 e a quantidade de clientes cadastrados*/
void reescreveDadosCliente(cliente *ptr, int quantidade){
    FILE *f;
    /*cria um arquivo temporario para salvar os dados*/
    f = fopen("clientesRes.txt", "w");
    /*se houve um erro ao tentar criar um arquivo, entao*/
    if(!f){
        /*avisa o usuario sobre o erro, e  o motivo dele*/
        printf("erro ao salvar arquivo!Novos dados nao foram salvos!\n");
        printf("%s\n", strerror(errno));
        return;//cancela a operacao
    }
    /*caso nao houve erro, entao printa a quantidade de clientes cadastrados na primeira linha do arquivo temporario*/
    fprintf(f, "%d\n", quantidade);
    
    /*enquanto nao escreveu todos os dados dos clientes no arquivo, continuar escrevendo*/
    for(int i = 0; i<quantidade; i++){
        
            /*escreve cada campo de informacao do cliente atual dentro do arquivo*/
            fprintf(f, "%d\n", ptr[i].codigo);
            fprintf(f, "%s\n", ptr[i].nomeCompleto);
            fprintf(f, "%s\n", ptr[i].rua);
            fprintf(f, "%s\n", ptr[i].bairro);
            fprintf(f, "%d\n", ptr[i].numeroDaCasa);
            fprintf(f, "%s\n", ptr[i].cpf);
            fprintf(f, "%d\n", ptr[i].telefone);
            fprintf(f, "%s\n", ptr[i].email);
            fprintf(f, "%d\n", ptr[i].sexo);
            fprintf(f, "%d\n", ptr[i].estadoCivil);
            fprintf(f, "%d\n", ptr[i].diaNascimento);
            fprintf(f, "%d\n", ptr[i].mes);
            fprintf(f, "%d\n", ptr[i].ano);
            fprintf(f, "%d\n\n", ptr[i].flag);

        
    }
    /*fecha o arquivo f*/
    fechaArquivo(&f);
    /*depois de salvar todos os dados com sucesso dentro do arquivo temporario, exclui o arquivo antigo*/
    remove("clientes.txt");
    /*entao renomeio o arquivo temporario para o nome original*/
    rename("clientesRes.txt", "clientes.txt");
    
    
}

/*funcao responsavel por encontrar um cliente pelo codigo
 recebe um ponteiro para todas as struct de clientes, a quantidade de clientes cadastrados, o codigo em forma de string, e um parametro int que 
 diz se deve ignorar clientes apagados ou nao*/
cliente *encontraClienteCodigo(cliente *ptr, int quantidade, char *codigo, int ignorar ){
    //se o primeiro codigo nao for um digito ou o tamanho da string for maior que 8, deve-se ignorar pois recebeu um codigo invalido
    if(!isdigit(codigo[0]) || strlen(codigo)>8){
        return NULL;//entao retorna null
    }
    //se nao, converte a string para um inteiro
    int cod = atoi(codigo);
    //como cada cliente novo recebe o codigo em ordem crescente de cadastrado, ou seja, se for o quinto cliente a ser cadastrado, 
    //recebera o codigo 4, logo por essa logica, se o codigo for maior que a quantidade total de clientes, retorna null
    if(cod>=quantidade){
        return NULL;
    }
    //se nome completo for igual a * e na chamada da funcao tiver mandado ignorar clientes apagados, entao retorna null
    if(!ptr[cod].flag && ignorar){
        return NULL;
    }
    /*se todos os testes foram falsos, entao o codigo e valido*/
    
    return (ptr+cod);
}
/*funcao responsavel por encontrar um cliente pelo nome*/
cliente *encontraClienteNome(cliente *ptr, int quantidade, char *nome){
    /*enquanto nao conferir se todos os clientes tem o nome pedido, continuar conferindo*/
    for(int i = 0; i<quantidade; i++){
        //se nome completo for igual a nome, entao retorne o ponteiro atual
        if(ptr[i].flag && !strcmp(ptr[i].nomeCompleto, nome)){
            return (ptr+i);
        }
    }
    /*se pra nenhum dos testes o if foi verdadeiro, retorna null*/
    return NULL;
}
/*funcao responsavel por encontrar o cliente pelo cpf, recebe 
 um ponteiro para a struct de cliente em memoria, a quantidade de clientes cadastrados, e uma string do cpf*/
cliente *encontraClienteCpf(cliente *ptr, int quantidade, char *cpf){
    /*enquanto nao conferiu se todos os clientes tem aquele cpf, continuar conferindo*/
    for(int i = 0; i<quantidade; i++){
        /*se o cliente atual tem o cpf pedido, entao retornar o ponteiro para este cliente*/
        if(!strcmp(ptr[i].cpf, cpf) && ptr[i].flag){
            return (ptr+i);
        }
    }
    /*se em nenhum teste deu true, entao retornar null*/
    return NULL;
}
/*funcao respoonsavel por tentar encontrar um cliente usando as 3 funcoes acimas
 recebe um ponteiro para onde esta a struct de cliente em memoria, a quantidade de clientes cadastrados, e a mensagem de erro que deve exibir*/
cliente *buscaCliente(cliente *buscar, int quantidade, char *mensagem){
    char *dado;//string que recebera o texto
    cliente *ptr;//ponteiro que recebera o endereco do cliente buscado
    do{
        digText(&dado, stdin);//pega a digitacao do dado que deve buscar
        /*com o dado digitado tenta buscar pelo cpf*/
        ptr = encontraClienteCpf(buscar, quantidade, dado);
        //se o ponteiro for um endereco nao null, e o nome for diferente de *
        if(ptr){
            //entao o cliente foi encontrado, logo limpa a memoria da string digitada, e retorna o ptr
            dado = limpaMemoria(dado);
            return ptr;
        }
        /*se o if anterior foi falso, entao quer dizer que nao conseguiu encontrar o cliente pelo cpf, logo tenta encontrar pelo nome
         usando o dado em que o usuario digitou*/
        ptr = encontraClienteNome(buscar, quantidade, dado);
        /*se o ponteiro devolvido pela funcao nao foi um ponteiro null, e o nome nao e "*", entao */
        if(ptr){
            /*limpa a memoria da string que o usuario digitou*/
            dado = limpaMemoria(dado);
            /*devolve o ponteiro*/
            return ptr;
        }
        /*se o if anterior foi falso, entao tenta encontrar o cliente pelo codigo*/
        ptr = encontraClienteCodigo(buscar, quantidade, dado,1);
        /*se o ponteiro devolvido pela funcao foi um ponteiro valido, entao*/
        if(ptr){
            /*limpa a memoria da string digitada pelo usuario*/
            dado = limpaMemoria(dado);
            /*retorna o ponteiro*/
            return ptr;
        }
        /*se o if anterior tb foi falso, entao o usuario digitou um dado que nenhum cliente possui*/
        dado = limpaMemoria(dado);
        /*mosta a mensagem de erro, e o loop reinicia pro usuario digitar novamente*/
        printf("%s\n", mensagem);
        
    }while(1);
}
/*funcao responsavel por editar os dados de um cliente ja cadastrado, recebe como argumento um inteiro que representa qual modo de abertura ela deve usar*/
void editaCliente(int modoAbertura){
    /*vetor de funcoes de leitura de dados*/ 
    int (*leDados[2])(cliente**) = {
        leDadosClientes, leDadosClientesBin
    };
    /*vetor de funcoes de reescrita de dados*/
    void (*reescreveDados[2])(cliente*, int)={
        reescreveDadosCliente, reescreveDadosClienteBin 
    };
    /*ponteiro para o cliente que sera editado*/
    cliente *ptr = NULL;
    /*ponteiro para o inicio da struct de clientes cadastrados em memoria*/
    cliente *clientes = NULL;
    /*var que representa a quantidade de clientes cadastrados*/
    int quantidadeClientes;
    
    /*faz a leitura dos dados do arquivo de clientes cadastrados segundo o argumento passado como parametro, se 0, entao usa a funcao de leitura de
     arquivo de texto, se um, entao usa a leitura de arquivo binario*/
    quantidadeClientes = (*leDados[modoAbertura])(&clientes);
    /*se a funcao retornar 0, entao quer dizer que nao existe clientes, se retornar um, existe*/
    if(!verificaExisteClientes(clientes, quantidadeClientes)){
        /*trocar pela funcao do menu*/
        /*se retornou 0, entao nao tem como editar clientes, pois nao existem clientes cadastrados*/
        /*logo printa a mensagem na tela dizendo que nao existe nenhum cliente cadastrado no momento*/
        printf("nao existe nenhum cliente cadastrado no momento\n");
        /*cancela a operacao*/
        Sleep(2000);
        return;
    }
    /*se quantidades nao for 0, entao existem clientes, e a operacao pode continuar normalmente*/
    printf("digite o nome, cpf ou codigo do cliente que deseja editar\n");
    /*mostra um resumo de nome codigo e cpf de todos os clientes atuais do sistema*/
    mostraListaClientes(clientes, quantidadeClientes);
    /*chamada da funcao para encontrar o cliente que o usuario deseja procurar*/
    ptr = buscaCliente(clientes, quantidadeClientes, "Nenhum cliente possui o dado digitado, por favor, digite um Cpf, nome ou codigo");
    
    /*variavel que representa o dado que o usuario deseja editar*/
    int dadoEditar;



    dadoEditar = escolheMenu("Qual campo deseja editar:", 11, 0,"Nome completo", "Rua", "Bairro", "Numero da casa", "CPF", "Telefone",
                             "E-mail", "Sexo", "Estado civil", "Data de nascimento", "Voltar");
    


    switch(dadoEditar){
            
        case 0:
            /*caso seja um campo de string, limpa a posicao de memoria anterior onde estava guardado a string, e pede a digitacao de uma nova
            string*/
            printf("digite o nome do cliente\n");
            ptr->nomeCompleto = limpaMemoria(ptr->nomeCompleto);
            pegaDadoTextEverificaRepeticao(clientes, quantidadeClientes, ptr, &ptr->nomeCompleto, "ja existe um cliente com este nome no sistema, por favor, digite outro nome!");
            break;
        
        case 1:
            printf("digite o nome da rua do cliente\n");
            ptr->rua = limpaMemoria(ptr->rua);
            digText(&ptr->rua, stdin);
            break;

            
        case 2:
            printf("digite o nome do bairro do cliente\n");
            ptr->bairro = limpaMemoria(ptr->bairro);
            digText(&ptr->bairro, stdin);
            break;
                
        case 3:
                    
            /*caso seja um campo numerico, somente pede a digitacao do novo numero*/
            printf("digite o numero da casa do cliente\n");
            verificaNumero(&ptr->numeroDaCasa, "%d");
            break;
                    
        case 4:
                
            printf("digite o numero do cpf do cliente\n");
            ptr->cpf = limpaMemoria(ptr->cpf);
            pegaDadoTextEverificaRepeticao(clientes, quantidadeClientes, ptr, &ptr->cpf, "ja existe um cliente com este cpf no sistema!");
            break;
                    
        case 5:
                            

            printf("digite o telefone do cliente\n");
            verificaNumero(&ptr->telefone, "%d");
            break;

        case 6:
                
            printf("digite o email do cliente\n");
            ptr->email = limpaMemoria(ptr->email);
            digText(&ptr->email, stdin);
            break;
               
        case 7:
            printf("digite o sexo do cliente:\n");
            printf("1-masculino\n2-feminino\n");
            verificaLimiteNumero(&ptr->sexo, 2, 1, "%d");
            break;

        case 8:
                                        

            printf("digite o estado civil do cliente\n");
            printf("1-solteiro\n2-casado\n3-viuvo\n4-divorciado\n");
            verificaLimiteNumero(&ptr->estadoCivil, 4, 1, "%d");
            break;

        case 9:
                  
            printf("digite o mes de nascimento\n");
            verificaLimiteNumero(&ptr->mes, 12, 0, "%d");
            printf("digite o dia de nascimento\n");
            verificaLimiteNumero(&ptr->diaNascimento, (ptr->mes == 2)?28:31, 1, "%d");
            printf("digite o ano de nascimento\n");
            verificaNumero(&ptr->ano, "%d");
            break;

        case 10:
            break;

                                            


    
    }
    /*reescreve os dados novos que foram editados segundo a foram passada como parametro, como ja explicado na funcao de leitura*/
    (*reescreveDados[modoAbertura])(clientes, quantidadeClientes);
    /*limpa os campos de texto dos clientes da memoria*/
    limpaDadosClienteMemoria(clientes, quantidadeClientes);
    /*limpa o vetor de struct de clientes da memoria*/
    clientes = limpaMemoria(clientes);
    ptr = NULL;//ptr de edicao recebe endereco null
    
    
    
}
/*funcao responsavel por cadastrar um cliente*/
void cadastraCliente(int modoLeitura){
    /*nesta funcao e necessario puchar toda a lista de clientes para a memoria para cadastrar mais um, pois sera preciso verificar se o cpf e o nome
     nao foram digitados de forma repetida, logo todos os dados devem estar em memoria para ser possivel comparar*/
    
    /*ponteiro onde sera guardado a alocacao da lista de clientes*/
    cliente *client;
    /*funcoes de leitura de dados do arquivo de cadastro*/
    int (*leDados[2])(cliente **) = {leDadosClientes, leDadosClientesBin};
    /*funcoes de reescrita de dados*/
    void (*reescreveDados[2])(cliente *, int) = {reescreveDadosCliente, reescreveDadosClienteBin};
    /*e feita a leitura dos dados do arquivo de clientes para a struct de client, e a quantidade de clientes retornada por ela e acrescentada de um*/
    int quantidadeCliente = leDados[modoLeitura](&client) + 1;
    
    /*se a quantidade de cliente for um, entao a funcao de leitura nao fez nenhuma alocacao para guardar os clientes, logo 
     sera necessario usar a funcao malloc, se for diferente de um, entao a funcao de leitura fez alocacoes para guardar em memoria
     os dados do arquivo, logo sera necessario usar a funcao realloc*/
    client = (quantidadeCliente == 1)?malloc(sizeof(cliente)):realloc(client, sizeof(cliente)*quantidadeCliente);
    //verifica se houve memoria o suficiente para executar a operacao
    verificaOperacao(client, ERROMEM, 1);
    
    /*client atual recebe o codigo de ordem de cadastro, ou seja, se for o 10, recebe codigo 9*/
    client[quantidadeCliente-1].codigo = quantidadeCliente-1;
    
    /*pega o nome do cliente e verifica se nao existe nenhum cliente com este nome*/
    printf("digite o nome do cliente\n");
    pegaDadoTextEverificaRepeticao(client, quantidadeCliente, &client[quantidadeCliente-1], &client[quantidadeCliente-1].nomeCompleto, "ja existe um cliente com esse nome, por favor digite outro nome");
    
    /*pega a rua do cliente*/
    printf("digite a rua do cliente\n");
    digText(&client[quantidadeCliente-1].rua, stdin);
    /*pega o bairro do cliente*/
    printf("digite o bairro\n");
    digText(&client[quantidadeCliente-1].bairro, stdin);
    /*pega o numero do cliente, e verifica se ele digitou somente numeros*/
    printf("digite o numero da casa do cliente\n");
    verificaNumero(&client[quantidadeCliente-1].numeroDaCasa, "%d");
    
    /*pega o cpf do cliente, verifica se o cpf tem as conficoes minimas para ser valido, e verifica se nao ha nenhum cpf igual no sistema*/
    printf("digite o cpf do cliente\n");
    pegaDadoTextEverificaRepeticao(client, quantidadeCliente, &client[quantidadeCliente-1], &client[quantidadeCliente-1].cpf, "ja existe um cliente com este cpf, por favor verifique se nao esta tentando cadastrar duas vezes o mesmo cliente");
    
    printf("digite o telefone do cliente\n");
    verificaNumero(&client[quantidadeCliente-1].telefone, "%d");
    
    /*pega o email, e verifica se o email foi digitado corretamente*/
    printf("digite o email do cliente\n");
    verificaText("@.", &client[quantidadeCliente-1].email, "Email digitado esta incompleto!\n");
    
    client[quantidadeCliente-1].sexo = escolheMenu("Qual sexo do cliente?", 2, 0,"Masculino", "Femino")+1;
    

    client[quantidadeCliente-1].estadoCivil = 1+escolheMenu("Qual estado civil do cliente?", 4, 0,"Solteiro", "Casado", "Viuvo", "Divorciado");

    printf("digite o mes de nascimento do cliente\n");
    verificaLimiteNumero(&client[quantidadeCliente-1].mes, 12, 0, "%d");
    
    printf("digite o dia do nascimento\n");
    verificaLimiteNumero(&client[quantidadeCliente-1].diaNascimento, (client[quantidadeCliente-1].mes == 2)?28:31,0, "%d");
    
    printf("digite o ano de nascimento do cliente\n");
    verificaNumero(&client[quantidadeCliente-1].ano, "%d");
    
    client[quantidadeCliente-1].flag = CLIENTEEXISTE;
    /*reescreve os dados no arquivo segundo o argumento passado como parametro pela funcao, se 0, arquivo text, se 1, binario*/
    (*reescreveDados[modoLeitura])(client, quantidadeCliente);
    /*limpa os campos de texto das structs de clientes da memoria*/
    limpaDadosClienteMemoria(client, quantidadeCliente);
    /*limpa todas as struct de clientes da memoria*/
    client = limpaMemoria(client);
    
    printf("cliente cadastrado com sucesso!\n");
    
    
}
/*funcao responsavel por apagar um cliente, recebe como argumento o modo de armazenamento usado atualmente pelo sistema*/
void apagaCliente(int modo){
     
    /*ponteiro para um vetor de stuct de cliente*/
    cliente *clientes;
    /*vetor de funcoes de letura de dados do arquivo de cliente*/
    int (*leDados[2])(cliente**) = {
      leDadosClientes, leDadosClientesBin  
    };
    /*vetor de funcoes de reescrita de dados de clientes*/
    void (*reescreveDados[2])(cliente*, int) = {  
        reescreveDadosCliente, reescreveDadosClienteBin
    };
    /*realiza a leitura do arquivo de cliente e cria uma struct em memoria com o tamanho adequado, e devolve o ponteiro inicial para clientes, e retornna
     a quatnidade de clientes para a var quantidade clientes*/
    int quantidadeClientes = (*leDados[modo])(&clientes);
    
    /*se a funcao retornar 0, entao nao existem clintes cadastrados, se retornar 1, entao existem clientes cadastrados*/
    if(!verificaExisteClientes(clientes, quantidadeClientes)){
        //se retornar 0, entao nao existem clientes cadastrados, logo nao e possivel apagar
        printf("ate o momento nao existe nenhum funcionario cadastrado\n");
        Sleep(2000);
        /*avisa o usuario que nao existem clientes cadastrados para serem apagados, e cancela a operacao*/
        return;
    }
    /*se a funcao retornou 1, entao existe clientes cadastrados e sera possivel editar os campos de clientes*/
    printf("digite o cpf, codigo ou nome do cliente que deseja apagar\n");
    /*mostra um resumo de nome codigo e cpf de todos os clientes, para o usuario poder consultar qual ele deseja apagar*/
    mostraListaClientes(clientes, quantidadeClientes);
    /*ponteiro que apontara para o cliente que sera apagado*/
    cliente *apagar;
    
    /*pega a digitacao de um dado de cpf nome ou codigo e tenta encontrar na struct qual cliente possui o dado digitado,
     se encontrar retorna o ponteiro desse cliente para apagar*/
    apagar = buscaCliente(clientes, quantidadeClientes, "Nenhum cliente tem o dado digitado, por favor, digite um nome, cpf ou codigo");
    
    apagar->flag = 0;
    
    
    (*reescreveDados[modo])(clientes, quantidadeClientes);
    /*limpa os campos de texto das structs de clientes da memoria*/
    limpaDadosClienteMemoria(clientes, quantidadeClientes);
    /*limpa a struct de clientes*/
    clientes = limpaMemoria(clientes);
    /*ponteiro que apontava para o cliente que deveria ser apagado recebe null*/
    apagar = NULL;
}
/*funcao responsavel por mostrar os clientes que existem nos sistema
 recebe como argumento um ponteiro para a primeira posicao da struct de cliente e a quantidade de clientes*/
void mostraListaClientes(cliente *ptr, int quantidade){
    /*enquanto nao chegar na ultima posicao de clientes cadastrados na memoria, continuar percorrendo*/
    for(int i = 0; i<quantidade; i++){
        //se o nome do cliente atual for diferente de *, entao ele devera ser mostrado
        if(ptr[i].flag != 0){
            /*printa as informacoes do cliente atual*/
            printf("___________________\n");
            printf("|Nome: %s\n", ptr[i].nomeCompleto);
            printf("|Codigo: %d\n", ptr[i].codigo);
            printf("|Cpf: %s\n", ptr[i].cpf);
            printf("|__________________\n");
        }
    }
}
void filtraCodigo(cliente *ptr, int quantidade, int codigo, int codigo1){
    int quantidadeClientes = 0;
    int i=0;
    for(i = codigo; i<quantidade && i<=codigo1; i++){
        if(ptr[i].flag == 1) {
            quantidadeClientes++;
        }
        mostraListaClientes(ptr+i, 1);
        
    }
    printf("quantidade de clientes nesta faixa de codigo: %d\n", quantidadeClientes);
}
cliente* procuraClienteEspecificoComFaixa(cliente *ptr, int quantidade, int codigo, int codigo1){
    cliente *busca = NULL;
    while(1) {
        printf("digite o codigo do cliente.\n");
        busca = buscaCliente(ptr, quantidade, "nao existe nenhum cliente com este codigo");

        if(busca->codigo >= codigo && busca->codigo <= codigo1){
            return busca;
        }
        printf("este cliente nao esta na faixa de codigo pesquisada! Faixa pesquisada e de %d a %d\n", codigo, codigo1);
        printf("digite novamente!\n");
    }
}

void filtraSexo(cliente *ptr, int quantidade, int sexo){
    int i = 0;
    int quantidadeClientesSexo = 0;
    int escolha;
    cliente *especifico = NULL;
    for(i = 0; i<quantidade; i++){
        if(ptr[i].sexo == sexo && ptr[i].flag){
            mostraListaClientes(ptr+i, 1);
            quantidadeClientesSexo++;
        }
    }
    printf("quantidade clientes do sexo %s: %d\n", (sexo== 1)?"masculino":"femino", quantidadeClientesSexo);

    printf("Deseja consultar algum cliente especifico dos clientes listados?\n");
    printf("F1 - Sim\nF2 - Nao\n");

    do{

        escolha = escolheOpcao();

        if(escolha == 59){
            while(1) {
                printf("digite o codigo, nome ou cpf do cliente:\n");
                especifico = buscaCliente(ptr, quantidade, "nao existe nenhum cliente com esta informacao digitada!");
                if(especifico->sexo == sexo){
                    break;
                }
                printf("codigo invalido! Digite novamente!\n");

            }
            mostraInformacoesClientes(especifico, 1);
        }else
            if(escolha != 60){
                printf("escolha uma opcao valida!\n");

            }


    }while(escolha != 60 && escolha != 59);
}
void listaClientes(int modoArm){
    
    cliente *todosClientes = NULL;
    int quantidadeClientes;
    quantidadeClientes = (modoArm)?leDadosClientesBin(&todosClientes):leDadosClientes(&todosClientes);
    
    int escolha;

    escolha = escolheMenu("Como deseja filtrar os clientes", 3, 0,"Faixa de codigo", "Sexo", "Voltar");


    switch(escolha){
        case 0:
            listaPeloCodigo(todosClientes,
                            quantidadeClientes,
                            (void (*)(void *, int))mostraInformacoesClientes,
                            "algum cliente",
                            (void (*)(void *, int, int, int))filtraCodigo,
                            (void *(*)(void *, int, int, int))procuraClienteEspecificoComFaixa);
            break;
                
        case 1:
            int sexo;

            sexo = escolheMenu("Escolha o sexo", 3,0,"Masculino", "Femino", "Voltar")+1;
            if(sexo == 3){
                break;
            }
            filtraSexo(todosClientes, quantidadeClientes, sexo);
                
            break;

        case 2:
            break;



    }
    limpaDadosClienteMemoria(todosClientes, quantidadeClientes);
    todosClientes = limpaMemoria(todosClientes);
}
void mostraInformacoesClientes(cliente *ptr, int quantidade){
    for(int i = 0; i<quantidade; i++){
        printf("_____________________________________\n");
        printf("Codigo: %d\n", ptr[i].codigo);
        printf("Nome completo: %s\n", ptr[i].nomeCompleto);
        printf("Endereco:\n");
        printf("    Rua: %s\n", ptr[i].rua);
        printf("    Bairro: %s\n", ptr[i].bairro);
        printf("    Numero: %d\n", ptr[i].numeroDaCasa);
        printf("CPF: %s\n", ptr[i].cpf);
        printf("Telefone: %d\n", ptr[i].telefone);
        printf("Email: %s\n", ptr[i].email);
        printf("Sexo: %s\n", (ptr[i].sexo == 1)?"Masculino":"Feminino");
        printf("EstadoCivil: %s\n", (ptr[i].estadoCivil == 1)?"Solteiro":(ptr[i].estadoCivil == 2)?"Casado":(ptr[i].estadoCivil == 3)?"Viuvo":"Divorciado");
        printf("Data de Nascimento: %d/%d/%d\n", ptr[i].diaNascimento, ptr[i].mes, ptr[i].ano);

    }
    printf("\n\nDigite qualquer tecla para sair");
    escolheOpcao();

}