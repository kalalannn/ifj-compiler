/*
** Implementace překladače imperativního jazyka IFJ18
** 
** Vorobiev Nikolaj	xvorob00@stud.fit.vutbr.cz
** Luhin Artsiom	xluhin00@stud.fit.vutbr.cz
*/



/* list.c **********************************************************}
{* Téma: Dvousměrně vázaný lineární seznam
**
**                   Návrh a referenční implementace: Bohuslav Křena, říjen 2001
**                            Přepracované do jazyka C: Martin Tuček, říjen 2004
**                                            Úpravy: Kamil Jeřábek, září 2018
**
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int.
** Seznam bude jako datová abstrakce reprezentován proměnnou
** typu tDLTokenList (DL znamená Double-Linked a slouží pro odlišení
** jmen konstant, typů a funkcí od jmen u jednosměrně vázaného lineárního
** seznamu). Definici konstant a typů naleznete v hlavičkovém souboru list.h.
**
** Vaším úkolem je implementovat následující operace, které spolu
** s výše uvedenou datovou částí abstrakce tvoří abstraktní datový typ
** obousměrně vázaný lineární seznam:
**
**      TLInitList ...... inicializace seznamu před prvním použitím,
**      TLDisposeList ... zrušení všech prvků seznamu,
**      TLInsertFirst ... vložení prvku na začátek seznamu,
**      TLInsertLast .... vložení prvku na konec seznamu,
**      TLFirst ......... nastavení aktivity na první prvek,
**      TLLast .......... nastavení aktivity na poslední prvek,
**NEFUNGUJE   TLCopyFirst ..... vrací hodnotu prvního prvku,
**NEFUNGUJE      TLCopyLast ...... vrací hodnotu posledního prvku,
**      TLDeleteFirst ... zruší první prvek seznamu,
**      TLDeleteLast .... zruší poslední prvek seznamu,
**      TLPostDelete .... ruší prvek za aktivním prvkem,
**      TLPreDelete ..... ruší prvek před aktivním prvkem,
**      TLPostInsert .... vloží nový prvek za aktivní prvek seznamu,
**      TLPreInsert ..... vloží nový prvek před aktivní prvek seznamu,
**NEFUNGUJE      TLCopy .......... vrací hodnotu aktivního prvku,
**      TLActualize ..... přepíše obsah aktivního prvku novou hodnotou,
**      TLSucc .......... posune aktivitu na další prvek seznamu,
**      TLPred .......... posune aktivitu na předchozí prvek seznamu,
**      TLActive ........ zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce
** explicitně uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam 
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**/

#include "token_list.h"
#include <stdbool.h>

int errflg;
int solved;

static bool tl_disp = false;

void TLError() {
/*
** Vytiskne upozornění na to, že došlo k chybě.
** Tato funkce bude volána z některých dále implementovaných operací.
**/	
    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;             /* globální proměnná -- příznak ošetření chyby */
    return;
}

void TLInitList (LTokenList *L) {
/*
** Provede inicializaci seznamu L před jeho prvním použitím (tzn. žádná
** z následujících funkcí nebude volána nad neinicializovaným seznamem).
** Tato inicializace se nikdy nebude provádět nad již inicializovaným
** seznamem, a proto tuto možnost neošetřujte. Vždy předpokládejte,
** že neinicializované proměnné mají nedefinovanou hodnotu.
**/

    L->first = NULL;
    L->act = NULL;
    L->last = NULL;
}

void TLDisposeList (LTokenList *L) {
/*
** Zruší všechny prvky seznamu L a uvede seznam do stavu, v jakém
** se nacházel po inicializaci. Rušené prvky seznamu budou korektně
** uvolněny voláním operace free. 
**/
	
    while (L->first != NULL) {
        LTokenPtr tmp = L->first;	//ulozi first do pomocnej premennej
        L->first = L->first->next; //2. element nastavy ako first

        strFree(&tmp->token);

        free(tmp);
	}

    L->last = NULL;
    L->act = NULL;
}

void TLInsert (LTokenList *L, Token *token_ptr) {
/*
** Vloží nový prvek na začátek seznamu L.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci TLError().
**/

    LTokenPtr tmp = malloc(sizeof(struct LToken));
    if(tmp == NULL) {
		TLError();
		return;
	}

    strInit(&tmp->token);
    copyToken(&tmp->token, token_ptr);

    if (L->first == NULL) {
        L->first = tmp;
        L->first->next = NULL;
        L->act = tmp;
        L->last = tmp;
    } else {
        L->last->next = tmp;
        L->last = tmp;
        tmp->next = NULL;
    }

}

/*
void TLFirst (LTokenList *L) {
	
    L->act = L->first;
}

void TLLast (LTokenList *L) {
	
    L->act = L->last;
}



void TLSucc (LTokenList *L) {

    if(L->act != NULL)
        L->act = L->act->next;

}

int TLActive (LTokenList *L) {

    return (L->act != NULL);
}
*/

void TLFirst(LTokenList *L) {
	L->act = L->first;
}

void TLGetToken(LTokenList *L, Token *token_ptr) {

	if (tl_disp == true) {
		while (L->first != L->act) {
			LTokenPtr tmp = L->first;
			L->first = L->first->next;

			strFree(&tmp->token);
			free(tmp);
		}
		tl_disp = false;
	}

	if (L->act != NULL) {
		if (L->act->token.id == EOL) {
			tl_disp = true;
		}
		copyToken(token_ptr, &L->act->token);
		L->act = L->act->next;
	}

	/*
	if (tl_disp == 1)
		tl_disp = 2;
		*/

}

void PrintArtList (LTokenList *L) {
	L->act = L->first;
	for (; L->act != NULL;) {
		printf("%s\t%d\n", L->act->token.str, L->act->token.id);
		L->act = L->act->next;
	}
}
/* Konec list.c*/
