#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include "deck.h"

int main() {
    int cartas[TAM_DEQUE][2];
    MAO_T c1, c2, c3, c4, c5;
    int pontos;

    printf("Criando baralho padrão.\n");
    criaBaralho(cartas);
    imprimeBaralhoVetor(cartas);

    printf("\nEmbaralhando com semente %d.\n", SEMENTE);
    embaralha(cartas, SEMENTE);
    imprimeBaralhoVetor(cartas);

    printf("\nTeste de Royal straight flush.\n");
    c1.valor = 1; c1.naipe = 0; c1.prox = &c2;
    c2.valor = 10; c2.naipe = 0; c2.prox = &c3;
    c3.valor = 11; c3.naipe = 0; c3.prox = &c4;
    c4.valor = 12; c4.naipe = 0; c4.prox = &c5;
    c5.valor = 13; c5.naipe = 0; c5.prox = NULL;
    pontos = contaPontos(&c1);
    printf("%d pontos.\n", pontos);

    printf("\nTeste de straight flush.\n");
    c1.valor = 9; c1.naipe = 0; c1.prox = &c2;
    c2.valor = 10; c2.naipe = 0; c2.prox = &c3;
    c3.valor = 11; c3.naipe = 0; c3.prox = &c4;
    c4.valor = 12; c4.naipe = 0; c4.prox = &c5;
    c5.valor = 13; c5.naipe = 0; c5.prox = NULL;
    pontos = contaPontos(&c1);
    printf("%d pontos.\n", pontos);

    printf("\nTeste de quadra.\n");
    c1.valor = 1; c1.naipe = 0; c1.prox = &c2;
    c2.valor = 7; c2.naipe = 1; c2.prox = &c3;
    c3.valor = 7; c3.naipe = 2; c3.prox = &c4;
    c4.valor = 7; c4.naipe = 3; c4.prox = &c5;
    c5.valor = 7; c5.naipe = 0; c5.prox = NULL;
    pontos = contaPontos(&c1);
    printf("%d pontos.\n", pontos);

    printf("\nTeste de full house.\n");
    c1.valor = 1; c1.naipe = 0; c1.prox = &c2;
    c2.valor = 1; c2.naipe = 1; c2.prox = &c3;
    c3.valor = 13; c3.naipe = 2; c3.prox = &c4;
    c4.valor = 13; c4.naipe = 3; c4.prox = &c5;
    c5.valor = 13; c5.naipe = 0; c5.prox = NULL;
    pontos = contaPontos(&c1);
    printf("%d pontos.\n", pontos);

    printf("\nTeste de flush.\n");
    c1.valor = 8; c1.naipe = 0; c1.prox = &c2;
    c2.valor = 10; c2.naipe = 0; c2.prox = &c3;
    c3.valor = 11; c3.naipe = 0; c3.prox = &c4;
    c4.valor = 12; c4.naipe = 0; c4.prox = &c5;
    c5.valor = 13; c5.naipe = 0; c5.prox = NULL;
    pontos = contaPontos(&c1);
    printf("%d pontos.\n", pontos);

    printf("\nTeste de sequencia.\n");
    c1.valor = 1; c1.naipe = 1; c1.prox = &c2;
    c2.valor = 2; c2.naipe = 0; c2.prox = &c3;
    c3.valor = 3; c3.naipe = 0; c3.prox = &c4;
    c4.valor = 4; c4.naipe = 0; c4.prox = &c5;
    c5.valor = 5; c5.naipe = 0; c5.prox = NULL;
    pontos = contaPontos(&c1);
    printf("%d pontos.\n", pontos);

    printf("\nTeste de trinca.\n");
    c1.valor = 1; c1.naipe = 0; c1.prox = &c2;
    c2.valor = 2; c2.naipe = 1; c2.prox = &c3;
    c3.valor = 2; c3.naipe = 2; c3.prox = &c4;
    c4.valor = 2; c4.naipe = 3; c4.prox = &c5;
    c5.valor = 5; c5.naipe = 0; c5.prox = NULL;
    pontos = contaPontos(&c1);
    printf("%d pontos.\n", pontos);

    printf("\nTeste de dois pares.\n");
    c1.valor = 1; c1.naipe = 0; c1.prox = &c2;
    c2.valor = 2; c2.naipe = 1; c2.prox = &c3;
    c3.valor = 2; c3.naipe = 2; c3.prox = &c4;
    c4.valor = 5; c4.naipe = 3; c4.prox = &c5;
    c5.valor = 5; c5.naipe = 0; c5.prox = NULL;
    pontos = contaPontos(&c1);
    printf("%d pontos.\n", pontos);

    printf("\nTeste de par.\n");
    c1.valor = 1; c1.naipe = 0; c1.prox = &c2;
    c2.valor = 2; c2.naipe = 1; c2.prox = &c3;
    c3.valor = 3; c3.naipe = 2; c3.prox = &c4;
    c4.valor = 3; c4.naipe = 3; c4.prox = &c5;
    c5.valor = 5; c5.naipe = 0; c5.prox = NULL;
    pontos = contaPontos(&c1);
    printf("%d pontos.\n", pontos);

    printf("\nTeste de mão vazia.\n");
    c1.valor = 1; c1.naipe = 0; c1.prox = &c2;
    c2.valor = 2; c2.naipe = 1; c2.prox = &c3;
    c3.valor = 3; c3.naipe = 2; c3.prox = &c4;
    c4.valor = 4; c4.naipe = 3; c4.prox = &c5;
    c5.valor = 6; c5.naipe = 0; c5.prox = NULL;
    pontos = contaPontos(&c1);
    printf("%d pontos.\n", pontos);

    printf("\nFim do teste.\n");

    exit(0);
}