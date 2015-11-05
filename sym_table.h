#include <stdio.h>
#include <stdlib.h>
//#include "str.h"

union Dat
{
   int *i;
   double *f;
   string *str;
} *datas;

typedef struct BT{
  string *ident;
  int type;
  int depth;
  int key;
  int def;
  struct BT *LBT;
  struct BT *RBT;
}  *BTree;

typedef struct BlockUnit{
  string *ident;
  int type;
  union Dat *data;
  int init;
} BU;
typedef struct BlockPtr{
  BU *BUPtr;
  int BUSize;
  struct BlockPtr *nxtBPtr;
} *BPtr;
typedef struct Block_Stack{
  BPtr First;
} *BlockStack;


typedef struct TST{
  BTree First;
  BTree Last;
} *tempST;

typedef struct FunctionNode{
  string *ident;
  string *type;
  int def;
  BTree BTroot;
  int BlockSize;
  tempST tempSTable;
  struct FunctionNode *LFN;
  struct FunctionNode *RFN;


} *FN;
typedef struct GlobalSystemTable{
  FN FunRoot;
} *GSTable;


typedef struct cTable{
  BTree First;
  BU *BUPtr;
  int BUSize;
  int BUAlloc;
} *constTable;



int BTinit(BTree *BTnode);
int BTAddID(BTree *BTroot,string *id, int type,int depth,int key);
BTree SearchBT(BTree BTroot, string *id);
int BTDelete(BTree *BTreeDisp,int depth,tempST *tmpST);
int tempSTinit(tempST *tempTable);
int tempSTadd(tempST *tempTable,BTree node);
int tempSTlength(tempST tempTable);
int tempST2BU(tempST tempTable,BU *newBU);
int GSTinit(GSTable *GST);
int GSTadd(GSTable *GST,string *id,int type);
FN SearchFN(FN FNroot, string *id);
union Dat *createDat(int type);
int updateDat(union Dat *pomDat,int type,int i,double d,string *s);
int deleteDat(union Dat *pomDat,int type);
int tempSTdelete(tempST *tempSTable);
int BUdelete(BU *Block, int BlockSize);
int BlockStackInit(BlockStack *BlStack);
int BlockStackAdd(BlockStack *BlStack,GSTable *GST,string *id);
int BlockStackDelete(BlockStack *BlStack);
