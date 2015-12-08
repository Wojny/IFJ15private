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
  listFirst(L);
  interpret(L,&GST,&newCTable);
  strPrint(GST->FunRoot->ident);
 // return interpret(L, GST, &newCTable);


}





