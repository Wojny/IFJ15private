//+------------------------------------------------------------------+
//| Project name:        IFJ - Team project                          |
//| Filename:            ilist.h                                     |
//| Author:                                                          |
//| Encoding:            UTF-8                                       |
//| Description:         Source of Instruction.                      |
//+------------------------------------------------------------------+

#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <malloc.h>
#include "error.h"
#include "ial.h"
#include "scanner.h"
#include "precedence.h"

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
//#define I_WHILE 121
//#define I_END_WHILE 122
//#define I_DO 123
//#define I_END_DO 124
#define I_FOR 125
#define I_END_FOR 126
//#define I_FUNC 127 //deklarace funkce
//#define I_END_FUNC 128
#define I_MAIN 129
#define I_END_MAIN 130
#define I_READ 131
#define I_WRITE 132
#define I_IF_COND 133 //Mezi instrukcemi I_IF a I_IF_COND je vyraz, ktery se vyhodnoti jestli podminka je splnena
#define I_END_ELSE 134
#define I_FOR_CHCK 135 // ins kam se vracime pro vyhodnoceni vyrayu                  for(dek; HERE podm; diff)
#define I_FOR_DIFF 136 // ins kam se vracime pro vyhodnoceni vyrayu diff po cyklu    for(dek; podm; HERE diff)
#define I_FOR_COND 137 //obdoba IF COND - za podminkou cyklu, zacatek insrukci       for(dek; podm; diff) HERE


// 3 adresny kod
typedef struct
{
  int Type;     //typ instrukce
  void *add1;
  void *add2;
  void *add3;
} tInstr;

typedef struct itemList
{
  tInstr *data;
  struct itemList *nextItem;
} tItemList;

typedef struct
{
  struct itemList *first;  //ukazatel na prvni prvek
  struct itemList *last;   //ukazatel na posledni prvek
  struct itemList *active; //ukazatel na aktivni prvek
} tList;
extern tList IList;

int CreateInst(int type, void *addr1, void *addr2, void *addr3, int Counter);
tInstr AddInst(int type, void *addr1, void *addr2, void *addr3);
void listInit(tList *L);
void listFree(tList *L);
void listInsertFirst(tList *L, tInstr I);
void listInsertLast(tList *L, tInstr I);
void listFirst(tList *L);
void listCopyFirst (tList *L, tInstr I);
void listDeleteFirst (tList *L);
void listPostDelete(tList *L);
void listPostInsert(tList *L, tInstr I);
void listCopy(tList *L, tInstr I);
void listActualize(tList *L, tInstr I);
void listSucc(tList *L);
void listGoto(tList *L, void *gotoInstr);
int listActive(tList *L);

#endif
