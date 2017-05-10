//
// Created by emilio on 10/05/17.
//

#include <lzma.h>
#include <stdbool.h>
#include <stdlib.h>
#include "utils.h"
#include "deck.h"

void bubbleSort(HAND_T *cards, bool aceIsOne, bool (*func)(int, int, bool)) {
    int i;
    CARD_T aux;
    int qtd = 0;

    if (cards == NULL) return;

    HAND_T *iterable = cards;

    while(iterable != NULL){
        qtd++;
        iterable = iterable->next;
    }

    for(i = 0; i < qtd; i++){
        iterable = cards;
        while(iterable->next != NULL){
            if(func(iterable->card.value,iterable->next->card.value,aceIsOne)){
                aux = iterable->card;
                iterable->card = iterable->next->card;
                iterable->next->card = aux;
            }
            iterable = iterable->next;
        }
    }
}

bool firstIsBigger(int first,int second,bool aceIsOne){
    if (aceIsOne){
        return first > second;
    } else return (first == 1) || ((first > second) && (second != 1));
}
bool secondIsBigger(int first,int second,bool aceIsOne){
    if (aceIsOne){
        return  second > first;
    } else return (second == 1) || ((second > first) && (first != 1));
}

HAND_T *sortHand(HAND_T *cards, bool aceIsOne) {
    bubbleSort(cards, aceIsOne, firstIsBigger);
    return cards;
}

HAND_T *sortHandDec(HAND_T *cards, bool aceIsOne) {
    bubbleSort(cards, aceIsOne, secondIsBigger);
    return cards;
}

HAND_T *addHandLast(HAND_T **cards, CARD_T card) {

    if (*cards == NULL){
        *cards = malloc(sizeof(HAND_T));
        (*cards)->card = card;
        (*cards)->next = NULL;
        return *cards;
    }

    HAND_T *new, *iterable;
    new = malloc(sizeof(HAND_T));
    new->card = card;
    new->next = NULL;
    iterable = *cards;
    while (iterable->next != NULL) iterable = iterable->next;
    iterable->next = new;
    return *cards;
}

int biggerOfThree(int num1, int num2, int num3){
    if (num1 > num2){
        if (num1 > num3)
            return 0;
        else
            return 2;
    } else if (num2 > num3)
        return 1;
    else
        return 2;
}

int chooseBigerValid(int a,int b, int c, int d, int invalid){
    int bigger;
    if (invalid == 0){
        bigger = biggerOfThree(b,c,d);
        if (bigger==0)
            return 1;
        else if (bigger==1)
            return 2;
        else
            return 3;
    } else if (invalid == 1){
        bigger = biggerOfThree(a,c,d);
        if (bigger==0)
            return 0;
        else if (bigger==1)
            return 2;
        else
            return 3;
    } else if (invalid == 2){
        bigger = biggerOfThree(a,b,d);
        if (bigger==0)
            return 0;
        else if (bigger==1)
            return 1;
        else
            return 3;
    } else {
        bigger = biggerOfThree(a,b,c);
        if (bigger==0)
            return 0;
        else if (bigger==1)
            return 1;
        else
            return 2;
    }
}

HAND_T *getFirsts(HAND_T *cards, bool *success, int count) {
    HAND_T *result = NULL, *aux;
    int k = 0;
    *success = false;
    if (cards == NULL) return NULL;

    aux = cards;
    while(aux != NULL && k < count){
        k++;
        addHandLast(&result,aux->card);
        aux = aux->next;
    }
    *success = (k == count);
    return result;
}

void divideBySuit(HAND_T *hand, HAND_T **diamonds, HAND_T **spades, HAND_T **hearts, HAND_T **clubs){
    HAND_T *iterable = hand, **aux_addr = NULL, *aux, *aux2;

    *diamonds = NULL;
    *spades = NULL;
    *hearts = NULL;
    *clubs = NULL;

    while(iterable){
        switch (iterable->card.suit){
            case DIAMOND:
                aux_addr = diamonds;
                break;
            case SPADE:
                aux_addr = spades;
                break;
            case HEART:
                aux_addr = hearts;
                break;
            case CLUB:
                aux_addr = clubs;
                break;
        }
        if (*aux_addr == NULL){
            *aux_addr = malloc(sizeof(HAND_T));
            (*aux_addr)->card.value = iterable->card.value;
            (*aux_addr)->card.suit = iterable->card.suit;
            (*aux_addr)->next = NULL;
        } else {
            aux = malloc(sizeof(HAND_T));
            aux->card.value = iterable->card.value;
            aux->card.suit = iterable->card.suit;
            aux->next = NULL;
            aux2 = *aux_addr;
            while(aux2->next != NULL){
                aux2 = aux2->next;
            }
            aux2->next = aux;
        }
        iterable = iterable->next;
    }
}

