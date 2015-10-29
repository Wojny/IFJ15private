#include <stdio.h>
#include <stdlib.h>

// definice pridat nebo ubrat podle potreby - type instr

#define I_ADD 101
#define I_SUB 102
#define I_MUL 103
#define I_DIV 104
#define I_LESS 105
#define I_MORE 106
#define I_LESSEQ 107
#define I_MOREEQ 108
#define I_NOTEQ 109
#define I_EQUAL 110
#define I_LENGHT 111
#define I_SUBSTR 112
#define I_CONCAT 113
#define I_FIND 114
#define I_SORT 115
#define I_IF 116
#define I_END_IF 117
#define I_ELSE 118
#define I_CALL 119 //volani funkce
#define I_RETURN 120
#define I_WHILE 121
#define I_END_WHILE 122
#define I_DO 123
#define I_END_DO 124
#define I_FOR 125
#define I_END_FOR 126
#define I_FUNC 127 //deklarace funkce
#define I_END_FUNC 128
#define I_MAIN 129

// 3 adresný kod
typedef struct
{
  int Type;     //typ instrukce
  void *add1;
  void *add2;
  void *add3;
} tInstr;

typedef struct itemList
{
  tInstr instr;
  struct Item *nextItem;
} tItemList;

typedef struct
{
  struct itemList *first;  //ukazatel na prvni prvek
  struct itemList *last;   //ukazatel na posledni prvek
  struct itemList *active; //ukazatel na aktivni prvek
} tList;

void listInit(tList *L);
void listFree(tList *L);
void listInsertLast(tList *L, tInstr I);
void listFirst(tList *L);
void listNext(tList *L);
void listGoto(tList *L, void *gotoInstr);
void *listGetPointerLast(tList *L);
tInstr *listGetData(tList *L);
