/*
** Implementace překladače imperativního jazyka IFJ18
** 
** Vorobiev Nikolaj	xvorob00@stud.fit.vutbr.cz
** Luhin Artsiom	xluhin00@stud.fit.vutbr.cz
*/

#include <stdio.h>
#include <stdlib.h>

#include "p2.h"
#include "token_list.h"
#include "itable.h"
#include "symtable.h"

extern Token token;
extern Token token_id;
extern Token token_f;
Token retv;
extern LTokenList t_list;

extern InstrTree iTree;
extern TSTree tree;
extern Param p1;
extern Param p2;
extern Param p3;
extern int result;

int typ_dat;
TStack pomocny;


int i_fn_def_pars() {
	switch (token.id) {
    case IDENTIFIER:
		GenId(token, &p1);
		OneParams(&p1, &p2, &p3);
		GenerateInstruction(&iTree, token_f, DEFVAR, p1, p2, p3);
		sPush(&pomocny, &token);

		TLGetToken(&t_list, &token);
		result = i_fn_def_pars1();
		if (result != OK)
			return result;
	break;

    case CLOSE_BRACKET:

			OneParams(&p1, &p2, &p3);
			while(sEmpty(&pomocny) == false) {
				sPop(&pomocny, &token);
				GenId(token, &p1);
				GenerateInstruction(&iTree, token_f, POPS, p1, p2, p3);
			}

		TLGetToken(&t_list, &token);
        break;
    }

	return OK;
}

int i_fn_def_pars1() {
	switch (token.id) {
    case COMMA:
		TLGetToken(&t_list, &token); // token == id
		//TLGetToken(&t_list, &token);	// token == comma/bracket/eol

		return i_fn_def_pars(); // CHECK IT!!

    case CLOSE_BRACKET:
			while(sEmpty(&pomocny) == false) {
				sPop(&pomocny, &token);
				GenId(token, &p1);
				GenerateInstruction(&iTree, token_f, POPS, p1, p2, p3);
			}
		TLGetToken(&t_list, &token); // token == eol
        break;

    case EOL:
        break;
    }
	return OK;
}

int i_fn_head() {

	copyToken(&token_id, &token); //jmeno funkci
	TLGetToken(&t_list, &token); // token == (
	TLGetToken(&t_list, &token); // token == id nebo )

	result = i_fn_def_pars();
	if (result != OK)
		return result;
	return OK;
}


int i_fn_def() {

	TLGetToken(&t_list, &token); // token == id
	copyToken(&token_f, &token);

	GenLabel(token, &p1);
	OneParams(&p1, &p2, &p3);
	GenerateInstruction(&iTree, token_f, LABEL, p1, p2, p3);

	ZeroParams(&p1, &p2, &p3);
	GenerateInstruction(&iTree, token_f, CREATEFRAME, p1, p2, p3);
	GenerateInstruction(&iTree, token_f, PUSHFRAME, p1, p2, p3);

	GenId(retv, &p1);
	OneParams(&p1, &p2, &p3);
	GenerateInstruction(&iTree, token_f, DEFVAR, p1, p2, p3);

	GenNil(&p2);
	TwoParams(&p1, &p2, &p3);
	GenerateInstruction(&iTree, token_f, MOVE, p1, p2, p3);

	result = i_fn_head();
	if (result != OK)
		return result;

	//TLGetToken(&t_list, &token); // token == EOL
	result = i_stat_list();
	if (result != OK)
		return result;

	ZeroParams(&p1, &p2, &p3);
	GenerateInstruction(&iTree, token_f, CLEARS, p1, p2, p3);

	GenId(retv, &p1);
	OneParams(&p1, &p2, &p3);
	GenerateInstruction(&iTree, token_f, PUSHS, p1, p2, p3);

	ZeroParams(&p1, &p2, &p3);
	GenerateInstruction(&iTree, token_f, POPFRAME, p1, p2, p3);
	GenerateInstruction(&iTree, token_f, RETURN, p1, p2, p3);

	strClear(&token_f );
	strAddString(&token_f, "def" ); //end



	//TLGetToken(&t_list, &token); // token == EOL
	return OK;
}

