#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>
#include <stdbool.h>
#include "main.h"
#include "deck.h"
#include "utils.h"

void test();
HAND_T *createHands(unsigned int seed);
HAND_T *orderHands(HAND_T *hand);
void checkForRoyalStraightFlash(HAND_T **hand, HAND_T **best_order);
void checkForStraightFlush(HAND_T **hand, HAND_T **best_order);
void checkForFourOfAKind(HAND_T **hand, HAND_T **best_order);
void checkForFullHouse(HAND_T **hand, HAND_T **best_order);
void checkForFlush(HAND_T **hand, HAND_T **best_order);
void checkForStraight(HAND_T **hand, HAND_T **best_order);
void checkForThreeOfAKind(HAND_T **hand, HAND_T **best_order);
void checkForTwoPairs(HAND_T **hand, HAND_T **best_order);
void checkForPair(HAND_T **hand, HAND_T **best_order);




func steps[6] = {
        checkForRoyalStraightFlash,
        checkForStraightFlush,
        checkForFourOfAKind,
        checkForFullHouse,
        checkForFlush,
        checkForStraight,
};

int main() {

    HAND_T *cards;
    int score;
    unsigned int seed;

    /*test();*/

    srand((unsigned int) time(NULL));

    /* GOOD SEEDS */
    /* 87132968    - 820  score */
    /* 1028929438  - 2700 score */
    /* 1535832847  - 2750 score */
    /* 1133152313  - 2900 score */
    /* 1586328659  - 2970 score */
    /* 2109996363  - 3250 score */
    /* 1861052978  - 3270 score */
    /* 570931470   - 3300 score */

    seed = (unsigned int) rand();
    printf("Shuffling using seed %d\n",seed);
    cards=createHands(seed);
    /*printHands(cards);*/
    printf("\n");
    cards= orderHands(cards);
    score = verifyPoints(cards, true);

    printf("Score: %d\n", score);



    exit(0);
}

HAND_T *createHands(unsigned int seed){
    int i;
    CARD_T **cards;
    HAND_T *new, *list=NULL;
    cards = malloc(DECK_SIZE * sizeof(cards));
    createDeck(cards);
    shuffle(cards, seed);
    for(i=0; i<25; i++){
        new = malloc(sizeof(HAND_T));
        new->card.value=cards[i]->value;
        new->card.suit=cards[i]->suit;
        new->next=list;
        list=new;
    }
    return list;
}

int verifyPoints(HAND_T *cards, bool display) {
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
        if (display){
            printHands(hand);
            printf(" %s (%d points)\n",handName,handPoints);
            printf("\n");
        }
        pt_card->next=aux;
        pt_card=pt_card->next;
    }
    return points;
}

HAND_T *orderHands(HAND_T *hand){
    /* seu codigo aqui */

    HAND_T *best_order = NULL;


    permute(steps,0,5,&hand,&best_order);

    checkForThreeOfAKind(&best_hand_leftover, &best_hand);
    checkForTwoPairs(&best_hand_leftover, &best_hand);
    checkForPair(&best_hand_leftover, &best_hand);

    addHandToHand(&best_hand,best_hand_leftover);
    return best_hand;

/*
    checkForRoyalStraightFlash(&hand, &best_order);
    checkForStraightFlush(&hand, &best_order);
    checkForFourOfAKind(&hand, &best_order);
    checkForFullHouse(&hand, &best_order);
    checkForFlush(&hand, &best_order);
    checkForStraight(&hand, &best_order);
    checkForThreeOfAKind(&hand, &best_order);
    checkForTwoPairs(&hand, &best_order);
    checkForPair(&hand, &best_order);


    addHandToHand(&best_order,hand);
    return best_order;
*/
}

