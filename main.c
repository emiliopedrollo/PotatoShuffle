#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "main.h"
#include "utils.h"

HAND_T *createHands(unsigned int seed);

HAND_T *orderHands(HAND_T *hand);

bool checkForRoyalStraightFlash(HAND_T **hand, HAND_T **best_order);

bool checkForStraightFlush(HAND_T **hand, HAND_T **best_order);

bool checkForFourOfAKind(HAND_T **hand, HAND_T **best_order);

bool checkForFullHouse(HAND_T **hand, HAND_T **best_order);

bool checkForFlush(HAND_T **hand, HAND_T **best_order);

bool checkForStraight(HAND_T **hand, HAND_T **best_order);

bool checkForThreeOfAKind(HAND_T **hand, HAND_T **best_order);

bool checkForTwoPairs(HAND_T **hand, HAND_T **best_order);

bool checkForPair(HAND_T **hand, HAND_T **best_order);

check checks[9] = {
        checkForRoyalStraightFlash,
        checkForStraightFlush,
        checkForFourOfAKind,
        checkForFullHouse,
        checkForFlush,
        checkForStraight,
        checkForThreeOfAKind,
        checkForTwoPairs,
        checkForPair
};

int main() {

    HAND_T *cards;
    int score;
    unsigned int seed;

    srand((unsigned int) time(NULL));

    /* GOOD SEEDS */
    /* 87132968    - 820  score                      */
    /* 1130123366  - 1670 score (RSF now worthy)     */
    /* 1028929438  - 2700 score                      */
    /* 1535832847  - 2750 score                      */
    /* 1133152313  - 2900 score                      */
    /* 1586328659  - 2970 score                      */
    /* 2109996363  - 3250 score                      */
    /* 1861052978  - 3270 score                      */
    /* 570931470   - 3300 score                      */

    seed = (unsigned int) rand();
    printf("Shuffling using seed %d\n", seed);
    cards = createHands(seed);
    /*printHands(cards);*/
    printf("\n");
    cards = orderHands(cards);
    score = verifyPoints(cards, true);

    printf("Score: %d\n", score);

    exit(0);

}

HAND_T *createHands(unsigned int seed) {

    int i;
    CARD_T **cards;
    HAND_T *new, *list = NULL;

    cards = malloc(DECK_SIZE * sizeof(cards));
    createDeck(cards);
    shuffle(cards, seed);
    for (i = 0; i < 25; i++) {
        new = malloc(sizeof(HAND_T));
        new->card.value = cards[i]->value;
        new->card.suit = cards[i]->suit;
        new->next = list;
        list = new;
    }

    return list;
}

int verifyPoints(HAND_T *cards, bool display) {

    int i, j, points = 0, handPoints = 0;
    HAND_T *pt_card, *hand, *aux;
    char *handName;

    handName = malloc(sizeof(char) * 30);
    pt_card = cards;
    /*maos devem estar ordenadas por valor, ascendente*/
    for (i = 0; i < 5; i++) {
        hand = pt_card;
        for (j = 0; j < 4; j++)
            pt_card = pt_card->next;
        aux = pt_card->next;
        pt_card->next = NULL;
        handPoints = countPointsWithDescription(hand, handName);
        points += handPoints;
        if (display) {
            printHands(hand);
            printf(" %s (%d points)\n", handName, handPoints);
            printf("\n");
        }
        pt_card->next = aux;
        pt_card = pt_card->next;
    }

    return points;

}

HAND_T *orderHands(HAND_T *hand) {

    int i, k = 0;
    HAND_T *best_order = NULL, *copy;
    check steps[9] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};

    for (i = 0; i < 9; i++) {
        copy = copyHand(hand);
        if (checks[i](&copy, &best_order)) {
            steps[k++] = checks[i];
        }
        free_hand(&copy);
        free_hand(&best_order);
    }

    permute(steps, 0, k - 1, hand);

    addHandToHand(&best_hand, best_hand_leftover);
    free_hand(&best_hand_leftover);

    return best_hand;

}

