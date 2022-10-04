#include<stdio.h> 
#include<stdlib.h>
#include<string.h>
#include<errno.h> 
#include "ManipulacoesDeTexto.h"
#include "ControleDeDadosLocadora.h"
#include "ControleDeNumeros.h"
#include "FuncUtilitarias.h"
#include "ControleClientes.h"
#include "controleFuncionarios.h"
#include "ControleFilmes.h"
#include "menus.h"



int main(){
    
    
    int modoArm;
    
    VerificaExisteArquivoLocadora();
    verificaModoArmazenamento(&modoArm);
    
    
    /*
    // para editar dados da locadora, precisa-se chamar a funcao operacaoComLocadora, com os parametros sobre qual arquivo ela deve editar, txt ou bin, e passar 
    // o ponteiro da funcao a qual e responsavel por editar os dados em memoria, e depois passar a funcao que rescreve os dados em binario se for este o modo usado
    // atualmente, ou txt
     
    
    printf("digite qual dado voce deseja editar sobre a locadora\n");
    //editaDadosLocadora(modoArm);
    
    
    //cadastraCliente(modoArm);
     
    //editaCliente(modoArm);
    
   // apagaCliente(modoArm);
    
    
    //cadastraFuncionario(modoArm);
    
    //editaFuncionario(modoArm);
    //apagaFuncionario(modoArm);
     
     */
       
    
    menuPrincipal(&modoArm);
    //trocaModoArmazenamento(&modoArm);
    return 0;
 
} 
