#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>
#include <stdbool.h>
#include "deck.h"
#include "utils.h"

void test();
HAND_T *createHands();
int verifyPoints(HAND_T *cards);
HAND_T *orderHands(HAND_T *hand);
void checkForRoyalStraightFlash(HAND_T **hand, HAND_T **best_order);
void checkForStraightFlush(HAND_T **hand, HAND_T **best_order);
void checkForFourOfAKind(HAND_T **hand, HAND_T **best_order);
void checkForFullHouse(HAND_T **hand, HAND_T **best_order);
void checkForFlush(HAND_T **hand, HAND_T **best_order);
void checkForStraight(HAND_T **hand, HAND_T **best_order);

int main() {

/*    CARD_T **cards;
//    cards = malloc(DECK_SIZE * sizeof (CARD_T));
//
//    printf("Criando baralho padrão.\n");
//    createDeck(cards);
//    printDeck(cards);
//
//    printf("\nEmbaralhando com semente %d.\n", SEED);
//    shuffle(cards, SEED);
//    printDeck(cards);  */


    HAND_T *cards;
    int points;

    cards=createHands();
    /*printHands(cards);*/
    printf("\n");
    cards= orderHands(cards);
    points = verifyPoints(cards);
    printf("Pontos: %d\n", points);


    exit(0);
}

HAND_T *createHands(){
    int i;
    unsigned int seed=1;
    srand((unsigned int) time(NULL));
    CARD_T **cards;
    HAND_T *new, *list=NULL;
    cards = malloc(DECK_SIZE * sizeof(cards));
    createDeck(cards);
    shuffle(cards, 1028929438); /*RSF*/
    /*shuffle(cards, (unsigned int) rand());*/
    for(i=0; i<25; i++){
        new = malloc(sizeof(HAND_T));
        new->card.value=cards[i]->value;
        new->card.suit=cards[i]->suit;
        new->next=list;
        list=new;
    }
    return list;
}

int verifyPoints(HAND_T *cards){
    int i,j,points=0,handPoints=0;
    char* handName;
    handName = malloc(sizeof(char)* 30);
    HAND_T *pt_card, *hand, *aux;
    pt_card=cards;
    /*maos devem estar ordenadas por valor, ascendente*/
    for(i=0; i<5; i++){
        hand=pt_card;
        for(j=0; j<4; j++)
            pt_card=pt_card->next;
        aux=pt_card->next;
        pt_card->next=NULL;
        handPoints=countPointsWithDescription(hand,handName);
        points+=handPoints;
        printHands(hand);
        printf(" %s (%d points)\n",handName,handPoints);
        printf("\n");
        pt_card->next=aux;
        pt_card=pt_card->next;
    }
    return points;
}

HAND_T *orderHands(HAND_T *hand){
    /* seu codigo aqui */

    HAND_T *best_order = NULL;

    checkForRoyalStraightFlash(&hand, &best_order);
    checkForStraightFlush(&hand, &best_order);
    checkForFourOfAKind(&hand, &best_order);
    checkForFullHouse(&hand, &best_order);
    checkForFlush(&hand, &best_order);
    checkForStraight(&hand, &best_order);

    addHandToHand(&best_order,hand);
    return best_order;
}

void checkForStraight(HAND_T **hand, HAND_T **best_order) {
    HAND_T *aux, *sequence;

    aux = sortHand(copyHand((*hand)), true);
    while (searchForSequence(&aux,&sequence)){
        subtractHandFromHand(&aux,sequence);
        subtractHandFromHand(hand, sequence);
        addHandToHand(best_order, sequence);
    }
}

void checkForFlush(HAND_T **hand, HAND_T **best_order) {

    int i,j;
    bool success;
    HAND_T *aux, *sequence, **addr = NULL;
    HAND_T *diamond, *spade, *heart, *club;
    divideBySuit(*hand,&diamond,&spade,&heart,&club);

    for(i=0;i<4;i++) {
        if (i == 0) addr = &diamond;
        else if (i == 1) addr = &spade;
        else if (i == 2) addr = &heart;
        else if (i == 3) addr = &club;

        for (j = countCardsInHand(*addr) / 5; j> 0 ; j--){

            aux = getFirsts(*addr,&success,5);

            subtractHandFromHand(addr, aux);
            subtractHandFromHand(hand, aux);
            addHandToHand(best_order, sortHand(aux,true));

        }

        aux = sortHand(copyHand(*addr), true);
        while (searchForSequence(&aux, &sequence)) {
        }
    }
}

