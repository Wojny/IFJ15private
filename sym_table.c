#include <stdio.h>
#include <stdlib.h>
//#include "str.c"
//#include "sym_table.h"
#include "scaner.c"



/*
pro syntaktickou analyzu
bude treba nadeklarovat nekolik pomocnych globalnich promennych pro praci s tabulkou symbolu:
napr:
GSTable GST; slouzi pro uchovani odkazu na globalni tabulku symbolu
FN FunNode; slouzi pro uchovani aktualne zpracovavane funkce
constTable ConTable; slouzi k uchovani odkazu na tabulku konstant
int key; slouzi k uchovani poradi promennych ve funkci
int depth; slouzi k uchovani hloubky zanoreni ve zdrojovem programu
*/

// inicializace polozky v tabulce symbolu
int BTinit(BTree *BTnode){
  if((((*BTnode)=malloc(sizeof (struct BT)))==NULL)){
        return -1;
  }
  (*BTnode)->ident=NULL;
  (*BTnode)->type=-1;
  (*BTnode)->depth=-1;
  (*BTnode)->key=-1;
  (*BTnode)->def=-1;
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
/* SYN: pokud narazis na deklaraci promenne tak zavolas tuhle funkci a predas ji odkaz na prave zpracovavanou
funkci(FunNode), potom predas identifikator promenne a typ promenne(typ tokenu pred identifikatorem),
potom predas hloubku zanoreni a pote predas poradi identifikatoru ve funkci (po kazdem volani teto funkce tuto hodnotu musis inkrementovat a pokud vystoupis pri analyze z uzivatelem definovane funkce tak globalni hodnotu key vynuluj)
*/
int BTAddID(FN *FunNode,string *id, int vtype,int depth,int key){
  int type;
  if(vtype==KINTEGER) type=IINTEGER;
  if(vtype==KDOUBLE) type=IDOUBLE;
  if(vtype==KSTRING) type=ISTR;
  BTree pomBT=(*FunNode)->BTroot;
  if(pomBT->ident==NULL){
    if((((*FunNode)->BTroot->ident=malloc(sizeof (string)))==NULL)){
      return -1;
    }
    strInit((*FunNode)->BTroot->ident);
    strCopyString((*FunNode)->BTroot->ident,id);
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
      if(pomBT->depth==depth) return -1;// semanticka kontrola!!!!!!!!
      //mozna i kontrola typu
      if(pomBT->LBT==NULL){
        BTree newBT;
        if(((newBT=malloc(sizeof (struct BT)))==NULL)){
          return -1;
        }
        if(((newBT->ident=malloc(sizeof (string)))==NULL)){
          return -1;
        }
        strInit(newBT->ident);
        strCopyString(newBT->ident,id);
        newBT->type=type;
        newBT->depth=depth;
        newBT->def=0;
        newBT->key=key;
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
        if(((newBT=malloc(sizeof (struct BT)))==NULL)){
          return -1;
        }
        if(((newBT->ident=malloc(sizeof (string)))==NULL)){
          return -1;
        }
        strInit(newBT->ident);
        strCopyString(newBT->ident,id);
        newBT->type=type;
        newBT->depth=depth;
        newBT->def=0;
        newBT->key=key;
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
        if(((newBT=malloc(sizeof (struct BT)))==NULL)){
          return -1;
        }
        if(((newBT->ident=malloc(sizeof (string)))==NULL)){
          return -1;
        }
        strInit(newBT->ident);
        strCopyString(newBT->ident,id);
        newBT->type=type;
        newBT->depth=depth;
        newBT->def=0;
        newBT->key=key;
        newBT->LBT=NULL;
        newBT->RBT=NULL;
        pomBT->LBT=newBT;
        return 0;
      }
      pomBT=pomBT->LBT;

    }
  }
  return -1;
}

//nastaveni definice promenne
void setVarDefined(BTree newBT){
  newBT->def=1;
}
int isVarDefined(BTree newBT){
  if(newBT->def==1) return 1;
  else return 0;
}



