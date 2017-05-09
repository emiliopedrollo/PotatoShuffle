#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include "deck.h"

void test();

int main() {

    CARD_T **cards;
    cards = malloc(DECK_SIZE * sizeof (CARD_T));

    printf("Criando baralho padrão.\n");
    createDeck(cards);
    printDeck(cards);

    printf("\nEmbaralhando com semente %d.\n", SEED);
    shuffle(cards, SEED);
    printDeck(cards);

    exit(0);
}


// STANDALONE

//
//MAO_T *criaListaCartas(){
//    int cartas[52][2], i, seed=1;
//    MAO_T *new, *listaCartas=NULL;
//    criaBaralho(cartas);
///*  embaralha(cartas, seed);*/
//    for(i=0; i<25; i++){
//        new = malloc(sizeof(MAO_T));
//        new->valor=cartas[i][0];
//        new->naipe=cartas[i][1];
//        new->prox=listaCartas;
//        listaCartas=new;
//    }
//    return listaCartas;
//}
//
//int verificaPontos(MAO_T *cartas){
//    int i,j,pontos=0;
//    MAO_T *pt_carta, *mao, *aux;
//    pt_carta=cartas;
//    /*maos devem estar ordenadas por valor, ascendente*/
//    for(i=0; i<5; i++){
//        mao=pt_carta;
//        for(j=0; j<4; j++)
//            pt_carta=pt_carta->prox;
//        aux=pt_carta->prox;
//        pt_carta->prox=NULL;
//        pontos+=contaPontos(mao);
//        pt_carta->prox=aux;
//        pt_carta=pt_carta->prox;
//    }
//    return pontos;
//}
//
//MAO_T *jogarMaosPoquer(MAO_T *cartas ){
//    /* seu codigo aqui */
//
//    return cartas;
//}
//
//
//int main(void)
//{
//    MAO_T *cartas;
//    int pontos;
//
//    cartas=criaListaCartas();
//    imprimeListaCartas(cartas);
//    cartas=jogarMaosPoquer(cartas);
//    pontos = verificaPontos(cartas);
//    printf("Pontos: %d\n", pontos);
//    exit(0);
//}
// FIM STANDALONE

