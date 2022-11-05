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
    
    
   
    
    menuPrincipal(&modoArm);
   
    return 0;
 
} 
