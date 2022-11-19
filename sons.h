
#ifndef SONS_H
#define SONS_H

typedef struct alturaSom{
    int frequencia;
    int duracao;
}alturaSom;
typedef struct sons{
    alturaSom SonsDeErro;
    alturaSom SonsDeConfirmacao;
}sons;

extern sons ConfiguracoeDeSons;


void leDadosSons(sons *ptr);
void leDadosSonsBin(sons *ptr);
void reescreveDadosSons(sons *ptr);
void reescreveDadosSonsBin(sons *ptr);
int selecionaValorDeSom(char *stringConf, int valorAt);
void editaDadosSons(int modoArm);
#endif
