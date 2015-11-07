#include <stdio.h>
#include <stdlib.h>
#include "str.c"
#include "sym_table.h"


// inicializace polozky v tabulce symbolu
int BTinit(BTree *BTnode){
  if((((*BTnode)=malloc(sizeof (struct BT)))==NULL)) return -1;
  (*BTnode)->ident=NULL;
  (*BTnode)->type=-1;
  (*BTnode)->depth=-1;
  (*BTnode)->key=-1;
  (*BTnode)->LBT=NULL;
  (*BTnode)->RBT=NULL;
  return 0;

}


// pridani nove polozky do tabulky symbolu
/*
@param1 koren tabulky symbolu
@param2 identifikator promenne
@param3 typ promenne
@param4 hloubka zanoreni promenne
@param5 poradi promenne ve funkci
return ne/uspesnost vlozeni
*/
int BTAddID(BTree *BTroot,string *id, int type,int depth,int key){
  BTree pomBT=(*BTroot);
  if(pomBT->ident==NULL){
    pomBT->ident=id;
    pomBT->type=type;
    pomBT->depth=depth;
    pomBT->def=0;
    pomBT->key=key;
    return 0;
  }
  int cmp=0;
  while(pomBT->ident!=NULL){
    cmp=strCmpString(pomBT->ident,id);
    if(cmp==0){
      if(pomBT->depth==depth) return -1;
      //mozna i kontrola typu
      if(pomBT->LBT==NULL){
        BTree newBT;
        if(((newBT=malloc(sizeof (struct BT)))==NULL)) return -1;
        newBT->ident=id;
        newBT->type=type;
        pomBT->depth=depth;
        pomBT->def=0;
        pomBT->key=key;
        newBT->LBT=NULL;
        newBT->RBT=NULL;
        pomBT->LBT=newBT;
        return 0;
      }
      pomBT=pomBT->LBT;
    }
    else if(cmp>0){
      if(pomBT->RBT==NULL){
        BTree newBT;
        if(((newBT=malloc(sizeof (struct BT)))==NULL)) return -1;
        newBT->ident=id;
        newBT->type=type;
        pomBT->depth=depth;
        pomBT->def=0;
        pomBT->key=key;
        newBT->LBT=NULL;
        newBT->RBT=NULL;
        pomBT->RBT=newBT;
        return 0;
      }
      pomBT=pomBT->RBT;
    }
    else{
      if(pomBT->LBT==NULL){
        BTree newBT;
        if(((newBT=malloc(sizeof (struct BT)))==NULL)) return -1;
        newBT->ident=id;
        newBT->type=type;
        pomBT->depth=depth;
        pomBT->def=0;
        pomBT->key=key;
        newBT->LBT=NULL;
        newBT->RBT=NULL;
        pomBT->LBT=newBT;
        return 0;
      }
      pomBT=pomBT->LBT;

    }
  }
}


//Prohledani stromove struktury tabulky symbolu zdali obsahuje dany identifikator
/*
@param1 koren tabulky symbolu
@param2 identifikator hledane polozky
return  vraci NULL pokud nenasel nebo polozku tabulky seznamu pokud nasel
*/
BTree SearchBT(BTree BTroot, string *id){

  if(BTroot==NULL) return NULL;
  if(BTroot->ident==NULL) return NULL;
  int cmp=0;
  if(BTroot->ident!=NULL){
    cmp=strCmpString(BTroot->ident,id);
    if(cmp==0){
      BTree pomBT2;
      pomBT2=SearchBT(BTroot->LBT,id);
      if(pomBT2!=NULL) return pomBT2;
      else return BTroot;
    }
    else if(cmp>0){
      return SearchBT(BTroot->RBT,id);
    }
    else{
      return SearchBT(BTroot->LBT,id);
    }
  }
  else return NULL;
}


