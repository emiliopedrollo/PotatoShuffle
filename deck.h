#ifndef POTATOSHUFFLE_DECK_H

#define DECK_SIZE	52

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

static const int SCORE_ROYAL_STRAIGHT_FLUSH = 1000;
static const int SCORE_STRAIGHT_FLUSH = 750;
static const int SCORE_FOUR_OF_A_KIND = 500;
static const int SCORE_FULL_HOUSE = 250;
static const int SCORE_FLUSH = 200;
static const int SCORE_STRAIGHT = 150;
static const int SCORE_THREE_OF_A_KIND = 100;
static const int SCORE_TWO_PAIRS = 50;
static const int SCORE_PAIR = 20;
static const int SCORE_HIGH_CARD = 0;

void printCard(CARD_T card);
void printHands(HAND_T *hand);
/*void printDeck(CARD_T **cards);*/
void createDeck(CARD_T **cards);
void shuffle(CARD_T **cards, unsigned int seed);
int countPoints(HAND_T *hand);
int countPointsWithDescription(HAND_T *hand,char* handName);


#define POTATOSHUFFLE_DECK_H

#endif /*POTATOSHUFFLE_DECK_H*/