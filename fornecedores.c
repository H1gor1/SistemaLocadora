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
#include "fornecedores.h"
#include <ctype.h>
#include <locale.h>
#include <windows.h>
#define ERROMEM "ERRO: Memoria indisponivel!"
#define FORNECEDORNAOEXISTE 0
#define FORNECEDOREXISTE 1


void mostraDadosFornecedores(fornecedor *ptr, int quantidade){
    for(int i = 0; i<quantidade; i++){
        if(ptr[i].flag){
            printf("%d:\nNome Fantasia: %s\nCNPJ: %s\n", i+1, ptr[i].nomeFantasia, ptr[i].CNPJ);
        }
    }
}
void limpaMemoriaStringsFornecedor(fornecedor *ptr, int quantidade){
    for(int i = 0; i<quantidade; i++){
        
        ptr[i].nomeFantasia = limpaMemoria(ptr[i].nomeFantasia);
        ptr[i].razaoSocial = limpaMemoria(ptr[i].razaoSocial);
        ptr[i].inscricaoEstadual = limpaMemoria(ptr[i].inscricaoEstadual);
        ptr[i].CNPJ = limpaMemoria(ptr[i].CNPJ);
        ptr[i].bairro = limpaMemoria(ptr[i].bairro);
        ptr[i].rua = limpaMemoria(ptr[i].rua);
        ptr[i].email = limpaMemoria(ptr[i].email);
        
        
    }
}

int leDadosFornecedores(fornecedor **ptr){
    
    int quantidadeFornecedores;
    
    FILE *f;
    f = fopen("fornecedores.txt", "r");
    
    if(!f){
        f = fopen("fornecedores.txt", "w");
        fprintf(f, "0\n");
        fechaArquivo(&f);
        return 0;
    }
    
    fscanf(f, "%d ", &quantidadeFornecedores);
    ptr[0] = malloc(sizeof(fornecedor)*quantidadeFornecedores);
    verificaOperacao(ptr[0], ERROMEM, 1);
    
    for(int i = 0; i<quantidadeFornecedores; i++){
        
        fscanf(f, "%d ", &ptr[0][i].codigo);
        
        digText(&ptr[0][i].nomeFantasia, f);
        
        digText(&ptr[0][i].razaoSocial, f);
        
        digText(&ptr[0][i].inscricaoEstadual, f);
        
        digText(&ptr[0][i].CNPJ, f);
        
        digText(&ptr[0][i].bairro, f);
        
        digText(&ptr[0][i].rua, f);
        
        fscanf(f, "%d ", &ptr[0][i].numero);
        
        fscanf(f, "%d ", &ptr[0][i].telefone);
        
        digText(&ptr[0][i].email, f);

        fscanf(f, "%d  ", &ptr[0][i].flag);
        
    }
    
    fechaArquivo(&f);
    return quantidadeFornecedores;
}
int leDadosFornecedoresBin(fornecedor **ptr){
    
    
    int quantidadeFornecedores;
    int quantidadeLetras;
    FILE *f;
    
    f = fopen("fornecedores.bin", "rb");
    
    if(!f){
        quantidadeFornecedores = 0;
        f = fopen("fornecedores.bin", "wb");
        fwrite(&quantidadeFornecedores, sizeof(int), 1, f);
        fechaArquivo(&f);
        return 0;
    }
    
    fread(&quantidadeFornecedores, sizeof(int), 1, f);
    
    ptr[0] = malloc(sizeof(fornecedor)*quantidadeFornecedores);
    verificaOperacao(ptr[0], ERROMEM, 1);
    
    for(int i = 0; i<quantidadeFornecedores; i++){
        
        fread(&ptr[0][i].codigo, sizeof(int), 1, f);

        fread(&quantidadeLetras, sizeof(int), 1, f);
        ptr[0][i].nomeFantasia = malloc(sizeof(char)*quantidadeLetras);
        verificaOperacao(ptr[0][i].nomeFantasia, ERROMEM, 1);
        fread(ptr[0][i].nomeFantasia, sizeof(char), quantidadeLetras, f);

        fread(&quantidadeLetras, sizeof(int), 1, f);
        ptr[0][i].razaoSocial = malloc(sizeof(char)*quantidadeLetras);
        verificaOperacao(ptr[0][i].razaoSocial, ERROMEM, 1);
        fread(ptr[0][i].razaoSocial, sizeof(char), quantidadeLetras, f);


        fread(&quantidadeLetras, sizeof(int), 1, f);
        ptr[0][i].inscricaoEstadual = malloc(sizeof(char)*quantidadeLetras);
        verificaOperacao(ptr[0][i].inscricaoEstadual, ERROMEM, 1);
        fread(ptr[0][i].inscricaoEstadual, sizeof(char), quantidadeLetras, f);    

        fread(&quantidadeLetras, sizeof(int), 1, f);
        ptr[0][i].CNPJ = malloc(sizeof(char)*quantidadeLetras);
        verificaOperacao(ptr[0][i].CNPJ, ERROMEM, 1);
        fread(ptr[0][i].CNPJ, sizeof(char), quantidadeLetras, f);

        fread(&quantidadeLetras, sizeof(int), 1, f);
        ptr[0][i].bairro = malloc(sizeof(char)*quantidadeLetras);
        verificaOperacao(ptr[0][i].bairro, ERROMEM, 1);
        fread(ptr[0][i].bairro, sizeof(char), quantidadeLetras, f);

        fread(&quantidadeLetras, sizeof(int), 1, f);
        ptr[0][i].rua = malloc(sizeof(char)*quantidadeLetras);
        verificaOperacao(ptr[0][i].rua, ERROMEM, 1);
        fread(ptr[0][i].rua, sizeof(char), quantidadeLetras, f);

        fread(&ptr[0][i].numero, sizeof(int), 1, f);

        fread(&ptr[0][i].telefone, sizeof(int), 1, f);

        fread(&quantidadeLetras, sizeof(int), 1, f);
        ptr[0][i].email = malloc(sizeof(char)*quantidadeLetras);
        verificaOperacao(ptr[0][i].email, ERROMEM, 1);
        fread(ptr[0][i].email, sizeof(char), quantidadeLetras, f);
    
        fread(&ptr[0][i].flag, sizeof(int), 1, f);
    }
    
    fechaArquivo(&f);
    return quantidadeFornecedores;
    
    
}