bool checkForPair(HAND_T **hand, HAND_T **best_order) {

    int i, j, k;
    HAND_T *aux = NULL, *aux2, *aux3;
    HAND_T *iterable;
    bool foundExtra;
    bool result = false;

    for (i = 1; i < 14; i++) {
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
        for (j = k / 2; j > 0; j--) {
            iterable = *hand;
            foundExtra = false;
            k = 0;
            aux3 = NULL;
            aux2 = getFirsts(aux, 2);
            while (iterable != NULL) {
                if (iterable->card.value != i) {
                    addHandToHand(&aux3, getFirsts(iterable, 1));
                    if (++k == 3) {
                        addHandToHand(&aux2, aux3);
                        foundExtra = true;
                        break;
                    }
                }
                iterable = iterable->next;
            }
            if (foundExtra) {
                subtractHandFromHand(&aux, aux2);
                subtractHandFromHand(hand, aux2);
                addHandToHand(best_order, sortHand(aux2, true));
                result = true;
            }
            free_hand(&aux2);
            free_hand(&aux3);
        }
        free_hand(&aux);
    }

    return result;

}

bool checkForTwoPairs(HAND_T **hand, HAND_T **best_order) {

    int i, j, k, l;
    HAND_T *aux = NULL, *aux2 = NULL, *aux3 = NULL;
    HAND_T *iterable;
    bool foundExtraPair, foundExtra;
    bool result = false;

    for (i = 1; i < 14; i++) {
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
        for (j = k / 2; j > 0; j--) {
            foundExtra = false;
            for (l = 1; l < 14; l++) {
                if (l == i) continue;
                iterable = *hand;
                foundExtra = false;
                foundExtraPair = false;
                k = 0;
                aux3 = NULL;
                free_hand(&aux2);
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
                free_hand(&aux3);
                if (foundExtraPair) {
                    iterable = *hand;
                    while (iterable != NULL) {
                        if (iterable->card.value != i && iterable->card.value != l) {
                            addHandToHand(&aux2, getFirsts(iterable, 1));
                            foundExtra = true;
                            break;
                        }
                        iterable = iterable->next;
                    }
                }
                if (foundExtra) break;
            }
            if (foundExtra) {
                subtractHandFromHand(&aux, aux2);
                subtractHandFromHand(hand, aux2);
                addHandToHand(best_order, sortHand(aux2, true));
                result = true;
            }
            free_hand(&aux2);
        }
        free_hand(&aux);
    }

    return result;

}

bool checkForThreeOfAKind(HAND_T **hand, HAND_T **best_order) {

    int i, j, k;
    HAND_T *aux = NULL, *aux2, *aux3;
    HAND_T *iterable;
    bool foundExtra;
    bool result = false;

    for (i = 1; i < 14; i++) {
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
        for (j = k / 3; j > 0; j--) {
            iterable = *hand;
            foundExtra = false;
            k = 0;
            aux3 = NULL;
            aux2 = getFirsts(aux, 3);
            while (iterable != NULL) {
                if (iterable->card.value != i) {
                    addHandToHand(&aux3, getFirsts(iterable, 1));
                    if (++k == 2) {
                        addHandToHand(&aux2, aux3);
                        foundExtra = true;
                        break;
                    }
                }
                iterable = iterable->next;
            }
            if (foundExtra) {
                subtractHandFromHand(&aux, aux2);
                subtractHandFromHand(hand, aux2);
                addHandToHand(best_order, sortHand(aux2, true));
                result = true;
            }
            free_hand(&aux2);
            free_hand(&aux3);
        }
        free_hand(&aux);
    }

    return result;

}

bool checkForStraight(HAND_T **hand, HAND_T **best_order) {

    HAND_T *aux, *sequence;
    bool result = false;

    aux = sortHand(copyHand((*hand)), true);
    while (searchForSequence(&aux, &sequence)) {
        subtractHandFromHand(&aux, sequence);
        subtractHandFromHand(hand, sequence);
        addHandToHand(best_order, sequence);
        free_hand(&sequence);
        result = true;
    }
    free_hand(&aux);

    return result;

}

bool checkForFlush(HAND_T **hand, HAND_T **best_order) {

    int i, j;
    bool result = false;
    HAND_T *aux, **addr = NULL;
    HAND_T *diamond = NULL, *spade = NULL, *heart = NULL, *club = NULL;

    divideBySuit(*hand, &diamond, &spade, &heart, &club);

    for (i = 0; i < 4; i++) {
        if (i == 0) addr = &diamond;
        else if (i == 1) addr = &spade;
        else if (i == 2) addr = &heart;
        else if (i == 3) addr = &club;

        for (j = countCardsInHand(*addr) / 5; j > 0; j--) {

            aux = getFirsts(*addr, 5);

            subtractHandFromHand(addr, aux);
            subtractHandFromHand(hand, aux);
            addHandToHand(best_order, sortHand(aux, true));
            free_hand(&aux);

            result = true;

        }
    }

    free_hand(&diamond);
    free_hand(&spade);
    free_hand(&heart);
    free_hand(&club);

    return result;

}