HAND_T *copyHand(HAND_T *hand){
    HAND_T *copy = NULL,*new, *iterable = hand, *copyIterable = NULL;
    if (hand == NULL) return NULL;

    while(iterable){
        new = malloc(sizeof(HAND_T));
        new->card = iterable->card;
        new->next = NULL;
        if (copy == NULL)
            copyIterable = copy = new;
        else
            copyIterable = copyIterable->next = new;
        iterable = iterable->next;
    }
    return copy;
}

int countCardsInHand(HAND_T *hand){
    int c = 0;
    HAND_T *iterable = hand;

    while(iterable != NULL){
        c++;
        iterable = iterable->next;
    }
    return c;
}

bool searchForSequence(HAND_T **hand, HAND_T **sequence){
    int i = 0,k = 0,s = -1;
    int last = 0;
    HAND_T *iterable, *first, *new, *sequenceIterable;

    if (*hand == NULL) return false;

    *sequence = NULL;

    iterable = copyHand(*hand);
    first = iterable;
    
    while (iterable != NULL){

        if (iterable->card.value == last) {
            //nothing happens
        } else if (iterable->card.value == last+1){
            k++;
            if (s == -1) s = i;
        } else {
            k = 0;
            s = -1;
        }

        if (k == 5) {
            break;
        }

        last = iterable->card.value;
        iterable = iterable->next;
        i++;
    }    
    if (k!=5){
        return false;
    }
    
    iterable = first;
    
    while(s > 0){
        iterable = iterable->next;
        s--;
    }

    *sequence = malloc(sizeof(HAND_T));
    (*sequence)->card = iterable->card;
    (*sequence)->next = NULL;
    sequenceIterable = (*sequence);
    iterable = iterable->next;
    while ((k > 1) && (iterable != NULL)){
        if (iterable->card.value != sequenceIterable->card.value){
            new = malloc(sizeof(HAND_T));
            new->card = iterable->card;
            new->next = NULL;
            sequenceIterable = sequenceIterable->next = new;
            k--;
        }
        iterable = iterable->next;
    }

    return true;
}

HAND_T *addHandToHand(HAND_T **hand, HAND_T *toAdd){
    HAND_T *aux;
    aux = toAdd;
    while(aux != NULL){
        addCardToHand(hand,aux->card);
        aux = aux->next;
    }
    return *hand;
}

HAND_T *addCardToHand(HAND_T **hand, CARD_T card){
    HAND_T *aux,*new;

    if (*hand == NULL){
        *hand = malloc(sizeof(HAND_T));
        (*hand)->card = card;
        (*hand)->next = NULL;
    } else {
        aux = *hand;
        while (aux->next != NULL) aux = aux->next;
        new = malloc(sizeof(HAND_T));
        new->card = card;
        new->next = NULL;
        aux->next = new;
    }

    return *hand;
}

HAND_T *subtractHandFromHand(HAND_T **hand, HAND_T *toRemove){
    HAND_T *aux;
    aux = toRemove;
    while(aux != NULL){
        subtractCardFromHand(hand,aux->card);
        aux = aux->next;
    }
    return *hand;
}

HAND_T *subtractCardFromHand(HAND_T **hand,CARD_T card){
    HAND_T *aux,*aux2;
    aux = *hand;

    if ((*hand)->card.value == card.value && (*hand)->card.suit == card.suit){
        *hand = (*hand)->next;
        free(aux);
    } else {
        while(aux->next != NULL){
            if (aux->next->card.value == card.value && aux->next->card.suit == card.suit){
                aux2 = aux->next;
                aux->next = aux->next->next;
                free(aux2);
                break;
            }
            aux = aux->next;
        }
    }

    return *hand;

}