//Prohledani stromove struktury tabulky symbolu zdali obsahuje dany identifikator
/*
@param1 aktualni zpracovavana funkce
@param2 identifikator hledane polozky
return  vraci NULL pokud nenasel nebo polozku tabulky seznamu pokud nasel
*/
/*
SYN: Pokud narazis na nejakou promennou a nebude to pri deklaraci tak timto overis zda promenna byla jiz deklarovana
vraci NULL pokud nebyla deklarovana
*/
BTree SearchBT(FN FunNode, string *id){
  return SearchGetBT(FunNode->BTroot,id);
}
//pomocna funkce pro SearchBT
BTree SearchGetBT(BTree BTroot,string *id){
  if(BTroot==NULL) return NULL;
  if(BTroot->ident==NULL) return NULL;
  int cmp=0;
  if(BTroot->ident!=NULL){
    cmp=strCmpString(BTroot->ident,id);
    if(cmp==0){
      BTree pomBT2;
      pomBT2=SearchGetBT(BTroot->LBT,id);
      if(pomBT2!=NULL) return pomBT2;
      else return BTroot;
    }
    else if(cmp>0){
      return SearchGetBT(BTroot->RBT,id);
    }
    else{
      return SearchGetBT(BTroot->LBT,id);
    }
  }
  else return NULL;

}

BTree SearchBTByKey(FN FunNode, int key){
  return SearchGetBTByKey(FunNode->BTroot,key);
}

