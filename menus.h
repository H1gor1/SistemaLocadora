#ifndef MENUS_H
#define MENUS_H

int menuPrincipal(int *modo);
int MenuAdm(int modo);
int MenuClientes(int modo);
int MenuFilmes(int modo);
int MenuFuncionarios(int modo);
int MenuCategoria(int modo);
void menuGraphics(int quantidadeArgumentos, char *frasePrincipal , ...);
int menuFornecedores(int modo);
void menuTroca(int *modo);
void menuContas(int modoArm);
void menuCaixa(int modoArm);

#endif