//odstraneni polozek z tabulky symbolu ktere jsou jiz neplatne (rekurzivne)
/*
@param1 koren tabulky symbolu
@param2 od jake hloubky zanoreni se bude odstranovat
@param3 pomocny seznam pro neplatne promenne z tabulky seznamu
return  vraci 0 pri uspechu jinak neuspech
*/
int BTDelete(BTree *BTreeDisp,int depth,tempST *tmpST){
  if((*BTreeDisp)!=NULL){ // pokud je strom neprazdny
    if(((*BTreeDisp)->LBT)!=NULL){ // pokud je levy podstrom neprazdny tak dojde k odstraneni leveho podstromu
      BTDelete(&((*BTreeDisp)->LBT),depth,tmpST);
    }
    if((*BTreeDisp)->RBT!=NULL){ // pokud je pravy podstrom neprazdny tak dojde k odstraneni praveho podstromu
      BTDelete(&((*BTreeDisp)->RBT),depth,tmpST);
    }
    if((*BTreeDisp)->ident!=NULL){
      if((*BTreeDisp)->depth==depth){
        tempSTadd(tmpST,*BTreeDisp);
      }
    }
    
  }
  return 0;
}

    
// inicializace zasobniku blokovych tabulek
int BlockStackInit(BlockStack *BlStack){
  if((((*BlStack)=malloc(sizeof (struct Block_Stack)))==NULL)) return -1;
  (*BlStack)->First=NULL;
}
//pridani nove tabulky do zasobniku
/*
@param1  zasobnik blokovych tabulek
@param2  globalni tabulka symbolu
return   vraci ne/uspesnost operace pridani
*/
int BlockStackAdd(BlockStack *BlStack,GSTable *GST){
  BU *newBU;
  BPtr newBPtr;
  if(((newBPtr=malloc(sizeof (struct BlockPtr)))==NULL)) return -1;
  FN pomFN=SearchFN((*GST)->FunRoot,id);
  int size=tempSTlength(pomFN->tempSTable);
  if(((newBU=malloc(sizeof (struct BlockUnit)*size))==NULL)) return -1;
  tempST2BU(pomFN->tempSTable,newBU);

  if((*BlStack)->First==NULL){
    newBPtr->nxtBPtr=NULL;
    newBPtr->BUSize=size;
    newBPtr->BUPtr=newBU;
    (*BlStack)->First=newBPtr;
  }
  else{
    newBPtr->nxtBPtr=(*BlStack)->First;
    newBPtr->BUSize=size;
    newBPtr->BUPtr=newBU;
    (*BlStack)->First=newBPtr;
  }
}

//odstrani blokovou tabulku z vrcholu zasobniku
/*
param1 zasobnik blokovych tabulek
return vraci ne/uspesnost operace
*/
int BlockStackDelete(BlockStack *BlStack){
  BPtr newBPtr=(*BlStack)->First;
  (*BlStack)->First=(*BlStack)->First->nxtBPtr;
  BUdelete(newBPtr->BUPtr, newBPtr->BUSize);
  free(newBPtr);

}

//inicializace pomocneho seznamu neplatnych promennych
int tempSTinit(tempST *tempTable){
  if((((*tempTable)=malloc(sizeof (struct TST)))==NULL)) return -1;
  (*tempTable)->First=NULL;
  (*tempTable)->Last=NULL;
  return 0;
}

//pridani polozky do pomocneho seznamu - vola se automaticky pri odstranovani z tabulky symbolu
int tempSTadd(tempST *tempTable,BTree node){
  if((*tempTable)->First==NULL){
    node->LBT=NULL;
    node->RBT=NULL;
    (*tempTable)->First=node;
    (*tempTable)->Last=node;
  }
  else{
    node->LBT=NULL;
    node->RBT=NULL;
    (*tempTable)->Last->RBT=node;
    (*tempTable)->Last=node;
  }
  return 0;
}

// pomocna funkce pro vypocet delky pomocneho seznamu
int tempSTlength(tempST tempTable){
  int i=0;
  BTree pomBT=tempTable->First;
  while(pomBT!=NULL){
    i++;
    pomBT=pomBT->RBT;
  }
  return i;
}

