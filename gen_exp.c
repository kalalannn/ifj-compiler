/*
** Implementace překladače imperativního jazyka IFJ18
** 
** Vorobiev Nikolaj	xvorob00@stud.fit.vutbr.cz
** Luhin Artsiom	xluhin00@stud.fit.vutbr.cz
*/

#include "gen_exp.h"
#include "token_list.h"
#include "postfix.h"
#include "symtable.h"
#include "itable.h"

extern TSTree tree;
extern InstrTree iTree;
extern Token token;
extern Token token_id;
extern Token token_f;
extern Param p1;
extern Param p2;
extern Param p3;

int generExpression(int until) {

	if (token_id.id == IDENTIFIER && getType(&tree, token_f, token_id) == NIL) {
		if (token.id == until)
			return NIL;
		else
		return 	SEM_STAT_ERROR;
	}

	if (token_id.id == NIL) {
		if (token.id == until)
			return NIL;
		else
			return SEM_STAT_ERROR;
	}

	LTokenList exp_token_list;
	TLInitList(&exp_token_list);


	getPostfix(&exp_token_list, until);
	TLInsert(&exp_token_list, &token); 		// insert "until"


	int type, result;			// result - vysledek kontroly
	bool retypeToF = false;		// retype to float
	LTokenPtr first = exp_token_list.first;


	if (strCmpConstStr(&token_f, "def") == 0) {
		if (first->token.id == IDENTIFIER) {
			type = getType(&tree, token_f, first->token);
			switch (type) {
			case INTEGER:
				if ((result = checkInt(&exp_token_list, &retypeToF)) != OK)
					return result;
				break;
			case FLOAT:
				if ((result = checkFloat(&exp_token_list, &retypeToF))!= OK)
					return result;
				break;
			case STRING:
				if ((result = checkString(&exp_token_list)) != OK)
					return result;
				break;
			default:
				return SEM_STAT_ERROR;
			}
		}
	}

	switch (first->token.id) {
	case IDENTIFIER:
		break;
	case INTEGER:
		if ((result = checkInt(&exp_token_list, &retypeToF)) != OK)
			return result;
		break;
	case FLOAT:
		if ((result = checkFloat(&exp_token_list, &retypeToF))!= OK)
			return result;
		break;
	case STRING:
		if ((result = checkString(&exp_token_list)) != OK)
			return result;
		break;
	default:
		return SEM_STAT_ERROR;
	}



	TLFirst(&exp_token_list);			// nastavime aktualni prvek na first
	TLGetToken(&exp_token_list, &token);	// get first token from exp_token_list
//	Token tmp;		// pamatovat prvni token
	//strInit(&tmp);
	//copyToken(&tmp, &token);
	bool int_fg = false;
	bool float_fg = false;
	bool relation_fg = false;

	int id_type;
	while (token.id != until) { // CHECK IT!!
		switch (token.id) {
		case IDENTIFIER:
			id_type = getType(&tree, token_f, token);

			if (id_type == NIL)
				return SEM_STAT_ERROR;
			else if (id_type == FLOAT)
				float_fg = true;
			else if (id_type == INTEGER)
				int_fg = true;
			else if (id_type == STRING) {
				str_stet(&exp_token_list, until);
				return STRING;
			}

			GenId(token, &p1);
			OneParams(&p1, &p2, &p3);
			GenerateInstruction(&iTree, token_f, PUSHS, p1, p2, p3);

			if (retypeToF && id_type == INTEGER) {
				ZeroParams(&p1, &p2, &p3);
				GenerateInstruction(&iTree, token_f, INT2FLOAT, p1, p2, p3);
				float_fg = true;
			}
			break;

		case INTEGER:
			GenConst(token, &p1);
			OneParams(&p1, &p2, &p3);
			GenerateInstruction(&iTree, token_f, PUSHS, p1, p2, p3);
			int_fg = true;
			if (retypeToF) {
				ZeroParams(&p1, &p2, &p3);
				GenerateInstruction(&iTree, token_f, INT2FLOAT, p1, p2, p3);
				float_fg = true;
			}
			break;

		case STRING:
				str_stet(&exp_token_list , until);
				return STRING;

		case FLOAT:
			GenConst(token, &p1);
			OneParams(&p1, &p2, &p3);
			GenerateInstruction(&iTree, token_f, PUSHS, p1, p2, p3);
			float_fg = true;
			break;

		case NIL:
			return SEM_STAT_ERROR;

		case PLUS:
			ZeroParams(&p1, &p2, &p3);
			GenerateInstruction(&iTree, token_f, ADDS, p1, p2, p3);
			break;

		case MINUS:
			ZeroParams(&p1, &p2, &p3);
			GenerateInstruction(&iTree, token_f, SUBS, p1, p2, p3);
			break;

		case TIMES:
			ZeroParams(&p1, &p2, &p3);
			GenerateInstruction(&iTree, token_f, MULS, p1, p2, p3);
			break;

		case LOMENO:
			ZeroParams(&p1, &p2, &p3);
			GenerateInstruction(&iTree, token_f, DIVS, p1, p2, p3);
			break;

		case LESS:
			ZeroParams(&p1, &p2, &p3);
			GenerateInstruction(&iTree, token_f, LTS, p1, p2, p3);
			relation_fg = true;
			break;

		case GREATER:
			ZeroParams(&p1, &p2, &p3);
			GenerateInstruction(&iTree, token_f, GTS, p1, p2, p3);
			relation_fg = true;
			break;

		case EQUAL:
			ZeroParams(&p1, &p2, &p3);
			GenerateInstruction(&iTree, token_f, EQS, p1, p2, p3);
			relation_fg = true;
			break;

		case NOT_EQUAL:
			ZeroParams(&p1, &p2, &p3);
			GenerateInstruction(&iTree, token_f, EQS, p1, p2, p3);
			ZeroParams(&p1, &p2, &p3);
			GenerateInstruction(&iTree, token_f, NOTS, p1, p2, p3);
			relation_fg = true;


		}

		TLGetToken(&exp_token_list, &token);	// get first token from exp_token_list
	}

	TLDisposeList(&exp_token_list);

	if (relation_fg)
		return BOOLEAN;
	else if (float_fg)
		return FLOAT;
	else if (int_fg)
		return INTEGER;
	else
		return OK;

}

