/*
** Implementace překladače imperativního jazyka IFJ18
**
** Koša Benjamín	xkosab00@stud.fit.vutbr.cz
*/

#include "expression.h"

extern Token token;
extern Token token_id;
extern Token token_f;
extern TSTree tree;
//extern LTokenList t_list;

void print_elements_of_list(tDLList TL)	{
	
	tDLList TempList=TL;
	int CurrListLength = 0;
	printf("-----------------");
	while ((TempList.First!=NULL) && (CurrListLength<MaxListLength))	{
		//printf("\n \t%d",TempList.First->data);
        switch(TempList.First->data){
            case PLUS: printf("\n \t+"); break;
            case MINUS: printf("\n \t-"); break;
            case TIMES: printf("\n \t*"); break;
            case LOMENO: printf("\n \t/"); break;
            case OPEN_BRACKET: printf("\n \t("); break;
            case CLOSE_BRACKET: printf("\n \t)"); break;
            case LESS: printf("\n \t<"); break;
            case GREATER: printf("\n \t>"); break;
            case LESS_EQUAL: printf("\n \t<="); break;
            case GREATER_EQUAL: printf("\n \t>="); break;
            case EQUAL: printf("\n \t=="); break;
            case NOT_EQUAL: printf("\n \t!="); break;
            case IDENTIFIER: printf("\n \tid"); break;
            case DOLLAR: printf("\n \t$"); break;
            case OPEN_HANDLE: printf("\n \t<"); break;
            case CLOSE_HANDLE: printf("\n \t>"); break;
            case E: printf("\n \tE"); break;

        }

		if ((TempList.First==TL.Act) && (TL.Act!=NULL))
			printf("\t <= toto je aktivní prvek ");
		TempList.First=TempList.First->rptr;
		CurrListLength++;
	}
    if (CurrListLength>=MaxListLength)
        printf("\nList exceeded maximum length!");
	printf("\n-----------------\n");     
}

//precedencna tabulka
int pr_get_oper(int a, int b){
    char pr_table [14][14] = {
    //          '0', '1', '2', '3', '4', '5', '6', '7', '8', '9','10','11','12','13'
    //          '+', '-', '*', '/', '(', ')', '<', '>','<=','>=','==','!=','id', '$'
    /* 0  +  */{'>', '>', '<', '<', '<', '>', '>', '>', '>', '>', '>', '>', '<', '>'},
    /* 1  -  */{'>', '>', '<', '<', '<', '>', '>', '>', '>', '>', '>', '>', '<', '>'},
    /* 2  *  */{'>', '>', '>', '>', '<', '>', '>', '>', '>', '>', '>', '>', '<', '>'},
    /* 3  /  */{'>', '>', '>', '>', '<', '>', '>', '>', '>', '>', '>', '>', '<', '>'},
    /* 4  (  */{'<', '<', '<', '<', '<', '=', '<', '<', '<', '<', '<', '<', '<', ' '},
    /* 5  )  */{'>', '>', '>', '>', ' ', '>', '>', '>', '>', '>', '>', '>', ' ', '>'},
    /* 6  <  */{'<', '<', '<', '<', '<', '>', ' ', ' ', ' ', ' ', '>', '>', '<', '>'},
    /* 7  >  */{'<', '<', '<', '<', '<', '>', ' ', ' ', ' ', ' ', '>', '>', '<', '>'},
    /* 8  <= */{'<', '<', '<', '<', '<', '>', ' ', ' ', ' ', ' ', '>', '>', '<', '>'},
    /* 9  >= */{'<', '<', '<', '<', '<', '>', ' ', ' ', ' ', ' ', '>', '>', '<', '>'},
    /* 10 == */{'<', '<', '<', '<', '<', '>', '<', '<', '<', '<', ' ', ' ', '<', '>'},
    /* 11 != */{'<', '<', '<', '<', '<', '>', '<', '<', '<', '<', ' ', ' ', '<', '>'},
    /* 12 id */{'>', '>', '>', '>', ' ', '>', '>', '>', '>', '>', '>', '>', ' ', '>'},
    /* 13 $  */{'<', '<', '<', '<', '<', ' ', '<', '<', '<', '<', '<', '<', '<', ' '} 
    };
    return pr_table[a][b];
}

