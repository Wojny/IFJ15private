#include <stdio.h>
#include <stdlib.h>
//#include "str.h"
//#include "scaner.h"
//-#include "scaner.c"
//#include "str.c"
#include "sym_table.c"

//     TEST TABULKY SYMBOLU


int main(void){
 /* FILE *f;
  int i=0;
  string s;
  strInit(&s);
  if( (f = fopen("./file.cpp", "r+")) == NULL )
  {
    return -1;
  }
  setSourceFile(f);
  while(i!=32){
    i=getNextToken(&s);
    printf("%d     ",i);
    strPrint(&s);
    strClear(&s);

  }
  strFree(&s);*/
  /*
  string *s;
  if(((s=malloc(sizeof (string)))==NULL)) return -1;
  string *t;
  if(((t=malloc(sizeof (string)))==NULL)) return -1;
  strInit(s);
  strInit(t);
  strAddChar(s,'b');
  strAddChar(t,'c');
  union Dat *d;
  d=createDat(27,0,0,&t);

  strAddChar(t,'c');
  strAddChar(t,'c');
  StackHead StHead;
  SHInit(&StHead);

  SHPushNew(&StHead);
  BTAddID(&StHead,s,27,d);
  d=createDat(27,0,0,&t);
  //BTUpdateData(&StHead,s, 27,d);





  if(SearchBT(&StHead,s)!=NULL) printf("sdfghjkl\n");
  strPrint(StHead->First->BTroot->data->str);
  SHDispose(&StHead);
*/

  string *s;
  if(((s=malloc(sizeof (string)))==NULL)) return -1;
  string *t;
  if(((t=malloc(sizeof (string)))==NULL)) return -1;
  string *u;
  if(((u=malloc(sizeof (string)))==NULL)) return -1;
  string *v;
  if(((v=malloc(sizeof (string)))==NULL)) return -1;
  strInit(s);
  strInit(t);
  strInit(u);
  strInit(v);
  strAddChar(s,'b');
  strAddChar(t,'c');
  strAddChar(u,'d');
  strAddChar(v,'1');
  strAddChar(v,'0');
  strAddChar(v,'0');
  strAddChar(v,'0');
  strAddChar(v,'0');
  strAddChar(v,'0');



  GSTable GST;
  GSTinit(&GST);
  FN newFN=GSTadd(&GST,s,KINTEGER);
  BTAddID(&newFN,t, KINTEGER,0,0);
  BTAddID(&newFN,u, KINTEGER,1,1);
  BTree newBT=SearchBT(newFN, u);
  BTree newBT1=SearchBT(newFN, t);
  BTDelete(&newFN->BTroot,1,&newFN->tempSTable);

  BTDelete(&newFN->BTroot,0,&newFN->tempSTable);
  BlockStack BlStack;
  BlockStackInit(&BlStack);
  BlockStackAdd(&BlStack,&GST,s);
  constTable newCTable;
  constTableInit(&newCTable);
  BTree newBT2=createConst(&newCTable,KINTEGER,v);
  union Dat *d=getDat(&newCTable,&BlStack->First,&newBT);
  union Dat *d1=getDat(&newCTable,&BlStack->First,&newBT1);
  *d1->i=3;
  union Dat *d2=getDat(&newCTable,&BlStack->First,&newBT2);
  *d->i=*d1->i+*d2->i;
  printf("\n\n%d\n\n",*d->i);




}





