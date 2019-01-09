/*
** Implementace překladače imperativního jazyka IFJ18
** 
** Vorobiev Nikolaj	xvorob00@stud.fit.vutbr.cz
** Luhin Artsiom	xluhin00@stud.fit.vutbr.cz
*/

#include <stdio.h>
#include <stdlib.h>

#include "p.h"
#include "itable.h"
#include "expression.h"
int count_params = 0;
bool zavorka = false;

extern Token token;
extern Token token_id;
extern Token token_f;
extern Value value;
extern TSTree tree; //вот так бля
//extern LTokenList t_list;

extern InstrTree iTree;

extern TSTree FakeTree;

int result;				// vysledna hodnota syntakticke kontroly



int fn_def_par() {
    switch (token.id) {
    case IDENTIFIER:
        strCopyString(&token_id, &token);
        if (TSSearchFunction(&tree, token_id) == NULL &&
                TSSearchId(&tree, token_f, token_id) == NULL)
            TSInsertId(&tree, token_f, token_id, PARAM, DCLR, value);
        else
            return SEM_ERROR;
        return OK;

    default:
        return SYNTAX_ERROR;
    }
}

int fn_def_pars() {
    switch (token.id) {
    case IDENTIFIER:
		copyToken(&token_id, &token);
        if (TSSearchFunction(&tree, token_id) == NULL &&
                TSSearchId(&tree, token_f, token_id) == NULL)
            TSInsertId(&tree, token_f, token_id, PARAM, DCLR, value);
        else
            return SEM_ERROR;

        if ((result = get_token(&token)) != OK)
			return result;
        return fn_def_pars1();

    case CLOSE_BRACKET:
        return OK;

    default:
        return SYNTAX_ERROR;
    }
}

int fn_def_pars1() {
	//TLInsert(&t_list, &token);
	switch (token.id) {
    case COMMA:
        if ((result = get_token(&token)) != OK)
            return result;
		//TLInsert(&t_list, &token);
		result = fn_def_par();
        if (result != OK)
            return result;

        if ((result = get_token(&token)) != OK)
            return result;
		return fn_def_pars1(); // CHECK IT!!

    case CLOSE_BRACKET:
        return OK;

    default:
        return SYNTAX_ERROR;
    }
}

int fn_head() {
	if (token.id != IDENTIFIER)
        return SYNTAX_ERROR;
	copyToken(&token_id, &token);			//.... new
	if ((result = get_token(&token)) != OK)
		return result;

    if (TSSearchFunction(&tree, token_id) == NULL &&
			TSSearchId(&tree, token_f, token_id) == NULL){
        TSInsertFunction(&tree, token_id);
		TIInsertFunction(&iTree, token_id);
	}
    else
        return SEM_ERROR;

	//TLInsert(&t_list, &token_id);
	//TLInsert(&t_list, &token);
	copyToken(&token_f, &token_id);


    if (token.id != OPEN_BRACKET)
        return SYNTAX_ERROR;

    if ((result = get_token(&token)) != OK)
        return result;

    return fn_def_pars();

}


int fn_def() {
	//TLInsert(&t_list, &token);
    if ((result = get_token(&token)) != OK)
        return result;

/////
    result = fn_head();
    if (result != OK)
        return result;

    if ((result = get_token(&token)) != OK)
        return result;

	//TLInsert(&t_list, &token);
    if (token.id != EOL)
        return SYNTAX_ERROR;

    if ((result = get_token(&token)) != OK)
        return result;

/////
    result = stat_list();
    if (result != OK)
        return result;


    if (token.id != END)
        return SYNTAX_ERROR;

    return result;
}

int fn_def_list() {
    switch (token.id) {
    case DEF:
        result = fn_def();
        if (result != OK)
            return result;

        if ((result = get_token(&token)) != OK)
            return result;
		//TLInsert(&t_list, &token);

        if (token.id != EOL)
            return SYNTAX_ERROR;

        strClear(&token_f);
        strAddString(&token_f, "def");
        //... new

        if ((result = get_token(&token)) != OK)
            return result;
        return fn_def_list();


    case EOL:
        if ((result = get_token(&token)) != OK)
            return result;
        return fn_def_list();

    default:
        return OK; // muze nasledovat token, ktery vyhovuje stat_list()
    }

}

