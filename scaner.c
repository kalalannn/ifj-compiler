/*
** Implementace překladače imperativního jazyka IFJ18
** 
** Koša Benjamín	xkosab00@stud.fit.vutbr.cz
*/

//jednoducha knihovna pro praci s nekonecne dlouhymi retezci
#include"token_list.h"
extern LTokenList t_list;


// konstanta STR_LEN_INC udava, na kolik bytu provedeme pocatecni alokaci pameti
// pokud nacitame retezec znak po znaku, pamet se postupne bude alkokovat na
// nasobky tohoto cisla 




int strInit(Token *s)
// funkce vytvori novy retezec
{
   if ((s->str = (char*) malloc(STR_LEN_INC)) == NULL)
	  return INERT_ERR;
   s->str[0] = '\0';
   s->used = 0;
   s->size = STR_LEN_INC;
   return OK;
}

void strFree(Token *s)
// funkce uvolni retezec z pameti
{
   free(s->str);
   s->str = NULL;
   s->used = s->size = 0;
   s->id = 0;
}

void strClear(Token *s)
// funkce vymaze obsah retezce
{
   s->str[0] = '\0';
   s->used = 0;
   s->id = 0;
}

int strAddChar(Token *s1, char c)
// prida na konec retezce jeden znak
{
   if (s1->used + 1 >= s1->size)
   {
      // pamet nestaci, je potreba provest realokaci
      if ((s1->str = (char*) realloc(s1->str, s1->used + STR_LEN_INC)) == NULL)
		 return INERT_ERR;
      s1->size = s1->used + STR_LEN_INC;
   }
   s1->str[s1->used] = c;
   s1->used++;
   s1->str[s1->used] = '\0';
   return OK;
}

void strAddString(Token *s1, char *c) {
	//prida *c do *s1
	int size = (int)strlen(c);
	for (int i = 0 ; i < size; i++) {
		strAddChar(s1, c[i]);
	}
}

int strCopyString(Token *s1, Token *s2)
// prekopiruje retezec s2 do s1
{
   size_t newLength = s2->used;
   if (newLength >= s1->size)
   {
      // pamet nestaci, je potreba provest realokaci
      if ((s1->str = (char*) realloc(s1->str, newLength + 1)) == NULL)
         return INERT_ERR;
      s1->size = newLength + 1;
   }
   strcpy(s1->str, s2->str);
   s1->used = newLength;
   return OK;
}

int copyToken(Token *t1, Token *t2) {
// prekopiruje token2 do t1
	if(t1->str == NULL)
		return FALSE;
    size_t newLength = t2->used;
    if (newLength >= t1->size) {
        if ((t1->str = (char *) realloc(t1->str, newLength + 1)) == NULL)
            return INERT_ERR;
        t1->size = newLength + 1;
    }
    strcpy(t1->str, t2->str);
    t1->used = newLength;

    t1->id = t2->id;
    return OK;
}

int strCmpString(Token *s1, Token *s2)
// porovna oba retezce a vrati vysledek
{
   return strcmp(s1->str, s2->str);
}

int strCmpConstStr(Token *s1, char* s2)
// porovna nas retezec s konstantnim retezcem
{
   return strcmp(s1->str, s2);
}

char *strGetStr(Token *s)
// vrati textovou cast retezce
{
   return s->str;
}

int strGetLength(Token *s)
// vrati delku daneho retezce
{
   return (int)s->used;
}

extern int lastC;
extern int firstScan;
int firstScan = 0;

