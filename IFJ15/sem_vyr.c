#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "syn_expression.c"
//#include "interpret.c"
tList *L;
/*void setL(tList *List){
  L=List;
}*/
string *Str0;
void initstr0(string **str)
{
    if(((*str=malloc(sizeof(string)))==NULL)) return ;
	strInit(*str);
	strAddChar(*str,'0');
}
/*string *str;
void initNullStr(){

  strInit(str);
  strAddChar(str,'0');
}*/
constTable newCTable;
void setNCT(constTable nCTable){
  newCTable=nCTable;
}
int sem_sym(sTree ST){
int pom2;
int pom3;
int type1;
if(ST==NULL) return -1;
if(ST->stype==DIVIDE)//Kontrolujeme zda se jedna o deleni
{
pom2=sem_sym(ST->LSynTree);//leva vetev
pom3=sem_sym(ST->RSynTree);//Prava vetev

if (isString(pom2) || isString(pom3)){//nesmi byt retezec
    AddERR(line,IFJ_ERR_SEM_ARIT);
    return -1;
}
/*if(){
  union Dat *d;
  d=getDat(&newCTable,NULL,&ST->RSynTree->node);
  if (d->i==0){//nesmime delit nulou
    AddERR(line,IFJ_ERR_SEM_ARIT);
    return -1;
  }
}
else*/
if (isDouble(pom2) || isDouble(pom3))//kontrola zda se jedna o double
{
type1=CDOUBLE;//Sem se da vysledek v double
ST->node=createConst(&newCTable,type1,Str0);
CreateInst(I_DIV,ST->node,ST->LSynTree->node,ST->RSynTree->node,L);//tvorba prostoru pro vysledek
return (ST->node->type);
}
else
{
type1=CINTEGER;// vysledek v integer
ST->node=createConst(&newCTable,type1,Str0);
CreateInst(I_DIV,ST->node,ST->LSynTree->node,ST->RSynTree->node,L);
return (ST->node->type);
}
}
if(ST->stype==PLUS)//Kontrolujeme scitani
{
pom2=sem_sym(ST->LSynTree);
pom3=sem_sym(ST->RSynTree);
if (isString(pom2) || isString(pom3)){
    AddERR(line,IFJ_ERR_SEM_ARIT);
    return -1;
}
else if (isDouble(pom2) || isDouble(pom3))
{
type1=CDOUBLE;
ST->node=createConst(&newCTable,type1,Str0);
CreateInst(I_ADD,ST->node,ST->LSynTree->node,ST->RSynTree->node,L);
return (ST->node->type);
}
else
{
type1=CINTEGER;
ST->node=createConst(&newCTable,type1,Str0);
CreateInst(I_ADD,ST->node,ST->LSynTree->node,ST->RSynTree->node,L);

return (ST->node->type);
}
}
if(ST->stype==MINUS)
{
pom2=sem_sym(ST->LSynTree);
pom3=sem_sym(ST->RSynTree);
if (isString(pom2) || isString(pom3)){
    AddERR(line,IFJ_ERR_SEM_ARIT);
    return -1;
}
else if (isDouble(pom2) || isDouble(pom3))
{
type1=CDOUBLE;
ST->node=createConst(&newCTable,type1,Str0);
CreateInst(I_SUB,ST->node,ST->LSynTree->node,ST->RSynTree->node,L);
return (ST->node->type);
}
else
{
type1=CINTEGER;
ST->node=createConst(&newCTable,type1,Str0);
CreateInst(I_SUB,ST->node,ST->LSynTree->node,ST->RSynTree->node,L);
return (ST->node->type);
}
}
if(ST->stype==TIMES)//Kontrola nasobeni
{
pom2=sem_sym(ST->LSynTree);
pom3=sem_sym(ST->RSynTree);
if (isString(pom2) || isString(pom3)){
    AddERR(line,IFJ_ERR_SEM_ARIT);
    return -1;
}
else if (isDouble(pom2) || isDouble(pom3))
{
type1=CDOUBLE;
ST->node=createConst(&newCTable,type1,Str0);
CreateInst(I_MUL,ST->node,ST->LSynTree->node,ST->RSynTree->node,L);
return (ST->node->type);
}
else
{
type1=CINTEGER;
ST->node=createConst(&newCTable,type1,Str0);
CreateInst(I_MUL,ST->node,ST->LSynTree->node,ST->RSynTree->node,L);
return (ST->node->type);
}
}
if(ST->stype==EQ)//Kontrola logické operace - je rovno
{
type1=CINTEGER;
pom2=sem_sym(ST->LSynTree);
pom3=sem_sym(ST->RSynTree);
if (isString(pom2) && (isInteger(pom3) || isDouble(pom3))){
    AddERR(line,IFJ_ERR_SEM_ARIT);
    return -1;
}
if (isString(pom3) && (isInteger(pom2) || isDouble(pom2))){
    AddERR(line,IFJ_ERR_SEM_ARIT);
    return -1;
}
else
{
ST->node=createConst(&newCTable,type1,Str0);
CreateInst(I_EQUAL,ST->node,ST->LSynTree->node,ST->RSynTree->node,L);
return (ST->node->type);
}
}
if(ST->stype==LEQ)//Kontrola logické operace - je menší, nebo rovno
{
type1=CINTEGER;
pom2=sem_sym(ST->LSynTree);
pom3=sem_sym(ST->RSynTree);
if (isString(pom2) && (isInteger(pom3) || isDouble(pom3))){
    AddERR(line,IFJ_ERR_SEM_ARIT);
    return -1;
}
if (isString(pom3) && (isInteger(pom2) || isDouble(pom2))){
    AddERR(line,IFJ_ERR_SEM_ARIT);
    return -1;
}
else
{
ST->node=createConst(&newCTable,type1,Str0);
CreateInst(I_LESSEQ,ST->node,ST->LSynTree->node,ST->RSynTree->node,L);
return (ST->node->type);
}
}
if(ST->stype==LE)//Kontrola logické operace - je menší
{
type1=CINTEGER;
pom2=sem_sym(ST->LSynTree);
pom3=sem_sym(ST->RSynTree);
if (isString(pom2) && (isInteger(pom3) || isDouble(pom3))){
    AddERR(line,IFJ_ERR_SEM_ARIT);
    return -1;
}
if (isString(pom3) && (isInteger(pom2) || isDouble(pom2))){
    AddERR(line,IFJ_ERR_SEM_ARIT);
    return -1;
}
else
{
ST->node=createConst(&newCTable,type1,Str0);
CreateInst(I_LESS,ST->node,ST->LSynTree->node,ST->RSynTree->node,L);
return (ST->node->type);
}
}
if(ST->stype==GEQ)//Kontrola logické operace - je vìtší, èi rovno
{
type1=CINTEGER;
pom2=sem_sym(ST->LSynTree);
pom3=sem_sym(ST->RSynTree);
if (isString(pom2) && (isInteger(pom3) || isDouble(pom3))){
    AddERR(line,IFJ_ERR_SEM_ARIT);
    return -1;
}
if (isString(pom3) && (isInteger(pom2) || isDouble(pom2))){
    AddERR(line,IFJ_ERR_SEM_ARIT);
    return -1;
}
else
{
ST->node=createConst(&newCTable,type1,Str0);
CreateInst(I_MOREEQ,ST->node,ST->LSynTree->node,ST->RSynTree->node,L);
return (ST->node->type);
}
}
if(ST->stype==GR)//Kontrola logické operace - je vìtší
{
type1=CINTEGER;
pom2=sem_sym(ST->LSynTree);
pom3=sem_sym(ST->RSynTree);
if (isString(pom2) && (isInteger(pom3) || isDouble(pom3))){
    AddERR(line,IFJ_ERR_SEM_ARIT);
    return -1;
}
if (isString(pom3) && (isInteger(pom2) || isDouble(pom2))){
    AddERR(line,IFJ_ERR_SEM_ARIT);
    return -1;
}
else
{
ST->node=createConst(&newCTable,type1,Str0);
CreateInst(I_MORE,ST->node,ST->LSynTree->node,ST->RSynTree->node,L);
return (ST->node->type);
}
}
if(ST->stype==NEQ)//Kontrola logické operace - není rovno
{
type1=CINTEGER;
pom2=sem_sym(ST->LSynTree);
pom3=sem_sym(ST->RSynTree);
if (isString(pom2) && (isInteger(pom3) || isDouble(pom3))){
    AddERR(line,IFJ_ERR_SEM_ARIT);
    return -1;
}
if (isString(pom3) && (isInteger(pom2) || isDouble(pom2))){
    AddERR(line,IFJ_ERR_SEM_ARIT);
    return -1;
}
else
{
ST->node=createConst(&newCTable,type1,Str0);
CreateInst(I_NOTEQ,ST->node,ST->LSynTree->node,ST->RSynTree->node,L);
return (ST->node->type);
}
}
else if(isConstOrVar(ST->stype)){//konstanta nebo promìnná
  CreateInst(I_ASSIGN,ST->node,ST->node,NULL,L);
  return ST->stype;
}
return -1;
}

