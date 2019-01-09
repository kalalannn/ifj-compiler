/*
** Implementace překladače imperativního jazyka IFJ18
** 
** Vorobiev Nikolaj	xvorob00@stud.fit.vutbr.cz
** Luhin Artsiom	xluhin00@stud.fit.vutbr.cz
*/

#include "itable.h"
extern Token token_f;

void HeadInstr() {
	printf(".IFJcode18\n");
	printf("JUMP def\n");
	printf("\n");
	/*printf("LABEL def\n");
	printf("CREATEFRAME\n");
	printf("PUSHFRAME\n");
	printf("POPFRAME\n");
	printf("\n");*/
}

void GenNil (Param *par) {
	strClear(&par->param);
	strAddString(&par->param, "nil@nil");
}

void TIListInit (iList *list) {
	list->First = NULL;
	list->Act = NULL;
	list->Last = NULL;
}

void TIInit (InstrTree *RootPtr) {
	*RootPtr = NULL;
}

void TIDisposeList(iList *list) {
	list->Act = list->First;
	void* tmp;
	for(; list->Act != NULL; ) {
		strFree(&list->Act->instr);
		strFree(&list->Act->par1.param);
		strFree(&list->Act->par2.param);
		strFree(&list->Act->par3.param);
		tmp = list->Act;
		list->Act = list->Act->next;
		free(tmp);
	}
}

void TIDispose (InstrTree *RootPtr) {
	if(*RootPtr != NULL) { 				//pokud mame co rusit
		TIDisposeList(&(*RootPtr)->List);
		TIDispose(&(*RootPtr)->LPtr); 	//rusime doleva
		TIDispose(&(*RootPtr)->RPtr); 	//rusime doprava
		free((*RootPtr)->Name_f.str);
		free(*RootPtr); 				//uvolnime pamet'
		TIInit(RootPtr); 				//inicializace stromu
	}
}
int if_counter = 0;
int while_counter =0;
int else_counter = 0;
int endif_counter = 0;
int endwhile_counter = 0;
void GenIfLabel(Param *par, int typ, int cykl) {
	char *number = malloc(sizeof (char) * 5);
	int tmp =0;

	switch(typ){
		case IF:
			strAddString(&par->param, "-if");
			sprintf(number, "%d", if_counter);
			strAddString(&par->param, number);
			if_counter++;
			break;

		case WHILE:
			strAddString(&par->param, "-while");
			sprintf(number, "%d", while_counter);
			strAddString(&par->param, number);
			while_counter++;
			break;

		case ELSE:
			strAddString(&par->param, "-else");
			sprintf(number, "%d", else_counter);
			strAddString(&par->param, number);
			else_counter++;
			break;

		case END:
			strAddString(&par->param, "-end");
			if(endif_counter != if_counter && cykl == IF) {
				tmp = endif_counter;
				endif_counter = if_counter;
				sprintf(number, "%d", endif_counter-1);
				strAddString(&par->param, number);
				endif_counter = tmp;
				endif_counter++;
			}
			else if(while_counter != endwhile_counter && cykl == WHILE) {
				if(endif_counter != if_counter && cykl == IF) {
					/*tmp = endwhile_counter;
					endwhile_counter = while_counter;
					sprintf(number, "%d", endif_counter);
					strAddString(&par->param, number);
					endif_counter = tmp;
					endif_counter++;*/
				}
			}
			break;

	}
}