int get_token(Token *localToken) {

    //subor sa cita po jednom znaku a ten s uklada do c
    int c = '\n';
    int lastC = '\n';

    //pomocna premenna 
    int dot = 0;//bodka v floar
    int e = 0;//exponent vo float
    int countDes = 0; //pocita destatinu cast
    int countExp = 0; //pocita exponencialnu cast
            
    while (1){

        //strFree(localToken);
        if(localToken->size != 0){
            strClear(localToken);
            strFree(localToken);
            strInit(localToken);
        }
        if(firstScan == 1)
		    c = getchar();// ziskame prvy znak zo suboru
        else
            firstScan = 1;
        
        switch(c){
            //filtrovanie jednoriadkovich komentarov
            case '#':
                while(1){
					lastC = c; c = getchar();
                    if(c == '\n' || c == EOF)
                        break;
                }
				ungetc(c, stdin);
                break;

            
            //identifikator
            case '_':
            case 'a'...'z':
                while(1){
					strAddChar(&(*localToken), (char) c);
					lastC = c; c = getchar();//a, =
                    switch(c){
                        case 'a'...'z':
                        case '0'...'9':
                        case 'A'...'Z':
                        case '_':
                            continue;
                        case '!':
                        case '?':
                            strAddChar(&(*localToken), (char) c);
                            lastC = c; c = getchar();
							(*localToken).id = IDENTIFIER;
                            break;
                        default:
                            break;
                    }
					ungetc(c, stdin); //vrati sa o char spat v subore
                    break;            
                }
                //priradovanie id
					(*localToken).id = IDENTIFIER;

                if(strcmp((*localToken).str, "def") == 0) (*localToken).id = DEF;
                else if(strcmp((*localToken).str, "do") == 0) (*localToken).id = DO;
                else if(strcmp((*localToken).str, "else") == 0) (*localToken).id = ELSE;
                else if(strcmp((*localToken).str, "end") == 0) (*localToken).id = END;
                else if(strcmp((*localToken).str, "if") == 0) (*localToken).id = IF;
                else if(strcmp((*localToken).str, "not") == 0) (*localToken).id = NOT;
                else if(strcmp((*localToken).str, "nil") == 0) (*localToken).id = NIL;
                else if(strcmp((*localToken).str, "then") == 0) (*localToken).id = THEN;
                else if(strcmp((*localToken).str, "while") == 0) (*localToken).id = WHILE;
                
                else if(strcmp((*localToken).str, "inputs") == 0) (*localToken).id = INPUTS;
                else if(strcmp((*localToken).str, "inputi") == 0) (*localToken).id = INPUTI;
                else if(strcmp((*localToken).str, "inputf") == 0) (*localToken).id = INPUTF;
                else if(strcmp((*localToken).str, "print") == 0) (*localToken).id = PRINT;
                else if(strcmp((*localToken).str, "length") == 0) (*localToken).id = LENGTH;
                else if(strcmp((*localToken).str, "substr") == 0) (*localToken).id = SUBSTR;
                else if(strcmp((*localToken).str, "ord") == 0) (*localToken).id = ORD;
                else if(strcmp((*localToken).str, "chr") == 0) (*localToken).id = CHR;

				TLInsert(&t_list, localToken);
                return 0;
            
            //detekcia int a float TODO nemam este uplne jasno v tom ako presne ma vyzerat cislo
            case '0'...'9':          
                while(1){
					strAddChar(&(*localToken),  (char)c);
					lastC = c; c = getchar();
                    if (dot == 1 && e == 0)
                        countDes++;
                    if (dot == 1 && e == 1)
                        countExp++;
                    switch(c){
                        case '0'...'9'://int
                            continue;
                        case '.'://float
                            dot += 1;
                            continue;
                        case 'e'://float with exponent
                        case 'E':
                            e += 1;
                            //strAddChar(&(*localToken), c);
							lastC = c; c = getchar(); //osterenie ze znamienko moze byt len za e/E
                            if(c == '+' || c == '-'){
                                strAddChar(&(*localToken), 'e');
                                continue;
                            }else{
								ungetc(c, stdin);
                                c = 'e';
                            }
                            continue;
                        default:
                            break;
                    }
					ungetc(c, stdin);
                    break;            
                }
                
                if (dot == 1 && e == 1){//cislo je typu float napr 1.5e2
                    if(countDes == 1 || countExp == 1)//cislo ma nulovy exponent alebo desatinu cast
                        return LEX_ERROR;
                    (*localToken).id = FLOAT;
                }else if (dot == 1 && e == 0){//cislo je typu float napr 1.5
                    if(countDes == 1)
                        return LEX_ERROR;
                    (*localToken).id = FLOAT;
                }else if (dot == 0 && e == 1){//cislo je typu float napr 15e2
                    if(countExp == 1)
                        return LEX_ERROR;
                    (*localToken).id = FLOAT;
                } else if (dot > 1 || e > 1) {//chyba float obsahuje viac ako 1 bodku/e
                    return LEX_ERROR;
                } else {//cislo je typu integer
                    char *endptr;
                    if (strtoimax((*localToken).str,&endptr,10) > INT_MAX)
                        return LEX_ERROR;
                    (*localToken).id = INTEGER;
                    //ungetc(c, stdin);
                }
                dot = 0;
                e = 0;
                //strFree(&(*localToken));
				TLInsert(&t_list, localToken);
				return 0;
            
            //jednosymbolove tokeny
            case '+':
                (*localToken).id = PLUS;
				TLInsert(&t_list, localToken);
				return 0;
            case '-':
                (*localToken).id = MINUS;
				TLInsert(&t_list, localToken);
				return 0;
            case '*': 
                (*localToken).id = TIMES;
				TLInsert(&t_list, localToken);
				return 0;
            case '/': 
                (*localToken).id = LOMENO;
				TLInsert(&t_list, localToken);
				return 0;
            case '(': 
                (*localToken).id = OPEN_BRACKET;
				TLInsert(&t_list, localToken);
				return 0;
            case ')': 
                (*localToken).id = CLOSE_BRACKET;
				TLInsert(&t_list, localToken);
				return 0;
            case ',': 
                (*localToken).id = COMMA;
				TLInsert(&t_list, localToken);
				return 0;

            //detekcia 1 alebo 2 znakovych operatorov
            case '!':
                lastC = c; c = getchar();
                if (c == '='){
                    (*localToken).id = NOT_EQUAL;
                    TLInsert(&t_list, localToken);
				    return 0;
                }else
                    return LEX_ERROR;
                break;
            case '=': if((*localToken).id == 0) (*localToken).id = ASSIGN;
            case '>': if((*localToken).id == 0) (*localToken).id = GREATER;
            case '<': if((*localToken).id == 0) (*localToken).id = LESS;
				//strAddChar(&(*localToken), (char)c);
				lastC = c; c = getchar();
                //ak nasiel v dalsom znaku '='
                if (c == '='){
					//strAddChar(&(*localToken), (char)c);
                    if((*localToken).id == GREATER) (*localToken).id = GREATER_EQUAL;
                    else if((*localToken).id == LESS) (*localToken).id = LESS_EQUAL;
                    else if((*localToken).id == ASSIGN) (*localToken).id = EQUAL;
                }else{
					ungetc(c, stdin);
                }
				TLInsert(&t_list, localToken);
				return 0;
            
            //detekcia stringu 
            case '"':
                lastC = c; c = getchar();
                if(c != '"'){
                    while(1){
                        if(c == 92){
                            strAddChar(&(*localToken), (char) c);
                            lastC = c; c = getchar();
                            if(c == EOF){
                                strFree(&(*localToken));
                                return LEX_ERROR;
                            }
                            strAddChar(&(*localToken), (char) c);
                            lastC = c; c = getchar();
                            continue;
                        }
                        //printf("\n%c - %d\n", c, c);

                        if(c == '"')
                            break;
                        //ostrenie nekonecneho stringu
                        if(c == EOF){
                            strFree(&(*localToken));
                            return LEX_ERROR;
                        }

                        strAddChar(&(*localToken), (char) c);
                        lastC = c; c = getchar();
                    }
                }
                (*localToken).id = STRING;
				TLInsert(&t_list, localToken);
				return 0;
            
            //filtrovanie medzier a tabulatorov
            case ' ':
            case '\t':
                continue;
            //localToken koniec riadka
            case '\n':
                lastC = c; c = getchar();
                
                //hladanie viacriadkoveho komentara
                if(c == '='){//nasiel zaciatok viacriadkoveho komentaru
                    //strAddChar(&(*localToken), c);
					lastC = c; c = getchar();
                    if(c == 'b'){
                        //strAddChar(&(*localToken), (char)c);
                        lastC = c; c = getchar();
                        if(c == 'e'){
                            //strAddChar(&(*localToken), (char)c);
                            lastC = c; c = getchar();
                            if(c == 'g'){
                                //strAddChar(&(*localToken), (char)c);
                                lastC = c; c = getchar();
                                if(c == 'i'){
                                    //strAddChar(&(*localToken), (char)c);
                                    lastC = c; c = getchar();
                                    if(c == 'n'){
                                        //strAddChar(&(*localToken), c);
                                        lastC = c; c = getchar();
                                        if(c == ' ' || c == '\n'){
                                            //strAddChar(&(*localToken), c);
                                            while(1) {//prechadzanie dkumentu kym nenajdem =end                                             
                                                lastC = c; c = getchar();
                                                if(c == '=' && lastC == '\n'){//hladanie =end na zaciatku riadka
                                                    //strAddChar(&(*localToken), c);
                                                    lastC = c; c = getchar();
                                                    if(c == 'e'){
                                                        //strAddChar(&(*localToken), c);
                                                        lastC = c; c = getchar();
                                                        if(c == 'n'){
                                                            //strAddChar(&(*localToken), c);
                                                            lastC = c; c = getchar();
                                                            if(c == 'd'){
                                                                //strAddChar(&(*localToken), c);
                                                                lastC = c; c = getchar();
                                                                if(c == ' ' || c == '\n'){
                                                                    //strAddChar(&(*localToken), c);
                                                                    ungetc(c, stdin);
                                                                    (*localToken).id = 0;//nasiel koniec komentara, nebudeme vracat (*localToken)
                                                                    break;
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                                if(c == EOF){//detekcia neukonceneho komentara
                                                    return LEX_ERROR;
                                                }
                                            }
                                        }
                                    }else
                                        return LEX_ERROR;//chyba pri zapisovani viacriadkoveho komentara        
                                }else
                                    return LEX_ERROR;//chyba pri zapisovani viacriadkoveho komentara       
                            }else
                                return LEX_ERROR;//chyba pri zapisovani viacriadkoveho komentara           
                        }else
                            return LEX_ERROR;//chyba pri zapisovani viacriadkoveho komentara
                    }else
                            return LEX_ERROR;//chyba pri zapisovani viacriadkoveho komentara     
                //strClear(&(*localToken));
                }else


                //nasiel novy riadok
                ungetc(c, stdin);
                (*localToken).id = EOL;
				TLInsert(&t_list, localToken);
				return 0;

            //koniec suboru
            case EOF:
                (*localToken).id = ENDOF;
				TLInsert(&t_list, localToken);
				return 0;
            //naslo znak ktory sa nepouziva v jazku ifj2018
            default:
                return LEX_ERROR;
        }
    }
//TLInsert(&t_list, localToken);
//return 0;
}
