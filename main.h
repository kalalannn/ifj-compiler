/*
** Implementace překladače imperativního jazyka IFJ18
** 
** Vorobiev Nikolaj	xvorob00@stud.fit.vutbr.cz
** Luhin Artsiom	xluhin00@stud.fit.vutbr.cz
** Koša Benjamín	xkosab00@stud.fit.vutbr.cz
*/

#ifndef MAIN_H__
#define MAIN_H__

#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stddef.h>


#include "token_list.h"
#include "p.h"
#include "p2.h"
#include "init.h"
#include "expression.h"
#include "gen_exp.h"


extern TSTree tree;
extern TSTree FakeTree;
extern InstrTree iTree;
extern LTokenList t_list;

extern Value value;

extern Token token;
extern Token token2;
extern Token token_id;
extern Token token_f;

extern Param p1;
extern Param p2;
extern Param p3;

#endif