int fn_par() {
    switch (token.id) {
    case IDENTIFIER:
    case INTEGER:
    case FLOAT:
    case STRING:
			count_params++;
		if (token.id == IDENTIFIER) {
			if (TSSearchFunction(&tree, token) != NULL)
                return SEM_ERROR;
            if (TSSearchId(&tree, token_f, token) == NULL)
                return SEM_ERROR;
        }
        return OK;

    default:
        return SYNTAX_ERROR;
    }
}

int fn_pars() {
    switch (token.id) {
    case IDENTIFIER:
    case INTEGER:
    case FLOAT:
    case STRING:
		//if (token_id.id == PRINT) {}//dopsat
		//else{
		count_params++;
		if(token.id == IDENTIFIER){
			/*if (TSSearchFunction(&tree, token) != NULL)
				return SEM_ERROR;*/
            if (TSSearchId(&tree, token_f, token) == NULL)
                return SEM_ERROR;
		}

		if(token_id.id == LENGTH){
			if(token.id == FLOAT || token.id == INTEGER)
				return SEM_STAT_ERROR; //параметр говно
		}
		/*else if(token_id.id == SUBSTR){
			if(count_params == 1 && (token.id == INTEGER || token.id == FLOAT) )
					return SEM_STAT_ERROR;
			else if((count_params >= 2 ) && (token.id == FLOAT ||\
											 token.id == STRING)){
				return SEM_STAT_ERROR;
			}
		}*/
		//}

        if ((result = get_token(&token)) != OK)
            return result;
        return fn_pars1();

    case CLOSE_BRACKET:
			if(token_id.id == PRINT){
				if (count_params == 0)
					return SEM_ERROR_PARAMS;
			}
			else {
				if(TSCountParams(&tree, token_id) != count_params)
						return SEM_ERROR_PARAMS;
			}
			if(zavorka == true)
				zavorka = false;
			else
				return SYNTAX_ERROR;
        return OK;
	case EOL:
			if(token_id.id == PRINT) {
				return SEM_ERROR_PARAMS;
			}
			else if(TSCountParams(&tree, token_id) != count_params){
						return SEM_ERROR_PARAMS;
			}
		return OK;

    default:
        return SYNTAX_ERROR;
    }
}

int fn_pars1() {
    switch (token.id) {
    case COMMA:
		//TLInsert(&t_list, &token);
        if ((result = get_token(&token)) != OK)
            return result;
        result = fn_par();
        if (result != OK)
            return result;

        if ((result = get_token(&token)) != OK)
            return result;
        return fn_pars1(); // CHECK IT!!

	case CLOSE_BRACKET:
			if(zavorka == true)
				zavorka = false;
			else
				return SYNTAX_ERROR;
		if ((result = get_token(&token)) != OK)
			return result;
		//TLInsert(&t_list, &token);

			return fn_pars1();

    case EOL:
		if(zavorka == true)
			return SYNTAX_ERROR;
		if(token_id.id == PRINT) {count_params = 0;}//dopsat
		else if(TSCountParams(&tree, token_id) != count_params)
			return SEM_ERROR_PARAMS;
		else
			count_params = 0;
        return OK;

    default:
        return SYNTAX_ERROR;
    }
}

