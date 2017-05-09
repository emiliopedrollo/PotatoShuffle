//
// Created by ncc on 09/05/17.
//

#ifndef POTATOSHUFFLE_DECK_H

#define DECK_SIZE	52
#define SEED		1

typedef enum {DIAMOND, SPADE, HEART, CLUB} SUIT_T;

typedef struct {
    int value;
    SUIT_T suit;
} CARD_T;

/* tipo a ser usado para a fila correspondente a mão do usuário */
typedef struct n {
    CARD_T card;
    struct n *next;
} HAND_T;

void printCard(CARD_T card);
void printDeck(CARD_T **cards);
void createDeck(CARD_T **cards);
void shuffle(CARD_T **cards, unsigned int seed);
int countPoints(HAND_T *mao);


#define POTATOSHUFFLE_DECK_H

#endif //POTATOSHUFFLE_DECK_H
