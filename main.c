
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
#include "EntradaDeFilmes.h"
#include "ContasAPagar.h"
#include "ControleFilmes.h"
#include "menus.h"
#include "ExportacaoXML.h"





int main(){
    
    
    int modoArm;
    leDadosDeSons(0);
    VerificaExisteArquivoLocadora();


    verificaModoArmazenamento(&modoArm);

    
    
   
    
    menuPrincipal(&modoArm);

    return 0;
 
} 