void TIInsertFunction (InstrTree *RootPtr, Token name_f) {
	if((*RootPtr) == NULL) { 		//uz jsme dolu, nebo strom je prazdny
		InstrTree item ; 			//vytvorime uzel
		if((item = malloc(sizeof(struct instrTree))) == NULL) { //alokujeme misto
			printf("Allocation Error\n");
		}
		else {
			strInit(&item->Name_f);
			strCopyString(&item->Name_f, &name_f); 					//Dame uzlovi klic
			item->RPtr = NULL; 				//Bude bezdetny
			item->LPtr = NULL; 				//-------------
			*RootPtr = item; 				//zapojime do stromu
			TIListInit(&(*RootPtr)->List);		//inicializace listu
		}
	}
	else if ((*RootPtr) != NULL) {	//Nejsme dolu, ale se LOMENOame na uzel
		if (strCmpString ( &(*RootPtr)->Name_f,  &name_f)  == 0 ) { // Kdyz koren je mensi jdeme doprava
		}
		if (strCmpString ( &(*RootPtr)->Name_f,  &name_f)  > 0 ) { // Kdyz koren je mensi jdeme doprava
			TIInsertFunction(&((*RootPtr)->RPtr), name_f);
		}
		if (strCmpString ( &(*RootPtr)->Name_f,  &name_f)  < 0 ) { // Kdyz koren je mensi jdeme doprava
			TIInsertFunction(&((*RootPtr)->LPtr), name_f);
		}
	}
}

void GenLabel(Token tok, Param *par) {
	copyToken(&par->param, &tok);
}
void GenConst(Token tok, Param *par) {
	Token pomoc;
	strInit(&pomoc);
	if (tok.id == INTEGER) {
		strAddString(&pomoc, "int@");
		strAddString(&pomoc, tok.str);
	}
	else if (tok.id == BOOLEAN) {
		strAddString(&pomoc, "bool@");
		strAddString(&pomoc, tok.str);
	}
	else if (tok.id == STRING) {
		char pomocna;
		int kod;
		char *need;
		strAddString(&pomoc, "string@");
		for (int i = 0; i < strGetLength(&tok); i++) {
			pomocna = tok.str[i] ;
			if ((pomocna >= 48 && pomocna <= 57) || (pomocna >=65 && pomocna <= 90) || \
					(pomocna >= 97 && pomocna <=122)) { //cislo, A-Z, a-z
				strAddChar(&pomoc, pomocna);
			}
			else {
				kod = (int) pomocna;
				need = malloc(sizeof (char) * 10);
				if(pomocna < 10) {
					sprintf(need, "%s%d","\\00", kod);
					strAddString(&pomoc, need);
				}
				else if(pomocna >= 10 && pomocna < 100) {
					sprintf(need, "%s%d","\\0", kod);
					strAddString(&pomoc, need);
				}
				else if (pomocna >= 100) {
					sprintf(need, "%s%d","\\", kod);
					strAddString(&pomoc, need);
				}
				free(need);
			}
		}
	}
	else if (tok.id == FLOAT) {
		char *flo = malloc(sizeof (char) * 33);
		double doub = strtod(tok.str, NULL);
		strAddString(&pomoc, "float@");
		sprintf(flo, "%a", doub);
		strAddString(&pomoc, flo);
		free(flo);
	}
	strCopyString(&par->param, &pomoc);
	strFree(&pomoc);
	par->defined = true;
}


void GenId(Token tok, Param *par){
	Token pomoc;
	strInit(&pomoc);
			strAddString(&pomoc, "LF@");
			strAddString(&pomoc, tok.str);
	strCopyString(&par->param, &pomoc);
	strFree(&pomoc);
}

void TIAddInstruction (InstrTree *RootPtr, Token name_f, Instruction instruct) {
	if((*RootPtr) == NULL) {
		return;
	}
	else if (strCmpString(&(*RootPtr)->Name_f, &name_f) == 0 ) {
		if((*RootPtr)->List.First == NULL) {
			instruct->next = NULL;
			(*RootPtr)->List.First = instruct;
			(*RootPtr)->List.Act = instruct;
			(*RootPtr)->List.Last = instruct;
		}
		else {
			(*RootPtr)->List.Last->next = instruct;
			(*RootPtr)->List.Last = instruct;
			(*RootPtr)->List.Last->next = NULL;
		}
	}
	else if (strCmpString(&(*RootPtr)->Name_f, &name_f) > 0 ) {
		TIAddInstruction(&(*RootPtr)->RPtr, name_f, instruct);
	}
	else if (strCmpString(&(*RootPtr)->Name_f, &name_f) < 0 ) {
		TIAddInstruction(&(*RootPtr)->LPtr, name_f, instruct);
	}
}

