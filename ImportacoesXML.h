#include "ControleDeDadosLocadora.h"

#ifndef UNTITLED_IMPORTACOESXML_H
#define UNTITLED_IMPORTACOESXML_H

void importaDadosLocadora(int modoArm);
int leArquivoOriginal(char *nomeArq, int (*f)(void **), void *ptr);
void importaDadosSons(int modoArm);
void pegaCaminho(char *discricaoCaminho, char **ptrGuardarCaminho);
void importaDadosClientes(int modoArm);
void importaDadosFuncs(int modoArm);
void importaDadosFornecedores(int modoArm);
void importaCategoria(int modoArm);
void importaDadosFilmes(int modoArm);
void importaDadosCompras(int modoArm);
void importaDadosLancamentos(int modoArm);
void importaDadosSons(int modoArm);
void importaLancamentosAprazo(int modoArm);
void importaLancamentosEntrada(int modoArm);
void importaEntradaAprazo(int modoArm);
#endif //UNTITLED_IMPORTACOESXML_H
