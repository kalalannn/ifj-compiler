/*
** Implementace překladače imperativního jazyka IFJ18
** 
** Vorobiev Nikolaj	xvorob00@stud.fit.vutbr.cz
** Luhin Artsiom	xluhin00@stud.fit.vutbr.cz
*/

#include "parser.h"
#include "token_list.h"
#include "postfix.h"

extern TSTree tree;
extern InstrTree iTree;

extern Value value;

extern Token token;
extern Token token_id;
extern Token token_f;

extern Param p1;
extern Param p2;
extern Param p3;

void init();
void unInit();
