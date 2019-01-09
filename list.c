/*
** Implementace překladače imperativního jazyka IFJ18
** 
** Koša Benjamín	xkosab00@stud.fit.vutbr.cz
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
** typu tDLList (DL znamená Double-Linked a slouží pro odlišení
** jmen konstant, typů a funkcí od jmen u jednosměrně vázaného lineárního
** seznamu). Definici konstant a typů naleznete v hlavičkovém souboru list.h.
**
** Vaším úkolem je implementovat následující operace, které spolu
** s výše uvedenou datovou částí abstrakce tvoří abstraktní datový typ
** obousměrně vázaný lineární seznam:
**
**      DLInitList ...... inicializace seznamu před prvním použitím,
**      DLDisposeList ... zrušení všech prvků seznamu,
**      DLInsertFirst ... vložení prvku na začátek seznamu,
**      DLInsertLast .... vložení prvku na konec seznamu,
**      DLFirst ......... nastavení aktivity na první prvek,
**      DLLast .......... nastavení aktivity na poslední prvek,
**      DLCopyFirst ..... vrací hodnotu prvního prvku,
**      DLCopyLast ...... vrací hodnotu posledního prvku,
**      DLDeleteFirst ... zruší první prvek seznamu,
**      DLDeleteLast .... zruší poslední prvek seznamu,
**      DLPostDelete .... ruší prvek za aktivním prvkem,
**      DLPreDelete ..... ruší prvek před aktivním prvkem,
**      DLPostInsert .... vloží nový prvek za aktivní prvek seznamu,
**      DLPreInsert ..... vloží nový prvek před aktivní prvek seznamu,
**      DLCopy .......... vrací hodnotu aktivního prvku,
**      DLActualize ..... přepíše obsah aktivního prvku novou hodnotou,
**      DLSucc .......... posune aktivitu na další prvek seznamu,
**      DLPred .......... posune aktivitu na předchozí prvek seznamu,
**      DLActive ........ zjišťuje aktivitu seznamu.
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

#include "list.h"

int errflg;
int solved;

void DLError() {
/*
** Vytiskne upozornění na to, že došlo k chybě.
** Tato funkce bude volána z některých dále implementovaných operací.
**/	
    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;             /* globální proměnná -- příznak ošetření chyby */
    return;
}

void DLInitList (tDLList *L) {
/*
** Provede inicializaci seznamu L před jeho prvním použitím (tzn. žádná
** z následujících funkcí nebude volána nad neinicializovaným seznamem).
** Tato inicializace se nikdy nebude provádět nad již inicializovaným
** seznamem, a proto tuto možnost neošetřujte. Vždy předpokládejte,
** že neinicializované proměnné mají nedefinovanou hodnotu.
**/

 	L->First = NULL;
	L->Act = NULL;
	L->Last = NULL;
}

void DLDisposeList (tDLList *L) {
/*
** Zruší všechny prvky seznamu L a uvede seznam do stavu, v jakém
** se nacházel po inicializaci. Rušené prvky seznamu budou korektně
** uvolněny voláním operace free. 
**/
	
	while (L->First != NULL) {
		tDLElemPtr tmp;
		tmp = L->First;	//ulozi first do pomocnej premennej
		L->First = L->First->rptr;//2. element nastavy ako first
		free(tmp);//uvolni tmp
	}

	L->Last = NULL;
	L->Act = NULL;
}

void DLInsertFirst (tDLList *L, int val) {
/*
** Vloží nový prvek na začátek seznamu L.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/

	tDLElemPtr tmp = malloc(sizeof(struct tDLElem));
	if(tmp == NULL){ //ak nastala chba
		DLError();
		return;
	}
	tmp->data = val;
	tmp->lptr = NULL; //sme na zaciatku zoznamu takze na lavo je NULL
	tmp->rptr = L->First; //na pravo je prvok co bol doteraz prvy
	if(L->Last == NULL){ //ak bol zoznam prazdny
		L->Last = tmp; //posledny je nas vlozeny prvok
	} else {
		L->First->lptr = tmp; //povodnemu prvemu prvku nastavime ze 
							  //na lavo od neho sa nachadya novy prvok
	}
	L->First = tmp; //novy prvok sa nastavy ako prvy

}

void DLInsertLast(tDLList *L, int val) {
/*
** Vloží nový prvek na konec seznamu L (symetrická operace k DLInsertFirst).
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/ 	
	
	tDLElemPtr tmp = malloc(sizeof(struct tDLElem));
	if(tmp == NULL){//ak nastala chyba
		DLError();
		return;
	}
	tmp->data = val;
	tmp->rptr = NULL; //sme na konci zoznamu takze na pravo je NULL
	tmp->lptr = L->Last; //na lavo je prvok co bol doteraz posledny
	if(L->Last == NULL){ //ak bol zoznam prazdny
		L->First = tmp; //prvy je nas vlozeny prvok
	} else {
		L->Last->rptr = tmp; //povodnemu poslednemu prvku nastavime ze 
							 //na pravo od neho sa nachadya novy prvok
	}
	L->Last = tmp; //novy prvok sa nastavy ako posledny
}

void DLFirst (tDLList *L) {
/*
** Nastaví aktivitu na první prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/
	
	L->Act = L->First;
}

void DLLast (tDLList *L) {
/*
** Nastaví aktivitu na poslední prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/
	
	L->Act = L->Last;
}

void DLCopyFirst (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu prvního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/

	if(L->First == NULL)//prazdny
		DLError();
	else
		*val = L->First->data;
}

void DLCopyLast (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu posledního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/

	if(L->First == NULL)//prazdny
		DLError();
	else
		*val = L->Last->data;
}

void DLDeleteFirst (tDLList *L) {
/*
** Zruší první prvek seznamu L. Pokud byl první prvek aktivní, aktivita 
** se ztrácí. Pokud byl seznam L prázdný, nic se neděje.
**/

	if(L->First != NULL){ //ak zoznam nieje prazdny
		tDLElemPtr tmp = L->First; //docasne ulozime prvy prvok
		if(L->First == L->Last){ //ak to bol jediny prvok v zozname tak vsetko nastavime na null
			L->First = NULL;
			L->Act = NULL;
			L->Last = NULL;
		} else {
			if(L->First == L->Act) //rusime aktivitu prvemu prvku
				L->Act = NULL;

			L->First = tmp->rptr; //druhy prvok sa stane prvym
			L->First->lptr = NULL; //nalavo od prveho prvku je NULL
		}
	free(tmp);
	}
}

