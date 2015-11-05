// Vytvoril Stanislav Svoboda
// TESTOVACI MAIN pro SEMANTICKOU ANALYZU

#include <stdio.h>
#include <stdlib.h>
//#include ""

//-------------------------------------------------------
#include "scaner.h"
#include "sym_table.c"

typedef struct SynTree{
  BTree node;
  struct SynTree *LSynTree;
  struct SynTree *RSynTree;
} *sTree;
typedef struct HeadSynTree{
  sTree Head;
} *HSynTree;

int HeadSynTreeInit(HSynTree *newHSTree){
  if((((*newHSTree)=malloc(sizeof (struct HeadSynTree)))==NULL)) return -1;
  (*newHSTree)->Head=NULL;

}


struct BT BTPlus={NULL,PLUS,0,0,0,NULL,NULL};
struct BT BTAssign={NULL,ASSIGN,0,0,0,NULL,NULL};
struct BT BTDivide={NULL,DIVIDE,0,0,0,NULL,NULL};

int getType(sTree newsTree){
  return newsTree->node->type;
}
int getKey(sTree newsTree){
  return newsTree->node->key;
}
union Dat *getData(constTable newCTable,int key){
  return newCTable->BUPtr[key].data;
}
int isInteger(int type){
  if((type==IINTEGER)||(type==CINTEGER)) return 0;
  return 1;
}
int isString(int type){
  if((type==ISTR)||(type==CSTR)) return 0;
  return 1;
}
int isDouble(int type){
  if((type==IDOUBLE)||(type==CDOUBLE)) return 0;
  return 1;
}

int isConstant(int type){
  if((type==CINTEGER)||(type==CDOUBLE)||(type==CSTR)) return 0;
  return 1;
}
//-------------------------------------------------------------------




int main(void){


  union Dat *d;// vytvoreni konstant
  d=createDat(CINTEGER);
  updateDat(d,CINTEGER,5,0,NULL);
  union Dat *d1;
  d1=createDat(CINTEGER);
  updateDat(d1,CINTEGER,0,0,NULL);


  constTable newCTable;
  constTableInit(&newCTable);
  BTree newBT;// ukazatel na identifikatory
  BTree newBT1;// ukazatel na identifikatory
  BTree newBT2=constTableAdd(&newCTable,CINTEGER,d);// ukazatel na konstanty
  BTree newBT3=constTableAdd(&newCTable,CINTEGER,d1);//  ukazatel na konstanty
  if(((newBT=malloc(sizeof (struct BT)))==NULL)) return -1;
  if(((newBT1=malloc(sizeof (struct BT)))==NULL)) return -1;
  newBT->type=IINTEGER;
  newBT->def=1;
  newBT1->type=ISTR;
  newBT1->def=1;
//---------------------------------------------

  HSynTree newHSynTree;
  HeadSynTreeInit(&newHSynTree);
  sTree newsTree;
  if(((newsTree=malloc(sizeof (struct SynTree)))==NULL)) return -1;
  sTree newsTree1;
  if(((newsTree1=malloc(sizeof (struct SynTree)))==NULL)) return -1;
  sTree newsTree2;
  if(((newsTree2=malloc(sizeof (struct SynTree)))==NULL)) return -1;
  sTree newsTree3;
  if(((newsTree3=malloc(sizeof (struct SynTree)))==NULL)) return -1;
  sTree newsTree4;
  if(((newsTree4=malloc(sizeof (struct SynTree)))==NULL)) return -1;

  newHSynTree->Head=newsTree;

  newsTree->node=&BTAssign;  // i=i+5
  newsTree->LSynTree=newsTree1;
  newsTree->RSynTree=newsTree2;

  newsTree1->node=newBT;
  newsTree1->LSynTree=NULL;
  newsTree1->RSynTree=NULL;

  newsTree2->node=&BTPlus;
  newsTree2->LSynTree=newsTree3;
  newsTree2->RSynTree=newsTree4;

  newsTree3->node=newBT;
  newsTree3->LSynTree=NULL;
  newsTree3->RSynTree=NULL;

  newsTree4->node=newBT2;
  newsTree4->LSynTree=NULL;
  newsTree4->RSynTree=NULL;

// ZAVOLEJ SEMANTICKOU ANALYZU!!!!!!!!

  newsTree->node=&BTAssign;//  i=string+int
  newsTree1->node=newBT;
  newsTree2->node=&BTPlus;
  newsTree3->node=newBT1;
  newsTree4->node=newBT2;

  // ZAVOLEJ SEMANTICKOU ANALYZU!!!!!!!!

  newsTree->node=&BTAssign;//  i=5/0
  newsTree1->node=newBT;
  newsTree2->node=&BTDivide;
  newsTree3->node=newBT2;
  newsTree4->node=newBT3;

  // ZAVOLEJ SEMANTICKOU ANALYZU!!!!!!!!

  union Dat *d2=getData(newCTable,getKey(newsTree3));
  int i=*d2->i;
  printf("%d  :tohle je int",i);
}





