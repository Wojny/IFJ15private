#include <stdio.h>
#include <stdlib.h>
//#include "sym_table.c"
#include "precedence.c"

#define STGREAT 0
#define STLESS 1
#define STEQ 2
#define STNULL -1
#define STEND -2
#define STLESSSEP -3

//struktura syntaktickeho stromu
typedef struct SynTree{
  BTree node; // ulozeni odkazu na promennou, pomocnou promennou nebo konstantu
  int stype; // typ uzlu syntaktickeho stromu
  int isE;   // je exp
  struct SynTree *nxt; // odkaz na nasledujici prvek v pomocnem zasobniku
  struct SynTree *LSynTree; // odkaz na levy podstrom synt. stromu
  struct SynTree *RSynTree; // odkaz na pravy podstrom syntaktickeho stromu
} *sTree;
// pomocny zasobnik
typedef struct SynTreeStack{
  sTree First;
} *sTreeStack;
typedef struct HeadSynTree{
  sTree Head;
} *HSynTree;

int comp(sTreeStack STST,sTree ST0);
int execRule(sTreeStack *STST,int rule);
int getRuleNum(sTreeStack STST);
int convConstTypes(int type);
int initSTStack(sTreeStack *STST);
int popRemoveSTStack(sTreeStack *STST);
int popSTStack(sTreeStack *STST);
int pushSTStack(sTreeStack *STST,sTree newSTree);
