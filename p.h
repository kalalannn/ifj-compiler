/*
** Implementace překladače imperativního jazyka IFJ18
** 
** Vorobiev Nikolaj	xvorob00@stud.fit.vutbr.cz
** Luhin Artsiom	xluhin00@stud.fit.vutbr.cz
*/

#ifndef P_H
#define P_H

#include "symtable.h"

extern int count_params;
extern bool zavorka;

int fn_def_par(void);
int fn_def_pars(void);
int fn_def_pars1(void);
int fn_head(void);
int fn_def(void);
int fn_def_list(void);
int fn_par(void);
int fn_pars(void);
int fn_pars1(void);
int exp_call(void);
int stat(void);
int stat_list(void);
int program(void);
int built_in_func(void);

#endif // P_H
