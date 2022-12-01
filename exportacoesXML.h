//
// Created by vitor on 27/11/2022.
//

#ifndef UNTITLED_EXPORTACOESXML_H
#include <stdio.h>
#define UNTITLED_EXPORTACOESXML_H

void avancaAtePalavra(char *palavra, FILE *f);
void exportaLocadora(int modoArm);
FILE *digitaCaminhoExport(char *discricaoCaminhoo, char **diretorio);
void exportaFuncionarios(int modoArm);
void exportaConfiguracoesSons(int modoArm);
void exportaFornecedores(int modoArm);
void exportaClientes(int modoArm);
void exportaCategoria(int modoArm);
#endif //UNTITLED_EXPORTACOESXML_H
