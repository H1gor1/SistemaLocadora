#include "ControleDeDadosLocadora.h"

#ifndef UNTITLED_IMPORTACOESXML_H
#define UNTITLED_IMPORTACOESXML_H

 void exportaLocadora(int modoArm);
int leArquivoOriginal(char *nomeArq, int (*f)(void **), void *ptr);
void exportaDadosSons(int modoArm);
void pegaCaminho(char *discricaoCaminho, char **ptrGuardarCaminho);
void exportaDadosClientes(int modoArm);
void exportaDadosFuncs(int modoArm);
void exportaDadosFornecedores(int modoArm);
void exportaCategoria(int modoArm);
void exportaDadosFilmes(int modoArm);
void exportaDadosCompras(int modoArm);
void exportaDadosLancamentos(int modoArm);
void exportaDadosSons(int modoArm);
void exportaLancamentosAprazo(int modoArm);
void exportaLancamentosEntrada(int modoArm);
void exportaEntradaAprazo(int modoArm);
#endif //UNTITLED_IMPORTACOESXML_H