//ak parser pred zavolanim precedencnej analyzy uz nacital prvy token z vyrazu
//tato funkcie ten token zahrnie do vyrazu
void pr_lastToken(Token *token, tDLList *inputList){
    //TSTree isFunction;
    switch(token->id){
        //najde nieco co moze byt vyraz
        case EQUAL:
        case LESS:
        case LESS_EQUAL:
        case GREATER:
        case GREATER_EQUAL:
        case NOT_EQUAL:
        case LOMENO:
        case TIMES:
        case PLUS:
        case MINUS:
        case OPEN_BRACKET:
        case CLOSE_BRACKET:
            //vlozi localToken.id do zoznamu
            DLInsertLast(inputList,token->id);
            break;
        case INTEGER:
        case FLOAT:
        case IDENTIFIER:
        case STRING:
            //vlozi IDENTIFIER do zoznamu
            DLInsertLast(inputList,IDENTIFIER);
            break;
    }
}


//vytiahne vyraz z dokumentu a vlozi do zoznamu, 
//vysledny  zoznam posle do dalsej funkcie na spracovanie
int pr_expressionAnalysis(int until){
    if(token_id.id == IDENTIFIER)
        if(TSSearchId(&tree, token_f, token_id) == NULL) //lex kontrola
            return SEM_ERROR;
    if(token.id == IDENTIFIER)   
        if(TSSearchId(&tree, token_f, token) == NULL) // lex kontrola
            return SEM_ERROR;
    if(token.id == until)
        return PREC_OK;

	if(token_id.id == NIL) {
		if(token.id== until) {
			return PREC_OK;
		}
		else
			return SEM_STAT_ERROR;
	}

    //inicializacia zoznamu pre vyraz 
    tDLList inputList;		
    DLInitList(&inputList);

    pr_lastToken(&token_id, &inputList);
    pr_lastToken(&token, &inputList);

    //TSTree isFunction;
    
	//strFree(&token);
    if(get_token(&token) != 0){
        DLDisposeList(&inputList);
		return LEX_ERROR;
    }

    do{
		if(token.id == IDENTIFIER){
			if(TSSearchId(&tree, token_f, token) == NULL){//lex kontrola
				DLDisposeList(&inputList);
				return SEM_ERROR;
			}
		}
		switch(token.id){
            //najde nieco co moze byt vyraz
            case EQUAL:
            case LESS:
            case LESS_EQUAL:
            case GREATER:
            case GREATER_EQUAL:
            case NOT_EQUAL:
            case LOMENO:
            case TIMES:
            case PLUS:
            case MINUS:
            case OPEN_BRACKET:
            case CLOSE_BRACKET:
                //vlozi token.id do zoznamu
                DLInsertLast(&inputList,token.id);
                break;

            case INTEGER:
            case FLOAT:
            case IDENTIFIER:
            case STRING:
                //vlozi IDENTIFIER do zoznamu
                DLInsertLast(&inputList,IDENTIFIER);
                break;

            //ak narazi na nieco co nemoze byt vyraz
            default:
                DLDisposeList(&inputList);
                return SYNTAX_ERROR;
        }
	//strFree(&token);
    if(get_token(&token) != 0){
        DLDisposeList(&inputList);
		return LEX_ERROR;
    }
    } while(token.id != until);

	DLInsertLast(&inputList,DOLLAR);
    //nacitany zoznam odosle na spracovanie
    return pr_finalAnalysis(&inputList);
}