void DLDeleteLast (tDLList *L) {
/*
** Zruší poslední prvek seznamu L. Pokud byl poslední prvek aktivní,
** aktivita seznamu se ztrácí. Pokud byl seznam L prázdný, nic se neděje.
**/

	if(L->First != NULL){ //ak zoznam nieje prazdny
		tDLElemPtr tmp = L->Last; //docasne ulozime posledny prvok
		if(L->First == L->Last){ //ak to bol jediny prvok v zozname tak vsetko nastavime na null
			L->First = NULL;
			L->Act = NULL;
			L->Last = NULL;
		} else {
			if(L->Last == L->Act) //rusime aktivitu poslednemu prvku
				L->Act = NULL;

			L->Last = tmp->lptr; //predposledny prvok sa stane poslednym
			L->Last->rptr = NULL; //napravo od posledneho je NULL
		}
	free(tmp);
	}
}

void DLPostDelete (tDLList *L) {
/*
** Zruší prvek seznamu L za aktivním prvkem.
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** posledním prvkem seznamu, nic se neděje.
**/

	if(L->Act == NULL || L->Act == L->Last)
		return;  //ak je zoznam neaktivny alebo act je posledny vyskakujeme

	tDLElemPtr tmp = L->Act->rptr; //ulozime si prvok na zrusenie
	L->Act->rptr = tmp->rptr; //na pravo od act vlozime prvok  na pravo od tmp
	
	if(tmp == L->Last) //ak je tmp posledny prvok
		L->Last = L->Act;
	else
		tmp->rptr->lptr = L->Act; //spravime prepojenie

	free(tmp);
}

void DLPreDelete (tDLList *L) {
/*
** Zruší prvek před aktivním prvkem seznamu L .
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** prvním prvkem seznamu, nic se neděje.
**/

	if(L->Act == NULL || L->Act == L->First)
		return;  //ak je zoznam neaktivny alebo act je prvy vyskakujeme

	tDLElemPtr tmp = L->Act->lptr; //ulozime si prvok na zrusenie
	L->Act->lptr = tmp->lptr; //na lavo od act vlozime prvok  na lavo od tmp
	
	if(tmp == L->First) //ak je tmp prvy prvok
		L->First = L->Act;
	else
		tmp->lptr->rptr = L->Act; //spravime prepojenie

	free(tmp);
}

void DLPostInsert (tDLList *L, int val) {
/*
** Vloží prvek za aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/

	if(L->Act == NULL) 
		return;

	tDLElemPtr tmp = malloc(sizeof(struct tDLElem));
	if(tmp == NULL)	{
		DLError();
		return;
	}

	tmp->data = val;
	tmp->lptr = L->Act;
	tmp->rptr = L->Act->rptr;
	L->Act->rptr = tmp;

	if(L->Act == L->Last) //ak bol aktivny prvok posledny
		L->Last = tmp;
	else
		tmp->rptr->lptr = tmp;
}

void DLPreInsert (tDLList *L, int val) {
/*
** Vloží prvek před aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/

	if(L->Act == NULL) 
		return;

	tDLElemPtr tmp = malloc(sizeof(struct tDLElem));
	if(tmp == NULL)	{
		DLError();
		return;
	}

	tmp->data = val;
	tmp->rptr = L->Act;
	tmp->lptr = L->Act->lptr;
	L->Act->lptr = tmp;

	if(L->Act == L->First) //ak bol aktivny prvok prvy
		L->First = tmp;
	else
		tmp->lptr->rptr = tmp;
}

void DLCopy (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu aktivního prvku seznamu L.
** Pokud seznam L není aktivní, volá funkci DLError ().
**/

	if(L->Act == NULL) //nieje aktivny
		DLError();
	else
		*val = L->Act->data;
}

void DLActualize (tDLList *L, int val) {
/*
** Přepíše obsah aktivního prvku seznamu L.
** Pokud seznam L není aktivní, nedělá nic.
**/

	if(L->Act != NULL)
		L->Act->data = val;
}

void DLSucc (tDLList *L) {
/*
** Posune aktivitu na následující prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
**/

	if(L->Act == NULL)
		return;

	if(L->Act == L->Last)
		L->Act = NULL;
	else
		L->Act = L->Act->rptr;
}

void DLPred (tDLList *L) {
/*
** Posune aktivitu na předchozí prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
**/

	if(L->Act == NULL)
		return;

	if(L->Act == L->First)
		L->Act = NULL;
	else
		L->Act = L->Act->lptr;

}

int DLActive (tDLList *L) {
/*
** Je-li seznam L aktivní, vrací nenulovou hodnotu, jinak vrací 0.
** Funkci je vhodné implementovat jedním příkazem return.
**/

	return(L->Act != NULL);
}

/* Konec list.c*/