int i_fn_def_list() {
	switch (token.id) {
    case DEF:
		result = i_fn_def();
		if (result != OK)
			return result;

		TLGetToken(&t_list, &token);
		result = i_fn_def_list();
		if (result != OK)
			return result;
			break;


    case EOL:
		//TLGetToken(&t_list, &token);
		//i_fn_def_list();
			break;

    default:
        break; // muze nasledovat token, ktery vyhovuje stat_list()
    }
	return OK;
}
int i_fn_pars() {
	switch (token.id) { //id = id (token_id) id(token), get(token)
						//id = id Int
	case IDENTIFIER: //id id
			GenId(token, &p1);
			OneParams(&p1, &p2, &p3);
			GenerateInstruction(&iTree, token_f, PUSHS, p1, p2, p3);

			TLGetToken(&t_list, &token);
			result = i_fn_pars1();
			if (result != OK)
				return result;

			break;

	case INTEGER:
	case FLOAT:
	case STRING:

			GenConst(token, &p1);
			OneParams(&p1, &p2, &p3);
			GenerateInstruction(&iTree, token_f, PUSHS, p1, p2, p3);
			TLGetToken(&t_list, &token);
			result = i_fn_pars1();
			if (result != OK)
				return result;

			break;

	case NIL:
			GenNil(&p1);
			OneParams(&p1, &p2, &p3);
			GenerateInstruction(&iTree, token_f, PUSHS, p1, p2, p3);
			TLGetToken(&t_list, &token);
			result = i_fn_pars1();
			if (result != OK)
				return result;

			break;

    case CLOSE_BRACKET:
		TLGetToken(&t_list, &token); // token == EOL
        break;

    case EOL:
		GenLabel(token_id, &p1);
		OneParams(&p1, &p2, &p3);
		GenerateInstruction(&iTree, token_f, CALL, p1, p2, p3);
		if(t_list.first->next->token.id == ASSIGN) {
			GenId(t_list.first->token, &p1);
			OneParams(&p1, &p2, &p3);
			GenerateInstruction(&iTree, token_f, POPS, p1, p2, p3);
		}

        break;
    }
	return OK;
}

int i_fn_pars1() { //id = id id, igf, jh
	switch (token.id) {
    case COMMA:
		TLGetToken(&t_list, &token); // token == id/int/float/string
		//TLGetToken(&t_list, &token);	// token = comma/bracket/eol
		result = i_fn_pars(); 	// CHECK IT!!
		if (result != OK)
			return result;

        break;

    case CLOSE_BRACKET:
		TLGetToken(&t_list, &token);	// token == eol
		result = i_fn_pars1();
		if (result != OK)
			return result;
		break;

    case EOL:
		GenLabel(token_id, &p1);
		OneParams(&p1, &p2, &p3);
		GenerateInstruction(&iTree, token_f, CALL, p1, p2, p3);

		if(t_list.first->next->token.id == ASSIGN){
			GenId(t_list.first->token, &p1);
			OneParams(&p1, &p2, &p3);
			GenerateInstruction(&iTree, token_f, POPS, p1, p2, p3);
		}

        break;
    }

	return OK;
}

