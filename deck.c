#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "deck.h"
#include "utf8.h"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "missing_default_case"
void printCard(CARD_T card){

    unsigned char  ch[UTF8_MAXLEN + 1];
    unsigned int   i;
    size_t         len;

    switch (card.suit){
        case DIAMOND:
            printf("\033[1;31m");
            i = 0x1F0C1 + (unsigned int)card.value - 0x1;
            break;
        case HEART:
            printf("\033[0;31m");
            i = 0x1F0B1 + (unsigned int)card.value - 0x1;
            break;
        case CLUB:
            printf("\033[1;37m");
            i = 0x1F0D1 + (unsigned int)card.value - 0x1;
            break;
        case SPADE:
        default:
            printf("\033[1;37m");
            i = 0x1F0A1 + (unsigned int)card.value - 0x1;
            break;
    }

    if (card.value > 11) i++; /* skip knight card */

    len = code_to_utf8(ch, i);
    if (len > 0) {
        ch[len] = '\0';
        printf("%s", ch);
    }

    printf("\033[1;37m");
}

/*void printDeck(CARD_T **cards){
    int i;
    for(i=0; i<DECK_SIZE; i++)
        printCard(*cards[i]);
    return;
}*/

void printHands(HAND_T *hand){
    HAND_T *pt_hand;
    pt_hand = hand;
    while(pt_hand){
        printCard(pt_hand->card);
        pt_hand = pt_hand->next;
    }
    return;
}

/* Cria um baralho padrao de 52 cartas em um vetor passado por referencia */
void createDeck(CARD_T **cards){
    int i, j=0;
    SUIT_T s;
    /* geração do baralho ordenado */
    for(s=DIAMOND; s<=CLUB; s++){
        for(i=1; i<=13; i++){
            cards[j] = malloc(sizeof(CARD_T));
            cards[j]->value = i;
            cards[j]->suit  = s;
            j++;
        }
    }
    return;
}

/* Embaralha as 52 cartas constantes em um vetor passado por referencia */
void shuffle(CARD_T **cards, unsigned int seed){
    int i, j, aux_v;
    SUIT_T aux_n;
    /* EMBARALHAMENTO */
    srand(seed);
    for(i=0; i<DECK_SIZE; i++){
        j = rand() % DECK_SIZE;
        aux_v = cards[i]->value;
        aux_n = cards[i]->suit;
        cards[i]->value = cards[j]->value;
        cards[i]->suit = cards[j]->suit;
        cards[j]->value = aux_v;
        cards[j]->suit = aux_n;
    }
    return;
}


int countPoints(HAND_T *hand){
    char* desc;
    desc = malloc(sizeof(char)*30);
    return countPointsWithDescription(hand,desc);
}

/* Conta os pontos de uma mao de 5 cartas, de acordo com as regras de pontuacao. */
/* A mao deve vir ordenada, por value da carta, em ordem ascendente. */
/* (IMPORTANTE: para fins desta ordenacao, o As vale 1) */
int countPointsWithDescription(HAND_T *hand,char* handName){
    HAND_T *ptrAux;
    int cont=0, flush=0, straight=0, royalStraight=0;
    int cont2=0;

    /* verifica Flush */
    for(ptrAux = hand->next; ptrAux != NULL; ptrAux = ptrAux->next){
        if(ptrAux->card.suit == hand->card.suit)
            cont ++;
    }
    if(cont == 4)
        flush = 1; /* é um flush */

    /* verifica Royal Straight */
    ptrAux = hand;
    if(ptrAux->card.value == 1 && ptrAux->next->card.value == 10){
        /* somente pode ser royal se a 1a carta é ás e 2a é 10 */
        for(ptrAux = hand->next, cont=0; ptrAux->next != NULL; ptrAux = ptrAux->next){
            if(ptrAux->card.value == (ptrAux->next->card.value - 1))
                cont ++;
        }
        if(cont == 3)
            royalStraight = 1; /* é um royal straight */
    }

    if(flush && royalStraight) /* mão é um royal straight flush */
    {
        strcpy(handName,"Royal Straight Flush");
        return SCORE_ROYAL_STRAIGHT_FLUSH;
    }

    /* verifica Straight simples */
    for(ptrAux = hand, cont=0; ptrAux->next != NULL; ptrAux = ptrAux->next){
        if(ptrAux->card.value == (ptrAux->next->card.value - 1))
            cont ++;
    }
    if(cont == 4)
        straight = 1; /* é um straight simples */

    if(flush && straight) /* mão é um straight flush */
    {
        strcpy(handName,"Straight Flush");
        return SCORE_STRAIGHT_FLUSH;
    }

    if(royalStraight || straight) /* mão é um straight */
    {
        strcpy(handName,"Straight");
        return SCORE_STRAIGHT;
    }
    if(flush) /* mão é flush */
    {
        strcpy(handName,"Flush");
        return SCORE_FLUSH;
    }

    /* verifica pares, trincas e quadras */
    cont = cont2 = 0;
    ptrAux = hand;
    while((ptrAux->next != NULL) && (ptrAux->card.value != ptrAux->next->card.value))
        ptrAux = ptrAux->next;
    while((ptrAux->next != NULL) && (ptrAux->card.value == ptrAux->next->card.value)){
        cont++;
        ptrAux = ptrAux->next;
    }
    while((ptrAux->next != NULL) && (ptrAux->card.value != ptrAux->next->card.value))
        ptrAux = ptrAux->next;
    while((ptrAux->next != NULL) && (ptrAux->card.value == ptrAux->next->card.value)){
        cont2++;
        ptrAux = ptrAux->next;
    }
    /* se foi encontrada algum par, trinca, four ou full house, é necessário */
    /* fazer cont e cont2 conter numero correto de cartas iguais encontradas */
    if(cont > 0) /* normalizando value do primeiro contador */
        cont++;
    if(cont2 > 0) /* normalizando value do segundo contador */
        cont2++;
    /* após normalizaçao, cont e cont2 podem ser usados para verificacao */
    switch(cont+cont2){
        case 5: /* mão é um full house */
            strcpy(handName,"Full house");
            return SCORE_FULL_HOUSE;
        case 4: /* four ou dois pares */
            if(cont == 4 || cont2 == 4) {
                strcpy(handName, "4 of a kind");
                return SCORE_FOUR_OF_A_KIND;
            } else {
                strcpy(handName, "Two Pairs");
                return SCORE_TWO_PAIRS;
            }
        case 3: /* trinca */
            strcpy(handName, "3 of a Kind");
            return SCORE_THREE_OF_A_KIND;
        case 2: /* par simples */
            strcpy(handName, "Pair");
            return SCORE_PAIR;
    }
    strcpy(handName, "High Card");
    return SCORE_HIGH_CARD;
}

#pragma clang diagnostic pop