/*
** Implementace překladače imperativního jazyka IFJ18
** 
** Koša Benjamín	xkosab00@stud.fit.vutbr.cz
*/

//hlavickovy soubor pro praci s nekonecne dlouhymi retezci
#ifndef SCANER_H__
#define SCANER_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <stddef.h>
#include <inttypes.h>
//#include <errno.h>
#include <limits.h>
//#include <float.h>
#include "errors.h"

#define STR_LEN_INC 8

  //operatory
  #define PLUS 0 //+
  #define MINUS 1 //-
  #define TIMES 2 //*
  #define LOMENO 3 ///

//operatory ktore sa vyuzivaju aj pri vyhodnocovani vyrazov
  //zatvorky
  #define OPEN_BRACKET 4 //(
  #define CLOSE_BRACKET 5//)

  //porovnavanie
  #define LESS 6 //<
  #define GREATER 7 //>

  #define LESS_EQUAL 8 //<=
  #define GREATER_EQUAL 9 //>=

  #define EQUAL 10 //==
  #define NOT_EQUAL 11 //!=

  #define IDENTIFIER 12
  #define DOLLAR 13

//vestavene funkce
#define INPUTS 20
#define INPUTI 21
#define INPUTF 22
#define PRINT 23
#define LENGTH 24
#define SUBSTR 25
#define ORD 26
#define CHR 27

//ostatne
#define ASSIGN 30 //=
#define COMMA 52 //,
#define NOT2 36 //!

//klucove slova
#define DEF 40
#define DO 41
#define ELSE 42
#define NOT 43
#define NIL 44
#define THEN 45
#define WHILE 46
#define END 47
#define IF 48

//datove
#define INTEGER 60
#define FLOAT 61
#define STRING 62
#define BOOLEAN 63


#define EOL 90
#define ENDOF 91


typedef struct
{
  char* str;		// misto pro dany retezec ukonceny znakem '\0'
  size_t used;		// skutecna delka retezce
  size_t size;	// velikost alokovane pameti
  int id;
} Token;

int strInit(Token *s);
void strFree(Token *s);

int copyToken(Token *t1, Token *t2);

void strClear(Token *s);
int strAddChar(Token *s1, char c);
void strAddString(Token *, char *);
int strCopyString(Token *s1, Token *s2);
int strCmpString(Token *s1, Token *s2);
int strCmpConstStr(Token *s1, char *s2);

char *strGetStr(Token *s);
int strGetLength(Token *s);
int get_token(Token *token);

#endif
