/*
** Implementace překladače imperativního jazyka IFJ18
** 
** Vorobiev Nikolaj	xvorob00@stud.fit.vutbr.cz
** Luhin Artsiom	xluhin00@stud.fit.vutbr.cz
*/

#define PPOKROJ 1

#include "parser.h"

/*
** ak som spravne pochopil tak ked najdeme zaciatok
** tela funkcie/cyklu/podmienky tak zaovolame tuto funkciu
*/

extern TSTree tree;
extern sList list;
extern InstrTree iTree;
extern Token token_f;

extern Param p1;
extern Param p2;
extern Param p3;


void param_test(){
	strAddString(&token_f, "def");
	strAddString(&p1.param, "nameoffu");
	p1.defined = true;
	GenerateInstruction(&iTree, token_f, LABEL, p1, p2, p3);


	TIDispose(&iTree);
}

void test_instr_table () {
	Token tos;
	strInit(&tos);
	tos.id = INTEGER;
	strAddString(&tos, "100");
	printf("%s\n", tos.str);

	strClear(&tos);

	tos.id = FLOAT;
	strAddString(&tos, "3.14");
	printf("%s\n", tos.str);

	strClear(&tos);

	tos.id = STRING;
	strAddString(&tos, "\n op \t some text 10 \n\t");
	printf("%s\n", tos.str);

	strClear(&tos);

	tos.id = BOOLEAN;
	strAddString(&tos, "true");
	printf("%s\n", tos.str);
	strFree(&tos);

	TIInit(&iTree);

	Token F_name;
	strInit(&F_name);
	strAddString(&F_name, "def");
	TIInsertFunction(&iTree, F_name);
	printf("%s\n", iTree->Name_f.str);

	strClear(&F_name);

	strAddString(&F_name, "print");
	TIInsertFunction(&iTree, F_name);
	printf("%s\n", iTree->LPtr->Name_f.str);

	strClear(&F_name);

	strAddString(&F_name, "neco");
	TIInsertFunction(&iTree, F_name);
	printf("%s\n", iTree->LPtr->RPtr->Name_f.str);

	strClear(&F_name);

	strFree(&F_name);
	TIDispose(&iTree);


	/*Sym symbol;
	Value val;
	int data_type;
	val.Integer = 1039023092;
	data_type = INTEGER;
	symbol = GenConst(val, data_type);
	printf("data_type : %d\nHodnota: %s\n\n", symbol.data_type, symbol.hodnota.str);
	strFree(&symbol.hodnota);

	val.Bool = false;
	data_type = BOOLEAN;
	symbol = GenConst(val, data_type);
	printf("data_type : %d\nHodnota: %s\n\n", symbol.data_type, symbol.hodnota.str);
	strFree(&symbol.hodnota);

	strInit(&val.String);
	strAddString(&val.String, "\tsome_text{{\n");
	data_type = STRING;
	symbol = GenConst(val, data_type);
	printf("data_type : %d\nHodnota: %s\n\n", symbol.data_type, symbol.hodnota.str);
	strFree(&val.String);
	strFree(&symbol.hodnota);

	val.Float = 132390230.532330942304923042;
	data_type = FLOAT;
	symbol = GenConst(val, data_type);
	printf("data_type : %d\nHodnota: %s\n\n", symbol.data_type, symbol.hodnota.str);
	strFree(&symbol.hodnota);


	Id vara = malloc(sizeof (struct id));
	vara->val.Integer = 10;
	Var new;
	strInit(&vara->name);
	strAddString(&vara->name, "name1var");
	new = GenId(vara, GF);
	printf("NAME : %s, \n", new.hodnota.str);
	strFree(&new.hodnota);
	strFree(&vara->name);
	free(vara);*/

}
void test_table() {
	TSInit(&tree);

	Token namef;
	Token name_id;
	Value vall;
	strInit(&name_id);
	strAddString(&namef, "length"); 	//main
	strAddString(&name_id, "first"); 	//main
	vall.Integer = 10;

	TSInsertId(&tree, namef, name_id, PARAM, INTEGER, vall);
	strClear(&name_id);
	strAddString(&name_id, "next"); 	//main
	vall.Float = 10.5;
	TSInsertId(&tree, namef, name_id, PARAM, FLOAT, vall);
	vall.Integer = 100;
	TSInsertId(&tree, namef, name_id, PARAM, INTEGER, vall);
	strInit(&vall.String);
	strAddString(&name_id, "string_prom");
	strAddString(&vall.String, "somestring");
	TSInsertId(&tree, namef, name_id, PARAM, STRING, vall);
	TSPrintTree(&tree);
	printf ("COUNT  :::%d\n", TSCountParams(&tree, namef));
	//printf("%s : hodnota : %d\n", Tree->list.First->name.str, Tree->list.First->val.Integer);
	//TSInsertId(&Tree, namef, name_id, PARAM, STRING, vall);

	Id some = TSSearchId(&tree, namef, name_id);
	if (some == NULL) {
		printf("does not work\n");
	}
	else printf("succeeesss\n");

	strFree(&vall.String);
	strFree(&namef);
	strFree(&name_id);
	TSDispose(&tree);
}

