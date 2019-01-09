/*
** Implementace překladače imperativního jazyka IFJ18
** 
** Vorobiev Nikolaj	xvorob00@stud.fit.vutbr.cz
** Luhin Artsiom	xluhin00@stud.fit.vutbr.cz
*/

#include "symtable.h"

extern int if_counter;
extern int while_counter;

#define GF 10
#define LF 11
#define TF 12

#define INT_VAR 100
#define FLOAT_VAR 101
#define STRING_VAR 102
#define BOOLEAN_VAR 103

#define LABEL_TYP 4
#define NILL 243

#ifndef DCLR
#define DCLR -1
#endif

typedef enum instr_enum {
	ADD,
	SUB,
	MUL,
	DIV,
	IDIV,
	DEFVAR,
	LABEL,
	JUMP,
	MOVE,
	CREATEFRAME,
	PUSHFRAME,
	POPFRAME,
	CALL,
	RETURN,
	PUSHS,
	POPS,
	CLEARS,
	ADDS,
	SUBS,
	MULS,
	DIVS,
	IDIVS,
	LT,
	GT,
	EQ,
	LTS,
	GTS,
	EQS,
	AND,
	OR,
	//NOT,
	ANDS,
	ORS,
	NOTS,
	INT2FLOAT,
	INT2CHAR,
	STRI2INT,
	INT2FLOATS,
	FLOAT2INTS,
	INT2CHARS,
	STRI2INTS,
	READ,
	WRITE,
	CONCAT,
	STRLEN,
	GETCHAR,
	SETCHAR,
	TYPE,
	JUMPIFEQ,
	JUMPIFNEQ,
	JUMPIFEQS,
	JUMPIFNEQS,
	EXIT,
	BREAK,
	DPRINT
} Instr_enum;

typedef struct parametr {
	Token param;
	bool defined;
} Param;

typedef struct instruction {
	Token instr;
	Param par1;
	Param par2;
	Param par3;
	struct instruction *next;
} *Instruction;

typedef struct ilist {
	Instruction First;			// Prvni prom
	Instruction Act;				// Actual prom
	Instruction Last;			// Last prom
} iList;

typedef struct instrTree { 		//indifier
	Token Name_f;			  		//name of funkcion
	iList List;					//instrukce
	struct instrTree * LPtr;                                    /* levý podstrom */
	struct instrTree * RPtr;                                   /* pravý podstrom */
} *InstrTree;


/*
 * def num a, b
 * 	  foo a
 * end
 *
 * def foo a
 * 	 write a
 * end
 *
 */

//void GenerateInstruction(Strom * strom, Token f_name, Instr_enum instr, Param par1, Param par2, Param par3)
void GenConst(Token , Param *);
void GenId(Token , Param *);
void GenLabel(Token , Param *);
void GenNil(Param *);
void GenIfLabel(Param *par, int typ, int cykl);

void HeadInstr(void);

void ZeroParams(Param *, Param *, Param *);
void OneParams(Param *, Param *, Param *);
void TwoParams(Param *, Param *, Param *);
void ThreeParams(Param *, Param *, Param *);

void TIInsertFunction (InstrTree *RootPtr, Token name_f);
void TIInit (InstrTree *RootPtr);
void TIListInit (iList *list);
void TIDispose (InstrTree *RootPtr);
void TIAddInstruction (InstrTree *RootPtr, Token name_f, Instruction instruct);
void GenerateInstruction(InstrTree *RootPtr, Token name_f, Instr_enum instr, Param par1,\
							 Param par2, Param par3);
void TIPrintTree(InstrTree *RootPtr);
void TIPrintList(iList *list, Token *);
void TIPrintDef(InstrTree *);


