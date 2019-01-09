/*
** Implementace překladače imperativního jazyka IFJ18
** 
** Vorobiev Nikolaj	xvorob00@stud.fit.vutbr.cz
** Luhin Artsiom	xluhin00@stud.fit.vutbr.cz
*/

#ifndef P2_H
#define P2_H

#include "symtable.h"
#include "postfix.h"
extern TStack pomocny;
extern int typ_dat;
extern Token retv;

int i_fn_def_par(void);
int i_fn_def_pars(void);
int i_fn_def_pars1(void);
int i_fn_head(void);
int i_fn_def(void);
int i_fn_def_list(void);
int i_fn_par(void);
int i_fn_pars(void);
int i_fn_pars1(void);
int i_exp_call(void);
int i_stat(void);
int i_stat_list(void);
int i_program(void);
int i_built_in_func(void);
int generExpression(int until);

int print_stat(void);
int length_stat(void);
int chr_stat(void);
int ord_stat(void);
int print_state(void);
int input_state(void);
int i_built_in_func(void);

#endif // P2_H
