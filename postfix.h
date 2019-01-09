/*
** Implementace překladače imperativního jazyka IFJ18
** 
** Vorobiev Nikolaj	xvorob00@stud.fit.vutbr.cz
** Luhin Artsiom	xluhin00@stud.fit.vutbr.cz
*/

#ifndef POSTFIX_H__
#define POSTFIX_H__

#include "scaner.h"
#include "stdbool.h"
#include "token_list.h"

#define SMAX 200


typedef struct tstack {
    Token array[SMAX];
    int top;
} TStack;

extern TStack t_stack;

bool pushOperator(Token *, LTokenList *);
void getPostfix(LTokenList *, int);
void sInit(TStack *);

void sPop(TStack *s, Token *t);
void sPush(TStack *s, Token *t);
Token *sTop(TStack *s);
void sInit(TStack *s);
bool sEmpty(TStack *s);
#endif