void reescreveDadosFornecedores(fornecedor *ptr, int quantidade){
    
    FILE *f;
    f = fopen("fornecedoresRes.txt", "w");
    
    if(!f){
        printf("erro ao salvar arquivos, dados alterados recentemento foram perdidos!\n");
        printf("%s\n", strerror(errno));
        return;
    }
    
    fprintf(f, "%d\n", quantidade);
    
    for(int i = 0; i<quantidade; i++){
        
        fprintf(f, "%d\n", ptr[i].codigo);
        
        fprintf(f, "%s\n", ptr[i].nomeFantasia);
        
        fprintf(f, "%s\n", ptr[i].razaoSocial);
        
        fprintf(f, "%s\n", ptr[i].inscricaoEstadual);
        
        fprintf(f, "%s\n", ptr[i].CNPJ);
        
        fprintf(f, "%s\n", ptr[i].bairro);
        
        fprintf(f, "%s\n", ptr[i].rua);
        
        fprintf(f, "%d\n", ptr[i].numero);
        
        fprintf(f, "%d\n", ptr[i].telefone);
        
        fprintf(f, "%s\n", ptr[i].email);
        
        fprintf(f, "%d\n\n", ptr[i].flag);
    }
    
    fechaArquivo(&f);
    remove("fornecedores.txt");
    rename("fornecedoresRes.txt", "fornecedores.txt");
    return;
}

void reescreveDadosFornecedoresBin(fornecedor *ptr, int quantidade){
    
    int quantidadeLetras;
    FILE *f;
    f = fopen("fornecedoresRes.bin", "wb");
    
    if(!f){
        printf("erro ao salvar arquivos, dados alterados recentemento foram perdidos!\n");
        printf("%s\n", strerror(errno));
        return;
    }
    
    fwrite(&quantidade, sizeof(int), 1, f);
    
    for(int i = 0; i<quantidade; i++){
        
        fwrite(&ptr[i].codigo, sizeof(int), 1, f);
        
        quantidadeLetras = strlen(ptr[i].nomeFantasia)+1;
        fwrite(&quantidadeLetras, sizeof(int), 1, f);
        fwrite(ptr[i].nomeFantasia, sizeof(char), quantidadeLetras, f);
        
        quantidadeLetras = strlen(ptr[i].razaoSocial)+1;
        fwrite(&quantidadeLetras, sizeof(int), 1, f);
        fwrite(ptr[i].razaoSocial, sizeof(char), quantidadeLetras, f);
        
        quantidadeLetras = strlen(ptr[i].inscricaoEstadual)+1;
        fwrite(&quantidadeLetras, sizeof(int), 1, f);
        fwrite(ptr[i].inscricaoEstadual, sizeof(char), quantidadeLetras, f);
        
        quantidadeLetras = strlen(ptr[i].CNPJ)+1;
        fwrite(&quantidadeLetras, sizeof(int), 1, f);
        fwrite(ptr[i].CNPJ, sizeof(char), quantidadeLetras, f);
        
        quantidadeLetras = strlen(ptr[i].bairro)+1;
        fwrite(&quantidadeLetras, sizeof(int), 1, f);
        fwrite(ptr[i].bairro, sizeof(char), quantidadeLetras, f);
        
        quantidadeLetras = strlen(ptr[i].rua)+1;
        fwrite(&quantidadeLetras, sizeof(int), 1, f);
        fwrite(ptr[i].rua, sizeof(char), quantidadeLetras, f);
        
        fwrite(&ptr[i].numero, sizeof(int), 1, f);
        
        fwrite(&ptr[i].telefone, sizeof(int), 1, f);
        
        quantidadeLetras = strlen(ptr[i].email)+1;
        fwrite(&quantidadeLetras, sizeof(int), 1, f);
        fwrite(ptr[i].email, sizeof(char), quantidadeLetras, f);
        
        fwrite(&ptr[i].flag, sizeof(int), 1, f);
        
    }
    
    fechaArquivo(&f);
    remove("fornecedores.bin");
    rename("fornecedoresRes.bin", "fornecedores.bin");
    
}

