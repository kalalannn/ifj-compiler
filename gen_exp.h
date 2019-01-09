/*
** Implementace překladače imperativního jazyka IFJ18
** 
** Vorobiev Nikolaj	xvorob00@stud.fit.vutbr.cz
** Luhin Artsiom	xluhin00@stud.fit.vutbr.cz
*/

#ifndef GEN_EXP_H
#define GEN_EXP_H

#include "token_list.h"
#include "stdbool.h"

int generExpression(int);
int checkInt(LTokenList *, bool *);
int checkFloat(LTokenList *, bool *);
int checkString(LTokenList *);
void str_stet(LTokenList *,int );

#endif // GEN_EXP_H