int i_exp_call() {
	switch (token.id) {
	case INPUTS: //id (=) (tok)
    case INPUTI:
    case INPUTF:
			result = input_state();
			if (result != OK)
				return result;
			break;
    case PRINT:
			result = print_stat();
			if (result != OK)
				return result;

			break;
    case LENGTH:
			result = length_stat();
			if (result != OK)
				return result;
			break;
    case SUBSTR:
			i_built_in_func();
			break;
    case ORD:
		result = ord_stat();
		if (result != OK)
			return result;
		break;
    case CHR:
		result = chr_stat();
		if (result != OK)
			return result;
		break;

	case IDENTIFIER: // id = id(token_id) (token)
		copyToken(&token_id, &token);
		TLGetToken(&t_list, &token);
		switch (token.id) {

			case OPEN_BRACKET: //volani funkci
				TLGetToken(&t_list, &token);
				result = i_fn_pars();
				if (result != OK)
					return result;
			break;

			case IDENTIFIER: //taky volani funcki
			case INTEGER:
			case FLOAT:
			case STRING:
				result = i_fn_pars();
				if (result != OK)
					return result;
			break;

			case PLUS :
			case MINUS :
			case LESS :
			case GREATER :
			case LESS_EQUAL :
			case GREATER_EQUAL :
			case TIMES:
			case LOMENO:
			case EQUAL:
			case NOT_EQUAL:
					typ_dat = generExpression(EOL);
					if(typ_dat == SEM_STAT_ERROR) {
						return typ_dat;
					}
					else if(t_list.first->next->token.id == ASSIGN) {
						ChangeDataType(&tree, token_f, t_list.first->token, typ_dat);
						GenId(t_list.first->token, &p1);
						OneParams(&p1, &p2, &p3);
						GenerateInstruction(&iTree, token_f, POPS, p1, p2, p3);
					}
					return OK;

			case EOL:
				if (TSSearchFunction(&tree, token_id) != NULL) {

				}
				else if(TSSearchId(&tree, token_f, token_id) != NULL) {
					typ_dat = generExpression(EOL);
					if(typ_dat == SEM_STAT_ERROR) {
						return typ_dat;
					}
					else if(t_list.first->next->token.id == ASSIGN) {
						ChangeDataType(&tree, token_f, t_list.first->token, typ_dat);
						GenId(t_list.first->token, &p1);
						OneParams(&p1, &p2, &p3);
						GenerateInstruction(&iTree, token_f, POPS, p1, p2, p3);
					}
					return OK;
				}
				break;

			default:
				break;

		}
			break;


    case INTEGER:
    case FLOAT:
    case STRING:
	case OPEN_BRACKET:
        // tady volame funkci na zpracovani vyrazu
		copyToken(&token_id, &token);
		TLGetToken(&t_list, &token);
		typ_dat = generExpression(EOL);
		if(typ_dat == SEM_STAT_ERROR) {
			return typ_dat;
		}
		else if(t_list.first->next->token.id == ASSIGN) {
			ChangeDataType(&tree, token_f, t_list.first->token, typ_dat);
			GenId(t_list.first->token, &p1);
			OneParams(&p1, &p2, &p3);
			GenerateInstruction(&iTree, token_f, POPS, p1, p2, p3);

			if(strCmpConstStr(&token_f, "def") != 0) {
				GenId(retv, &p1);
				GenId(t_list.first->token, &p2);
				TwoParams(&p1, &p2, &p3);
				GenerateInstruction(&iTree, token_f, MOVE, p1, p2, p3);
			}
		}
		else if(strCmpConstStr(&token_f, "def") != 0) {
			GenId(retv, &p1);
			OneParams(&p1, &p2, &p3);
			GenerateInstruction(&iTree, token_f, POPS, p1, p2, p3);
		}
		return OK;

    default:
        break;		// vracime se zpet do stat_list
    }
	return  OK;
}

