/*
** Implementace překladače imperativního jazyka IFJ18
** 
** Koša Benjamín	xkosab00@stud.fit.vutbr.cz
*/


#ifndef LIST_H__
#define LIST_H__


/* Předmět: Algoritmy (IAL) - FIT VUT v Brně
 * Hlavičkový soubor pro list.c (Dvousměrně vázaný lineární seznam)
 * Vytvořil: Martin Tuček, září 2005
 * Upravil: Kamil Jeřábek, září 2018
 *  
 * Tento soubor, prosíme, neupravujte!  
 */

#include<stdio.h>
#include<stdlib.h>

#define FALSE 0
#define TRUE 1

extern int errflg;
extern int solved;
 
typedef struct tDLElem {                 /* prvek dvousměrně vázaného seznamu */ 
        int data;                                            /* užitečná data */
        struct tDLElem *lptr;          /* ukazatel na předchozí prvek seznamu */
        struct tDLElem *rptr;        /* ukazatel na následující prvek seznamu */
} *tDLElemPtr;

typedef struct {                                  /* dvousměrně vázaný seznam */
    tDLElemPtr First;                      /* ukazatel na první prvek seznamu */
    tDLElemPtr Act;                     /* ukazatel na aktuální prvek seznamu */
    tDLElemPtr Last;                    /* ukazatel na posledni prvek seznamu */
} tDLList;

                                             /* prototypy jednotlivých funkcí */
void DLInitList (tDLList *);
void DLDisposeList (tDLList *);
void DLInsertFirst (tDLList *, int);
void DLInsertLast(tDLList *, int);
void DLFirst (tDLList *);
void DLLast (tDLList *);
void DLCopyFirst (tDLList *, int *);
void DLCopyLast (tDLList *, int *);
void DLDeleteFirst (tDLList *);
void DLDeleteLast (tDLList *);
void DLPostDelete (tDLList *);
void DLPreDelete (tDLList *);
void DLPostInsert (tDLList *, int);
void DLPreInsert (tDLList *, int);
void DLCopy (tDLList *, int *);
void DLActualize (tDLList *, int);
void DLSucc (tDLList *);
void DLPred (tDLList *);
int DLActive (tDLList *);

/* Konec hlavičkového souboru list.h */

#endif