void cadastraFornecedor(int modoAbertura){
    
    fornecedor *fornecedores;
    
    int (*leDados[2])(fornecedor **) = {
        leDadosFornecedores, leDadosFornecedoresBin
    };
    void (*reescreveDados[2])(fornecedor *, int) = {
        reescreveDadosFornecedores, reescreveDadosFornecedoresBin
    };
    
    int quantidadeFornecedores = (*leDados[modoAbertura])(&fornecedores)+1;
    
    fornecedores = (quantidadeFornecedores == 1)
            ?malloc(sizeof(fornecedor))
            :realloc(fornecedores, sizeof(fornecedor)*quantidadeFornecedores);
    
    fornecedores[quantidadeFornecedores-1].codigo = quantidadeFornecedores-1;
    
    printf("Digite o nome fantasia do novo fornecedor:\n");
    digText(&fornecedores[quantidadeFornecedores-1].nomeFantasia, stdin);
    
    printf("Digite a razao social:\n");
    digText(&fornecedores[quantidadeFornecedores-1].razaoSocial, stdin);
    
    printf("Digite a inscricao estadual:\n");
    validaInsEstadual(&fornecedores[quantidadeFornecedores-1].inscricaoEstadual);
    
    printf("Digite o CNPJ:\n");
    validaCnpjouCpf(&fornecedores[quantidadeFornecedores-1].CNPJ, 14, "543298765432", "6543298765432", 12, 12, "CNPJ digitado invalido!", 0, 0);

    printf("Digite o bairro:\n");
    digText(&fornecedores[quantidadeFornecedores-1].bairro, stdin);
    
    printf("Digite a rua:\n");
    digText(&fornecedores[quantidadeFornecedores-1].rua, stdin);
    
    printf("Digite o numero:\n");
    verificaNumero(&fornecedores[quantidadeFornecedores-1].numero, "%d");
    
    printf("Digite o telefone:\n");
    verificaNumero(&fornecedores[quantidadeFornecedores-1].telefone, "%d");
    
    printf("Digite o email:\n");
    verificaText("@.", &fornecedores[quantidadeFornecedores-1].email, "Email digitado foi invalido! Por favor, digite um email valido!");
    
    fornecedores[quantidadeFornecedores-1].flag = FORNECEDOREXISTE;
    
    (*reescreveDados[modoAbertura])(fornecedores, quantidadeFornecedores);
    limpaMemoriaStringsFornecedor(fornecedores, quantidadeFornecedores);
    fornecedores = limpaMemoria(fornecedores);
    
}
int verificaExisteFornecedores(fornecedor *ptr, int quantidade){
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

fornecedor * encontraFornecedorCNPJ(fornecedor *ptr, int quantidade, char *dado){
    for(int i = 0; i<quantidade; i++){
        if(!strcmp(ptr[i].CNPJ, dado) && ptr[i].flag){
            return (ptr+i);
        }
    }
    return NULL;
}
fornecedor *encontraFornecedorNomeFantasia(fornecedor *ptr, int quantidade, char *dado){
    for(int i = 0; i<quantidade; i++){
        if(!strcmp(ptr[i].nomeFantasia, dado) && ptr[i].flag){
            return (ptr+i);
        }
    }
    return 0;
}
fornecedor *buscaFornecedor(fornecedor *ptr, int quantidade){
    
    char *dado;
    fornecedor *ptrEnc = NULL;
    
    
    do{
        
        digText(&dado, stdin);
        
        ptrEnc = encontraFornecedorNomeFantasia(ptr, quantidade, dado);
        
        if(ptrEnc){
            dado = limpaMemoria(dado);
            return ptrEnc;
        }
        ptrEnc = encontraFornecedorCNPJ(ptr, quantidade, dado);
        if(ptrEnc){
            dado = limpaMemoria(dado);
            return ptrEnc;
        }
        printf("Nenhum fornecedor possui o dado %s, por favor, digite novamente:\n", dado);
        dado = limpaMemoria(dado);
    }while(1);  
}
void editaFornecedor(int modoAbertura){
    
    int escolha;
    fornecedor * fornecedores;
    fornecedor *editar;
    int (*leDados[2])(fornecedor **) = {
        leDadosFornecedores, leDadosFornecedoresBin
    };
    void (*reescreveDados[2])(fornecedor *, int) = {
        reescreveDadosFornecedores, reescreveDadosFornecedoresBin
    };
    
    
    int quantidadeFornecedores = (*leDados[modoAbertura])(&fornecedores);
    
    if(!verificaExisteFornecedores(fornecedores, quantidadeFornecedores)){
        printf("nao existem fornecedores cadastrados ate o momento!\n");
        Sleep(2000);
        return;
    }
    mostraDadosFornecedores(fornecedores, quantidadeFornecedores);
    printf("Digite o nome ou CNPJ do fornecedor que deseja editar:\n");
    editar = buscaFornecedor(fornecedores, quantidadeFornecedores);
    
    printf("digite o dado que deseja editar"); 
    menuGraphics(7, "Opcoes:", "nome fantasia", "razao social", "inscricao estadual", "CNPJ", "endereco", "telefone", "email");
    
    verificaNumero(&escolha, "%d");
    
    while(1){
        switch(escolha){

            case 1:
                printf("digite o novo nome fantasia:\n");
                editar->nomeFantasia = limpaMemoria(editar->nomeFantasia);
                digText(&editar->nomeFantasia, stdin);
                break;

            case 2:
                printf("digite a razao social:\n");
                editar->razaoSocial = limpaMemoria(editar->razaoSocial);
                digText(&editar->razaoSocial, stdin);
                break;

            case 3:
                printf("digite a inscricao estadual:\n");
                editar->inscricaoEstadual = limpaMemoria(editar->inscricaoEstadual);
                validaInsEstadual(&editar->inscricaoEstadual);
                break;

            case 4:
                printf("digite o CNPJ:\n");
                editar->CNPJ = limpaMemoria(editar->CNPJ);
                validaCnpjouCpf(&editar->CNPJ, 14, "543298765432", "6543298765432", 12, 12, "CNPJ digitado invalido!", 0, 0);
                break;

            case 5:
                editar->bairro = limpaMemoria(editar->bairro);
                editar->rua = limpaMemoria(editar->rua);
                printf("digite o bairro:\n");
                digText(&editar->bairro, stdin);
                printf("digite a rua:\n");
                digText(&editar->rua, stdin);
                printf("digite o numero:\n");
                verificaNumero(&editar->numero, "%d");
                break;

            case 6:
                printf("digite o telefone:\n");
                verificaNumero(&editar->telefone, "%d");
                break;

            case 7:
                editar->email = limpaMemoria(editar->email);
                printf("digite o email:\n");
                verificaText("@.", &editar->email, "Email invalido, por favor digite novamente!\n");
                break;
                
            default:
                printf("voce digitou uma opcao invalida, por favor, digite novamente:\n");
                menuGraphics(7, "Opcoes:", "nome fantasia", "razao social", "inscricao estadual", "CNPJ", "endereco", "telefone", "email");
                verificaNumero(&escolha, "%d");
                continue;
                
        }
        break;
    
    }
    
    (*reescreveDados[modoAbertura])(fornecedores, quantidadeFornecedores);
    limpaMemoriaStringsFornecedor(fornecedores, quantidadeFornecedores);
    fornecedores = limpaMemoria(fornecedores);
    editar = NULL;
}
void apagaFornecedor(int modoAbertura){
    int escolha;
    fornecedor * fornecedores;
    fornecedor *apagar;
    int (*leDados[2])(fornecedor **) = {
        leDadosFornecedores, leDadosFornecedoresBin
    };
    void (*reescreveDados[2])(fornecedor *, int) = {
        reescreveDadosFornecedores, reescreveDadosFornecedoresBin
    };
    
    
    int quantidadeFornecedores = (*leDados[modoAbertura])(&fornecedores);
    
    if(!verificaExisteFornecedores(fornecedores, quantidadeFornecedores)){
        printf("nao existem fornecedores cadastrados ate o momento!\n");
        Sleep(2000);
        return;
    }
    mostraDadosFornecedores(fornecedores, quantidadeFornecedores);
    printf("Digite o nome ou CNPJ do fornecedor que deseja apagar:\n");
    apagar = buscaFornecedor(fornecedores, quantidadeFornecedores);
    
    apagar->flag = FORNECEDORNAOEXISTE;
    
    (*reescreveDados[modoAbertura])(fornecedores, quantidadeFornecedores);
    limpaMemoriaStringsFornecedor(fornecedores, quantidadeFornecedores);
    fornecedores = limpaMemoria(fornecedores);
    apagar = NULL;
}