int print_stat() {
	TLGetToken(&t_list, &token);
	switch (token.id) {
		case COMMA:
		case OPEN_BRACKET:
		case CLOSE_BRACKET:
			result = print_stat();
			if (result != OK)
				return result;
			break;

		case IDENTIFIER:
			/*if(getType(&tree, token_f, token) == NIL ||\
					getType(&tree, token_f, token) == DCLR){
				return SEM_STAT_ERROR;
			}*/
			//else {
				GenId(token, &p1);
				OneParams(&p1, &p2, &p3);
				GenerateInstruction(&iTree, token_f, WRITE, p1, p2, p3);
			//}
			result = print_stat();
			if (result != OK)
				return result;
			break;

		case INTEGER:
		case FLOAT:
		case STRING:
			GenConst(token, &p1);
			OneParams(&p1, &p2, &p3);
			GenerateInstruction(&iTree, token_f, WRITE, p1, p2, p3);
			result = print_stat();
			if (result != OK)
				return result;
			break;

		case EOL:
			if(t_list.first->next->token.id == ASSIGN) {
				 GenId(t_list.first->token, &p1);
				 GenNil(&p2);
				 TwoParams(&p1, &p2, &p3);
				 GenerateInstruction(&iTree, token_f, MOVE, p1, p2, p3);
				 ChangeDataType(&tree, token_f, t_list.first->token, NILL);
			}
			return OK;
	}
	return OK;
}

int length_stat() {
		TLGetToken(&t_list, &token);
		switch (token.id) {
			case OPEN_BRACKET:
				result = length_stat();
				if (result != OK)
					return result;
				break;

			case STRING :
				break;

			case IDENTIFIER:
			if(getType(&tree, token_f, token) != STRING ) {
				return SEM_STAT_ERROR;
			}
				break;

			case INTEGER:
			case FLOAT:
				return SEM_STAT_ERROR;

		}
		if(t_list.first->token.id == LENGTH) {
			while (token.id != EOL)
				TLGetToken(&t_list, &token);
		}
		else {
			if(token.id == IDENTIFIER) {
				GenId(t_list.first->token, &p1);
				GenId(token, &p2);
				TwoParams(&p1, &p2, &p3);
				GenerateInstruction(&iTree, token_f, STRLEN, p1, p2, p3);

			}
			else if (token.id == STRING) {
				GenId(t_list.first->token, &p1);
				GenConst(token, &p2);
				TwoParams(&p1, &p2, &p3);
				GenerateInstruction(&iTree, token_f, STRLEN, p1, p2, p3);
			}
			ChangeDataType(&tree, token_f, t_list.first->token, INTEGER);
			while (token.id != EOL) {
				TLGetToken(&t_list, &token);
			}
		}
		while (token.id != EOL)
			TLGetToken(&t_list, &token);
		return OK;
}

int input_state() { //dopsat
	if(t_list.first->next->token.id == ASSIGN) {
		GenId(t_list.first->token, &p1);
		TwoParams(&p1, &p2, &p3);
		strClear(&p2.param);
		switch(token.id) {
			case INPUTI:
				strAddString(&p2.param, "int");
				ChangeDataType(&tree, token_f, t_list.first->token, INTEGER);
				break;

			case INPUTF:
				strAddString(&p2.param, "float");
				ChangeDataType(&tree, token_f, t_list.first->token, FLOAT);
				break;

			case INPUTS:
				strAddString(&p2.param, "string");
				ChangeDataType(&tree, token_f, t_list.first->token, STRING);
				break;
		}
		GenerateInstruction(&iTree, token_f, READ, p1, p2, p3);
	}

	while (token.id != EOL)		// EDIT!!
		TLGetToken(&t_list, &token);
	return OK;
}

