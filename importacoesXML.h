//
// Created by vitor on 27/11/2022.
//

#ifndef UNTITLED_EXPORTACOESXML_H
#include <stdio.h>
#define UNTITLED_EXPORTACOESXML_H

void avancaAtePalavra(char *palavra, FILE *f);
void importaLocadora(int modoArm);
FILE *digitaCaminhoImport(char *discricaoCaminhoo, char **diretorio);
void importaFuncionarios(int modoArm);
void importaConfiguracoesSons(int modoArm);
void importaFornecedores(int modoArm);
void importaClientes(int modoArm);
void importaCategoria(int modoArm);
void importaFilmes(int modoArm);
void importaDevolucoes(int modoArm);
void importaLancamentosCaixa(int modoArm);
void importaCompraAprazo(int modoArm);
#endif //UNTITLED_EXPORTACOESXML_H