// naplneni blokove tabulky polozkami z pomocneho seznamu
/*
@param1 pomocny seznam
@param2 nova blokova tabulka
return vraci ne/uspesnost operace
*/
int tempST2BU(tempST tempTable,BU *newBU){
  BTree pomBT=tempTable->First;

  while(pomBT!=NULL){
    newBU[pomBT->key].ident=pomBT->ident;
    newBU[pomBT->key].type=pomBT->type;
    newBU[pomBT->key].data=createDat(pomBT->type);
    newBU[pomBT->key].init=0;
    pomBT=pomBT->RBT;
  }
  return 0;
}

//odstraneni pomocneho seznamu
int tempSTdelete(tempST *tempSTable){
  BTree pomBT=(*tempSTable)->First;
  while(pomBT!=NULL){
   
    pomBT->RBT->LBT=pomBT;
    pomBT=pomBT->RBT;
    free(pomBT->LBT);
  }
  return 0;
}
// odstraneni blokove tabulky
int BUdelete(BU *Block, int BlockSize){
  int i=0;
  while(i<BlockSize){
    strFree(Block[i].ident);
    deleteDat(Block[i].data,Block[i].type);
    i++;
  }
  free(Block);
}


//------------------------------------------
//--------------------------GTS-------------
// inicializace struktury obsahujici odkaz na koren globalni tabulky symbolu
int GSTinit(GSTable *GST){
  if((((*GST)=malloc(sizeof (struct GlobalSystemTable)))==NULL)) return -1;
  (*GST)->FunRoot=NULL;
}
// pridani nove funkce do globalni tabulky symbolu
/*
@param1 globalni tabulka symbolu
@param2 identifikator funkce
@param3 navratova hodnota funkce
return vraci ne/uspesnost operace
*/ 
int GSTadd(GSTable *GST,string *id,int type){
  FN FunNode;
  if(((FunNode=malloc(sizeof (struct FunctionNode)))==NULL)) return -1;

  if((*GST)->FunRoot==NULL){
    (*GST)->FunRoot=FunNode;
    if(((FunNode->ident=malloc(sizeof (string)))==NULL)) return -1;
    if(((FunNode->type=malloc(sizeof (string)))==NULL)) return -1;
    strInit(FunNode->ident);
    strCopyString(FunNode->ident,id);
    strInit(FunNode->type);
    if(type==27) strAddChar(FunNode->type,'s');
    else if(type==28) strAddChar(FunNode->type,'i');
    else if(type==29) strAddChar(FunNode->type,'d');
    FunNode->LFN=NULL;
    FunNode->RFN=NULL;
    FunNode->def=0;
    tempSTinit(&(FunNode->tempSTable));
    BTinit(&(FunNode->BTroot));
  }
  else{
    FN pomFN=(*GST)->FunRoot;
    int cmp=0;
    while(pomFN->ident!=NULL){
      cmp=strCmpString(pomFN->ident,id);
      if(cmp==0){

        //mozna i kontrola typu
        if(pomFN->LFN==NULL){
          pomFN->LFN=FunNode;
          if(((FunNode->ident=malloc(sizeof (string)))==NULL)) return -1;
          if(((FunNode->type=malloc(sizeof (string)))==NULL)) return -1;
          strInit(FunNode->ident);
          strCopyString(FunNode->ident,id);
          strInit(FunNode->type);
          if(type==27) strAddChar(FunNode->type,'s');
          else if(type==28) strAddChar(FunNode->type,'i');
          else if(type==29) strAddChar(FunNode->type,'d');
          FunNode->LFN=NULL;
          FunNode->RFN=NULL;
          FunNode->def=0;
          tempSTinit(&(FunNode->tempSTable));
          BTinit(&(FunNode->BTroot));
          return 0;
        }
        pomFN=pomFN->LFN;
      }
      else if(cmp>0){
        if(pomFN->RFN==NULL){
          pomFN->RFN=FunNode;
          if(((FunNode->ident=malloc(sizeof (string)))==NULL)) return -1;
          if(((FunNode->type=malloc(sizeof (string)))==NULL)) return -1;
          strInit(FunNode->ident);
          strCopyString(FunNode->ident,id);
          strInit(FunNode->type);
          if(type==27) strAddChar(FunNode->type,'s');
          else if(type==28) strAddChar(FunNode->type,'i');
          else if(type==29) strAddChar(FunNode->type,'d');
          FunNode->LFN=NULL;
          FunNode->RFN=NULL;
          FunNode->def=0;
          tempSTinit(&(FunNode->tempSTable));
          BTinit(&(FunNode->BTroot));
          return 0;
        }
        pomFN=pomFN->RFN;
      }
      else{
        if(pomFN->LFN==NULL){
          pomFN->LFN=FunNode;
          if(((FunNode->ident=malloc(sizeof (string)))==NULL)) return -1;
          if(((FunNode->type=malloc(sizeof (string)))==NULL)) return -1;
          strInit(FunNode->ident);
          strCopyString(FunNode->ident,id);
          strInit(FunNode->type);
          if(type==27) strAddChar(FunNode->type,'s');
          else if(type==28) strAddChar(FunNode->type,'i');
          else if(type==29) strAddChar(FunNode->type,'d');
          FunNode->LFN=NULL;
          FunNode->RFN=NULL;
          FunNode->def=0;
          tempSTinit(&(FunNode->tempSTable));
          BTinit(&(FunNode->BTroot));
          return 0;
        }
        pomFN=pomFN->LFN;
      }
    }
  }
}


