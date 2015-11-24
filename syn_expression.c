#include <stdio.h>
#include <stdlib.h>
#include "syn_expression.h"

//-------------------------------------------------------
//#include "scaner.h"
// inicializace pomocneho zasobniku
int initSTStack(sTreeStack *STST){
  if((((*STST)=malloc(sizeof (struct SynTreeStack)))==NULL)) return -1;
  (*STST)->First=NULL;
}
// odebrani a zruseni vrcholu zasobniku
int popRemoveSTStack(sTreeStack *STST){
  sTree pomSTree=(*STST)->First;
  (*STST)->First=(*STST)->First->nxt;
  free(pomSTree);
}
// odebrani vrcholu zasobniku
int popSTStack(sTreeStack *STST){
  (*STST)->First=(*STST)->First->nxt;
}
// vlozeni na vrchol zasobniku
int pushSTStack(sTreeStack *STST,sTree newSTree){
  if(newSTree->stype==STLESSSEP){
    sTree STpom=(*STST)->First;
    if(STpom->isE==1){
      newSTree->nxt=(*STST)->First->nxt;
      (*STST)->First->nxt=newSTree;
    }
    else{
      newSTree->nxt=(*STST)->First;
      (*STST)->First=newSTree;
    }
  }
  else{
    newSTree->nxt=(*STST)->First;
    (*STST)->First=newSTree;
  }
}
// vytvoreni uzlu syntaktickeho stromu
sTree createSTree(BTree newBT,int isE,int stype){
  sTree newSTree;
  if(((newSTree=malloc(sizeof (struct SynTree)))==NULL)) return NULL;
  newSTree->node=newBT;
  newSTree->isE=isE;
  newSTree->stype=stype;
  newSTree->nxt=NULL;
  newSTree->LSynTree=NULL;
  newSTree->RSynTree=NULL;
  return newSTree;
}
// ziskani typ uzlu synt. stromu
/*
@param1 uzel synt. stromu
return typ uzlu
*/
int getSTType(sTree newsTree){
  return newsTree->node->type;
}
// ziskani indexu do tabulky konstant z uzlu synt. stromu
/*
@param1 uzel synt. stromu
return vraci klic do tabulky konstant
*/
int getKey(sTree newsTree){
  return newsTree->node->key;
}
/*
@param1 tabulka konstant
@param2 klic(index) do tabulky konstant
return vraci odkaz na data z tabulky konstant
*/
union Dat *getData(constTable newCTable,int key){
  return newCTable->BUPtr[key].data;
}

