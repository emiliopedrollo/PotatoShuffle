#include "deck.h"

#ifndef POTATOSHUFFLE_UTILS_H
#define POTATOSHUFFLE_UTILS_H

typedef void (*func)(HAND_T **hand, HAND_T **best_order);

void divideBySuit(HAND_T *hand, HAND_T **diamonds, HAND_T **spades, HAND_T **hearts, HAND_T **clubs);
int countCardsInHand(HAND_T *hand);
bool searchForSequence(HAND_T **hand, HAND_T **sequence);
HAND_T *copyHand(HAND_T *hand);
HAND_T *sortHand(HAND_T *cards, bool aceIsOne);
HAND_T *sortHandDec(HAND_T *cards, bool aceIsOne);
HAND_T *getFirsts(HAND_T *cards, int count);
HAND_T *addHandToHand(HAND_T **hand, HAND_T *toAdd);
HAND_T *addCardToHand(HAND_T **hand, CARD_T card);
HAND_T *subtractHandFromHand(HAND_T **hand, HAND_T *toRemove);
HAND_T *subtractCardFromHand(HAND_T **hand,CARD_T card);
void permute(func *a, int l, int r,HAND_T **hand, HAND_T **best_order);
void swap(func *x, func *y);

#endif