int ord_stat() {
	TLGetToken(&t_list, &token);
	Token tmp;
	strInit(&tmp);

	switch (token.id) {
	case OPEN_BRACKET:
		result = ord_stat();
		if (result != OK)
			return result;
		break;

	case IDENTIFIER:
		copyToken(&tmp, &token);
		if (getType(&tree, token_f, token) != STRING)
			return SEM_STAT_ERROR;
		TLGetToken(&t_list, &token); // comma
		TLGetToken(&t_list, &token);
		switch (token.id) {
		case INTEGER:
			break;
		case IDENTIFIER:
			if (getType(&tree, token_f, token) != INTEGER)
				return SEM_STAT_ERROR;
			break;
		default:
			return SEM_STAT_ERROR;
		}

		break;

	case STRING:
		copyToken(&tmp, &token);
		TLGetToken(&t_list, &token); // comma
		TLGetToken(&t_list, &token);
		switch (token.id) {
		case INTEGER:
			break;
		case IDENTIFIER:
			if (getType(&tree, token_f, token) != INTEGER)
				return SEM_STAT_ERROR;
			break;
		default:
			return SEM_STAT_ERROR;
		}

		break;


	default:
		return SEM_STAT_ERROR;
	}


	if (t_list.first->token.id == ORD) {
		while (token.id != EOL) {
			TLGetToken(&t_list, &token);
		}
	} else {
		if (token.id == IDENTIFIER) {
			GenId(t_list.first->token, &p1);
			if (tmp.id == IDENTIFIER) {
				GenId(tmp, &p2);
			} else {
				GenConst(tmp, &p2); // const string
			}
			GenId(token, &p3);
			ThreeParams(&p1, &p2, &p3);
			GenerateInstruction(&iTree, token_f, GETCHAR, p1, p2, p3);
		} else if (token.id == INTEGER) {
			GenId(t_list.first->token, &p1);
			if (tmp.id == IDENTIFIER) {
				GenId(tmp, &p2);
			} else {
				GenConst(tmp, &p2); // const string
			}
			GenConst(token, &p3);
			ThreeParams(&p1, &p2, &p3);
			GenerateInstruction(&iTree, token_f, GETCHAR, p1, p2, p3);
		}
		ChangeDataType(&tree, token_f, t_list.first->token, STRING);
		while (token.id != EOL) {
			TLGetToken(&t_list, &token);
		}
	}

	strFree(&tmp);

	return OK;
}


int chr_stat() {
	TLGetToken(&t_list, &token);

	switch (token.id) {
	case OPEN_BRACKET:
		TLGetToken(&t_list, &token);
		if (token.id == INTEGER)
			break;
		if (token.id == IDENTIFIER) {
			if (getType(&tree, token_f, token) != INTEGER)
				return SEM_STAT_ERROR;
		}
		break;

	case IDENTIFIER:
		if (getType(&tree, token_f, token) != INTEGER)
			return SEM_STAT_ERROR;
		break;

	case INTEGER:
		break;

	default:
		return SEM_STAT_ERROR;
	}

	if (atoi(token.str) < 0 || atoi(token.str) > 255)
		return INERT_ERR;		// chyba 58

	if (t_list.first->token.id == CHR) {
		while (token.id != EOL) {
			TLGetToken(&t_list, &token);
		}
	} else {
		if (token.id == IDENTIFIER) {
			GenId(t_list.first->token, &p1);
			GenId(token, &p2);
			TwoParams(&p1, &p2, &p3);
			GenerateInstruction(&iTree, token_f, INT2CHAR, p1, p2, p3);
		} else if (token.id == INTEGER) {
			GenId(t_list.first->token, &p1);
			GenConst(token, &p2);
			TwoParams(&p1, &p2, &p3);
			GenerateInstruction(&iTree, token_f, INT2CHAR, p1, p2, p3);
		}
		ChangeDataType(&tree, token_f, t_list.first->token, STRING);
		while (token.id != EOL) {
			TLGetToken(&t_list, &token);
		}
	}

	return OK;
}

