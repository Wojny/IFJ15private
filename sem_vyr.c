#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "syn_expression.c"

tList *L;
void setL(tList *List)
{
	L=List;
}

string str;
void initNullStr(){
  strInit(&str);
  strAddChar(&str,'0');
}
constTable newCTable;
void setNCT(constTable nCTable){
  newCTable=nCTable;
}
int sem_sym(sTree ST){
int pom2;
int pom3;
int type1;
if(ST->stype==DIVIDE)
{
pom2=sem_sym(ST->LSynTree);
pom3=sem_sym(ST->RSynTree);
union Dat *d;
d=getDat(&newCTable,NULL,&ST->RSynTree->node);
if (isString(pom2) || isString(pom3))
    printf("Sematicka chyba");
if (d->i==0)
    printf("Sematicka chyba");
else if (isDouble(pom2) || isDouble(pom3))
{
type1=CDOUBLE;
ST->node=createConst(&newCTable,type1,&str);
CreateInst(I_DIV,(void *)ST->node,(void *)ST->LSynTree->node,(void *)ST->RSynTree->node,L);
return (ST->node->type);
}
else
{
type1=CINTEGER;
ST->node=createConst(&newCTable,type1,&str);
CreateInst(I_DIV,(void *)ST->node,(void *)ST->LSynTree->node,(void *)ST->RSynTree->node,L);
return (ST->node->type);
}
}
if(ST->stype==PLUS)//Kontrolujeme
{
pom2=sem_sym(ST->LSynTree);
pom3=sem_sym(ST->RSynTree);
if (isString(pom2) || isString(pom3))
    printf("Sematicka chyba");
else if (isDouble(pom2) || isDouble(pom3))
{
type1=CDOUBLE;
ST->node=createConst(&newCTable,type1,&str);
CreateInst(I_ADD,(void *)ST->node,(void *)ST->LSynTree->node,(void *)ST->RSynTree->node,L);
return (ST->node->type);
}
else
{
type1=CINTEGER;
ST->node=createConst(&newCTable,type1,&str);
CreateInst(I_ADD(void *)ST->node,(void *)ST->LSynTree->node,(void *)ST->RSynTree->node,L);
return (ST->node->type);
}
}
if(ST->stype==MINUS)
{
pom2=sem_sym(ST->LSynTree);
pom3=sem_sym(ST->RSynTree);
if (isString(pom2) || isString(pom3))
    printf("Sematicka chyba");
else if (isDouble(pom2) || isDouble(pom3))
{
type1=CDOUBLE;
ST->node=createConst(&newCTable,type1,&str);
CreateInst(I_SUB,(void *)ST->node,(void *)ST->LSynTree->node,(void *)ST->RSynTree->node,L);
return (ST->node->type);
}
else
{
type1=CINTEGER;
ST->node=createConst(&newCTable,type1,&str);
reateInst(I_SUB,(void *)ST->node,(void *)ST->LSynTree->node,(void *)ST->RSynTree->node,L);
return (ST->node->type);
}
}
if(ST->stype==TIMES)
{
pom2=sem_sym(ST->LSynTree);
pom3=sem_sym(ST->RSynTree);
if (isString(pom2) || isString(pom3))
    printf("Sematicka chyba");
else if (isDouble(pom2) || isDouble(pom3))
{
type1=CDOUBLE;
ST->node=createConst(&newCTable,type1,&str);
CreateInst(I_MUL,(void *)ST->node,(void *)ST->LSynTree->node,(void *)ST->RSynTree->node,L);
return (ST->node->type);
}
else
{
type1=CINTEGER;
ST->node=createConst(&newCTable,type1,&str);
CreateInst(I_MUL,(void *)ST->node,(void *)ST->LSynTree->node,(void *)ST->RSynTree->node,L);
return (ST->node->type);
}
}
if(ST->stype==EQ)
{
type1=CINTEGER;
pom2=sem_sym(ST->LSynTree);
pom3=sem_sym(ST->RSynTree);
if (isString(pom2) && (isInteger(pom3) || isDouble(pom3)))
    printf("Sematicka chyba");
if (isString(pom3) && (isInteger(pom2) || isDouble(pom2)))
    printf("Sematicka chyba");
else
{
ST->node=createConst(&newCTable,type1,&str);
CreateInst(I_EQUAL,(void *)ST->node,(void *)ST->LSynTree->node,(void *)ST->RSynTree->node,L);
return (ST->node->type);
}
}
if(ST->stype==LEQ)
{
type1=CINTEGER;
pom2=sem_sym(ST->LSynTree);
pom3=sem_sym(ST->RSynTree);
if (isString(pom2) && (isInteger(pom3) || isDouble(pom3)))
    printf("Sematicka chyba");
if (isString(pom3) && (isInteger(pom2) || isDouble(pom2)))
    printf("Sematicka chyba");
else
{
ST->node=createConst(&newCTable,type1,&str);
CreateInst(I_LESSEQ,(void *)ST->node,(void *)ST->LSynTree->node,(void *)ST->RSynTree->node,L);
return (ST->node->type);
}
}
if(ST->stype==LE)
{
type1=CINTEGER;
pom2=sem_sym(ST->LSynTree);
pom3=sem_sym(ST->RSynTree);
if (isString(pom2) && (isInteger(pom3) || isDouble(pom3)))
    printf("Sematicka chyba");
if (isString(pom3) && (isInteger(pom2) || isDouble(pom2)))
    printf("Sematicka chyba");
else
{
ST->node=createConst(&newCTable,type1,&str);
CreateInst(I_LESS,ST->node,(void *)ST->node,(void *)ST->LSynTree->node,(void *)ST->RSynTree->node,L);
return (ST->node->type);
}
}
if(ST->stype==GEQ)
{
type1=CINTEGER;
pom2=sem_sym(ST->LSynTree);
pom3=sem_sym(ST->RSynTree);
if (isString(pom2) && (isInteger(pom3) || isDouble(pom3)))
    printf("Sematicka chyba");
if (isString(pom3) && (isInteger(pom2) || isDouble(pom2)))
    printf("Sematicka chyba");
else
{
ST->node=createConst(&newCTable,type1,&str);
CreateInst(I_MOREEQ,(void *)ST->node,(void *)ST->LSynTree->node,(void *)ST->RSynTree->node,L);
return (ST->node->type);
}
}
if(ST->stype==GR)
{
type1=CINTEGER;
pom2=sem_sym(ST->LSynTree);
pom3=sem_sym(ST->RSynTree);
if (isString(pom2) && (isInteger(pom3) || isDouble(pom3)))
    printf("Sematicka chyba");
if (isString(pom3) && (isInteger(pom2) || isDouble(pom2)))
    printf("Sematicka chyba");
else
{
ST->node=createConst(&newCTable,type1,&str);
CreateInst(I_MORE,(void *)ST->node,(void *)ST->LSynTree->node,(void *)ST->RSynTree->node,L);
return (ST->node->type);
}
}
if(ST->stype==NEQ)
{
type1=CINTEGER;
pom2=sem_sym(ST->LSynTree);
pom3=sem_sym(ST->RSynTree);
if (isString(pom2) && (isInteger(pom3) || isDouble(pom3)))
    printf("Sematicka chyba");
if (isString(pom3) && (isInteger(pom2) || isDouble(pom2)))
    printf("Sematicka chyba");
else
{
ST->node=createConst(&newCTable,type1,&str);
CreateInst(I_NOTEQ,(void *)ST->node,(void *)ST->LSynTree->node,(void *)ST->RSynTree->node,L);
return (ST->node->type);
}
}
}

//je to int isVarDefined(BTree newBT); vraci 0 pokud nebyla definovana jinak 1
int CheckRedefProm(int int1,int int2)
{
if (int1==int2)
{
printf("Sématická chyba");
return false;
}
else return true;
}


int CheckParam(int poz,FN FNODE,int type)
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

int CheckVarDefined(BTree newBT)
{
	if (isVarDefined(newBT)==1)
	return true;
	else
	{
printf("Sématická chyba");
return false;
	}
}


