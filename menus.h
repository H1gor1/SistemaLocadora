#ifndef MENUS_H
#define MENUS_H

int menuPrincipal(int *modo);
int MenuAdm(int modo);
int MenuClientes(int modo);
int MenuFilmes(int modo);
int MenuFuncionarios(int modo);
int MenuCategoria(int modo);
void menuGraphics(int quantidadeArgumentos, char *frasePrincipal, ...);
void menuGraphicsComSeta(int quantidadeArgumentos, char *frasePrincipal , int linha, char **, int espacamentoMaior);
int menuFornecedores(int modo);
void menuTroca(int *modo);
void menuContas(int modoArm);
void menuCaixa(int modoArm);
int escolheMenu(char *mensagem, int quantidade,int espacamento, ...);
void menuContasPagar(int modoArm);
#endif