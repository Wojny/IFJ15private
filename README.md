//+------------------------------------------------------------------+
//| Project name:        IFJ - Team project                          |
//| Filename:            errors.c                                    |
//| Author:                                                          |
//| Encoding:            UTF-8                                       |
//| Description:         Error repors.                               |
//+------------------------------------------------------------------+

#include "errors.h"

#define SIZE 30

// Init global premennych 
int ErrFlag = 0;
int ErrCounter = 0;
int ReallocCounter = 0;


// Chybove hlasenia 
const char *ERRMSG[] =
{
  // EUNKNOWN 
  "Vyskytla se neznama chyba!",
  // Lexiklani chyba
  "sa vyskytla lexikalna chyba.\n",
  // Syntakticka chyba
  "sa vyskytla syntakticka chyba.\n",
  // Semanticka chyba 
  "sa vyskytla semanticka chyba.\n",
  // Chyba interpretace
  "sa vyskytla chyba interpretacie dat.",
  // Vnutorna chyba interpretu
  "Za behu programu sa vyskytla vnutorna chyba! \n"
};

// Vytlaci jedno chybove hlasenie 
void PrintERRItem(TErr *p_temp)
{ 
  if((p_temp->ErrorType < Unknown) || (p_temp->ErrorType > ProgErr))  
    p_temp->ErrorType = Unknown;
  
  if(p_temp->ErrorType == ProgErr) 
    fprintf(stderr, "\n%s", ERRMSG[p_temp->ErrorType]);
  else 
    fprintf(stderr, "Na riadku %d %s \n", p_temp->LineNumber, ERRMSG[(p_temp->ErrorType)]);

}

// Vytvori a inicializuje zoznam chyb 
int MakeERRL(void)
{ 
  if(((ERRList) = malloc((sizeof(TErr))*SIZE)) == NULL)
  {
    fprintf(stderr, "\n%s", ERRMSG[ProgErr]);
    FreeERR();
  return ProgErr;
  }
  for(int i=0; i< SIZE; i++)
  {
    ERRList[i].LineNumber = 0;
    ERRList[i].ErrorType = 0;
  }
return 0;
}

// Vytvory novy zaznam o chybe 
void AddERR(int ERRLine, int ERRType)
{ 
  TErr *p_temp;
  ErrFlag = ERRType;
  // Je vice chyb nez hranice pole 
  if(ErrCounter >= (SIZE + SIZE * ReallocCounter -1))
  { 
    ReallocCounter++;
    if((p_temp = realloc((ERRList), sizeof(TErr)*(SIZE+1))) == NULL)
    {
      fprintf(stderr, "\n%s", ERRMSG[ProgErr]);
      FreeERR();
    }
   ERRList = p_temp;
  }
  ERRList[ErrCounter].LineNumber = ERRLine;
  ERRList[ErrCounter].ErrorType = ERRType;
  ErrCounter++;
}

// Funkcia vytlaci vsetky chybove hlasenia 
void PrintERR(void)
{ 
  for(int i=0;i <ErrCounter; i++)
  {
    PrintERRItem(&(ERRList[i]));
  }
}

// Uvolnuje pole chyb 
void FreeERR(void)
{ 
  free(ERRList);
  ERRList = NULL;
}
