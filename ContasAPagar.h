#ifndef UNTITLED_CONTASAPAGAR_H
#define UNTITLED_CONTASAPAGAR_H

#include <time.h>

typedef struct entrada entrada;

typedef struct contaApag{
    long int codigoCompra;
    int modoPagamento;
    int parcelas;
    float valorParc;
    float entrada;
    struct tm dataAluga;
}contaApag;

#endif //UNTITLED_CONTASAPAGAR_H