bool checkForFullHouse(HAND_T **hand, HAND_T **best_order) {

    int i, j, k, l;
    HAND_T *aux = NULL, *aux2, *aux3;
    HAND_T *iterable;
    bool foundExtra;
    bool result = false;

    for (i = 1; i < 14; i++) {
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
        for (j = k / 3; j > 0; j--) {
            foundExtra = false;
            aux2 = getFirsts(aux, 3);
            for (l = 1; l < 14; l++) {
                if (l == i)continue;
                k = 0;
                aux3 = NULL;
                iterable = *hand;
                while (iterable != NULL) {
                    if (iterable->card.value == l) {
                        addHandToHand(&aux3, getFirsts(iterable, 1));
                        if (++k == 2) {
                            addHandToHand(&aux2, aux3);
                            foundExtra = true;
                            break;
                        }
                    }
                    iterable = iterable->next;
                }
                free_hand(&aux3);
                if (foundExtra) break;
            }
            if (foundExtra) {
                subtractHandFromHand(&aux, aux2);
                subtractHandFromHand(hand, aux2);
                addHandToHand(best_order, sortHand(aux2, true));
                result = true;
            }
            free_hand(&aux2);
        }
        free_hand(&aux);
    }

    return result;

}

bool checkForFourOfAKind(HAND_T **hand, HAND_T **best_order) {

    int i, j, k;
    HAND_T *aux = NULL, *aux2;
    HAND_T *iterable;
    bool foundExtra;
    bool result = false;

    for (i = 1; i < 14; i++) {
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
        for (j = k / 4; j > 0; j--) {
            iterable = *hand;
            foundExtra = false;
            aux2 = getFirsts(aux, 4);
            while (iterable != NULL) {
                if (iterable->card.value != i) {
                    addHandToHand(&aux2, getFirsts(iterable, 1));
                    foundExtra = true;
                    break;
                }
                iterable = iterable->next;
            }
            if (foundExtra) {
                subtractHandFromHand(&aux, aux2);
                subtractHandFromHand(hand, aux2);
                addHandToHand(best_order, sortHand(aux2, true));
                result = true;
            }
            free_hand(&aux2);

        }
        free_hand(&aux);
    }

    return result;

}

bool checkForStraightFlush(HAND_T **hand, HAND_T **best_order) {

    int i;
    bool result = false;
    HAND_T *aux, *sequence, **addr = NULL;
    HAND_T *diamond = NULL, *spade = NULL, *heart = NULL, *club = NULL;

    divideBySuit(*hand, &diamond, &spade, &heart, &club);

    for (i = 0; i < 4; i++) {
        if (i == 0) addr = &diamond;
        else if (i == 1) addr = &spade;
        else if (i == 2) addr = &heart;
        else if (i == 3) addr = &club;

        aux = sortHand(copyHand(*addr), true);
        while (searchForSequence(&aux, &sequence)) {
            subtractHandFromHand(&aux, sequence);
            subtractHandFromHand(hand, sequence);
            addHandToHand(best_order, sequence);
            free_hand(&sequence);
            result = true;
        }
        free_hand(&aux);
    }

    free_hand(&diamond);
    free_hand(&spade);
    free_hand(&heart);
    free_hand(&club);

    return result;

}

bool checkForRoyalStraightFlash(HAND_T **hand, HAND_T **best_order) {

    int i;
    bool result = false;
    HAND_T *aux, **addr = NULL;
    HAND_T *diamond = NULL, *spade = NULL, *heart = NULL, *club = NULL;

    divideBySuit(*hand, &diamond, &spade, &heart, &club);

    for (i = 0; i < 4; i++) {
        if (i == 0) addr = &diamond;
        else if (i == 1) addr = &spade;
        else if (i == 2) addr = &heart;
        else if (i == 3) addr = &club;

        aux = sortHand(getFirsts(sortHandDec(*addr, false), 5), true);
        if (countCardsInHand(aux) == 5) {

            if (countPoints(aux) == SCORE_ROYAL_STRAIGHT_FLUSH) {
                subtractHandFromHand(hand, aux);
                addHandToHand(best_order, aux);
                result = true;
            }
        }
        free_hand(&aux);
    }

    free_hand(&diamond);
    free_hand(&spade);
    free_hand(&heart);
    free_hand(&club);

    return result;

}