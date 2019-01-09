/*
** Implementace překladače imperativního jazyka IFJ18
** 
** Vorobiev Nikolaj	xvorob00@stud.fit.vutbr.cz
** Luhin Artsiom	xluhin00@stud.fit.vutbr.cz
** Koša Benjamín	xkosab00@stud.fit.vutbr.cz
*/

#include "main.h"

Token token;
Token token2;
Token token_id;
Token token_f;

TSTree tree;
TSTree FakeTree;
InstrTree iTree;

Value value;
LTokenList t_list;

Param p1;
Param p2;
Param p3;

void PrevodTok() {
	while(token.id != ENDOF) {
		get_token(&token);
	}
}

int main(void){
	int result = OK;
	init();
	if ((result = get_token(&token) != OK))
		return result;
	strAddString(&token_f, "def");
	result = program();
	switch (result) {
		case OK:
			//printf("ALL GOOD\n");
			//PrevodTok();
			strClear(&token);
			strClear(&token_id);
			strClear(&token_f);
			strAddString(&token_f, "def");
			TLGetToken(&t_list, &token); // token muze byt ENDOF
			result = i_program();
			switch (result) {
				case OK:
					strClear(&token_f);
					strAddString(&token_f, "def");
					ZeroParams(&p1, &p2, &p3);

					GenerateInstruction(&iTree, token_f, POPFRAME, p1, p2, p3 );
					HeadInstr();
					TIPrintTree(&iTree);
					TIPrintDef(&iTree);
					unInit();
					return OK;

				default:
					unInit();
					return result;
			}
		case LEX_ERROR:
			printf("LEX_ERROR\n");
			unInit();
			return LEX_ERROR;

		case SYNTAX_ERROR:
			printf("SYNT_ERROR\n");
			unInit();
			return SYNTAX_ERROR;

		case SEM_ERROR:
			printf("SEM_ERROR\n");
			unInit();
			return SEM_ERROR;

		case SEM_STAT_ERROR:
			printf("SEM_STAT_ERROR\n");
			unInit();
			return SEM_STAT_ERROR;

		case SEM_ERROR_PARAMS:
			printf("SEM_ERROR_PARAMS\n");
			unInit();
			return SEM_ERROR_PARAMS;
	}
	//PrintArtList(&t_list);
    return 0;
}

/*


#include "main.h"

Token token;
Token token2;
Token token_id;
Token token_f;

TSTree tree;
TSTree FakeTree;
InstrTree iTree;

Value value;
LTokenList t_list;

Param p1;
Param p2;
Param p3;


int main(void){
	int result = OK;
do{
	if ((result = get_token(&token) != OK))
		return result;
	printf("token: %s, %d\n", token.str, token.id);
}while(token.id != ENDOF);
    return 0;
}


*/