/*
 * 				     F, A	  Last
 *   	  			  | 	   |
 *   t -> t -> EOL -> t -> t ->t -> NULL
 *
 */

void ZeroParams(Param *p1, Param *p2, Param *p3) {
	p1->defined = p2->defined = p3->defined = false;
}

void OneParams(Param *p1, Param *p2, Param *p3) {
	p1->defined = true;
	p2->defined = p3->defined = false;
}

void TwoParams(Param *p1, Param *p2, Param *p3) {
	p1->defined = p2->defined = true;
	p3->defined = false;
}

void ThreeParams(Param *p1, Param *p2, Param *p3) {
	p1->defined = p2->defined = p3->defined = true;
}

void GenerateInstruction(InstrTree *RootPtr, Token name_f, Instr_enum instr, Param par1,\
							 Param par2, Param par3) {
	Instruction instruct = malloc(sizeof (struct instruction));
	strInit(&instruct->instr);
	strInit(&instruct->par1.param);
	strInit(&instruct->par2.param);
	strInit(&instruct->par3.param);
	switch (instr) {

		case CLEARS:
		case ADDS:
		case SUBS:
		case MULS:
		case DIVS:
		case IDIVS:
		case CREATEFRAME:
		case PUSHFRAME:
		case POPFRAME:
		case RETURN:
		case INT2FLOAT:
		case LTS:
		case GTS:
		case EQS:
		case NOTS:
			if(!(par1.defined || par2.defined || par3.defined)){
				switch (instr) {
					case NOTS:
						strAddString(&instruct->instr, "NOTS");
						break;

					case LTS:
						strAddString(&instruct->instr, "LTS");
						break;

					case GTS:
						strAddString(&instruct->instr, "GTS");
						break;

					case EQS:
						strAddString(&instruct->instr, "EQS");
						break;

					case INT2FLOAT:
						strAddString(&instruct->instr, "INT2FLOAT");
						break;

					case RETURN:
						strAddString(&instruct->instr, "RETURN");
						break;

					case CLEARS:
						strAddString(&instruct->instr, "CLEARS");
						break;

					case ADDS:
						strAddString(&instruct->instr, "ADDS");
						break;

					case SUBS:
						strAddString(&instruct->instr, "SUBS");
						break;

					case MULS:
						strAddString(&instruct->instr, "MULS");
						break;

					case DIVS:
						strAddString(&instruct->instr, "DIVS");
						break;

					case IDIVS:
						strAddString(&instruct->instr, "IDIVS");
						break;

					case CREATEFRAME:
						strAddString(&instruct->instr, "CREATEFRAME");
						break;

					case PUSHFRAME:
						strAddString(&instruct->instr, "PUSHFRAME");
						break;

					case POPFRAME:
						strAddString(&instruct->instr, "POPFRAME");
						break;

					default:
						break;

				}
				instruct->par1.defined = instruct->par2.defined\
						= instruct->par3.defined = false;
				TIAddInstruction(RootPtr, name_f, instruct);
				break;

		case LABEL:
		case PUSHS:
		case POPS:
		case DEFVAR:
		case CALL:
		case WRITE:
		case JUMP:
		case JUMPIFEQS:
			if(par1.defined) {
				switch (instr) {

					case JUMPIFEQS:
						strAddString(&instruct->instr, "JUMPIFEQS");
					break;

					case JUMP:
						strAddString(&instruct->instr, "JUMP");
					break;

					case WRITE:
						strAddString(&instruct->instr, "WRITE");
					break;

					case CALL:
						strAddString(&instruct->instr, "CALL");
						break;

					case DEFVAR:
						strAddString(&instruct->instr, "DEFVAR");
						break;

					case LABEL:
						strAddString(&instruct->instr, "LABEL");
						break;

					case PUSHS:
						strAddString(&instruct->instr, "PUSHS");
						break;

					case POPS:
						strAddString(&instruct->instr, "POPS");
						break;

					default :
						break;
				}
				strCopyString(&instruct->par1.param, &par1.param);
				instruct->par1.defined = true;
				instruct->par2.defined = instruct->par3.defined = false;
				TIAddInstruction(RootPtr, name_f, instruct);
				break;

			case STRLEN:
			case READ:
			case MOVE:
			case INT2CHAR:
				if(par1.defined && par2.defined) {
					switch (instr) {
						case MOVE:
							strAddString(&instruct->instr, "MOVE");
							break;

						case STRLEN:
							strAddString(&instruct->instr, "STRLEN");
							break;

						case READ:
							strAddString(&instruct->instr, "READ");
							break;

						case INT2CHAR:
							strAddString(&instruct->instr, "INT2CHAR");
							break;

						default:
							break;
					}
				}
				strCopyString(&instruct->par1.param, &par1.param);
				strCopyString(&instruct->par2.param, &par2.param);
				instruct->par1.defined = instruct->par2.defined = true;
				instruct->par3.defined = false;
				TIAddInstruction(RootPtr, name_f, instruct);
				break;

			case CONCAT:
			case GETCHAR:
				if(par1.defined && par2.defined && par3.defined) {
					switch (instr) {
						case CONCAT:
							strAddString(&instruct->instr, "CONCAT");
							break;

						case GETCHAR:
							strAddString(&instruct->instr, "GETCHAR");
							break;

						default :
							break;
					}
				}
				strCopyString(&instruct->par1.param, &par1.param);
				strCopyString(&instruct->par2.param, &par2.param);
				strCopyString(&instruct->par3.param, &par3.param);
				instruct->par1.defined = instruct->par2.defined =\
						instruct->par3.defined = true;
				TIAddInstruction(RootPtr, name_f, instruct);
				break;
			}
			break;
		}
		break;
	default:
		break;
	}
}

