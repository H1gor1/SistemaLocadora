#include "ControleDeDadosLocadora.h"

#ifndef UNTITLED_IMPORTACOESXML_H
#define UNTITLED_IMPORTACOESXML_H

void importaDadosLocadora(int modoArm);
int leArquivoOriginal(char *nomeArq, int (*f)(void **), void *ptr);
void importaDadosSons(int modoArm);
void pegaCaminho(char *discricaoCaminho, char **ptrGuardarCaminho);
void importaDadosClientes(int modoArm);
void importaDadosFuncs(int modoArm);
#endif //UNTITLED_IMPORTACOESXML_H
