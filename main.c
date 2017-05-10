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
void checkForRoyalStraightFlash(HAND_T **hand_addr, HAND_T **copy, HAND_T **best_order);
void checkForStraightFlush(HAND_T **hand_addr, HAND_T **copy, HAND_T **best_order);

int main() {

//    CARD_T **cards;
//    cards = malloc(DECK_SIZE * sizeof (CARD_T));
//
//    printf("Criando baralho padrão.\n");
//    createDeck(cards);
//    printDeck(cards);
//
//    printf("\nEmbaralhando com semente %d.\n", SEED);
//    shuffle(cards, SEED);
//    printDeck(cards);


    HAND_T *cards;
    int points;

    cards=createHands();
//    printHands(cards);
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
    shuffle(cards, 1028929438); //RSF
//    shuffle(cards, (unsigned int) rand());
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
        printf("%s (%d points)\n",handName,handPoints);
        printHands(hand);
        printf("\n");
        pt_card->next=aux;
        pt_card=pt_card->next;
    }
    return points;
}

HAND_T *orderHands(HAND_T *hand){
    /* seu codigo aqui */

    int i,j,k;
    HAND_T *diamond, *spade, *heart, *club;
    HAND_T *copy, *aux, *aux2, *aux3, *aux4;
    HAND_T **hand_addr;
    HAND_T *sequence = NULL;
    HAND_T *best_order = NULL;

    char *handName;
    handName = malloc(sizeof(char)*30);

    bool *success;
    success = malloc(sizeof(bool));
    *success = false;

    copy = copyHand(hand);

    diamond = NULL;
    spade = NULL;
    heart = NULL;
    club = NULL;


    for (i=0;i<2;i++){
        divideBySuit(copy,&diamond,&spade,&heart,&club);

        for (j=0;j<4;j++){

            hand_addr = NULL;
            if (j==0) hand_addr = &diamond;
            else if (j==1) hand_addr = &spade;
            else if (j==2) hand_addr = &heart;
            else if (j==3) hand_addr = &club;

            if (i == 0){
                // Search for Royal Straight Flush
                checkForRoyalStraightFlash(hand_addr, &copy, &best_order);
            } else if (i == 1){
                // Search for Straight Flush
                checkForStraightFlush(hand_addr, &copy, &best_order);
            }
        }
    }

    for (i=0;i<4;i++){
        divideBySuit(copy,&diamond,&spade,&heart,&club);
        hand_addr = NULL;
        if (i==0) hand_addr = &diamond;
        else if (i==1) hand_addr = &spade;
        else if (i==2) hand_addr = &heart;
        else if (i==3) hand_addr = &club;

        while (countCardsInHand(*hand_addr) >= 4){
            aux2 = getFirsts(*hand_addr, success, 4);

            k = chooseBigerValid(countCardsInHand(diamond),
                                 countCardsInHand(spade),
                                 countCardsInHand(heart),
                                 countCardsInHand(club),
                                 j);

            if (k==0) aux3 = diamond;
            else if (k==1) aux3 = spade;
            else if (k==2) aux3 = heart;
            else aux3 = club;

            aux4 = getFirsts(aux3,success,1);
            subtractHandFromHand(&aux3,aux4);
            subtractHandFromHand(hand_addr,aux2);

            addHandToHand(&aux2,aux4);

            //FIXME error here

            subtractHandFromHand(&copy,aux2);

            addHandToHand(&best_order,sortHand(aux2,true));

        }
    }


    // Search for sequences
    aux = sortHand(copyHand(copy),true);
    while (searchForSequence(&aux,&sequence)){
        subtractHandFromHand(&aux,sequence);
        subtractHandFromHand(&copy,sequence);
        addHandToHand(&best_order,sequence);
    }

    addHandToHand(&best_order,copy);
    return best_order;
}

void checkForStraightFlush(HAND_T **hand_addr, HAND_T **copy, HAND_T **best_order) {
    HAND_T *aux, *sequence;
    aux = sortHand(copyHand(*hand_addr), true);
    while(searchForSequence(&aux,&sequence)){
        subtractHandFromHand(hand_addr,sequence);
        subtractHandFromHand(copy, sequence);
        addHandToHand(best_order, sequence);
    }
}

void checkForRoyalStraightFlash(HAND_T **hand_addr, HAND_T **copy, HAND_T **best_order) {
    HAND_T *aux;
    bool success;
    aux = sortHand(getFirsts(sortHandDec(*hand_addr, false), &success, 5), true);
    if (success) {

        if (countPoints(aux) == SCORE_ROYAL_STRAIGHT_FLUSH) {
            subtractHandFromHand(copy, aux);
            addHandToHand(best_order, aux);
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