/*void Add2() {
	int err = SYNTAX_OK;
	Token token;
	Token name_f;
	Token prom;
	Token *params;
	strInit(&token);
	strInit(&prom);
	strInit(&name_f);
	err = get_token(&token);
	Error(err);
	for (; token.id != ENDOF ; ) {
		if(token.id == DEF) {
			err = get_token(&token);
			Error(err);
			printf("zapiseme jmeno funkci: %s, %d\n", token.str, token.id);
//## nameOfFunktion
			strCopyString(&name_f, &token);
//## Params
			err = get_token(&token);
			Error(err);
			switch (token.id) {
				case OPEN_BRACKET :
					err = get_token(&token);
					Error(err);
					if(token.id == CLOSE_BRACKET) {
						printf("No params\n");
					}
					else if (token.id  == IDENTIFIER) {
						for( ; token.id = IDENTIFIER; count_params++){
							strInit(&params[count_params]);
							strCopyString(&params[count_params], &token);
							err = get_token(&token);
							Error(err);
							if(token.id == CLOSE_BRACKET) {
								err = get_token(&token);
								Error(err);
								if(token.id == EOL) {
									printf("GOOD Num params: %d\n", count_params);
									break;
								}
								else {
									printf("ERROROROR\n");
									exit(-1);
								}
							}

						}
					}

			}

			
		}
		err = get_token(&token);
		Error(err);
	}
	printf("all is good\n");
	for(int i = 0; i != count_params; i++) {
		strFree(&params[i]);
	}
	strFree(&token);
	strFree(&name_f);
	strFree(&prom);
}*/

void Error(int err) {
		if(err == LEX_ERROR) {
			printf("LEX_ERROR\n");
			exit(-1);
		}
		else if(err == SYNTAX_ERROR) {
			printf("SYNT_ERROR\n");
			exit(-1);
		}
}

