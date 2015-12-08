// Vytvoril Stanislav Svoboda
// TESTOVACI MAIN pro SEMANTICKOU ANALYZU

#include <stdio.h>
#include <stdlib.h>
//#include ""

//-------------------------------------------------------
#include "parser.c"



int main(void){
  FILE *f;
  if( (f = fopen("./file.cpp", "r+")) == NULL )
  {
    return -1;
  }
  setSourceFile(f);
  parser();
  printf("\n");
  printf("\n");
  printf("funkce");
  strPrint(GST->FunRoot->ident);
  printf("\n");
  FN pomocFN=SearchDefinedFN(GST->FunRoot,GST->FunRoot->ident);
  if(pomocFN==NULL) printf("aaaaaa");
  BTree pomocBT=GST->FunRoot->tempSTable->First;

  while(pomocBT!=NULL){
    printf("promenna ");
  strPrint(pomocBT->ident);
  printf("\n");
 /* data=getDat(&newCTable,&blockst->First,&pomocBT);
  *data->i=5;
  printf("%d\n",data->i);*/
  pomocBT=pomocBT->RBT;

  }
  BlockStack blockst;
  BlockStackInit(&blockst);
  BlockStackAdd(&blockst,&GST,GST->FunRoot->ident);
  union Dat *data;
 // listFirst(L);
//  interpret(L,&GST,&newCTable);
//  strPrint(GST->FunRoot->ident);
 // return interpret(L, GST, &newCTable);


}