int ICountParams(Param par1, Param par2, Param par3) {
	int count = 0;
	count = par1.defined + par2.defined + par3.defined;
	return count;
}
void TIPrintList(iList *list, Token *namef) {
	int c = 0;
	if(list->Act != NULL && strCmpConstStr(namef, "def") != 0) {
		c = ICountParams(list->Act->par1, list->Act->par2, list->Act->par3);
		switch (c){
			case 0:
				printf("%s \n", \
						list->Act->instr.str);
				break;
			case 1:
				printf("%s %s\n", \
						list->Act->instr.str, list->Act->par1.param.str);
				break;
			case 2:
				printf("%s %s %s\n", \
						list->Act->instr.str, list->Act->par1.param.str, \
						list->Act->par2.param.str);
				break;
			case 3:
				printf("%s %s %s %s\n", \
						list->Act->instr.str, list->Act->par1.param.str, \
						list->Act->par2.param.str, list->Act->par3.param.str);
				break;
		}
		list->Act = list->Act->next;
		TIPrintList(list, namef);
	}
	else
		list->Act = list->First;
}

void TIPrintTree(InstrTree *RootPtr) {
	if(*RootPtr != NULL) { 				//pokud mame co rusit
			TIPrintList(&(*RootPtr)->List, &(*RootPtr)->Name_f);
			printf("\n");
			TIPrintTree(&(*RootPtr)->LPtr); 	//rusime doleva
			TIPrintTree(&(*RootPtr)->RPtr); 	//rusime doprava
	}
}

void TIPrintDef(InstrTree *RootPtr) {
	strAddString(&token_f, "lalala");
	TIPrintList(&(*RootPtr)->List, &token_f);
}