int pr_finalAnalysis(tDLList *inputList){
    //inicializacia
    tDLList pushdown;		
    DLInitList(&pushdown);
    DLInsertFirst(&pushdown,DOLLAR);
    DLFirst(&pushdown);

    tDLList inputExpression = *inputList;
    int a;//aktualny znak na pushdown
    int b;//vrchol inputExpression
    int value;

/*
        printf("inputExpression");
        print_elements_of_list(inputExpression);
        printf("pushdown");
        print_elements_of_list(pushdown);
*/
    while(1){
        DLCopy(&pushdown, &a);
        DLCopyFirst(&inputExpression, &b);
        if(a == DOLLAR && b == DOLLAR)
            break;
        char rule = pr_get_oper(a, b);
        //printf("%d %c %d\n",a , rule, b);
        switch(rule){
            case '=':
                //push b
                DLInsertLast(&pushdown,b);
                
                DLLast(&pushdown);
                //precitaj dalsi zo vstpu
                DLDeleteFirst(&inputExpression);
                break;
            case '<':
                //vlozime < za aktivny prvok
                DLPostInsert(&pushdown, OPEN_HANDLE);
                //vlozime b na koniec zoznamu
                DLInsertLast(&pushdown,b);
                //nastavime aktivitu
                DLLast(&pushdown);
                DLDeleteFirst(&inputExpression);
                break;
            case '>':
                DLCopyLast(&pushdown, &a);
                DLDeleteLast(&pushdown);
                switch(a){
                    case IDENTIFIER:
                        //nacitame dalsi znak
                        DLCopyLast(&pushdown, &a);
                        DLDeleteLast(&pushdown);
                        if(a != OPEN_HANDLE){
                            DLDisposeList(&pushdown);
                            return SYNTAX_ERROR;
                        }
                        DLInsertLast(&pushdown,E);
                        DLLast(&pushdown);
                        DLPred(&pushdown); 
                        break;
                    case E:
                        DLCopyLast(&pushdown, &a);
                        DLDeleteLast(&pushdown);
                        switch(a){
                            case EQUAL:
                            case LESS:
                            case LESS_EQUAL:
                            case GREATER:
                            case GREATER_EQUAL:
                            case NOT_EQUAL:
                            case LOMENO:
                            case TIMES:
                            case PLUS:
                            case MINUS:
                                break;
                            default:
                            DLDisposeList(&pushdown);
                            return SYNTAX_ERROR;
                        }
                        DLCopyLast(&pushdown, &a);
                        DLDeleteLast(&pushdown);
                        if(a != E){ 
                            DLDisposeList(&pushdown);
                            return SYNTAX_ERROR;
                        }
                        DLCopyLast(&pushdown, &a);
                        DLDeleteLast(&pushdown);
                        if(a != OPEN_HANDLE){
                            DLDisposeList(&pushdown);
                            return SYNTAX_ERROR;
                        }//insert E
                        DLInsertLast(&pushdown,E);
                        //presun aktivity
                        DLLast(&pushdown);
                        DLCopy(&pushdown, &value);
                        if(value == E || value == OPEN_HANDLE)
                            DLPred(&pushdown);
                        break;
                    
                    case CLOSE_BRACKET:
                        DLCopyLast(&pushdown, &a);
                        DLDeleteLast(&pushdown);
                        if(a != E){
                            DLDisposeList(&pushdown);
                            return SYNTAX_ERROR;
                        }
                        DLCopyLast(&pushdown, &a);
                        DLDeleteLast(&pushdown);
                        if(a != OPEN_BRACKET){
                            DLDisposeList(&pushdown);
                            return SYNTAX_ERROR;
                        }
                        DLCopyLast(&pushdown, &a);
                        DLDeleteLast(&pushdown);
                        if(a != OPEN_HANDLE){
                            DLDisposeList(&pushdown);
                            return SYNTAX_ERROR;
                        }
                        //presn aktivity
                        DLInsertLast(&pushdown,E);
                        DLLast(&pushdown);
                        DLCopy(&pushdown, &value);
                        if(value == E || value == OPEN_HANDLE)
                            DLPred(&pushdown);
                        break;
                    default:
                        DLDisposeList(&pushdown);
                        return SYNTAX_ERROR;
                }
                break;

            case ' ':
                DLDisposeList(&pushdown);
                return SYNTAX_ERROR;
        }
        /*printf("inputExpression");
        print_elements_of_list(inputExpression);
        printf("pushdown");
        print_elements_of_list(pushdown);
        printf("\n------------------------------------------------------------\n");   */
    }
    




    //print_elements_of_list(inputExpression);
    DLDisposeList(&pushdown);
    DLDisposeList(&inputExpression);
    return PREC_OK;
}

