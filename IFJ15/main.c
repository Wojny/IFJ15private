

#include <stdio.h>
#include <stdlib.h>

#include "parser.c"



int main(int argc, char **argv){
  FILE *f;
  int FErr;

  if(argc != 2)
  {
      fprintf(stderr, "Spatny pocet argumentu interpretu.\n");
      return 99;
  }

  if( (f = fopen(argv[1], "r+")) == NULL )
  {
      fprintf(stderr, "Zdrojovy program nelze otevrit.\n");
      return 99;
  }
  MakeERRL();

  setSourceFile(f); // nastaveni zdrojoveho souboru v lexikalni analyze
  string mainName;
  strInit(&mainName);
  strAddChar(&mainName,'m');
  strAddChar(&mainName,'a');
  strAddChar(&mainName,'i');
  strAddChar(&mainName,'n');

  parser();

  if((FErr=PrintERR()) != 0){
    //dealokace globalni tabulky symbolu a tabulky konstant
    deleteConstTable(&newCTable);
    GSTdelete(&GST);
    if(FErr==11) return 99;
    else return FErr;
  }
  FN pomFN=SearchFN(GST->FunRoot,&mainName);
  if(pomFN==NULL){
    AddERR(line,IFJ_ERR_SEMANTIC);
    if((FErr=PrintERR()) != 0)
      return FErr;

  }
  L->active=pomFN->instr;

  interpret(L,&GST,&newCTable);
  //dealokace globalni tabulky symbolu a tabulky konstant
  deleteConstTable(&newCTable);
  GSTdelete(&GST);

  if((FErr=PrintERR()) != 0){
    if(FErr==11) return 99;
    else return FErr;
  }


}