void test() {
    int score;
    HAND_T c1, c2, c3, c4, c5;
    printf("\nTeste de Royal straight flush.\n");
    c1.card.value = 1;
    c1.card.suit = DIAMOND;
    c1.next = &c2;
    c2.card.value = 10;
    c2.card.suit = DIAMOND;
    c2.next = &c3;
    c3.card.value = 11;
    c3.card.suit = DIAMOND;
    c3.next = &c4;
    c4.card.value = 12;
    c4.card.suit = DIAMOND;
    c4.next = &c5;
    c5.card.value = 13;
    c5.card.suit = DIAMOND;
    c5.next = NULL;
    score = countPoints(&c1);
    printf("%d score.\n", score);

    printf("\nTeste de straight flush.\n");
    c1.card.value = 9;
    c1.card.suit = DIAMOND;
    c1.next = &c2;
    c2.card.value = 10;
    c2.card.suit = DIAMOND;
    c2.next = &c3;
    c3.card.value = 11;
    c3.card.suit = DIAMOND;
    c3.next = &c4;
    c4.card.value = 12;
    c4.card.suit = DIAMOND;
    c4.next = &c5;
    c5.card.value = 13;
    c5.card.suit = DIAMOND;
    c5.next = NULL;
    score = countPoints(&c1);
    printf("%d score.\n", score);

    printf("\nTeste de quadra.\n");
    c1.card.value = 1;
    c1.card.suit = DIAMOND;
    c1.next = &c2;
    c2.card.value = 7;
    c2.card.suit = SPADE;
    c2.next = &c3;
    c3.card.value = 7;
    c3.card.suit = HEART;
    c3.next = &c4;
    c4.card.value = 7;
    c4.card.suit = CLUB;
    c4.next = &c5;
    c5.card.value = 7;
    c5.card.suit = DIAMOND;
    c5.next = NULL;
    score = countPoints(&c1);
    printf("%d score.\n", score);

    printf("\nTeste de full house.\n");
    c1.card.value = 1;
    c1.card.suit = DIAMOND;
    c1.next = &c2;
    c2.card.value = 1;
    c2.card.suit = SPADE;
    c2.next = &c3;
    c3.card.value = 13;
    c3.card.suit = HEART;
    c3.next = &c4;
    c4.card.value = 13;
    c4.card.suit = CLUB;
    c4.next = &c5;
    c5.card.value = 13;
    c5.card.suit = DIAMOND;
    c5.next = NULL;
    score = countPoints(&c1);
    printf("%d score.\n", score);

    printf("\nTeste de flush.\n");
    c1.card.value = 8;
    c1.card.suit = DIAMOND;
    c1.next = &c2;
    c2.card.value = 10;
    c2.card.suit = DIAMOND;
    c2.next = &c3;
    c3.card.value = 11;
    c3.card.suit = DIAMOND;
    c3.next = &c4;
    c4.card.value = 12;
    c4.card.suit = DIAMOND;
    c4.next = &c5;
    c5.card.value = 13;
    c5.card.suit = DIAMOND;
    c5.next = NULL;
    score = countPoints(&c1);
    printf("%d score.\n", score);

    printf("\nTeste de sequencia.\n");
    c1.card.value = 1;
    c1.card.suit = SPADE;
    c1.next = &c2;
    c2.card.value = 2;
    c2.card.suit = DIAMOND;
    c2.next = &c3;
    c3.card.value = 3;
    c3.card.suit = DIAMOND;
    c3.next = &c4;
    c4.card.value = 4;
    c4.card.suit = DIAMOND;
    c4.next = &c5;
    c5.card.value = 5;
    c5.card.suit = DIAMOND;
    c5.next = NULL;
    score = countPoints(&c1);
    printf("%d score.\n", score);

    printf("\nTeste de trinca.\n");
    c1.card.value = 1;
    c1.card.suit = DIAMOND;
    c1.next = &c2;
    c2.card.value = 2;
    c2.card.suit = SPADE;
    c2.next = &c3;
    c3.card.value = 2;
    c3.card.suit = HEART;
    c3.next = &c4;
    c4.card.value = 2;
    c4.card.suit = CLUB;
    c4.next = &c5;
    c5.card.value = 5;
    c5.card.suit = DIAMOND;
    c5.next = NULL;
    score = countPoints(&c1);
    printf("%d score.\n", score);

    printf("\nTeste de dois pares.\n");
    c1.card.value = 1;
    c1.card.suit = DIAMOND;
    c1.next = &c2;
    c2.card.value = 2;
    c2.card.suit = SPADE;
    c2.next = &c3;
    c3.card.value = 2;
    c3.card.suit = HEART;
    c3.next = &c4;
    c4.card.value = 5;
    c4.card.suit = CLUB;
    c4.next = &c5;
    c5.card.value = 5;
    c5.card.suit = DIAMOND;
    c5.next = NULL;
    score = countPoints(&c1);
    printf("%d score.\n", score);

    printf("\nTeste de par.\n");
    c1.card.value = 1;
    c1.card.suit = DIAMOND;
    c1.next = &c2;
    c2.card.value = 2;
    c2.card.suit = SPADE;
    c2.next = &c3;
    c3.card.value = 3;
    c3.card.suit = HEART;
    c3.next = &c4;
    c4.card.value = 3;
    c4.card.suit = CLUB;
    c4.next = &c5;
    c5.card.value = 5;
    c5.card.suit = DIAMOND;
    c5.next = NULL;
    score = countPoints(&c1);
    printf("%d score.\n", score);

    printf("\nTeste de mão vazia.\n");
    c1.card.value = 1;
    c1.card.suit = DIAMOND;
    c1.next = &c2;
    c2.card.value = 2;
    c2.card.suit = SPADE;
    c2.next = &c3;
    c3.card.value = 3;
    c3.card.suit = HEART;
    c3.next = &c4;
    c4.card.value = 4;
    c4.card.suit = CLUB;
    c4.next = &c5;
    c5.card.value = 6;
    c5.card.suit = DIAMOND;
    c5.next = NULL;
    score = countPoints(&c1);
    printf("%d score.\n", score);

    printf("\nFim do teste.\n");
}