int exp_call() {
	copyToken(&token_id, &token);
	if ((result = get_token(&token)) != OK)
		return result;
	//TLInsert(&t_list, &token_id);
	//TLInsert(&t_list, &token);
	switch (token_id.id) {
    case INPUTS:
    case INPUTI:
    case INPUTF:
    case PRINT:
    case LENGTH:
    case SUBSTR:
    case ORD:
    case CHR:

	case IDENTIFIER:
		if(strCmpConstStr(&token_f, "def") == 0) {
			if(TSSearchFunction(&tree, token_id) != NULL) {
				//all good
			}
		}
		else {
			if (TSSearchFunction(&tree, token_id) != NULL && TSSearchId(&tree, token_f, token_id) == NULL) {
			}
			else if (TSSearchFunction(&tree, token_id) == NULL && TSSearchId(&tree, token_f, token_id) != NULL) {
			}
			else {
				if(token.id == ASSIGN) {}
				else if(token.id == IDENTIFIER) { //volani nezname funkci
					count_params++;
					TSInsertFunction(&FakeTree, token_id);
				}
				else
					return SEM_ERROR;
			}
		}

	switch (token_id.id) {
		case IDENTIFIER:
			if (token.id == OPEN_BRACKET) {
				zavorka = true;
				if ((result = get_token(&token)) != OK)
					return result;
				result = fn_pars();
				if (result != OK)
					return result;
			} else if (token.id == IDENTIFIER || token.id == INTEGER || token.id == FLOAT || token.id == STRING) {

					result = fn_pars();
					if (result != OK)
						return result;

			} else if (token.id == EOL){
				if (TSSearchFunction(&tree, token_id) != NULL ) {
					if(TSCountParams(&tree, token_id) != 0)
						return SEM_ERROR_PARAMS;
				}
				else if (TSSearchId(&tree, token_f, token_id) != NULL) {
					return OK;
				}
				else {
					return SEM_ERROR;
				}

			} else if (token.id == PLUS || token.id == MINUS || token.id == TIMES || token.id == LOMENO ||
					   token.id == LESS ||token.id == GREATER || token.id == EQUAL || token.id == NOT_EQUAL ||
					   token.id == LESS_EQUAL || token.id == GREATER_EQUAL) {
				if ((result = pr_expressionAnalysis(EOL)) != PREC_OK) {
					return result;
				}
				else
					return OK;
			} else
				return SYNTAX_ERROR;
			return result;

		case INPUTS:
		case INPUTI:
		case INPUTF:
		case PRINT:
		case LENGTH:
		case SUBSTR:
		case ORD:
		case CHR:
			if (token.id == OPEN_BRACKET) {
				zavorka = true;
				if ((result = get_token(&token)) != OK)
					return result;
				result = fn_pars();
				if (result != OK)
					return result;
			} else if (token.id == IDENTIFIER || token.id == INTEGER || token.id == FLOAT || token.id == STRING) {
				result = fn_pars();
				if (result != OK)
					return result;
			} else if (token.id == EOL) {
				result = fn_pars();
				if (result != OK)
					return result;
			} else
				return SYNTAX_ERROR;
			return result;
	}

    case INTEGER:
    case FLOAT:
    case STRING:
    case OPEN_BRACKET:
	case NIL:
		if ((result = pr_expressionAnalysis(EOL)) != PREC_OK) {
			return result;
		}
		else
			return OK;
		/*while (token.id != EOL)		// EDIT!!
            get_token(&token);
		return OK;*/
/*
    case OPEN_BRACKET:
        result = psa();
*/
        // VYRAZ MUZE ZACINAT TERMY. PRIDAT JE!!
    default:
        return SYNTAX_ERROR;
    }

}