void checkForPair(HAND_T **hand, HAND_T **best_order) {
    int i,j,k;
    HAND_T *aux = NULL, *aux2, *aux3;
    HAND_T *iterable;
    bool foundExtra;


    for (i=1;i<14;i++){
        k = 0;
        iterable = *hand;
        aux = NULL;
        while (iterable != NULL){
            if (iterable->card.value == i) {
                addHandToHand(&aux, getFirsts(iterable, 1));
                k++;
            }
            iterable = iterable->next;
        }
        for(j=k/2;j>0;j--){
            iterable = *hand;
            foundExtra = false;
            k = 0;
            aux3 = NULL;
            aux2 = getFirsts(aux, 2);
            while (iterable != NULL){
                if (iterable->card.value != i){
                    addHandToHand(&aux3, getFirsts(iterable, 1));
                    if (++k == 3){
                        addHandToHand(&aux2,aux3);
                        foundExtra = true;
                        break;
                    }
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


void checkForTwoPairs(HAND_T **hand, HAND_T **best_order) {
    int i,j,k,l;
    HAND_T *aux = NULL, *aux2 = NULL, *aux3 = NULL;
    HAND_T *iterable;
    bool foundExtraPair,foundExtra;

    for (i=1;i<14;i++) {
        k = 0;
        iterable = *hand;
        aux = NULL;
        while (iterable != NULL) {
            if (iterable->card.value == i) {
                addHandToHand(&aux, getFirsts(iterable, 1));
                k++;
            }
            iterable = iterable->next;
        }
        for(j=k/2;j>0;j--){
            foundExtra = false;
            for(l=1;l<14;l++) {
                if (l==i) continue;
                iterable = *hand;
                foundExtra = false;
                foundExtraPair = false;
                k = 0;
                aux3 = NULL;
                aux2 = getFirsts(aux, 2);
                while (iterable != NULL) {
                    if (iterable->card.value == l) {
                        addHandToHand(&aux3, getFirsts(iterable, 1));
                        if (++k == 2) {
                            addHandToHand(&aux2, aux3);
                            foundExtraPair = true;
                            break;
                        }
                    }
                    iterable = iterable->next;
                }
                if (foundExtraPair){
                    iterable = *hand;
                    while (iterable != NULL){
                        if (iterable->card.value != i && iterable->card.value != l){
                            addHandToHand(&aux2, getFirsts(iterable, 1));
                            foundExtra = true;
                            break;
                        }
                        iterable = iterable->next;
                    }
                }
                if (foundExtra) break;
            }
            if (foundExtra){
                subtractHandFromHand(&aux,aux2);
                subtractHandFromHand(hand,aux2);
                addHandToHand(best_order, sortHand(aux2,true));
            }
        }
    }
}


void checkForThreeOfAKind(HAND_T **hand, HAND_T **best_order) {
    int i,j,k;
    HAND_T *aux = NULL, *aux2, *aux3;
    HAND_T *iterable;
    bool foundExtra;


    for (i=1;i<14;i++){
        k = 0;
        iterable = *hand;
        aux = NULL;
        while (iterable != NULL){
            if (iterable->card.value == i) {
                addHandToHand(&aux, getFirsts(iterable, 1));
                k++;
            }
            iterable = iterable->next;
        }
        for(j=k/3;j>0;j--){
            iterable = *hand;
            foundExtra = false;
            k = 0;
            aux3 = NULL;
            aux2 = getFirsts(aux, 3);
            while (iterable != NULL){
                if (iterable->card.value != i){
                    addHandToHand(&aux3, getFirsts(iterable, 1));
                    if (++k == 2){
                        addHandToHand(&aux2,aux3);
                        foundExtra = true;
                        break;
                    }
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
    HAND_T *aux, *sequence, **addr = NULL;
    HAND_T *diamond, *spade, *heart, *club;
    divideBySuit(*hand,&diamond,&spade,&heart,&club);

    for(i=0;i<4;i++) {
        if (i == 0) addr = &diamond;
        else if (i == 1) addr = &spade;
        else if (i == 2) addr = &heart;
        else if (i == 3) addr = &club;

        for (j = countCardsInHand(*addr) / 5; j> 0 ; j--){

            aux = getFirsts(*addr, 5);

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
    bool foundExtra;


    for (i=1;i<14;i++){
        k = 0;
        iterable = *hand;
        aux = NULL;
        while (iterable != NULL){
            if (iterable->card.value == i) {
                addHandToHand(&aux, getFirsts(iterable, 1));
                k++;
            }
            iterable = iterable->next;
        }
        for(j=k/3;j>0;j--){
            foundExtra = false;
            aux2 = getFirsts(aux, 3);
            for(l=1;l<14;l++){
                if (l==i)continue;
                k=0;
                aux3 = NULL;
                iterable = *hand;
                while (iterable != NULL){
                    if (iterable->card.value == l){
                        addHandToHand(&aux3, getFirsts(iterable, 1));
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
    bool foundExtra;


    for (i=1;i<14;i++){
        k = 0;
        iterable = *hand;
        aux = NULL;
        while (iterable != NULL){
            if (iterable->card.value == i) {
                addHandToHand(&aux, getFirsts(iterable, 1));
                k++;
            }
            iterable = iterable->next;
        }
        for(j=k/4;j>0;j--){
            iterable = *hand;
            foundExtra = false;
            aux2 = getFirsts(aux, 4);
            while (iterable != NULL){
                if (iterable->card.value != i){
                    addHandToHand(&aux2, getFirsts(iterable, 1));
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
    HAND_T *diamond, *spade, *heart, *club;

    divideBySuit(*hand,&diamond,&spade,&heart,&club);

    for(i=0;i<4;i++){
        if (i==0) addr = &diamond;
        else if (i==1) addr = &spade;
        else if (i==2) addr = &heart;
        else if (i==3) addr = &club;

        aux = sortHand(getFirsts(sortHandDec(*addr, false), 5), true);
        if (countCardsInHand(aux) == 5) {

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