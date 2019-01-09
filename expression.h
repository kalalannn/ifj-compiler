/*
** Implementace překladače imperativního jazyka IFJ18
**
** Koša Benjamín	xkosab00@stud.fit.vutbr.cz
*/

#ifndef SEXPRESSION_H__
#define SEXPRESSION_H__

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "scaner.h"
#include "list.h"
#include "symtable.h"
//#include "token_list.h"
//#include "main.h"

#define MaxListLength 100

#define OPEN_HANDLE 14 //<
#define CLOSE_HANDLE 15 //>
#define E 16
#define PR_PLUS 0

#define PREC_OK 0

int pr_get_oper(int a, int b);
int pr_expressionAnalysis(int until);
int pr_finalAnalysis();

#endif