int i_stat() {
	switch (token.id) {
    case INPUTS:
    case INPUTI:
    case INPUTF:
		result = input_state();
		if(result != OK)
			return result;
		break;

	case PRINT:
		result = print_stat();
		if(result != OK)
			return result;
		break;

    case LENGTH:
		result = length_stat();
		if(result != OK)
			return result;
		break;
	case SUBSTR:
			i_built_in_func();
			break;
    case ORD:
			result = ord_stat();
			if (result != OK)
				return result;
	case CHR:
			result = chr_stat();
			if (result != OK)
				return result;


        break;

	case INTEGER: //id id
	case FLOAT: //id float
	case STRING: //id string
    case OPEN_BRACKET:

		result = generExpression(EOL);
		while (token.id != EOL)		// EDIT!!
			TLGetToken(&t_list, &token);
		break;


    case IDENTIFIER:
		copyToken(&token_id, &token);
		TLGetToken(&t_list, &token);
		switch (token.id) {
			case ASSIGN: //id =
				if(getType(&tree, token_f, token_id) == DCLR) {
					GenId(token_id, &p1);
					OneParams(&p1, &p2, &p3);
					GenerateInstruction(&iTree, token_f, DEFVAR, p1, p2, p3);

					GenNil(&p2);
					TwoParams(&p1, &p2, &p3);
					GenerateInstruction(&iTree, token_f, MOVE, p1, p2, p3);
					ChangeDataType(&tree, token_f, token_id, NIL);

				}

				TLGetToken(&t_list, &token);
				result = i_exp_call();
				if (result != OK)
					return result;
			return OK;

			case OPEN_BRACKET: //id (
				TLGetToken(&t_list, &token);
				result = i_fn_pars();
				if (result != OK)
					return result;
			break;

			case IDENTIFIER:
			case INTEGER:
			case FLOAT:
			case STRING:
				result = i_fn_pars();
				if (result != OK)
					return result;
			break;

			case PLUS :
			case MINUS :
			case LESS :
			case GREATER :
			case LESS_EQUAL :
			case GREATER_EQUAL :
			case TIMES:
			case LOMENO:
			case EQUAL:
			case NOT_EQUAL:
					typ_dat = generExpression(EOL);
					if(typ_dat == SEM_STAT_ERROR) {
						return typ_dat;
					}
					else if(t_list.first->next->token.id == ASSIGN) {
						ChangeDataType(&tree, token_f, t_list.first->token, typ_dat);
						GenId(t_list.first->token, &p1);
						OneParams(&p1, &p2, &p3);
						GenerateInstruction(&iTree, token_f, POPS, p1, p2, p3);
					}
					else if(strCmpConstStr(&token_f, "def") != 0) {
						GenId(retv, &p1);
						OneParams(&p1, &p2, &p3);
						GenerateInstruction(&iTree, token_f, POPS, p1, p2, p3);
					}
					return OK;

			case EOL:
				if(getType(&tree, token_f, token_id) == DCLR) {
					GenId(token_id, &p1);
					OneParams(&p1, &p2, &p3);
					GenerateInstruction(&iTree, token_f, DEFVAR, p1, p2, p3);

					GenId(token_id, &p1);
					GenNil(&p2);
					TwoParams(&p1, &p2, &p3);
					GenerateInstruction(&iTree, token_f, MOVE, p1, p2, p3);
					ChangeDataType(&tree, token_f, token_id, NIL);
				}
				return OK;

			default:
				break;

		}
			break;

    case IF:
		TLGetToken(&t_list, &token);
		copyToken(&token_id, &token);

		typ_dat = generExpression(THEN);
		//na zasobike hodnota nebo id

		Param _if;
		Param _else;
		Param _end;

		strInit(&_if.param);
		strInit(&_else.param);
		strInit(&_end.param);

		GenIfLabel(&_if, IF, IF);
		GenIfLabel(&_else, ELSE, IF);
		GenIfLabel(&_end, END, IF);



		switch(typ_dat){
			case SEM_STAT_ERROR:
				return SEM_STAT_ERROR;

			case BOOLEAN:
				strClear(&p1.param);
				strAddString(&p1.param, "bool@false");
				OneParams(&p1, &p2, &p3);
				GenerateInstruction(&iTree, token_f, PUSHS, p1, p2, p3);

				OneParams(&_else, &p2, &p3);
				GenerateInstruction(&iTree, token_f, JUMPIFEQS, _else, p2, p3);
				break;

			case NIL:
				OneParams(&_else, &p2, &p3);
				GenerateInstruction(&iTree, token_f, JUMP, _else, p2, p3);
				break;

			case INTEGER:
			case FLOAT:
			case STRING:
				OneParams(&p1, &p2, &p3);
				GenerateInstruction(&iTree, token_f, JUMP, _if, p2, p3);
				break;

		}

		OneParams(&_if, &p2, &p3);
		GenerateInstruction(&iTree, token_f, LABEL, _if, p2, p3);

		TLGetToken(&t_list, &token); // token == END

		result = i_stat_list(); //EOL
		if (result != OK)
			return result;

		OneParams(&_end, &p2, &p3);
		GenerateInstruction(&iTree, token_f, JUMP, _end, p2, p3);


		OneParams(&_else, &p2, &p3);
		GenerateInstruction(&iTree, token_f, LABEL, _else, p2, p3);


		TLGetToken(&t_list, &token); // token == ELSE
		TLGetToken(&t_list, &token); // token == EOL

		result = i_stat_list();
		if (result != OK)
			return result;

		OneParams(&_end, &p2, &p3);
		GenerateInstruction(&iTree, token_f, LABEL, _end, p2, p3);


		TLGetToken(&t_list, &token); // token == END
		TLGetToken(&t_list, &token); // token == EOL

		strFree(&_if.param);
		strFree(&_else.param);
		strFree(&_end.param);

        break;
        /*
        if (token.id != EOL)		// muze byt tady ENDOF???
            return SYNTAX_ERROR;
            */

    case WHILE:
		TLGetToken(&t_list, &token);

        // tady volame funkci na zpracovani vyrazu
		while (token.id != DO) {
			TLGetToken(&t_list, &token);		// EDIT!!
        }

		TLGetToken(&t_list, &token);	// token == EOL

		result = i_stat_list();
		if (result != OK)
			return result;

		TLGetToken(&t_list, &token);	// token == END
		TLGetToken(&t_list, &token);	// token == EOL

        break;


    default:
        break;		// vracime se zpet do stat_list

    }
	return OK;

}

