#include <stdio.h>
#include <stdlib.h>

#include "ilist.c"

// ***************************************** STRUKTURY PRO BLOKOVOU TABULKU, TABULKU KONSTANT A TABULKU SYMBOLU *******************************************
/*
 * Vypracoval: Stanislav Svoboda
 * Login: xsvobo0b
*/

union Dat   // data promennych a konstant
{
   int i;
   double f;
   string *str;
} *datas;


// listy binarniho vyhledavaciho stromu tabulky symbolu
//(plni funkci odkazu do blokove tabulky promennych a konstant za pomoci key coz oznacuje index)
typedef struct BT{
  string *ident;  // jmeno promenne
  int type;       // typ promenne
  int depth;      // uroven zanoreni promenne
  int key;        // poradi identifikatoru ve funkci a take index do blokove tabulky
  int def;        // doslo k definici promenne
  struct BT *LBT; // levy podstrom
  struct BT *RBT; // pravy podstrom
}  *BTree;


//polozka v blokove tabulce
typedef struct BlockUnit{
  string *ident;   // jmeno promenne
  int type;        // typ promenne
  union Dat *data; // data promenne
  int init;        // je nainicializovana
} BU;

//polozka jedna blokova tabulka
typedef struct BlockPtr{
  BU *BUPtr;       // pole polozek structury BlockUnit
  int BUSize;      // velikost pole
  struct BlockPtr *nxtBPtr;  // odkaz na dalsi blokovou tabulku
} *BPtr;
// zasobnik ukazujici na vrchol blokovych tabulek
typedef struct Block_Stack{
  BPtr First;   // odkaz na posledni vytvorenou blokovou tabulku
} *BlockStack;

// pomocny seznam pro ulozeni neplatnych promennych ve funkci z tabulky symbolu
typedef struct TST{
  BTree First;  // prvni v seznamu
  BTree Last;   // posledni v seznamu
} *tempST;


// polozka jedne funkce
typedef struct FunctionNode{
  string *ident;  // identifikator funkce
  string *type;   // typ funkce
  int def;        // definice funkce
  BTree BTroot;   // odkaz na tabulku symbolu
  int BlockSize;  // pocet promennych ve funkci
  tempST tempSTable;  // odkaz na pomocny seznam neplatnych promennych ve funkci
  tItemList *instr;
  struct FunctionNode *LFN;  // levy podstrom
  struct FunctionNode *RFN;  // pravy podstrom


} *FN;

// struktura obsahujici odkaz na koren globalni tabulky symbolu
typedef struct GlobalSystemTable{
  FN FunRoot;   // koren
} *GSTable;

// tabulka konstant
typedef struct cTable{
  BTree First; // seznam odkazu do blokove tabulky konstant
  BU *BUPtr;   // blokova tabulka konstant
  int BUSize;  // velikost blokove tabulky konstant
  int BUAlloc; // velikost naalokovane tabulky konstant
} *constTable;



int BTinit(BTree *BTnode);
int BTAddID(FN *FunNode,string *id, int vtype,int depth,int key);
BTree SearchBT(FN FunNode, string *id);
BTree SearchGetBT(BTree BTroot,string *id);
int BTDelete(BTree *BTreeDisp,int depth,tempST *tmpST);
int tempSTinit(tempST *tempTable);
int tempSTadd(tempST *tempTable,BTree *node);
int tempSTlength(tempST tempTable);
int tempST2BU(tempST tempTable,BU *newBU);
int GSTinit(GSTable *GST);
FN GSTadd(GSTable *GST,string *id,int type);
int addFunType(FN *FunNode,int type);
FN SearchFN(FN FNroot, string *id);
union Dat *createDat(int type);
int updateDat(union Dat *pomDat,int type,int i,double d,string *s);
int deleteDat(union Dat *pomDat,int type);
int tempSTdelete(tempST *tempSTable);
int BUdelete(BU *Block, int BlockSize);
int BlockStackInit(BlockStack *BlStack);
int BlockStackAdd(BlockStack *BlStack,GSTable *GST,string *id);
int BlockStackDelete(BlockStack *BlStack);
BTree createConst(constTable *newCTable,int type,string *str);
int constTableInit(constTable *newCTable);
int convStrToInt(string *s);
double convStrToDouble(string *s);
union Dat *getDat(constTable *newCTable,BPtr *BP,BTree *BTpom);
int isConstOrVar(int type);
int isInteger(int type);
int isString(int type);
int isDouble(int type);
int isConstant(int type);
int isVar(int type);
int getType(BTree *BTpom);
BTree SearchBTByKey(FN FunNode, int key);
BTree SearchGetBTByKey(BTree BTroot,int key);
int setType(BTree newBT,int newtype);
void addFunInst(FN *FunNode,tItemList *instr);
tItemList *getFunInst(FN *FunNode);
FN SearchDefinedFN(FN FNroot, string *id);
int isMultipleDefinedFun(GSTable GST, FN pomFNvzor);
int isMultipleDefinedFN(FN pomFN, FN pomFNvzor);
int isParamEqual(GSTable GST, FN pomFNvzor);
int isParamEqualFN(FN pomFN, FN pomFNvzor);
int checkFunParams(FN pomFN,FN pomFNvzor);
union Dat *getDatByKey(BPtr *BP,int key);
int getTypeByKey(BPtr *BP,int key);
void setFunDefined(FN FNpom);
BTree SearchTempSTByKey(FN pomFN,int i);
int CheckRedefProm(int int1,int int2);
int GSTdelete(GSTable *GST);
int deleteFN(FN FNroot);
