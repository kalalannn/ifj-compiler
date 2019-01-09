/*
** Implementace překladače imperativního jazyka IFJ18
** 
** Vorobiev Nikolaj	xvorob00@stud.fit.vutbr.cz
** Luhin Artsiom	xluhin00@stud.fit.vutbr.cz
*/


/**   TSInit ...... inicializace vyhledávacího stromu
**   BSTSearch .... vyhledávání hodnoty uzlu zadaného klíčem
**   BSTInsert .... vkládání nové hodnoty
**   BSTDelete .... zrušení uzlu se zadaným klíčem
**   TSDispose ... zrušení celého stromu
**/

#include "symtable.h"


void TSListInit (sList *list) {
	list->First = NULL;
	list->Act = NULL;
	list->Last = NULL;
}

void TSInit (TSTree *RootPtr) {
	*RootPtr = NULL;
}

Id SearchId (sList *list, Token name_id) {  //najde a vraci ukazatel na promenu
	Id pomoc = list->Act;
	if (list->Act != NULL) {
		if (strCmpString(&list->Act->name, &name_id) == 0) {
			list->Act = list->First;
		}
		else {
			list->Act = list->Act->next;
			return SearchId(list, name_id);
		}
	}
	else if (list->Act == NULL){
		list->Act = list->First;
		return NULL;
	}
	return pomoc;
}

void TSListInsertId (sList *list, Token name_id, int type, int data_type, Value value) {
	Id item = NULL;
	item = SearchId(list, name_id);

	if (item == NULL) {
		if ((item = malloc (sizeof(struct id))) == NULL) { //identifier
			printf("Allocation Error\n");
		}
		strInit(&item->name);
		strCopyString(&item->name, &name_id);
		item->type = type;
		item->data_type = data_type;
		if(data_type == STRING) {
			strInit(&item->val.String);
			strCopyString(&item->val.String, &value.String);
		}
		else if (data_type == INTEGER) {
			item->val.Integer = value.Integer;
		}
		else if (data_type == FLOAT) {
			item->val.Float = value.Float;
		}
		else if(data_type == DCLR) {}
		item->next = NULL; 			//vzdycky push na konec
		if(list->First == NULL) {
			list->First = item;
			list->Act = item;
			list->Last = item;
		}
		else {
			list->Last->next = item;  //null -> item
			list->Last = item;		  //Null -> item
		}							  //before -> item(list->Last) -> null
	}
	else {
		if(item->data_type == STRING) { //puvodni byl string
			if(data_type == STRING) {   //cil taky string
				strClear(&item->val.String);
				strCopyString(&item->val.String, &value.String);
			}
			else {
				strFree(&item->val.String);
			}
		}
		else {
			if(data_type == STRING) {
				strInit(&item->val.String);
				strCopyString(&item->val.String, &value.String);
			}
			else if (data_type == INTEGER) {
				item->val.Integer = value.Integer;
			}
			else if (data_type == FLOAT) {
				item->val.Float = value.Float;
			}
			else if(data_type == DCLR) {}
		}
		item->data_type = data_type;
	}
}

void ChangeTypParam(TSTree *RootPtr, Token *name_f, int NumPar, int data_type){
	if((*RootPtr) == NULL) {
		return;
	}
	else if (strCmpString(&(*RootPtr)->Key, name_f) == 0 ) { //nasli funkci
		(*RootPtr)->list.Act = (*RootPtr)->list.First;
		while(NumPar != 0){
			(*RootPtr)->list.Act = (*RootPtr)->list.Act->next;
		}
		(*RootPtr)->list.Act->data_type = data_type;
		(*RootPtr)->list.Act = (*RootPtr)->list.First;
	}
	else if (strCmpString(&(*RootPtr)->Key, name_f) > 0 ) {
		ChangeTypParam(&(*RootPtr)->RPtr, name_f, NumPar, data_type);
	}
	else if (strCmpString(&(*RootPtr)->Key, name_f) < 0 ) {
		ChangeTypParam(&(*RootPtr)->LPtr, name_f, NumPar, data_type);
	}
}

void TSInsertId (TSTree *RootPtr, Token name_f, Token name_id, int type, int data_type, Value value) { //prom
	if ((*RootPtr) == NULL) {
		return;
	}
	else if(strCmpString(&(*RootPtr)->Key, &name_f) == 0) {
		TSListInsertId(&(*RootPtr)->list, name_id, type, data_type, value);
	}
	else if(strCmpString(&(*RootPtr)->Key, &name_f) > 0) {
		TSInsertId(&((*RootPtr)->RPtr), name_f, name_id, type, data_type, value);
	}
	else if(strCmpString(&(*RootPtr)->Key, &name_f) < 0) {
		TSInsertId(&((*RootPtr)->LPtr), name_f, name_id, type, data_type, value);
	}
}

int TSListCountParams(sList *list) {
	int count = 0;
	list->Act = list->First;
	for (;list->Act != NULL && list->Act->type == PARAM; count++) {
		list->Act = list->Act->next;
	}
	list->Act = list->First;
	return count;
}
int TSCountParams(TSTree *RootPtr, Token name_f) {
	if ((*RootPtr) == NULL) {
		return -1;
	}
	else if(strCmpString(&(*RootPtr)->Key, &name_f) == 0) { //nasli sme
		return TSListCountParams(&(*RootPtr)->list);
	}
	else if(strCmpString(&(*RootPtr)->Key, &name_f) > 0) {
		return TSCountParams(&((*RootPtr)->RPtr), name_f);
	}
	else if(strCmpString(&(*RootPtr)->Key, &name_f) < 0) {
		return TSCountParams(&((*RootPtr)->LPtr), name_f);
	}
	return OK; //nikdy se nevykona
}