int i_stat_list() {
	switch (token.id) {
	case IDENTIFIER: // a
	case IF: //if
	case WHILE: //while
	case OPEN_BRACKET: // (

	case INPUTS: //inputs
    case INPUTI:
    case INPUTF:
    case PRINT:
    case LENGTH:
    case SUBSTR:
    case ORD:
    case CHR:

		result = i_stat();
		if(result != OK)
			return result;

		TLGetToken(&t_list, &token);
		result = i_stat_list();
		if(result != OK)
			return result;

		break;

	case EOL: //zadne generovvani
		TLGetToken(&t_list, &token);
		result = i_stat_list();
		if (result != OK)
			return result;
			break;
	case END:
			break;

    default:
        break; // muze nasledovat token, ktery vyhovuje fn_def_list()
    }
	return OK;

}

int i_built_in_func() {
	switch (token.id) {
	case INPUTS:
	case INPUTI:
	case INPUTF:
		;

	case PRINT:
		;

	case LENGTH:
	case SUBSTR:
	case ORD:
	case CHR:
		;

	}

	return OK;
}

int i_program() {

	// pravidlo <prog> -> <stat-t_list> <fn-def-t_list> <prog> EOF
	result = i_stat_list();
	if(result != OK) {
		return result;
	}

// pravidlo <prog> -> e <fn-def-t_list> <prog> EOF
	result = i_fn_def_list();
	if(result != OK) {
		return result;
	}


	TLGetToken(&t_list, &token); // token muze byt ENDOF
	if (token.id == ENDOF) {
		return 0;
    }
	else
		return i_program();
}

