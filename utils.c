#include <stdbool.h>
#include <stdlib.h>
#include "utils.h"
#include "main.h"

int high_score = 0;

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

HAND_T *getFirsts(HAND_T *cards, int count) {
    HAND_T *result = NULL, *aux;
    int k = 0;
    if (cards == NULL) return NULL;

    aux = cards;
    while(aux != NULL && k < count){
        k++;
        addHandLast(&result,aux->card);
        aux = aux->next;
    }
    return result;
}

void divideBySuit(HAND_T *hand, HAND_T **diamonds, HAND_T **spades, HAND_T **hearts, HAND_T **clubs){
    HAND_T *iterable = hand, **aux_addr = NULL, *aux, *aux2;

    free_hand(diamonds);
    free_hand(spades);
    free_hand(hearts);
    free_hand(clubs);

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
    int lastValue = 0;
    HAND_T *iterable, *first, *new, *sequenceIterable;

    if (*hand == NULL) return false;

    *sequence = NULL;

    iterable = copyHand(*hand);
    first = iterable;

    lastValue = iterable->card.value;
    while (iterable != NULL){

        if (iterable->card.value == lastValue) {
            /*nothing happens*/
        } else if (iterable->card.value == lastValue+1){
            k++;
            if (s == -1) s = i;
        } else {
            k = 0;
            s = -1;
        }

        if (k == 4) {
            break;
        }

        lastValue = iterable->card.value;
        iterable = iterable->next;
        i++;
    }    
    if (k!=4){
        return false;
    }
    
    iterable = first;
    
    while(s > 1){
        iterable = iterable->next;
        s--;
    }

    *sequence = malloc(sizeof(HAND_T));
    (*sequence)->card = iterable->card;
    (*sequence)->next = NULL;
    sequenceIterable = (*sequence);
    iterable = iterable->next;
    while ((k > 0) && (iterable != NULL)){
        if (iterable->card.value != sequenceIterable->card.value){
            new = malloc(sizeof(HAND_T));
            new->card = iterable->card;
            new->next = NULL;
            sequenceIterable = sequenceIterable->next = new;
            k--;
        }
        iterable = iterable->next;
    }
    free_hand(&first);

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

    if ((*hand) == NULL) return NULL;

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

void free_hand(HAND_T **hand){
    HAND_T *next;
    if (*hand == NULL) return;

    while(*hand != NULL){
        next = (*hand)->next;
        free(*hand);
        *hand = next;
    }
}

/* Function to swap values at two pointers */
void swap(check *x, check *y)
{
    check temp;
    temp = *x;
    *x = *y;
    *y = temp;
}

/* Function to print permutations of string
   This function takes three parameters:
   1. String
   2. Starting index of the string
   3. Ending index of the string. */
void permute(check *a, int l, int r, HAND_T *hand)
{
    int i,j=0,score;
    HAND_T *copy,*copy2,*best_order = NULL;

    if (l == r){
        j=0;
        copy = copyHand(hand);
        while (j <= r){
            a[j++](&copy,&best_order);
        }
        copy2 = copyHand(best_order);
        addHandToHand(&copy2,copy);
        score = verifyPoints(copy2,false);
        if (score > high_score){
            high_score = score;
            free_hand(&best_hand);
            free_hand(&best_hand_leftover);
            best_hand = copyHand(best_order);
            best_hand_leftover = copyHand(copy);
        }
        free_hand(&best_order);
        free_hand(&copy);
        free_hand(&copy2);
    }
    else
    {
        for (i = l; i <= r; i++)
        {
            swap((a+l), (a+i));
            permute(a, l + 1, r, hand);
            swap((a+l), (a+i)); /*backtrack*/
        }
    }
}