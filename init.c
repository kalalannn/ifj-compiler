/*
** Implementace překladače imperativního jazyka IFJ18
** 
** Vorobiev Nikolaj	xvorob00@stud.fit.vutbr.cz
** Luhin Artsiom	xluhin00@stud.fit.vutbr.cz
*/

#include "init.h"

extern TStack t_stack;
extern TStack pomocny;
extern TSTree tree;
extern TSTree FakeTree;
extern InstrTree iTree;
extern LTokenList t_list;
extern Token retv;

extern Value value;

extern Token token;
extern Token token_id;
extern Token token_f;

extern Param p1;
extern Param p2;
extern Param p3;


void init(){
	TSInit(&tree);
	TIInit(&iTree);
	TSInit(&FakeTree);
	TLInitList(&t_list);


	strInit(&token);
	strInit(&retv);
	strInit(&token_id);
	strInit(&token_f);

	strInit(&p1.param);
	strInit(&p2.param);
	strInit(&p3.param);

	Token namef;
	Token first;
	Token second;
	Token third;

	strInit(&namef);
	strInit(&first);
	strInit(&second);
	strInit(&third);

	strAddString(&namef, "def");
	strAddString(&retv, "-retv");
	TSInsertFunction(&tree, namef);  	//zakladni uzel
	TIInsertFunction(&iTree, namef);	//zakladni uzel

	strAddString(&p1.param, "def");
	OneParams(&p1, &p2, &p3);
	GenerateInstruction(&iTree, namef, LABEL, p1, p2, p3 );
	strClear(&p1.param);
	ZeroParams(&p1, &p2, &p3);
	GenerateInstruction(&iTree, namef, CREATEFRAME, p1, p2, p3 );
	GenerateInstruction(&iTree, namef, PUSHFRAME, p1, p2, p3 );



	strAddString(&first, "if");
	strAddString(&second, "then");
	strAddString(&third, "else");

	strClear(&namef);
	strAddString(&namef, "print");
	//pridani funkci
	TSInsertFunction(&tree, namef);

	strClear(&namef);
	strAddString(&namef, "inputi");
	//pridani funkci
	TSInsertFunction(&tree, namef);  	//zakladni uzel


	strClear(&namef);
	strAddString(&namef, "inputf");
	//pridani funkci
	TSInsertFunction(&tree, namef);  	//zakladni uzel

	strClear(&namef);
	strAddString(&namef, "inputs");
	//pridani funkci
	TSInsertFunction(&tree, namef);  	//zakladni uzel

	strClear(&namef);
	strAddString(&namef, "ord");
	//pridani funkci
	TSInsertFunction(&tree, namef);  	//zakladni uzel

	//pridani parametru
	TSInsertId(&tree, namef, first, PARAM, DCLR, value);
	TSInsertId(&tree, namef, second, PARAM, DCLR, value);


	strClear(&namef);
	strAddString(&namef, "chr");
	//pridani funkci
	TSInsertFunction(&tree, namef);  	//zakladni uzel

	//pridani parametru
	TSInsertId(&tree, namef, first, PARAM, DCLR, value);

	strClear(&namef);
	strAddString(&namef, "length");
	//pridani funkci
	TSInsertFunction(&tree, namef);  	//zakladni uzel

	//pridani parametru
	TSInsertId(&tree, namef, first, PARAM, DCLR, value);

	strClear(&namef);
	strAddString(&namef, "substr");
	//pridani funkci
	TSInsertFunction(&tree, namef);  	//zakladni uzel

	//pridani parametru
	TSInsertId(&tree, namef, first, PARAM, DCLR, value);
	TSInsertId(&tree, namef, second, PARAM, DCLR, value);
	TSInsertId(&tree, namef, third, PARAM, DCLR, value);

	strFree(&first);
	strFree(&second);
	strFree(&third);
	strFree(&namef);
}

void unInit() {
	strFree(&token);
	strFree(&token_id);
	strFree(&token_f);
	strFree(&retv);

	strFree(&p1.param);
	strFree(&p2.param);
	strFree(&p3.param);

	TIDispose(&iTree);
	TSDispose(&tree);
	TSDispose(&FakeTree);
	TLDisposeList (&t_list);
}