//hlavni funkce syntakticke analyzy vyrazu
/*
@param1 typ prvniho lexemu
@param2 obsah prvniho lexemu
@param3 odkaz na aktualne provadenou funkci z globalni tabulky symbolu
@param4 odkaz na tabulku konstant
return vraci koren syntaktickeho stromu
*/
sTree syn_exp(int inittype,string *inits,FN newFN,constTable newCTable){
  PTableInit (PTable);
  sTreeStack STST;
  initSTStack(&STST);
  pushSTStack(&STST,createSTree(NULL,0,SEMICOLON));
  sTree act;

  if(inittype==ID){
    BTree newBT2=SearchBT(newFN,inits);
    act=createSTree(newBT2,0,newBT2->type);
  }
  else if(isConstant(inittype)){
    BTree newBT2=createConst(&newCTable,inittype,inits);
    act=createSTree(newBT2,0,inittype);
  }
  else{
    act=createSTree(NULL,0,inittype);
  }

  int c;
  while(1){
    c=comp(STST,act);
    if(c==STLESS){//<
      pushSTStack(&STST,createSTree(NULL,0,STLESSSEP));
      pushSTStack(&STST,act);
      string str;
      strInit(&str);
      int type=getNextToken(&str);
      if(type==ID){
        BTree newBT2=SearchBT(newFN,&str);
        act=createSTree(newBT2,0,newBT2->type);

      }
      else if(isConstant(type)){
        BTree newBT2=createConst(&newCTable,type,&str);
        act=createSTree(newBT2,0,type);
      }
      else{
        act=createSTree(NULL,0,type);
      }

    }
    else if(c==STEQ){ //=
      pushSTStack(&STST,act);
      string str;
      strInit(&str);
      int type=getNextToken(&str);
      if(type==ID){
        BTree newBT2=SearchBT(newFN,&str);
        act=createSTree(newBT2,0,newBT2->type);

      }
      else if(isConstant(type)){
        BTree newBT2=createConst(&newCTable,type,&str);
        act=createSTree(newBT2,0,type);
      }
      else{
        act=createSTree(NULL,0,type);
      }
    }
    else if(c==STGREAT){//>
      int rule=getRuleNum(STST);
      int isOK=execRule(&STST,rule);
    }
    else if(c==STEND){
      return STST->First;
    }
    else {
        return NULL;
    }
  }
}
// ziskani hodnoty z precedencni tabulky
int comp(sTreeStack STST,sTree ST0){
  sTree STpom=STST->First;
  int i;
  while(STpom->isE!=0) STpom=STpom->nxt;
  if((STpom->stype==SEMICOLON)&&(ST0->stype==SEMICOLON)) return STEND;
  i=PTable[convConstTypes(STpom->stype)][convConstTypes(ST0->stype)];
  return i;
}
// konverze konstant
int convConstTypes(int type){
  switch(type){
    case EQ: return EQUAL;
             break;
    case NEQ: return NOTEQUAL;
              break;
    case LEQ: return LESSEQUAL;
              break;
    case GEQ: return MOREEQUAL;
              break;
    case LE: return LESS;
             break;
    case GR: return MORE;
             break;
    case PLUS: return ADD;
               break;
    case MINUS: return SUB;
                break;
    case TIMES: return MUL;
                break;
    case DIVIDE: return DIV;
                 break;
    case SEMICOLON: return TERMINAL;
                   break;
    case LPARENTH: return LBRACE;
                   break;
    case RPARENTH: return RBRACE;
                   break;
  }
  if(isConstOrVar(type)) return IDF;
  else return -1;
}
// jake pravidlo se ma vykonat
int getRuleNum(sTreeStack STST){
  sTree ST1;
  if(STST->First!=NULL) ST1=STST->First;
  else return -1;
  sTree ST2;
  sTree ST3;
  sTree ST4;
  if(ST1->isE==0){
    if(ST1->stype==RPARENTH){
      if(ST1->nxt!=NULL) ST2=ST1->nxt; else return -1;
      if(ST2->nxt!=NULL) ST3=ST2->nxt; else return -1;
      if(ST3->nxt!=NULL) ST4=ST3->nxt; else return -1;
      if((ST2->isE==1)&&(ST3->stype==LPARENTH)&&(ST4->stype==STLESSSEP)) return 11;
      else return -1;
    }
    else if(isConstOrVar(ST1->stype)){
      if(ST1->nxt!=NULL) ST2=ST1->nxt;
      else return -1;
      if(ST2->stype==STLESSSEP) return 10;

    }
  }
  else if(STST->First->isE==1){

    if(ST1->nxt!=NULL) ST2=ST1->nxt; else return -1;
    if(ST2->nxt!=NULL) ST3=ST2->nxt; else return -1;
    if(ST3->nxt!=NULL) ST4=ST3->nxt; else return -1;
    if((ST3->isE==1)&&(ST4->stype==STLESSSEP)){
      switch(ST2->stype){
        case EQ: return 0;
                 break;
        case NEQ: return 1;
                  break;
        case LEQ: return 2;
                  break;
        case GEQ: return 3;
                  break;
        case LE: return 4;
                 break;
        case GR: return 5;
                 break;
        case PLUS: return 6;
                   break;
        case MINUS: return 7;
                    break;
        case TIMES: return 8;
                    break;
        case DIVIDE: return 9;
                     break;
      }
    }
  }
  return -1;
}

// provedeni pravidla
int execRule(sTreeStack *STST,int rule){
  sTree STreeL;
  sTree STreeR;
  sTree STreeOP;
  switch(rule){
    case 10:(*STST)->First->isE=1;
           sTree pom=(*STST)->First->nxt;
           (*STST)->First->nxt=(*STST)->First->nxt->nxt;
           free(pom);
           break;
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
            STreeL=(*STST)->First->nxt->nxt;
            STreeR=(*STST)->First;
            STreeOP=(*STST)->First->nxt;
            popSTStack(STST);
            popSTStack(STST);
            popSTStack(STST);
            popRemoveSTStack(STST);
            STreeOP->RSynTree=STreeR;
            STreeOP->LSynTree=STreeL;
            pushSTStack(STST,STreeOP);
            (*STST)->First->isE=1;
            break;
    case 11:popRemoveSTStack(STST);
            sTree STE=(*STST)->First;
            popSTStack(STST);
            popRemoveSTStack(STST);
            popRemoveSTStack(STST);
            pushSTStack(STST,STE);
            break;
  }
  return 0;
}