void checkForFullHouse(HAND_T **hand, HAND_T **best_order) {
    int i,j,k,l;
    HAND_T *aux = NULL, *aux2, *aux3;
    HAND_T *iterable;
    bool success,foundExtra;


    for (i=1;i<14;i++){
        k = 0;
        iterable = *hand;
        aux = NULL;
        while (iterable != NULL){
            if (iterable->card.value == i) {
                addHandToHand(&aux,getFirsts(iterable,&success,1));
                k++;
            }
            iterable = iterable->next;
        }
        for(j=k/3;j>0;j--){
            foundExtra = false;
            aux2 = getFirsts(aux,&success,3);
            for(l=1;l<14;l++){
                if (l==i)continue;
                k=0;
                aux3 = NULL;
                iterable = *hand;
                while (iterable != NULL){
                    if (iterable->card.value == l){
                        addHandToHand(&aux3,getFirsts(iterable,&success,1));
                        if (++k == 2){
                            addHandToHand(&aux2,aux3);
                            foundExtra = true;
                            break;
                        }
                    }
                    iterable = iterable->next;
                }
                if (foundExtra) break;
            }
            if (foundExtra){
                subtractHandFromHand(&aux,aux2);
                subtractHandFromHand(hand, aux2);
                addHandToHand(best_order, sortHand(aux2,true));
            }

        }
    }
}

void checkForFourOfAKind(HAND_T **hand, HAND_T **best_order) {
    int i,j,k;
    HAND_T *aux = NULL, *aux2;
    HAND_T *iterable;
    bool success,foundExtra;


    for (i=1;i<14;i++){
        k = 0;
        iterable = *hand;
        aux = NULL;
        while (iterable != NULL){
            if (iterable->card.value == i) {
                addHandToHand(&aux,getFirsts(iterable,&success,1));
                k++;
            }
            iterable = iterable->next;
        }
        for(j=k/4;j>0;j--){
            iterable = *hand;
            foundExtra = false;
            aux2 = getFirsts(aux,&success,4);
            while (iterable != NULL){
                if (iterable->card.value != i){
                    addHandToHand(&aux2,getFirsts(iterable,&success,1));
                    foundExtra = true;
                    break;
                }
                iterable = iterable->next;
            }
            if (foundExtra){
                subtractHandFromHand(&aux,aux2);
                subtractHandFromHand(hand, aux2);
                addHandToHand(best_order, sortHand(aux2,true));
            }

        }
    }

}

void checkForStraightFlush(HAND_T **hand, HAND_T **best_order) {

    int i;
    HAND_T *aux, *sequence, **addr = NULL;
    HAND_T *diamond, *spade, *heart, *club;


    divideBySuit(*hand,&diamond,&spade,&heart,&club);

    for(i=0;i<4;i++) {
        if (i == 0) addr = &diamond;
        else if (i == 1) addr = &spade;
        else if (i == 2) addr = &heart;
        else if (i == 3) addr = &club;

        aux = sortHand(copyHand(*addr), true);
        while (searchForSequence(&aux, &sequence)) {
            subtractHandFromHand(&aux, sequence);
            subtractHandFromHand(hand, sequence);
            addHandToHand(best_order, sequence);
        }
    }
}

void checkForRoyalStraightFlash(HAND_T **hand, HAND_T **best_order) {

    int i;
    HAND_T *aux, **addr = NULL;
    bool success;
    HAND_T *diamond, *spade, *heart, *club;


    divideBySuit(*hand,&diamond,&spade,&heart,&club);

    for(i=0;i<4;i++){
        if (i==0) addr = &diamond;
        else if (i==1) addr = &spade;
        else if (i==2) addr = &heart;
        else if (i==3) addr = &club;

        aux = sortHand(getFirsts(sortHandDec(*addr, false), &success, 5), true);
        if (success) {

            if (countPoints(aux) == SCORE_ROYAL_STRAIGHT_FLUSH) {
                subtractHandFromHand(hand, aux);
                addHandToHand(best_order, aux);
            }
        }
    }
}

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