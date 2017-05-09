//
// Created by ncc on 09/05/17.
//

#ifndef POTATOSHUFFLE_DECK_H

#define TAM_DEQUE	52
#define SEMENTE		1
#define VALOR		0
#define	NAIPE		1

typedef enum {OUROS, ESPADAS, COPAS, BASTOS} NAIPES_T;

/* tipo a ser usado para a fila correspondente a mão do usuário */
typedef struct n {
    int valor;
    int naipe;
    struct n *prox;
} MAO_T;

void imprimeBaralhoVetor(int cartas[][2]);
void criaBaralho(int cartas[][2]);
void embaralha(int cartas[][2], unsigned int semente);
int contaPontos(MAO_T *mao);


#define POTATOSHUFFLE_DECK_H

#endif //POTATOSHUFFLE_DECK_H
