#include <stdio.h>
#include <stdlib.h>
#include "deck.h"

void imprimeBaralhoVetor(int cartas[][2]){
    int i;
    for(i=0; i<TAM_DEQUE; i++)
        printf("[%i %i]", cartas[i][0], cartas[i][1]);
    return;
}

/* Cria um baralho padrao de 52 cartas em um vetor passado por referencia */
void criaBaralho(int cartas[][2]){
    int i, j=0;
    NAIPES_T n;
    /* geração do baralho ordenado */
    for(n=OUROS; n<=BASTOS; n++){
        for(i=1; i<=13; i++){
            cartas[j][VALOR] = i;
            cartas[j][NAIPE] = n;
            j++;
        }
    }
    return;
}

/* Embaralha as 52 cartas constantes em um vetor passado por referencia */
void embaralha(int cartas[][2], unsigned int semente){
    int i, j, aux_v, aux_n;
    /* EMBARALHAMENTO */
    srand(semente);
    for(i=0; i<TAM_DEQUE; i++){
        j = rand() % TAM_DEQUE;
        aux_v = cartas[i][VALOR];
        aux_n = cartas[i][NAIPE];
        cartas[i][VALOR] = cartas[j][VALOR];
        cartas[i][NAIPE] = cartas[j][NAIPE];
        cartas[j][VALOR] = aux_v;
        cartas[j][NAIPE] = aux_n;
    }
    return;
}

/* Conta os pontos de uma mao de 5 cartas, de acordo com as regras de pontuacao. */
/* A mao deve vir ordenada, por valor da carta, em ordem ascendente. */
/* (IMPORTANTE: para fins desta ordenacao, o As vale 1) */
int contaPontos(MAO_T *mao){
    MAO_T *ptrAux;
    int cont=0, flush=0, straight=0, royalStraight=0;
    int cont2=0;

    /* verifica Flush */
    for(ptrAux = mao->prox; ptrAux != NULL; ptrAux = ptrAux->prox){
        if(ptrAux->naipe == mao->naipe)
            cont ++;
    }
    if(cont == 4)
        flush = 1; /* é um flush */

    /* verifica Royal Straight */
    ptrAux = mao;
    if(ptrAux->valor == 1 && ptrAux->prox->valor == 10){
        /* somente pode ser royal se a 1a carta é ás e 2a é 10 */
        for(ptrAux = mao->prox, cont=0; ptrAux->prox != NULL; ptrAux = ptrAux->prox){
            if(ptrAux->valor == (ptrAux->prox->valor - 1))
                cont ++;
        }
        if(cont == 3)
            royalStraight = 1; /* é um royal straight */
    }

    if(flush && royalStraight) /* mão é um royal straight flush */
        return 1000;

    /* verifica Straight simples */
    for(ptrAux = mao, cont=0; ptrAux->prox != NULL; ptrAux = ptrAux->prox){
        if(ptrAux->valor == (ptrAux->prox->valor - 1))
            cont ++;
    }
    if(cont == 4)
        straight = 1; /* é um straight simples */

    if(flush && straight) /* mão é um straight flush */
        return 750;

    if(royalStraight || straight) /* mão é um straight */
        return 150;

    if(flush) /* mão é flush */
        return 200;

    /* verifica pares, trincas e quadras */
    cont = cont2 = 0;
    ptrAux = mao;
    while((ptrAux->prox != NULL) && (ptrAux->valor != ptrAux->prox->valor))
        ptrAux = ptrAux->prox;
    while((ptrAux->prox != NULL) && (ptrAux->valor == ptrAux->prox->valor)){
        cont++;
        ptrAux = ptrAux->prox;
    }
    while((ptrAux->prox != NULL) && (ptrAux->valor != ptrAux->prox->valor))
        ptrAux = ptrAux->prox;
    while((ptrAux->prox != NULL) && (ptrAux->valor == ptrAux->prox->valor)){
        cont2++;
        ptrAux = ptrAux->prox;
    }
    /* se foi encontrada algum par, trinca, four ou full house, é necessário */
    /* fazer cont e cont2 conter numero correto de cartas iguais encontradas */
    if(cont > 0) /* normalizando valor do primeiro contador */
        cont++;
    if(cont2 > 0) /* normalizando valor do segundo contador */
        cont2++;
    /* após normalizaçao, cont e cont2 podem ser usados para verificacao */
    switch(cont+cont2){
        case 5: /* mão é um full house */
            return 250;
        case 4: /* four ou dois pares */
            if(cont == 4 || cont2 == 4)
                return 500;
            else
                return 50;
        case 3: /* trinca */
            return 100;
        case 2: /* par simples */
            return 20;
    }
    return 0;
}

