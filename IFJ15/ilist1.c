//+------------------------------------------------------------------+
//| Project name:        IFJ - Team project                          |
//| Filename:            ilist.c                                     |
//| Author:                                                          |
//| Encoding:            UTF-8                                       |
//| Description:         Source of Instruction.                      |
//+------------------------------------------------------------------+



/* Částečně převzato z ukázkového Jednochuché interpretu - upravit a okomentovat*/
#include "ilist.h"

int CreateInst(int type, void *addr1, void *addr2, void *addr3)
{
  tInstr *newInst;
  newInst = malloc(sizeof(tInstr));
  if(newInst == NULL)
  {
    AddERR(line,IFJ_ERR_PROGRAM);
    return 1;
  }

  *newInst = AddInst(type, addr1, addr2, addr2);

  listInsertLast(&IList, newInst);

  return 0;
}

tInstr AddInst(int type, void *addr1, void *addr2, void *addr3)
{
  tInstr Inst = {type,addr1,addr2,addr3};
  return Inst;
}

//Init list
void listInit(tList *L)
{
  L->first  = NULL;
  L->last   = NULL;
  L->active = NULL;
}

//Free list
void listFree(tList *L)
{
  tItemList *ptr;
  while (L->first != NULL)
  {
    ptr = L->first->nextItem;    
    free(L->first->data);
    free(L->first);
    L->first = ptr;
  }
  L->active = NULL;
}

//Insert item to front of the list
void listInsertFirst(tList *L, tInstr I)
{
  tItemList *newItem;
  newItem = malloc(sizeof (tItemList));
  if(newItem == NULL)
  {
    AddERR(0,IFJ_ERR_PROGRAM);
    listFree(L);
  }
  newItem->data = I;
  newItem->nextItem = NULL;
  if (L->first == NULL || L->last == NULL)
     L->last = newItem;
  
  L->first=newItem;
}

//Insert item to the last place of the list
void listInsertLast(tList *L, tInstr I)
{
  tItemList *newItem;
  newItem = malloc(sizeof (tItemList));
  if(newItem == NULL)
  {
    AddERR(0,IFJ_ERR_PROGRAM);
    listFree(L);
  }
  newItem->data = I;
  newItem->nextItem = NULL;
  if (L->first == NULL || L->last == NULL)
     L->first = newItem;
  if ( L->last != NULL)
    L->last->nextItem = newItem;
  L->last=newItem;
}

//Add first item to Active
void listFirst(tList *L)
{
  L->active = L->first;
}

//Copy first item from list
void listCopyFirst (tList *L, tInstr I)
{
  if(L->first == NULL)
    AddERR(0,IFJ_ERR_PROGRAM);
  else
  {
    I->Type = L->first->data->Type;
    I->addr1 = L->first->data->addr1;
    I->addr2 = L->first->data->addr2;
    I->addr3 = L->first->data->addr3;
  }
}

//Delete first item in list
void listDeleteFirst (tList *L)
{
  if(L->first != NULL)
  {
    tItemList *newItem;
    if(L->active == L->first)
      L->active = NULL;
    newItem = L->first;
    L->first = L->first->nextItem;
    free(newItem);
  }
}

//Delete last item after Active
void listPostDelete(tList *L)
{
  if(L->active != NULL && L->active->nextItem != NULL)
  {
    tItemList *newItem;
    newItem = L->active->nextItem;
    L->active->nextItem = newItem->nextItem;
    free(newItem);
  }
}

//Insert item after active element
void listPostInsert(tList *L, tInstr I)
{
  if(L->active != NULL)
  {
    tItemList * newItem;
    
    newItem = malloc(sizeof(tItemList));
    if(newItem == NULL)
    {
      AddERR(0,IFJ_ERR_PROGRAM);
      listFree(L);
    }
    newItem->nextItem = L->active->nextItem;
    newItem->data = I;
    L->active->nextItem = newItem;
  }
}

//Get value of Active item
void listCopy(tList *L, tInstr I)
{
  if(L->active != NULL)
    I = L->active->data;
  else
  {
    AddERR(0,IFJ_ERR_PROGRAM);
    listFree(L);
  }
}

//Actualize data of Active item 
void listActualize(tList *L, tInstr I)
{
  if(L->active != NULL)
    L->active->data = I;
}

//Push L->Active to next item  
void listSucc(tList *L)
{
  if(L->Active != NULL)
  {
    if(L->active->nextItem == NULL)
      L->active = NULL;
    else
      L->active = L->active->nextItem;
  }
}

// Jump to choosen adress
void listGoto(tList *L, void *gotoInstr)
{
  L->active = (tItemList*) gotoInstr;
}

//if list active return TRUE
int listActive(tList *L)
{
  return L->active == NULL ? false:true;
}