BTree SearchGetBTByKey(BTree BTroot,int key){
  if(BTroot==NULL) return NULL;
  if(BTroot->key==-1) return NULL;
  if(BTroot->key!=-1){
    if(BTroot->key==key){
      return BTroot;
    }
    else{
      BTree pomBT=SearchGetBTByKey(BTroot->LBT,key);
      if(pomBT==NULL) return SearchGetBTByKey(BTroot->RBT,key);
      else return pomBT;
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
/*
SYN: Pri opusteni funkce nebo pri vychodu z if nebo for tak zavolas tuto funkci
jako prvni parametr zavolas globalni parametr aktualne zpracovavane funkce:
FN *FunNode; je deklarace pomocne globalni promenne pro ulozeni odkazu na aktualne zpracovanou funkci
a potom v teto funkci pouzijes BTDelete(FunNode->BTroot,depth,FunNode->tempSTable);
*/
int BTDelete(BTree *BTreeDisp,int depth,tempST *tmpST){
  if((*BTreeDisp)!=NULL){ // pokud je strom neprazdny

    if(((*BTreeDisp)->LBT)!=NULL){ // pokud je levy podstrom neprazdny tak dojde k odstraneni leveho podstromu
      BTDelete(&((*BTreeDisp)->LBT),depth,tmpST);
      if(((*BTreeDisp)->depth!=depth)&&((*BTreeDisp)->LBT->depth==depth)) (*BTreeDisp)->LBT=NULL;
    }
    if((*BTreeDisp)->RBT!=NULL){ // pokud je pravy podstrom neprazdny tak dojde k odstraneni praveho podstromu
      BTDelete(&((*BTreeDisp)->RBT),depth,tmpST);
      if(((*BTreeDisp)->depth!=depth)&&((*BTreeDisp)->RBT->depth==depth)) (*BTreeDisp)->RBT=NULL;
    }
    if((*BTreeDisp)->ident!=NULL){
      if((*BTreeDisp)->depth==depth){
        tempSTadd(tmpST,BTreeDisp);
      }
    }

  }
  return 0;
}


// inicializace zasobniku blokovych tabulek
int BlockStackInit(BlockStack *BlStack){
  if((((*BlStack)=malloc(sizeof (struct Block_Stack)))==NULL)){
    return -1;
  }
  (*BlStack)->First=NULL;
  return 0;
}
//pridani nove tabulky do zasobniku
/*
@param1  zasobnik blokovych tabulek
@param2  globalni tabulka symbolu
return   vraci ne/uspesnost operace pridani
*/
int BlockStackAdd(BlockStack *BlStack,GSTable *GST,string *id){
  BU *newBU;
  BPtr newBPtr;
  if(((newBPtr=malloc(sizeof (struct BlockPtr)))==NULL)){
    return -1;
  }
  FN pomFN=SearchDefinedFN((*GST)->FunRoot,id);
  int size=tempSTlength(pomFN->tempSTable);
  if(((newBU=malloc(sizeof (struct BlockUnit)*size))==NULL)){
    return -1;
  }
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
  return 0;
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
  return 0;
}

//inicializace pomocneho seznamu neplatnych promennych
int tempSTinit(tempST *tempTable){
  if((((*tempTable)=malloc(sizeof (struct TST)))==NULL)){
    return -1;
  }
  (*tempTable)->First=NULL;
  (*tempTable)->Last=NULL;
  return 0;
}

//pridani polozky do pomocneho seznamu - vola se automaticky pri odstranovani z tabulky symbolu
int tempSTadd(tempST *tempTable,BTree *node){
  if((*tempTable)->First==NULL){
    (*node)->LBT=NULL;
    (*node)->RBT=NULL;
    (*tempTable)->First=(*node);
    (*tempTable)->Last=(*node);
  }
  else{
    (*node)->LBT=NULL;
    (*node)->RBT=NULL;
    (*tempTable)->Last->RBT=(*node);
    (*tempTable)->Last=(*node);
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
  return 0;
}


//------------------------------------------
//--------------------------GTS-------------
// inicializace struktury obsahujici odkaz na koren globalni tabulky symbolu
int GSTinit(GSTable *GST){
  if((((*GST)=malloc(sizeof (struct GlobalSystemTable)))==NULL)){
    return -1;
  }
  (*GST)->FunRoot=NULL;
  return 0;
}
// pridani nove funkce do globalni tabulky symbolu
/*
@param1 globalni tabulka symbolu
@param2 identifikator funkce
@param3 navratova hodnota funkce
return odkaz na nove vytvorenou funkci
*/
/*
SYN: tuto funkci zavolas, pokud dojde k deklaraci nove funkce. Predas pomocnou promennou pro ulozeni globalni
promenne ktera obsahuje odkaz na globalni tabulku symbolu (treba GSTable GST;)
pote identifikator funkce a pote navratovy typ funkce
*/
FN GSTadd(GSTable *GST,string *id,int type){
  FN FunNode;
  if(((FunNode=malloc(sizeof (struct FunctionNode)))==NULL)){
    return NULL;
  }

  if((*GST)->FunRoot==NULL){
    (*GST)->FunRoot=FunNode;
    if(((FunNode->ident=malloc(sizeof (string)))==NULL)){
      return NULL;
    }
    if(((FunNode->type=malloc(sizeof (string)))==NULL)){
      return NULL;
    }
    strInit(FunNode->ident);
    strCopyString(FunNode->ident,id);
    strInit(FunNode->type);
    if(type==KSTRING) strAddChar(FunNode->type,'s');
    else if(type==KINTEGER) strAddChar(FunNode->type,'i');
    else if(type==KDOUBLE) strAddChar(FunNode->type,'d');
    FunNode->instr=NULL;
    FunNode->LFN=NULL;
    FunNode->RFN=NULL;
    FunNode->def=0;
    tempSTinit(&(FunNode->tempSTable));
    BTinit(&(FunNode->BTroot));
    return FunNode;
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
          if(((FunNode->ident=malloc(sizeof (string)))==NULL)){
            return NULL;
          }
          if(((FunNode->type=malloc(sizeof (string)))==NULL)){
            return NULL;
          }
          strInit(FunNode->ident);
          strCopyString(FunNode->ident,id);
          strInit(FunNode->type);
          if(type==KSTRING) strAddChar(FunNode->type,'s');
          else if(type==KINTEGER) strAddChar(FunNode->type,'i');
          else if(type==KDOUBLE) strAddChar(FunNode->type,'d');
          FunNode->instr=NULL;
          FunNode->LFN=NULL;
          FunNode->RFN=NULL;
          FunNode->def=0;
          tempSTinit(&(FunNode->tempSTable));
          BTinit(&(FunNode->BTroot));
          return FunNode;
        }
        pomFN=pomFN->LFN;
      }
      else if(cmp>0){
        if(pomFN->RFN==NULL){
          pomFN->RFN=FunNode;
          if(((FunNode->ident=malloc(sizeof (string)))==NULL)){
            return NULL;
          }
          if(((FunNode->type=malloc(sizeof (string)))==NULL)){
            return NULL;
          }
          strInit(FunNode->ident);
          strCopyString(FunNode->ident,id);
          strInit(FunNode->type);
          if(type==KSTRING) strAddChar(FunNode->type,'s');
          else if(type==KINTEGER) strAddChar(FunNode->type,'i');
          else if(type==KDOUBLE) strAddChar(FunNode->type,'d');
          FunNode->instr=NULL;
          FunNode->LFN=NULL;
          FunNode->RFN=NULL;
          FunNode->def=0;
          tempSTinit(&(FunNode->tempSTable));
          BTinit(&(FunNode->BTroot));
          return FunNode;
        }
        pomFN=pomFN->RFN;
      }
      else{
        if(pomFN->LFN==NULL){
          pomFN->LFN=FunNode;
          if(((FunNode->ident=malloc(sizeof (string)))==NULL)){
            return NULL;
          }
          if(((FunNode->type=malloc(sizeof (string)))==NULL)){
            return NULL;
          }
          strInit(FunNode->ident);
          strCopyString(FunNode->ident,id);
          strInit(FunNode->type);
          if(type==KSTRING) strAddChar(FunNode->type,'s');
          else if(type==KINTEGER) strAddChar(FunNode->type,'i');
          else if(type==KDOUBLE) strAddChar(FunNode->type,'d');
          FunNode->instr=NULL;
          FunNode->LFN=NULL;
          FunNode->RFN=NULL;
          FunNode->def=0;
          tempSTinit(&(FunNode->tempSTable));
          BTinit(&(FunNode->BTroot));
          return FunNode;
        }
        pomFN=pomFN->LFN;
      }
    }
  }
  return NULL;
}


void addFunInst(FN *FunNode,tItemList *instr){
  (*FunNode)->instr=instr;
}

tItemList *getFunInst(FN *FunNode){
  return (*FunNode)->instr;
}

//pridani typu parametru funkce do aktualne zpracovane funkce
/*
@param1 aktualne zpracovavana funkce
@param2 typ dalsiho parametru funkce
return vraci ne/uspesnost operace
*/
/*
SYN: zavolas pri kazdem nacteni parametru funkce
*/
int addFunType(FN *FunNode,int type){
  if(type==KSTRING) strAddChar((*FunNode)->type,'s');
  else if(type==KINTEGER) strAddChar((*FunNode)->type,'i');
  else if(type==KDOUBLE) strAddChar((*FunNode)->type,'d');
  return 0;
}
int getFunParamType(FN *FunNode,int num){
  if(strGetLength((*FunNode)->type)>num){
    return (*FunNode)->type->str[num];
  }
  else return -1;

}


//hledani funkce v globalni tabulce symbolu
/*
@param1 koren globalni tabulky symbolu
@param2 identifikator hledane funkce
return  odkaz na polozku funkce v globalni tabulce symbolu
*/
/*
SYN: zavolas pokud najdes pouziti nektere uzivatelem definovane funkce
(napr: SearchFN(GST->FunRoot,ident))
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
// vraci definovanou funkci podle jmena
FN SearchDefinedFN(FN FNroot, string *id){
  FN pomFN=FNroot;
  if(FNroot==NULL) return NULL;
  if(pomFN->ident==NULL) return NULL;
  int cmp=0;
  if(pomFN->ident!=NULL){
    cmp=strCmpString(pomFN->ident,id);
    if(cmp==0){
      if(pomFN->def==1) return pomFN;
      else return SearchDefinedFN(pomFN->LFN,id);
    }
    else if(cmp>0){
      return SearchDefinedFN(pomFN->RFN,id);
    }
    else{
      return SearchDefinedFN(pomFN->LFN,id);
    }
  }
  else return NULL;


}

// kontrola vicero definici jedne funkce
// pri vicero definici vraci 1 jinak 0
int defined;
int isMultipleDefinedFun(GSTable GST, FN pomFNvzor){
  defined=0;
  return isMultipleDefinedFN(GST->FunRoot,pomFNvzor);
}
int isMultipleDefinedFN(FN pomFN, FN pomFNvzor){
  if(pomFN==NULL) return 0;
  if(pomFN->ident==NULL) return 0;
  int cmp=0;
  if(pomFN->ident!=NULL){
    cmp=strCmpString(pomFN->ident,pomFNvzor->ident);
    if(cmp==0){
      if((defined==1)&&(pomFN->def==1)) return 1;
      if(pomFN->def==1) defined=1;
      return isMultipleDefinedFN(pomFN->LFN,pomFNvzor);
    }
    else if(cmp>0){
      return isMultipleDefinedFN(pomFN->RFN,pomFNvzor);
    }
    else{
      return isMultipleDefinedFN(pomFN->LFN,pomFNvzor);
    }
  }
  else return 0;


}

// kontrola shodnosti parametru nove funkce a ostatnich definovanych/deklarovanych
// pri shode vraci 1 jinak 0
int isParamEqual(GSTable GST, FN pomFNvzor){
  return isParamEqualFN(GST->FunRoot,pomFNvzor);
}
int isParamEqualFN(FN pomFN, FN pomFNvzor){
  if(pomFN==NULL) return 1;
  if(pomFN->ident==NULL) return 1;
  int cmp=0;
  if(pomFN->ident!=NULL){
    cmp=strCmpString(pomFN->ident,pomFNvzor->ident);
    if(cmp==0){
      int cmp2=strCmpString(pomFN->type,pomFNvzor->type);
      if(cmp2!=0) return 0;
      cmp2=checkFunParams(pomFN,pomFNvzor);
      if(cmp2!=0) return 0;
      return isParamEqualFN(pomFN->LFN,pomFNvzor);
    }
    else if(cmp>0){
      return isParamEqualFN(pomFN->RFN,pomFNvzor);
    }
    else{
      return isParamEqualFN(pomFN->LFN,pomFNvzor);
    }
  }
  else return 0;
}
// kontrola parametru funkce
int checkFunParams(FN pomFN,FN pomFNvzor){
  int len=strGetLength(pomFN->type);
  int i;
  BTree pomBT;
  BTree pomBT2;
  for(i=1;i<=len;i++){
    pomBT=SearchBTByKey(pomFN, i);
    if(pomBT==NULL) return -1;
    pomBT2=SearchBT(pomFNvzor, pomBT->ident);
    if(pomBT2==NULL) return -1;
    if(pomBT->key!=pomBT2->key) return -1;
  }
  return 0;
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
  return 0;

}

//--------------------konstanty---------------------

//inicializace tabulky  konstant
int constTableInit(constTable *newCTable){
  if((((*newCTable)=malloc(sizeof (struct cTable)))==NULL)){
    return -1;
  }
  (*newCTable)->First=NULL;
  if((((*newCTable)->BUPtr=malloc(sizeof (struct BlockUnit)*10))==NULL)){
    return -1;
  }
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
    if(((newBTree=malloc(sizeof (struct BT)))==NULL)){
      return NULL;
    }
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
    if(((newBTree=malloc(sizeof (struct BT)))==NULL)){
      return NULL;
    }
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
    if(((*newCTable)->BUPtr=realloc((*newCTable)->BUPtr, sizeof(struct BlockUnit)*((*newCTable)->BUSize + 10)))){
      return NULL;
    }
    (*newCTable)->BUAlloc=(*newCTable)->BUSize + 10;
  }
  (*newCTable)->BUPtr[(*newCTable)->BUSize].ident=NULL;
  (*newCTable)->BUPtr[(*newCTable)->BUSize].type=type;
  (*newCTable)->BUPtr[(*newCTable)->BUSize].data=data;
  (*newCTable)->BUPtr[(*newCTable)->BUSize].init=1;
  (*newCTable)->BUSize++;
  return newBTree;
}


union Dat *getDat(constTable *newCTable,BPtr *BP,BTree *BTpom){
  union Dat *d;
  if(isConstant((*BTpom)->type)){
    d=(*newCTable)->BUPtr[(*BTpom)->key].data;
    return d;
  }
  else if(isVar((*BTpom)->type)){
    d=(*BP)->BUPtr[(*BTpom)->key].data;
    return d;
  }
  return NULL;
}

union Dat *getDatByKey(BPtr *BP,int key){
  union Dat *d;
  
    d=(*BP)->BUPtr[key].data;
    return d;
  
}

int getTypeByKey(BPtr *BP,int key){

  
    return (*BP)->BUPtr[key].type;
    
  
}

int getType(BTree *BTpom){
  return (*BTpom)->type;
}





//Vytvori novou konstantu a prida ji do tabulky konstant
/*
@param1 odkaz na tabulku konstant
@param2 typ konstanty
@param3 konstanta ulozena jako retezec
return vraci odkaz na polozku v tabulce konstant
*/
/*
SYN: zavolas pokud narazis na nejakou konstantu
*/
BTree createConst(constTable *newCTable,int type,string *str){
  union Dat *d;// vytvoreni konstant
  BTree newBT=NULL;
  if(type==KINTEGER){
    d=createDat(CINTEGER);
    updateDat(d,CINTEGER,convStrToInt(str),0,NULL);
    newBT=constTableAdd(newCTable,CINTEGER,d);
  }
  else if(type==KDOUBLE){
    d=createDat(CDOUBLE);
    updateDat(d,CDOUBLE,0,convStrToDouble(str),NULL);
    newBT=constTableAdd(newCTable,CDOUBLE,d);
  }
  else if(type==KSTRING){
    d=createDat(CSTR);
    updateDat(d,CSTR,0,0,str);
    newBT=constTableAdd(newCTable,CSTR,d);
  }
  return newBT;

}
int convStrToInt(string *s){
  return atoi(s->str);
}
double convStrToDouble(string *s){
  return atof(s->str);
}


//----------------------------------------
//inicializace datove polozky
/*
@param1 typ novych dat
return odkaz na nova data
*/
union Dat *createDat(int type){
  union Dat *pomDat;
  if((type==ISTR)||(type==CSTR)){
    if(((pomDat=malloc(sizeof (union Dat)))==NULL)){
      return NULL;
    }
    string *s;
    if(((s=malloc(sizeof (string))))==NULL){
      return NULL;
    }
    strInit(s);
    pomDat->str=s;

  }
  else if((type==IINTEGER)||(type==CINTEGER)){
    int *pomI;
    if(((pomI=malloc(sizeof (int)))==NULL)){
      return NULL;
    }
    if(((pomDat=malloc(sizeof (union Dat)))==NULL)){
      return NULL;
    }
    pomDat->i=pomI;
  }
  else if((type==IDOUBLE)||(type==CDOUBLE)){
    double *pomD;
    if(((pomD=malloc(sizeof (double)))==NULL)){
      return NULL;
    }
    if(((pomDat=malloc(sizeof (union Dat)))==NULL)){
      return NULL;
    }
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
  if((type==ISTR)||(type==CSTR)){
    strFree(pomDat->str);
    pomDat->str=s;
    if(((s=malloc(sizeof (string))))==NULL){
      return -1;
    }
    strInit(s);
  }
  else if((type==IINTEGER)||(type==CINTEGER)){
    *(pomDat->i)=i;
  }
  else if((type==IDOUBLE)||(type==CDOUBLE)){
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
  if((type==ISTR)||(type==CSTR)){
    strFree(pomDat->str);
  }
  else if((type==IINTEGER)||(type==CINTEGER)){
    free(pomDat->i);
  }
  else if((type==IDOUBLE)||(type==CDOUBLE)){
    free(pomDat->f);
  }
  else return -1;
  free(pomDat);
  return 0;
}

//zmeni typ promenne
/*
@param1 odkaz na promennou
@param2 novy typ promenne
return vraci ne/uspesnost operace
*/
int setType(BTree newBT,int newtype){
  newBT->type=newtype;
  return 0;
}

int isConstOrVar(int type){
  if((isConstant(type))||(isVar(type))) return 1;
  else return 0;
}
int isInteger(int type){
  if((type==IINTEGER)||(type==CINTEGER)) return 1;
  return 0;
}
int isString(int type){
  if((type==ISTR)||(type==CSTR)) return 1;
  return 0;
}
int isDouble(int type){
  if((type==IDOUBLE)||(type==CDOUBLE)) return 1;
  return 0;
}

int isConstant(int type){
  if((type==CINTEGER)||(type==CDOUBLE)||(type==CSTR)) return 1;
  return 0;
}
int isVar(int type){
  if((type==IINTEGER)||(type==IDOUBLE)||(type==ISTR)) return 1;
  return 0;
}


void setFunDefined(FN FNpom){
  FNpom->def=1;
}