/*void AddFunkcion() {
	int err = SYNTAX_OK;
	Token token;
	strInit(&token);
	for (; token.id != ENDOF ; ) {
		err = get_token(&token);
		Error(err);
		if(token.id == DEF) {
			err = get_token(&token);
			Error(err);
			if(token.id == IDENTIFIER) {
				Token name;
				strInit(&name);
				strCopyString(&name, &token);
				err = get_token(&token);
				Error(err);
				Token *params;
				switch (token.id) {
					case OPEN_BRACKET :
						err = get_token(&token);
						Error(err);
						for(int i = 0 ;token.id != CLOSE_BRACKET; i++) {
							if(token.id == IDENTIFIER) {
								strInit(&params[i]);
								strCopyString(&params[i], &token);
							}
							else {
								exit(-1);///errrrrrrr
							}
						}
						err = get_token(&token);
						Error(err);
						if(token.id == EOL) {
							break; //all is good
						}
						else {
								exit(-1);///errrrrrrr
						}

					case IDENTIFIER : 
						strInit(&params[0]);
						strCopyString(&params[0], &token);
						err = get_token(&token);
						Error(err);
						for(int i = 1 ;token.id != EOL; i++) {
							if(token.id == IDENTIFIER) {
								strInit(&params[i]);
								strCopyString(&params[i], &token);
								printf("this works\n");
							}
							else {
								exit(-1);///errrrrrrr
							}
						}
						break;

					case EOL : //zadny param
						strInit(&params[0]);
						params[0].str = NULL;
						params[0].id = NIL;
					break;	

					default:
						exit(-1); //chyba
						break;
				}
				Token minuly;
				for(;token.id != END;){
					err = get_token(&token);
					Error(err);
				}
			}	
			else {
				exit(-1);///errrrrr
			}
		}
	}
}*/


/*void FirstGo() {

	TSListInit(&list);

	Value value;
	Token prom;
	Token data;

	strInit(&prom);
	strAddString(&prom, "my_prom");

	strInit(&data);
	strAddString(&data, "some_string");

	int type = PROM;
	int data_type = STR;

	strInit(&value.String);
	strCopyString(&value.String, &data);

	Insert(&list, prom, type, data_type, value );


	strClear(&prom);
	strAddString(&prom, "my222_prom");

	strClear(&data);
	strAddString(&data, "some_string2222");

	strCopyString(&value.String, &data);
	Insert(&list, prom, type, data_type, value );

	strClear(&prom);
	strAddString(&prom, "NDEF_prom");
	type = DCLR;
	data_type = DCLR;
	Insert(&list, prom, type, data_type, value );

	


	TSPrintList(&list);
	strFree(&prom);
	strFree(&data);
	strFree(&value.String);
	TSDisposeList(&list);
	return;






	Value val;
	Token string;
	//int err = SYNTAX_OK;
	strInit(&string);
	TSInit(&Tree);
	
	strAddString(&string, "nfdslkjfoqifqow;eco");
	val.Integer = 10;
	BSTInsert (&Tree, string, val);
	strClear(&string);

	strAddString(&string, "nic");
	val.Integer = 10;
	BSTInsert (&Tree, string, val);
	strClear(&string);

	strAddString(&string, "eco");
	val.Integer = 10;
	BSTInsert (&Tree, string, val);
	strClear(&string);

	token->str = "nic";
	val.Integer = 130;
	BSTInsert (&Tree, *token, val);
	
	token->str = "eco";
	val.Integer = 10939;
	BSTInsert (&Tree, *token, val);

	token->str = "neco";
	BSTSearch(Tree, *token, &val);
	printf("value: %d\n", val.Integer);

	token->str = "nic";
	BSTSearch(Tree, *token, &val);
	printf("value: %d\n", val.Integer);

	token->str = "eco";
	BSTSearch(Tree, *token, &val);
	printf("value: %d\n", val.Integer);

	printf("\n");
*/
	//printf("Middle : %s \nLeft: %s\nRight: %s\n", Tree->Key.str, Tree->LPtr->Key.str, Tree->RPtr->Key.str);
	
	//strFree(&string);
	//TSDispose(&Tree);
	/*for (int i = 0; token->id != ENDOF ; i++) {
	 	//err = SYNTAX_OK; 
		err = get_token(token);
		printf("this :%s : %d\n ", token->str, i);
		if (token->id == DEF) {
			get_token(token);
			BSTInsert(&Tree, *token, type);
		}
	}*/
	    /*do{
	         int err = get_token(&token); //zavolame funkciu s adresou na token
	         if(err){
	           printf("\nError with Token: %s, %d", token.str, token.id);
	             break;
	         }
	         printf("\nToken: %s, %d", token.str, token.id);
	         if(token.id == EOL)
	            printf(" line no: %d", ++lineCounter);
	  }while(token.id != EOF);*/
//}
