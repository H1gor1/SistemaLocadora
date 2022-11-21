


#ifndef MENUS_H
#define MENUS_H

typedef struct AlturaSons{
    int frequencia, duracao;
}AlturaSons;
typedef struct sons{
    AlturaSons sonsDeConfirmacao, sonsDeErro;
    int modoSilencioso;
}sons;
void disparaSom(char *frase, int erro);
void leDadosDeSons(int manterValor);
void editaDadosSons(int modoArm);
void reescreveDadosSons(sons *ptr);
void reescreveDadosSonsBin(sons *ptr);
void leDadosSons(sons *ptr, int manterValor);
void leDadosSonsBin(sons *ptr, int manterValor);
void editaDadosSons(int modoArm);
int selecionaValorDeSom(char *stringConf, int valorAt);
int menuPrincipal(int *modo);
int MenuAdm(int modo);
int MenuClientes(int modo);
int MenuFilmes(int modo);
int MenuFuncionarios(int modo);
int MenuCategoria(int modo);
void menuGraphics(int quantidadeArgumentos, char *frasePrincipal, ...);
void menuGraphicsComSeta(int quantidadeArgumentos, char *frasePrincipal , int linha, int linhaAnterior, int chamadas, char **frases, int espacamentoMaior);
int menuFornecedores(int modo);
void menuTroca(int *modo);
void menuContas(int modoArm);
void menuCaixa(int modoArm);
int escolheMenu(char *mensagem, int quantidade,int espacamento, ...);
void menuContasPagar(int modoArm);
void menuConfiguracoes(int *modoArm);


#endif