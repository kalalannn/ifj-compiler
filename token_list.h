/*
** Implementace překladače imperativního jazyka IFJ18
** 
** Vorobiev Nikolaj	xvorob00@stud.fit.vutbr.cz
** Luhin Artsiom	xluhin00@stud.fit.vutbr.cz
*/


#ifndef TOKEN_LIST_H__
#define TOKEN_LIST_H__


/* Předmět: Algoritmy (IAL) - FIT VUT v Brně
 * Hlavičkový soubor pro list.c (Dvousměrně vázaný lineární seznam)
 * Vytvořil: Martin Tuček, září 2005
 * Upravil: Kamil Jeřábek, září 2018
 *  
 * Tento soubor, prosíme, neupravujte!  
 */

#include<stdio.h>
#include<stdlib.h>
#include"scaner.h"

#define FALSE 0
#define TRUE 1

extern int errflg;
extern int solved;
 
typedef struct LToken {                 /* prvek dvousměrně vázaného seznamu */
    Token token;
    struct LToken *next;          /* ukazatel na předchozí prvek seznamu */
} *LTokenPtr;


typedef struct {                                  /* dvousměrně vázaný seznam */
    LTokenPtr first;                      /* ukazatel na první prvek seznamu */
    LTokenPtr act;                     /* ukazatel na aktuální prvek seznamu */
    LTokenPtr last;                    /* ukazatel na posledni prvek seznamu */
} LTokenList;

                                             /* prototypy jednotlivých funkcí */
void TLInitList (LTokenList *);
void TLDisposeList (LTokenList *);
void TLInsert (LTokenList *, Token *);
void TLFirst(LTokenList *);
void TLGetToken(LTokenList *, Token *);
void PrintArtList(LTokenList *);

/* Konec hlavičkového souboru list.h */
#endif