void str_stet(LTokenList *exp_token_list, int until) {

	Token retval;
	Token str;
	strInit(&retval);
	strInit(&str);
	strAddString(&retval, "retval"); //LF@retval
	strAddString(&str, "string@");	 //string@
	GenId(retval, &p1);
	OneParams(&p1, &p2, &p3);
	GenerateInstruction(&iTree, token_f, DEFVAR, p1, p2, p3); //defvar LF@retval

	GenLabel(str, &p2);
	TwoParams(&p1, &p2, &p3);
	GenerateInstruction(&iTree, token_f, MOVE, p1, p2, p3); // MOVE LF@retval string@

	GenId(retval, &p1);
	GenId(retval, &p2);

	while(token.id != until){
		switch(token.id){
			case STRING:
				GenConst(token, &p3); 										//p1 = LF@retval
				ThreeParams(&p1, &p2, &p3);
				GenerateInstruction(&iTree, token_f, CONCAT, p1, p2, p3);	//p2 = LF@retval
				break;

			case IDENTIFIER:
				GenId(token, &p3); 											//p1 = LF@retval
				ThreeParams(&p1, &p2, &p3);
				GenerateInstruction(&iTree, token_f, CONCAT, p1, p2, p3);	//p2 = LF@retval
				break;

			case PLUS:
				break;
			default: 	//EOL
				break;
		}
			TLGetToken(exp_token_list, &token);	// get first token from exp_token_list
		//твой лист геттокен
	}
	GenerateInstruction(&iTree, token_f, PUSHS, p1, p2, p3); //PUSHS LF@retval

	strFree(&retval);
	strFree(&str);
}

int checkInt(LTokenList *list, bool *retype) {
	LTokenPtr act = list->act;
	int type;

	switch (act->token.id) {
	case STRING:
		return SEM_STAT_ERROR;
	case FLOAT:
		*retype = true;
		break;
	case IDENTIFIER:
		type = getType(&tree, token_f, act->token);
		if (type == FLOAT)
			*retype = true;
		else if (type == STRING || type == DCLR || type == DP)
			return SEM_STAT_ERROR;
		break;
	}

	if (list->act->next != NULL) {
		list->act = list->act->next;
		checkInt(list, retype);
	}

	return OK;

}

int checkFloat(LTokenList *list, bool *retype) {
	LTokenPtr act = list->act;
	int type;

	switch (act->token.id) {
	case STRING:
		return SEM_STAT_ERROR;
	case INTEGER:
		*retype = true;
		break;
	case IDENTIFIER:
		type = getType(&tree, token_f, act->token);
		if (type == INTEGER)
			*retype = true;
		else if (type == STRING || type == DCLR || type == DP)
			return SEM_STAT_ERROR;
		break;
	}

	if (list->act->next != NULL) {
		list->act = list->act->next;
		checkFloat(list, retype);
	}

	return OK;
}

int checkString(LTokenList *list) {
	LTokenPtr act = list->act;
	int type;

	switch (act->token.id) {
	case FLOAT:
	case INTEGER:
		return SEM_STAT_ERROR;
	case IDENTIFIER:
		type = getType(&tree, token_f, act->token);
		if (type == FLOAT || type == INTEGER || type == DCLR || type == DP)
			return SEM_STAT_ERROR;
		break;
	}

	if (list->act->next != NULL) {
		list->act = list->act->next;
		checkString(list);
	}

	return OK;
}