TSTree TSSearchFunction (TSTree *RootPtr, Token name_f){
	if ((*RootPtr) == NULL) {
		return NULL;
	}
	else if(strCmpString(&(*RootPtr)->Key, &name_f) == 0) { //nasli sme
		return *RootPtr;
	}
	else if(strCmpString(&(*RootPtr)->Key, &name_f) > 0) {
		return TSSearchFunction(&((*RootPtr)->RPtr), name_f);
	}
	else if(strCmpString(&(*RootPtr)->Key, &name_f) < 0) {
		return TSSearchFunction(&((*RootPtr)->LPtr), name_f);
	}
	return OK; //nikdy se nevykona
}
Id TSSearchId(TSTree *RootPtr, Token name_f, Token name_id) {
	if ((*RootPtr) == NULL) {
		return NULL;
	}
	else if(strCmpString(&(*RootPtr)->Key, &name_f) == 0) { //nasli sme
		(*RootPtr)->list.Act = (*RootPtr)->list.First;
		return SearchId(&(*RootPtr)->list, name_id);
	}
	else if(strCmpString(&(*RootPtr)->Key, &name_f) > 0) {
		return TSSearchId(&((*RootPtr)->RPtr), name_f, name_id);
	}
	else if(strCmpString(&(*RootPtr)->Key, &name_f) < 0) {
		return TSSearchId(&((*RootPtr)->LPtr), name_f, name_id);
	}
	return OK;
}

int getType (TSTree * RootPtr, Token nameF, Token nameId){
	Id uk;
	uk = TSSearchId(RootPtr, nameF, nameId);
	return uk->data_type;
}

void ChangeDataType(TSTree *RootPtr, Token nameF, Token nameId, int data_type) {
	Id uk =TSSearchId(RootPtr, nameF, nameId);
	uk->data_type = data_type;
}

void TSInsertFunction (TSTree* RootPtr, Token name_f) {
	if((*RootPtr) == NULL) { 		//uz jsme dolu, nebo strom je prazdny
		TSTree item ; 			//vytvorime uzel
		if((item = malloc(sizeof(struct symbTree))) == NULL) { //alokujeme misto
		}
		else {
			strInit(&item->Key);
			strCopyString(&item->Key, &name_f); 					//Dame uzlovi klic
			item->RPtr = NULL; 				//Bude bezdetny
			item->LPtr = NULL; 				//-------------
			*RootPtr = item; 				//zapojime do stromu
			TSListInit(&(*RootPtr)->list);		//inicializace listu
		}
	} 
	else if ((*RootPtr) != NULL) {	//Nejsme dolu, ale se LOMENOame na uzel 
		if (strCmpString ( &(*RootPtr)->Key,  &name_f)  == 0 ) { // Kdyz koren je mensi jdeme doprava
		}
		if (strCmpString ( &(*RootPtr)->Key,  &name_f)  > 0 ) { // Kdyz koren je mensi jdeme doprava
			TSInsertFunction(&((*RootPtr)->RPtr), name_f);
		}
		if (strCmpString ( &(*RootPtr)->Key,  &name_f)  < 0 ) { // Kdyz koren je mensi jdeme doprava
			TSInsertFunction(&((*RootPtr)->LPtr), name_f);
		}
	} 
}
void TSPrintList(sList *list) {
	if(list->Act != NULL) {
		if(list->Act->data_type == STRING) {
			printf("\nName: %s\nTyp: %d\nData_type: %d\nValue: %s\n\n", \
					list->Act->name.str, list->Act->type, \
					list->Act->data_type, list->Act->val.String.str);
		}
		else if(list->Act->data_type == INTEGER) {
			printf("\nName: %s\nTyp: %d\nData_type: %d\nValue: %d\n\n", \
					list->Act->name.str, list->Act->type, \
					list->Act->data_type, list->Act->val.Integer);
		}
		else if (list->Act->data_type == FLOAT) {
			printf("\nName: %s\nTyp: %d\nData_type: %d\nValue: %f\n\n", \
					list->Act->name.str, list->Act->type, \
					list->Act->data_type, (double)list->Act->val.Float);
		}
		else if (list->Act->data_type == DCLR) {
			printf("\nName: %s\nTyp: %d\nData_type: %d\nValue: %s\n\n", \
					list->Act->name.str, list->Act->type, \
					list->Act->data_type, "DCLR");
		}
		list->Act = list->Act->next;
		TSPrintList(list);
	}
	else
		list->Act = list->First;
}

void TSPrintTree(TSTree *RootPtr) {
	if(*RootPtr != NULL) { 				//pokud mame co rusit
		TSPrintTree(&(*RootPtr)->LPtr); 	//rusime doleva
		TSPrintTree(&(*RootPtr)->RPtr); 	//rusime doprava
		printf("Name of Funkcion : %s\n", (*RootPtr)->Key.str);
		TSPrintList(&(*RootPtr)->list);
	}
}

void TSDisposeList(sList *list) {
	list->Act = list->First;
	Id tmp;
	for(; list->Act != NULL; ) {
		strFree(&list->Act->name);
		if(list->Act->data_type == STRING) {
			strFree(&list->Act->val.String);
		}
		tmp = list->Act;
		list->Act = list->Act->next;
		free(tmp);
	}
}

void TSDispose (TSTree *RootPtr) {
	if(*RootPtr != NULL) { 				//pokud mame co rusit
		TSDisposeList(&(*RootPtr)->list);
		TSDispose(&(*RootPtr)->LPtr); 	//rusime doleva
		TSDispose(&(*RootPtr)->RPtr); 	//rusime doprava
		free((*RootPtr)->Key.str);
		free(*RootPtr); 				//uvolnime pamet'
		TSInit(RootPtr); 				//inicializace stromu
	}
}
