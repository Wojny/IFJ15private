// Vytvoril Stanislav Svoboda
// TESTOVACI MAIN pro SEMANTICKOU ANALYZU

#include <stdio.h>
#include <stdlib.h>

#include "parser.c"



int main(void){
  FILE *f;
  if( (f = fopen("./file.cpp", "r+")) == NULL )
  {
    return -1;
  }
  MakeERRL();

  setSourceFile(f);
  string mainName;
  strInit(&mainName);
  strAddChar(&mainName,'m');
  strAddChar(&mainName,'a');
  strAddChar(&mainName,'i');
  strAddChar(&mainName,'n');
  parser();
  int FErr;
  if((FErr=PrintERR()) != 0){
    deleteConstTable(&newCTable);
    GSTdelete(&GST);
    return FErr;
  }
  L->active=(SearchFN(GST->FunRoot,&mainName))->instr;

  //printf("ZDE ZACINA RVA INTERPRETACE!!!!!!!!!!!!!!!!!!!!");

  interpret(L,&GST,&newCTable);
  deleteConstTable(&newCTable);
  GSTdelete(&GST);
  if((FErr=PrintERR()) != 0)
    return FErr;

}





