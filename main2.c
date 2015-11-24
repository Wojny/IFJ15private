#include <stdio.h>
#include <stdlib.h>
#include "syn_expression.c"




int main(void){


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
  strAddChar(t,'a');
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
  constTable newCTable;
  constTableInit(&newCTable);
  FILE *f;
  int i=0;
  string w;
  strInit(&w);
  if( (f = fopen("./file.cpp", "r+")) == NULL )
  {
    return -1;
  }
  setSourceFile(f);
  i=getNextToken(&w);
  sTree headST=syn_exp(i,&w,newFN,newCTable);
}





