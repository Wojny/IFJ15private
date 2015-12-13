//+------------------------------------------------------------------+
//| Project name:        IFJ - Team project                          |
//| Filename:            errors.c                                    |
//| Author:                                                          |
//| Encoding:            UTF-8                                       |
//| Description:         Error repors.                               |
//+------------------------------------------------------------------+

#include "errors.h"

#define SIZE 50

// Init global 
int ErrCounter = 0;

// Chybove hlasenia
const char *ERRMESSAGE[] =
{
  "OK.\n",
  // Lexiklani chyba
  "sa vyskytla lexikalna chyba.",
  // Syntakticka chyba
  "sa vyskytla syntakticka chyba.",
  // Semanticka chyba
  "sa vyskytla semanticka chyba v programu.",
  // Semanticka chyba
  "sa vyskytla semanticka chyba typove kompatibility.",
  // Semanticka chyba
  "sa vyskytla chyba pri odvozovani data typu promenne.",
  // Semanticka chyba
  "sa vyskytla semanticka chyba.",
  // Chyba interpretace
  "sa vyskytla chyba pri nacitani ze vstupu.",
  // Chyba interpretace
  "sa vyskytla chyba pri praci s neinicializovanou promennou.",
  // Chyba interpretace
  "sa vyskytla chyba deleni nulou.",
  // Chyba interpretace
  "sa vyskytla chyba interpretacie.",
  // Vnutorna chyba interpretu
  "Za behu programu sa vyskytla vnutorna chyba! "
};


// Vytlaci jedno chybove hlasenie
void PrintERRItem(TErr *temp)
{

  if(temp->ErrorType == IFJ_ERR_PROGRAM)
    fprintf(stderr, "\n%s Error: %d \n", ERRMESSAGE[temp->ErrorType],99);
  else
    fprintf(stderr, "Na riadku %d %s Error: %d \n", temp->LineNumber, ERRMESSAGE[(temp->ErrorType)],temp->ErrorType);

}

// Init list
int MakeERRL(void)
{  
  if(((ERRList) = malloc((sizeof(TErr))*SIZE)) == NULL)
  {
    fprintf(stderr, "\n%s Error: %d \n", ERRMESSAGE[IFJ_ERR_PROGRAM],99);
    FreeERR();   
    return IFJ_ERR_PROGRAM;
  }
  for(int i=0; i< SIZE; i++)
  {
    ERRList[i].LineNumber = 0;
    ERRList[i].ErrorType = 0;
  }  
  return 0;
}

// Free list
void FreeERR(void)
{
  free(ERRList);
  ERRList = NULL;
}

//vytlaci chybove hlasenia
int PrintERR(void)
{
  for(int i=0;i <ErrCounter; i++)
  {
    PrintERRItem(&(ERRList[i]));
  }
  return ERRList[0].ErrorType;
}
// Vytvory novy zaznam o chybe
void AddERR(int ERRLine, int ERRType)
{  
  ERRList[ErrCounter].LineNumber = ERRLine;
  ERRList[ErrCounter].ErrorType = ERRType;
  ErrCounter++;
}