//hledani funkce v globalni tabulce symbolu
/*
@param1 koren globalni tabulky symbolu
@param2 identifikator hledane funkce
return  odkaz na polozku funkce v globalni tabulce symbolu
*/
FN SearchFN(FN FNroot, string *id){
  FN pomFN=FNroot;
  if(FNroot==NULL) return NULL;
  if(pomFN->ident==NULL) return NULL;
  int cmp=0;
  if(pomFN->ident!=NULL){
    cmp=strCmpString(pomFN->ident,id);
    if(cmp==0){
      FN pomFN2;
      pomFN2=SearchFN(pomFN->LFN,id);
      if(pomFN2!=NULL) return pomFN2;
      else return pomFN;
    }
    else if(cmp>0){
      return SearchFN(pomFN->RFN,id);
    }
    else{
      return SearchFN(pomFN->LFN,id);
    }
  }
  else return NULL;


}

//odstraneni funkci z tabulky symbolu (rekurzivne)
/*
@param1 koren globalni tabulky symbolu
return vraci ne/uspesnost operace
*/
int deleteFN(FN FNroot){
  if(FNroot==NULL) return 0;
  if(FNroot->LFN!=NULL) deleteFN(FNroot->LFN);
  if(FNroot->RFN!=NULL) deleteFN(FNroot->RFN);
  if(FNroot->ident!=NULL) strFree(FNroot->ident);
  if(FNroot->type!=NULL) strFree(FNroot->type);
  tempSTdelete(&(FNroot->tempSTable));
  free(FNroot);


}

//--------------------konstanty---------------------

//inicializace tabulky  konstant
int constTableInit(constTable *newCTable){
  if((((*newCTable)=malloc(sizeof (struct cTable)))==NULL)) return -1;
  (*newCTable)->First=NULL;
  if((((*newCTable)->BUPtr=malloc(sizeof (struct BlockUnit)*10))==NULL)) return -1;
  (*newCTable)->BUSize=0;
  (*newCTable)->BUAlloc=10;
  return 0;
}