int stat() {
        // VYRAZ MUZE ZACINAT TERMY. PRIDAT JE!!
    switch (token.id) {
    /*
    case IDENTIFIER:
        get_token(&token);
        if (token.id == ASSIGN) {
            get_token(&token);
            exp_call();
        } else {
            unget_token(); 		// EDIT!!
            exp_call();
        }
    */
		/*copyToken(&token_id, &token);
		if ((result = get_token(&token)) != OK)
			return result;
		result = built_in_func();
        if (result != OK)
			return result;*/

    case INTEGER:
    case FLOAT:
    case STRING:
    case OPEN_BRACKET:
		copyToken(&token_id, &token);
		if ((result = get_token(&token)) != OK)
			return result;
		//TLInsert(&t_list, &token_id);
		//TLInsert(&t_list, &token);
		if ((result = pr_expressionAnalysis(EOL)) != PREC_OK) {
			return result;
		}
		else
			return OK;
		/*while (token.id != EOL)		// EDIT!!
            get_token(&token);
		return OK;*/


	case INPUTS:
	case INPUTI:
	case INPUTF:
	case PRINT:
	case LENGTH:
	case SUBSTR:
	case ORD:
	case CHR:

	case IDENTIFIER:
        copyToken(&token_id, &token);	//.... new
		if ((result = get_token(&token)) != OK)
			return result;
		if (strCmpConstStr(&token_f, "def") == 0) {
            if (TSSearchFunction(&tree, token_id) != NULL && TSSearchId(&tree, token_f, token_id) == NULL) {
			}
			else if (TSSearchFunction(&tree, token_id) == NULL && TSSearchId(&tree, token_f, token_id) != NULL) {
			}
			else {
				if(token.id != ASSIGN  && token.id != EOL) {
					return SEM_ERROR;
				}
			}
        }
		else {
			if (TSSearchFunction(&tree, token_id) != NULL && TSSearchId(&tree, token_f, token_id) == NULL) {
			}
			else if (TSSearchFunction(&tree, token_id) == NULL && TSSearchId(&tree, token_f, token_id) != NULL) {
			}
			else {
				if(token.id == ASSIGN) {}
				else if(token.id == IDENTIFIER) { //volani nezname funkci
					count_params++;
					TSInsertFunction(&FakeTree, token_id);
				}
				else
					return SEM_ERROR;
			}
		}
		switch (token_id.id) {
			case IDENTIFIER:
				if (token.id == ASSIGN) {
					if (TSSearchFunction(&tree, token_id) == NULL &&\
							TSSearchId(&tree, token_f, token_id) == NULL){
						TSInsertId(&tree, token_f, token_id, PROM, DCLR, value); //..... new
					}
					else if (TSSearchId(&tree, token_f, token_id) != NULL){

					}
					else
						return SEM_ERROR;

					if ((result = get_token(&token)) != OK)
						return result;
					result = exp_call();

				} else if (token.id == OPEN_BRACKET) {
					zavorka = true;
					if ((result = get_token(&token)) != OK)
						return result;
					result = fn_pars();
				} else if (token.id == IDENTIFIER || token.id == INTEGER || token.id == FLOAT || token.id == STRING) {
					result = fn_pars();

				} else if (token.id == PLUS || token.id == MINUS || token.id == TIMES || token.id == LOMENO ||
						   token.id == LESS ||token.id == GREATER || token.id == EQUAL || token.id == NOT_EQUAL ||
						   token.id == LESS_EQUAL || token.id == GREATER_EQUAL) {

					if ((result = pr_expressionAnalysis(EOL)) != PREC_OK) {
						return result;
					}
					else
						return OK;

				} else if (token.id == EOL)  {
					if (TSSearchFunction(&tree, token_id) == NULL) {
						TSInsertId(&tree, token_f, token_id, PROM, DCLR, value); //..... new
					}
					else {
						if(TSCountParams(&tree, token_id) == 0)
							return OK;
						else
							return SEM_ERROR_PARAMS;
					}
				}
				else
					return SYNTAX_ERROR;
				break;

			case INPUTS:
			case INPUTI:
			case INPUTF:
			case PRINT:
			case LENGTH:
			case SUBSTR:
			case ORD:
			case CHR:
				if (token.id == OPEN_BRACKET) {
					zavorka = true;
					if ((result = get_token(&token)) != OK)
						return result;
					result = fn_pars();
				} else if (token.id == IDENTIFIER || token.id == INTEGER || token.id == FLOAT || token.id == STRING) {
					result = fn_pars();
				}
				else if (token.id == EOL)  {
					result = fn_pars();
				}
				else
					return SYNTAX_ERROR;
		}
			break;
    case IF:

		//TLInsert(&t_list, &token);
        if ((result = get_token(&token)) != OK)
            return result;

		copyToken(&token_id, &token);
		if ((result = get_token(&token)) != OK)
			return result;
		//TLInsert(&t_list, &token_id);
		//TLInsert(&t_list, &token);

		if ((result = pr_expressionAnalysis(THEN)) != PREC_OK) {
			return result;
		}
		/*while (token.id != THEN) {
            get_token(&token);
		}*/
        /*
        if ((result = psa()) != OK)		// EDIT!
            return result;
        */

        if (token.id != THEN)
            return SYNTAX_ERROR;

        if ((result = get_token(&token)) != OK)
            return result;

		//TLInsert(&t_list, &token);
		if (token.id != EOL)
            return SYNTAX_ERROR;

        if ((result = get_token(&token)) != OK)
            return result;

///
        result = stat_list();
        if (result != OK)
            return result;

		//TLInsert(&t_list, &token);
        if (token.id != ELSE)
            return SYNTAX_ERROR;

        if ((result = get_token(&token)) != OK)
            return result;

		//TLInsert(&t_list, &token);

        if (token.id != EOL)
            return SYNTAX_ERROR;

        if ((result = get_token(&token)) != OK)
            return result;

///
        result = stat_list();
        if (result != OK)
            return result;

		//TLInsert(&t_list, &token);
        if (token.id != END)
            return SYNTAX_ERROR;

        if ((result = get_token(&token)) != OK)
            return result;

        if (token.id != EOL)
            return SYNTAX_ERROR;


        break;
        /*
        if (token.id != EOL)		// muze byt tady ENDOF???
            return SYNTAX_ERROR;
            */

    case WHILE:
		//TLInsert(&t_list, &token);
		if ((result = get_token(&token)) != OK)
			return result;

		copyToken(&token_id, &token);
		if ((result = get_token(&token)) != OK)
			return result;
		//TLInsert(&t_list, &token_id);
		//TLInsert(&t_list, &token);

		if ((result = pr_expressionAnalysis(DO)) != PREC_OK) {
			return result;
		}



        /*
        if ((result = psa()) != OK)		// EDIT!
            return result;
        */
		/*while (token.id != DO)
			get_token(&token);*/

        if (token.id != DO)
            return SYNTAX_ERROR;

        if ((result = get_token(&token)) != OK)
            return result;
		//TLInsert(&t_list, &token);

        if (token.id != EOL)
            return SYNTAX_ERROR;

        if ((result = get_token(&token)) != OK)
            return result;
///
        result = stat_list();
        if (result != OK)
            return result;
		//TLInsert(&t_list, &token);

        if (token.id != END)
            return SYNTAX_ERROR;

        if ((result = get_token(&token)) != OK)
            return result;
		//TLInsert(&t_list, &token);

        if (token.id != EOL)
            return SYNTAX_ERROR;

        break;

        /*
    case OPEN_BRACKET:
        result = psa();		EDIT!!
        */

    }

    return result;
}