//je to int isVarDefined(BTree newBT); vraci 0 pokud nebyla definovana jinak 1
/*int CheckRedefProm(int int1,int int2)
{
if (int1==int2)
{
AddERR(line,IFJ_ERR_SEMANTIC);
return false;
}
else return true;
}*/


int CheckParam(int poz,FN FNODE,int type)//kontrola parametrù
{
	int c;

if (strGetLength(FNODE->type)<=poz)
{
printf("Sématická chyba");
return false;
}
else
{
c=FNODE->type->str[poz];
if ((c='i' && (isInteger(type))) || (c='i' && (isDouble(type))) || (c='f' && (isInteger(type))) || (c='f' && (isDouble(type))) || (c='s' && (isString(type))))
return true;
else
{
printf("Sématická chyba");
return false;
}
}
}

int CheckVarDefined(BTree newBT)//Kontrola zda je definováno
{
	if (isVarDefined(newBT)==1)
	return true;
	else
	{
printf("Sématická chyba");
return false;
	}
}
bool isCompatible(int param1,int param2){//
  if((param1==param2)||((param1==CSTR)&&(param2==ISTR))||((param1==ISTR)&&(param2==CSTR))) return true;
  else if((param1==param2)||((param1==CINTEGER)&&(param2==IINTEGER))||((param1==IINTEGER)&&(param2==CINTEGER))) return true;
  else if((param1==param2)||((param1==CDOUBLE)&&(param2==IDOUBLE))||((param1==IDOUBLE)&&(param2==CDOUBLE))) return true;
  else if(((isDouble(param1))&&(isInteger(param2)))||((isInteger(param1))&&(isDouble(param2)))) return true;
  else return false;

}

//isMultipleDefinedFun
//checkFunParams
//isParamEqual


