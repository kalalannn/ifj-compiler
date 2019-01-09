/*
** Implementace překladače imperativního jazyka IFJ18
** 
** Vorobiev Nikolaj	xvorob00@stud.fit.vutbr.cz
** Luhin Artsiom	xluhin00@stud.fit.vutbr.cz
*/

#include "main.h"
#include "postfix.h"

extern LTokenList t_list;
TStack t_stack;

void sInit(TStack *s) {
    s->top = 0;
}

void sPush(TStack *s, Token *t) {
    if (s->top < SMAX) {
        strInit(&s->array[s->top]);
        copyToken(&s->array[s->top], t);
        s->top++;
    }
}

void sPop(TStack *s, Token *t) {
    if (s->top > 0) {
        copyToken(t, &s->array[s->top-1]);
        strFree(&s->array[s->top-1]);
        s->top--;
    }
}

Token *sTop(TStack *s) {
    if (s->top > 0)
        return &s->array[s->top-1];
    else
        return NULL;
}

bool sEmpty(TStack *s) {
    return s->top == 0;
}

//LTokenList *list

void getPostfix(LTokenList *list, int until) {
//  list pro toukeny v postfix tvaru

    Token new_token;
    Token tmp;
    strInit(&new_token);
    strInit(&tmp);

	switch (token_id.id) {
	case IDENTIFIER:
	case INTEGER:
	case FLOAT:
	case STRING:
		TLInsert(list, &token_id);
		break;

	case OPEN_BRACKET:
		sPush(&t_stack, &token_id);
		break;
	}

	while (token.id != until) {

		switch (token.id) {
        case IDENTIFIER:
        case INTEGER:
        case FLOAT:
        case STRING:
			TLInsert(list, &token);

			TLGetToken(&t_list, &token);
            break;

        case OPEN_BRACKET:
			sPush(&t_stack, &token);

			TLGetToken(&t_list, &token);
            break;

        case PLUS:
        case MINUS:
        case TIMES:
        case LOMENO:
        case LESS:
        case GREATER:
        case LESS_EQUAL:
        case GREATER_EQUAL:
        case EQUAL:
        case NOT_EQUAL:

			while (!pushOperator(&token, list))
                ; // volame pushOperator dokud se nepodari vlozit operator na vrchol stacku

			TLGetToken(&t_list, &token);
            break;


        case CLOSE_BRACKET:
            sPop(&t_stack, &tmp);

            while (tmp.id != OPEN_BRACKET) {
                TLInsert(list, &tmp);
                sPop(&t_stack, &tmp);
            }

			TLGetToken(&t_list, &token);
            break;
/*
        case ASSIGN:
            while (!sEmpty(&t_stack)) {
                token_p = sPop(&t_stack);
                copyToken(&new_token, token_p);
                TLInsert(&t_list, &new_token);
            }

            copyToken(&new_token, &ltoken_p->token);
            TLInsert(&t_list, &new_token);

            break;
*/

        }
    }

    while (!sEmpty(&t_stack)) {
        sPop(&t_stack, &tmp);
        TLInsert(list, &tmp);
    }

    strFree(&new_token);
    strFree(&tmp);

}

int getPriority(Token *t) {
    switch (t->id) {
    case TIMES:
    case LOMENO:
        return 1;

    case PLUS:
    case MINUS:
        return 2;

    case LESS:
    case GREATER:
    case LESS_EQUAL:
    case GREATER_EQUAL:
        return 3;

    case EQUAL:
    case NOT_EQUAL:
        return 4;

    default:
        return -1; // chyba!!
    }

}

bool pushOperator(Token *t, LTokenList *list) {
    Token *toptoken_p; // docasny token pointer na vrchol stacku
    Token new_token;
    strInit(&new_token);

    if (sEmpty(&t_stack)) {
        sPush(&t_stack, t);
        strFree(&new_token);
        return true;
    } else {
        toptoken_p = sTop(&t_stack);  // docasny token pointer na vrchol stacku

        if (toptoken_p->id == OPEN_BRACKET) {
            sPush(&t_stack, t);
            strFree(&new_token);
            return true;
        } else {
            if (getPriority(toptoken_p) > getPriority(t)) {
                sPush(&t_stack, t);
                strFree(&new_token);
                return true;
            } else {
                sPop(&t_stack, &new_token);
                TLInsert(list, &new_token);
            }
        }
    }

    strFree(&new_token);
    return false;
}