//vytvoreni nove konstanty
/*
@param1 blokova tabulka konstant
@param2 typ nove konstanty
@param3 data nove konstanty
return vraci odkaz na polozku v blokove tabulce konstant
*/
BTree constTableAdd(constTable *newCTable,int type,union Dat *data){
  BTree newBTree;
  if((*newCTable)->First==NULL){
    if(((newBTree=malloc(sizeof (struct BT)))==NULL)) return NULL;
    (*newCTable)->First=newBTree;
    newBTree->ident=NULL;
    newBTree->type=type;
    newBTree->depth=0;
    newBTree->def=1;
    newBTree->key=(*newCTable)->BUSize;
    newBTree->LBT=NULL;
    newBTree->RBT=NULL;
  }
  else{
    if(((newBTree=malloc(sizeof (struct BT)))==NULL)) return NULL;
    newBTree->ident=NULL;
    newBTree->type=type;
    newBTree->depth=0;
    newBTree->def=1;
    newBTree->key=(*newCTable)->BUSize;
    newBTree->LBT=NULL;
    newBTree->RBT=(*newCTable)->First;
    (*newCTable)->First=newBTree;
  }
  if((*newCTable)->BUSize>=(*newCTable)->BUAlloc){
    if((*newCTable)->BUPtr=realloc((*newCTable)->BUPtr, sizeof(struct BlockUnit)*((*newCTable)->BUSize + 10))) return NULL;
    (*newCTable)->BUAlloc=(*newCTable)->BUSize + 10;
  }
  (*newCTable)->BUPtr[(*newCTable)->BUSize].ident=NULL;
  (*newCTable)->BUPtr[(*newCTable)->BUSize].type=type;
  (*newCTable)->BUPtr[(*newCTable)->BUSize].data=data;
  (*newCTable)->BUPtr[(*newCTable)->BUSize].init=1;
  (*newCTable)->BUSize++;
  return newBTree;
}




//----------------------------------------
//inicializace datove polozky
/*
@param1 typ novych dat
return odkaz na nova data
*/
union Dat *createDat(int type){
  union Dat *pomDat;
  if((type==27)||(type==30)){
    if(((pomDat=malloc(sizeof (union Dat)))==NULL)) return NULL;
    string *s;
    if(((s=malloc(sizeof (string))))==NULL) return NULL;
    strInit(s);
    pomDat->str=s;

  }
  else if((type==28)||(type==31)){
    int *pomI;
    if(((pomI=malloc(sizeof (int)))==NULL)) return NULL;
    if(((pomDat=malloc(sizeof (union Dat)))==NULL)) return NULL;
    pomDat->i=pomI;
  }
  else if((type==29)||(type==32)){
    double *pomD;
    if(((pomD=malloc(sizeof (double)))==NULL)) return NULL;
    if(((pomDat=malloc(sizeof (union Dat)))==NULL)) return NULL;
    pomDat->f=pomD;
  }
  else return NULL;
  return pomDat;
}

//naplneni nebo aktualizovani dat
/*
@param1  odkaz na polozku dat
@param2  typ novych dat
@param3  pokud byl typ int vlozi se do dat tento parametr
@param4  pokud byl typ double vlozi se do dat tento parametr
@param5  pokud byl typ string vlozi se do dat tento parametr
return  vraci ne/uspesnost operace
*/
int updateDat(union Dat *pomDat,int type,int i,double d,string *s){
  if((type==27)||(type==30)){
    strFree(pomDat->str);
    pomDat->str=s;
    if(((s=malloc(sizeof (string))))==NULL) return -1;
    strInit(s);
  }
  else if((type==28)||(type==31)){
    *(pomDat->i)=i;
  }
  else if((type==29)||(type==32)){
    *(pomDat->f)=d;
  }
  else return -1;
  return 0;
}

//odstraneni datove polozky
/*
@param1 odkaz na datovou polozku
return typ dat ulozenych v datove polozce
*/
int deleteDat(union Dat *pomDat,int type){
  if(type==27){
    strFree(pomDat->str);
  }
  else if(type==28){
    free(pomDat->i);
  }
  else if(type==29){
    free(pomDat->f);
  }
  else return -1;
  free(pomDat);
  return 0;
}