int stat_list() {
    switch (token.id) {
		case IDENTIFIER:
		case IF:
		case WHILE:
		case OPEN_BRACKET:

		case INPUTS:
		case INPUTI:
		case INPUTF:
		case PRINT:
		case LENGTH:
		case SUBSTR:
		case ORD:
		case CHR:
			result = stat();
			if (result != OK)
				return result;

			if ((result = get_token(&token) != OK))
				return result;
			return stat_list();


		case EOL:
			if ((result = get_token(&token) != OK))
				return result;
			return stat_list();

		default:
			return OK; // muze nasledovat token, ktery vyhovuje fn_def_list()
		}
}

int program() {
    switch (token.id) {
	case ENDOF:
			return OK;
    case IDENTIFIER:
    case IF:
    case WHILE:
    case DEF:
    case EOL:
    case OPEN_BRACKET:

    case INPUTS:
    case INPUTI:
    case INPUTF:
    case PRINT:
    case LENGTH:
    case SUBSTR:
    case ORD:
    case CHR:
//    case TERM:
//    case ENDOF: 		CHECK IT!!

        // pravidlo <prog> -> <stat-list> <fn-def-list> <prog> EOF
        result = stat_list();
        if (result != OK)
            return result;
        // pravidlo <prog> -> e <fn-def-list> <prog> EOF
        result = fn_def_list();
        if (result != OK)
            return result;

        // add get_token() call ??
        else
            return program();

    default:
        result = SYNTAX_ERROR;
    }

    return result;
}


