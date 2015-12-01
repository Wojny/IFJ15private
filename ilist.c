/* Částečně převzato z ukázkového Jednochuché interpretu - upravit a okomentovat*/

#include <stdio.h>
#include <malloc.h>
#include "ilist.h"

void listInit(tList *L)
{
  L->first  = NULL;
  L->last   = NULL;
  L->active = NULL;
}

void listFree(tList *L)
{
  tItemList *ptr;
  while (L->first != NULL)
  {
    ptr = L->first;
    L->first = L->first->nextItem;
    free(ptr);
  }
}

void listInsertLast(tList *L, tInstr I)
{
  tItemList *newItem;
  newItem = malloc(sizeof (tItemList));
  newItem->instr = I;
  newItem->nextItem = NULL;
  if (L->first == NULL)
     L->first = newItem;
  else
     L->last->nextItem=newItem;
  L->last=newItem;
}

void listFirst(tList *L)
{
  L->active = L->first;
}

void listNext(tList *L)
{
  if (L->active != NULL)
  L->active = L->active->nextItem;
}

void listGoto(tList *L, void *gotoInstr)
{
  L->active = (tItemList*) gotoInstr;
}

void *listGetPointerLast(tList *L)
{
  return (void*) L->last;
}

tInstr *listGetData(tList *L)
{
  if (L->active == NULL)
  {
    printf("Chyba, zadna instrukce neni aktivni");
    return NULL;
  }
  else return &(L->active->instr);
}

tInstr *listGetNextData(tList *L)
{
  if (L->active == NULL)
  {
    printf("Chyba, zadna instrukce neni aktivni");
    return NULL;
  }
  else return &(L->active->nextItem->instr);
}
