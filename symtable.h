/*
** Implementace překladače imperativního jazyka IFJ18
** 
** Vorobiev Nikolaj	xvorob00@stud.fit.vutbr.cz
** Luhin Artsiom	xluhin00@stud.fit.vutbr.cz
*/


#ifndef SYMTABLE_H__
#define SYMTABLE_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>

#include "scaner.h"

#define TRUE 1
#define FALSE 0

#define PROM 0
#define PARAM 1
#define RETVAL 2

#define DCLR -1
#define DP 320


typedef union retType { //toto je obsah promene
	Token String;
	int Integer;
	float Float;
	bool Bool;
} Value;

typedef struct id { 	//toto je promena
	Token name;			//jmeno
	struct id *next;	//pristi promena(realiz pomoci listu)
	int type;			// PROM , PARAM, RETVAL, DCLR
	int data_type;		// STR , INT , FLO , DCLR
	Value val;			//obsah
} *Id;

typedef struct list {
	Id First;			// Prvni prom
	Id Act;				// Actual prom
	Id Last;			// Last prom
} sList;


typedef struct symbTree { 		//indifier
	Token Key;			  		//name of funkcion
	sList list;					//promene
	struct symbTree * LPtr;                                    /* levý podstrom */
	struct symbTree * RPtr;                                   /* pravý podstrom */
} *TSTree;

/* prototypy funkcí */

int TSListCountParams(sList *list); 	//neee
int TSCountParams(TSTree *RootPtr, Token name_f);	//tuto pouzivame

TSTree TSSearchFunction (TSTree *RootPtr, Token name_f); 	//tuto taky
Id TSSearchId(TSTree *RootPtr, Token name_f, Token name_id);	//tuto pouzivame

void TSListInit (sList *); 	//neeee
void TSInit (TSTree *);	//tuto pouzivame

void TSInsertFunction (TSTree *RootPtr, Token name_f);	//tuto pouzivame

void TSListInsertId (sList *list, Token name_id, int type,int data_type, Value val); //neeee
void TSInsertId (TSTree* RootPtr, Token name_f, Token name_id, int type, int data_type, Value value ) ;	//tuto pouzivame

void TSPrintList(sList *); //neeee
void TSPrintTree(TSTree * RootPtr);	//tuto pouzivame

void TSDisposeList (sList *); 	///neeee
void TSDispose(TSTree *);	//tuto pouzivame
int getType(TSTree * RootPtr, Token nameF, Token nameId);
void ChangeDataType(TSTree *RootPtr, Token nameF, Token nameId, int data_type) ;
void ChangeTypParam(TSTree *RootPtr, Token *name_f, int NumPar, int data_type);

int getType (TSTree *, Token, Token);

#endif
