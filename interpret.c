#include <stdio.h>
#include <stdlib.h>
//#include <string.h>
#include <stdbool.h>
#include <limits.h>
//#include "interpret.h"
//#include "ilist.h"
//#include "sym_table.h"
#include "sym_table.c"
#include "scaner.h"
#include "errors.h"
//#include "str.h"
#include "ial.c"

void concat(char * s1,char * s2,char * vysledok);

int interpret(/*tList *L, GSTable *G, constTable *CT*/)
{
    //*G->FunRoot + string funkce,

  string *s;
  if(((s=malloc(sizeof (string)))==NULL)) return -1;
  string *t;
  if(((t=malloc(sizeof (string)))==NULL)) return -1;
  string *u;
  if(((u=malloc(sizeof (string)))==NULL)) return -1;
  string *v;
  if(((v=malloc(sizeof (string)))==NULL)) return -1;
  strInit(s);
  strInit(t);
  strInit(u);
  strInit(v);
  strAddChar(s,'b');
  strAddChar(t,'a');
  strAddChar(u,'d');
  strAddChar(v,'1');
  strAddChar(v,'0');
  strAddChar(v,'0');
  strAddChar(v,'0');
  strAddChar(v,'0');
  strAddChar(v,'0');

  tList *L;
  if(((L=malloc(sizeof (tList)))==NULL)) return -1;
  listInit(L);

  GSTable G;
  GSTinit(&G);

  FN newFN=GSTadd(&G,s,KINTEGER);
  BTAddID(&newFN,t, KINTEGER,0,0);
  BTAddID(&newFN,u, KINTEGER,1,1);
  BTree newBT=SearchBT(newFN, u);
  BTree newBT1=SearchBT(newFN, t);
  BTDelete(&newFN->BTroot,1,&newFN->tempSTable);

  BTDelete(&newFN->BTroot,0,&newFN->tempSTable);
  BlockStack BlStack;
  BlockStackInit(&BlStack);
  BlockStackAdd(&BlStack,&G,s);
  constTable CT;
  constTableInit(&CT);
  BTree newBT2=createConst(&CT,KINTEGER,v);
  union Dat *d=getDat(&CT,&BlStack->First,&newBT);
  union Dat *d1=getDat(&CT,&BlStack->First,&newBT1);
  *d1->i=1;
  setType(newBT1,IINTEGER);
  union Dat *d2=getDat(&CT,&BlStack->First,&newBT2);
  *d->i=*d1->i+*d2->i;


//    CreateInst(I_WRITE, (void*) newBT, NULL, NULL, L);
//    CreateInst(I_DIV, (void*) newBT , (void*) newBT2, (void*) newBT1, L);
//    CreateInst(I_WRITE, (void*) newBT, NULL, NULL, L);
    CreateInst(I_FOR, NULL, NULL, NULL, L);
    //dek
    CreateInst(I_FOR_CHCK, NULL, NULL, NULL, L);
    CreateInst(I_LESS, (void*) newBT, (void*) newBT1, (void*) newBT2, L);
    CreateInst(I_FOR_DIFF, NULL, NULL, NULL, L);
    CreateInst(I_ADD, (void*) newBT1, (void*) newBT1, (void*) newBT1, L);
    CreateInst(I_FOR_COND, NULL, NULL, NULL, L);
    CreateInst(I_WRITE, (void*) newBT, NULL, NULL, L);
    CreateInst(I_WRITE, (void*) newBT1, NULL, NULL, L);

  /*  CreateInst(I_WRITE, (void*) newBT2, NULL, NULL, L);
    CreateInst(I_WRITE, (void*) newBT1, NULL, NULL, L);
    CreateInst(I_WRITE, (void*) newBT2, NULL, NULL, L);
    CreateInst(I_WRITE, (void*) newBT1, NULL, NULL, L);
    CreateInst(I_WRITE, (void*) newBT2, NULL, NULL, L);
    CreateInst(I_WRITE, (void*) newBT1, NULL, NULL, L);
    CreateInst(I_WRITE, (void*) newBT2, NULL, NULL, L);*/
    CreateInst(I_END_FOR, NULL, NULL, NULL, L);

    listFirst(L);
    tInstr *I;
    tInstr *nextI;

    tInstr *pomIn;
    void *instForChck;
    void *instForDiff;
    //FOR ZATIM BEZ ZANORENI

    //BlockStack BlStack;

    // Pomocne promenne pro pracy s daty
    union Dat *dat1, *dat2, *dat3;
    int type1, type2, type3;
    int skipIf = 0;
    int zanoreniIf = 0;
    int podminkyIf[50]; //max 49 zanoreni
    int zanoreniFor = -1;
    int podminkaFor = 0;
    int diffJump = 0;
    char inChar;

   /* string *nameF;
    if(((nameF=malloc(sizeof (string)))==NULL)) return -1;
    strAddChar(nameF,'m');
    strAddChar(nameF,'a');
    strAddChar(nameF,'i');
    strAddChar(nameF,'n');*/

    //BlockStackInit(&BlStack);
    //BlockStackAdd(&BlStack,G,nameF);

    memset(podminkyIf, -1, sizeof(podminkyIf));

    while(1)
    {
        I = listGetData(L);
        if(L->active->nextItem != NULL) nextI = listGetNextData(L);

        switch(I->Type)
        {
            /*
            add1 = add2 + add3
            */
            case I_ADD:
                dat1 = getDat(&CT,&BlStack->First,((BTree *) &I->add1));
                type1 = getType((BTree *) &I->add1);
                dat2 = getDat(&CT,&BlStack->First,((BTree *) &I->add2));
                type2 = getType((BTree *) &I->add2);
                dat3 = getDat(&CT,&BlStack->First,((BTree *) &I->add3));
                type3 = getType((BTree *) &I->add3);

                // Scitani pouze INT or DOUBLE
                if(((isInteger(type1) || isDouble(type1)) && (isInteger(type2) || isDouble(type2)))) {}
                else
                {
                    return IFJ_ERR_INTERPRET;
                }

                // Kontrola typu double a int cisel -> 1-2 double ma double vysledek, oba int maji int vysledek
                if(isDouble(type2))
                {
                    setType(I->add1,IDOUBLE);
                    if(isDouble(type3))
                    {
                        *dat1->f =  *dat2->f + *dat3->f;
                    }
                    else
                    {
                        *dat1->f = *dat2->f + *(double *)dat3->i;
                    }
                }
                else
                {
                    if(isDouble(type3))
                    {
                        setType(I->add1,IDOUBLE);
                        *dat1->f = *(double *)dat2->i + *dat3->f;
                    }
                    else
                    {
                        setType(I->add1,IINTEGER);
                        *dat1->i = *dat2->i + *dat3->i;
                    }
                }
                break;

            /*
            add1 = add2 - add3
            */
            case I_SUB:
                dat1 = getDat(&CT,&BlStack->First,((BTree *) &I->add1));
                type1 = getType((BTree *) &I->add1);
                dat2 = getDat(&CT,&BlStack->First,((BTree *) &I->add2));
                type2 = getType((BTree *) &I->add2);
                dat3 = getDat(&CT,&BlStack->First,((BTree *) &I->add3));
                type3 = getType((BTree *) &I->add3);

                // Odcitani pouze INT or DOUBLE
                if(((isInteger(type1) || isDouble(type1)) && (isInteger(type2) || isDouble(type2)))) {}
                else return IFJ_ERR_INTERPRET;

                // Kontrola typu double a int cisel -> 1-2 double ma double vysledek, oba int maji int vysledek
                if(isDouble(type2))
                {
                    setType(I->add1,IDOUBLE);
                    if(isDouble(type3))
                    {
                        *dat1->f = *dat2->f - *dat3->f;
                    }
                    else
                    {
                        *dat1->f = *dat2->f - *(double *)dat3->i;
                    }
                }
                else
                {
                    if(isDouble(type3))
                    {
                        setType(I->add1,IDOUBLE);
                        *dat1->f = *(double *)dat2->i - *dat3->f;
                    }
                    else
                    {
                        setType(I->add1,IINTEGER);
                        *dat1->i = *dat2->i - *dat3->i;
                    }
                }
                break;

            /*
            add1 = add2 * add3
            */
            case I_MUL:
                dat1 = getDat(&CT,&BlStack->First,((BTree *) &I->add1));
                type1 = getType((BTree *) &I->add1);
                dat2 = getDat(&CT,&BlStack->First,((BTree *) &I->add2));
                type2 = getType((BTree *) &I->add2);
                dat3 = getDat(&CT,&BlStack->First,((BTree *) &I->add3));
                type3 = getType((BTree *) &I->add3);

                // Nasobeni pouze INT or DOUBLE
                if(((isInteger(type1) || isDouble(type1)) && (isInteger(type2) || isDouble(type2)))) {}
                else return IFJ_ERR_INTERPRET;

                if(isDouble(type2))
                {
                    setType(I->add1,IDOUBLE);
                    if(isDouble(type3))
                    {
                        *dat1->f = *dat2->f * *dat3->f;
                    }
                    else
                    {
                        *dat1->f = *dat2->f * *(double *)dat3->i;
                    }
                }
                else
                {
                    if(isDouble(type3))
                    {
                        setType(I->add1,IDOUBLE);
                        *dat1->f = *(double *)dat2->i * *dat3->f;
                    }
                    else
                    {
                        setType(I->add1,IINTEGER);
                        *dat1->i = *dat2->i * *dat3->i;
                    }
                }
                break;

            /*
            add1 = add2 / add3
            */
            case I_DIV:
                dat1 = getDat(&CT,&BlStack->First,((BTree *) &I->add1));
                type1 = getType((BTree *) &I->add1);
                dat2 = getDat(&CT,&BlStack->First,((BTree *) &I->add2));
                type2 = getType((BTree *) &I->add2);
                dat3 = getDat(&CT,&BlStack->First,((BTree *) &I->add3));
                type3 = getType((BTree *) &I->add3);

                // Nasobeni pouze INT or DOUBLE
                if(((isInteger(type1) || isDouble(type1)) && (isInteger(type2) || isDouble(type2)))) {}
                else return IFJ_ERR_INTERPRET;

                // Deleni nulou
                if((isInteger(type3) && dat3->i == 0) || (isDouble(type3) && dat3->f == 0)) return IFJ_ERR_INTERPRET;

                if(isDouble(type2))
                {
                    setType(I->add1,IDOUBLE);
                    if(isDouble(type3))
                    {
                        *dat1->f = *dat2->f / *dat3->f;
                    }
                    else
                    {
                        *dat1->f = *dat2->f / *(double *)dat3->i;
                    }
                }
                else
                {
                    if(isDouble(type3))
                    {
                        setType(I->add1,IDOUBLE);
                        *dat1->f = *(double *)dat2->i / *dat3->f;
                    }
                    else
                    {
                        setType(I->add1,IINTEGER);
                        *dat1->i = *dat2->i / *dat3->i;
                    }
                }
                break;

            /*
            add1 = 1 pokud add2 < add3, jinak add1 = 0
            */

            case I_LESS:
                dat1 = getDat(&CT,&BlStack->First,((BTree *) &I->add1));
                type1 = getType((BTree *) &I->add1);
                dat2 = getDat(&CT,&BlStack->First,((BTree *) &I->add2));
                type2 = getType((BTree *) &I->add2);
                dat3 = getDat(&CT,&BlStack->First,((BTree *) &I->add3));
                type3 = getType((BTree *) &I->add3);

                setType(I->add1,IINTEGER);

                if(isInteger(type2))
                {
                    if(isInteger(type3))
                    {
                        if(*dat2->i < *dat3->i)
                        {
                            *dat1->i = 1;
                        }
                        else *dat1->i = 0;
                    }
                    else if(isDouble(type3))
                    {
                        if(*(double *)dat2->i < *dat3->f)
                        {
                            *dat1->i = 1;
                        }
                        else *dat1->i = 0;
                    }
                    else return IFJ_ERR_INTERPRET;
                }
                else if(isDouble(type2))
                {
                    if(isInteger(type3))
                    {
                        if(*dat2->f < *(double *)dat3->i)
                        {
                            *dat1->i = 1;
                        }
                        else *dat1->i = 0;
                    }
                    else if(isDouble(type3))
                    {
                        if(*dat2->f < *dat3->f)
                        {
                            *dat1->i = 1;
                        }
                        else *dat1->i = 0;
                    }
                    else return IFJ_ERR_INTERPRET;
                }
                else if(isString(type2))
                {
                    if(isString(type3))
                    {
                        if(strcmp(dat2->str->str,dat3->str->str) < 0)
                        {
                            *dat1->i = 1;
                        }
                        else *dat1->i = 0;
                    }
                    else return IFJ_ERR_INTERPRET;
                }
                else
                {
                    return IFJ_ERR_INTERPRET;
                }
                break;

            /*
            add1 = 1 pokud add2 > add3, jinak add1 = 0
            */
            case I_MORE:
                dat1 = getDat(&CT,&BlStack->First,((BTree *) &I->add1));
                type1 = getType((BTree *) &I->add1);
                dat2 = getDat(&CT,&BlStack->First,((BTree *) &I->add2));
                type2 = getType((BTree *) &I->add2);
                dat3 = getDat(&CT,&BlStack->First,((BTree *) &I->add3));
                type3 = getType((BTree *) &I->add3);

                setType(I->add1,IINTEGER);

                if(isInteger(type2))
                {
                    if(isInteger(type3))
                    {
                        if(*dat2->i > *dat3->i)
                        {
                            *dat1->i = 1;
                        }
                        else *dat1->i = 0;
                    }
                    else if(isDouble(type3))
                    {
                        if(*(double *)dat2->i > *dat3->f)
                        {
                            *dat1->i = 1;
                        }
                        else *dat1->i = 0;
                    }
                    else return IFJ_ERR_INTERPRET;
                }
                else if(isDouble(type2))
                {
                    if(isInteger(type3))
                    {
                        if(*dat2->f > *(double *)dat3->i)
                        {
                            *dat1->i = 1;
                        }
                        else *dat1->i = 0;
                    }
                    else if(isDouble(type3))
                    {
                        if(*dat2->f > *dat3->f)
                        {
                            *dat1->i = 1;
                        }
                        else *dat1->i = 0;
                    }
                    else return IFJ_ERR_INTERPRET;
                }
                else if(isString(type2))
                {
                    if(isString(type3))
                    {
                        if(strcmp(dat2->str->str,dat3->str->str) > 0)
                        {
                            *dat1->i = 1;
                        }
                        else *dat1->i = 0;
                    }
                    else return IFJ_ERR_INTERPRET;
                }
                else
                {
                    return IFJ_ERR_INTERPRET;
                }
                break;

            /*
            add1 = 1 pokud add2 <= add3, jinak add1 = 0
            */
            case I_LESSEQ:
                dat1 = getDat(&CT,&BlStack->First,((BTree *) &I->add1));
                type1 = getType((BTree *) &I->add1);
                dat2 = getDat(&CT,&BlStack->First,((BTree *) &I->add2));
                type2 = getType((BTree *) &I->add2);
                dat3 = getDat(&CT,&BlStack->First,((BTree *) &I->add3));
                type3 = getType((BTree *) &I->add3);

                setType(I->add1,IINTEGER);

                if(isInteger(type2))
                {
                    if(isInteger(type3))
                    {
                        if(*dat2->i <= *dat3->i)
                        {
                            *dat1->i = 1;
                        }
                        else *dat1->i = 0;
                    }
                    else if(isDouble(type3))
                    {
                        if(*(double *)dat2->i <= *dat3->f)
                        {
                            *dat1->i = 1;
                        }
                        else *dat1->i = 0;
                    }
                    else return IFJ_ERR_INTERPRET;
                }
                else if(isDouble(type2))
                {
                    if(isInteger(type3))
                    {
                        if(*dat2->f <= *(double *)dat3->i)
                        {
                            *dat1->i = 1;
                        }
                        else *dat1->i = 0;
                    }
                    else if(isDouble(type3))
                    {
                        if(*dat2->f <= *dat3->f)
                        {
                            *dat1->i = 1;
                        }
                        else *dat1->i = 0;
                    }
                    else return IFJ_ERR_INTERPRET;
                }
                else if(isString(type2))
                {
                    if(isString(type3))
                    {
                        if(strcmp(dat2->str->str,dat3->str->str) <= 0)
                        {
                            *dat1->i = 1;
                        }
                        else *dat1->i = 0;
                    }
                    else return IFJ_ERR_INTERPRET;
                }
                else
                {
                    return IFJ_ERR_INTERPRET;
                }
                break;

            /*
            add1 = 1 pokud add2 >= add3, jinak add1 = 0
            */
            case I_MOREEQ:
                dat1 = getDat(&CT,&BlStack->First,((BTree *) &I->add1));
                type1 = getType((BTree *) &I->add1);
                dat2 = getDat(&CT,&BlStack->First,((BTree *) &I->add2));
                type2 = getType((BTree *) &I->add2);
                dat3 = getDat(&CT,&BlStack->First,((BTree *) &I->add3));
                type3 = getType((BTree *) &I->add3);

                setType(I->add1,IINTEGER);

                if(isInteger(type2))
                {
                    if(isInteger(type3))
                    {
                        if(*dat2->i >= *dat3->i)
                        {
                            *dat1->i = 1;
                        }
                        else *dat1->i = 0;
                    }
                    else if(isDouble(type3))
                    {
                        if(*(double *)dat2->i >= *dat3->f)
                        {
                            *dat1->i = 1;
                        }
                        else *dat1->i = 0;
                    }
                    else return IFJ_ERR_INTERPRET;
                }
                else if(isDouble(type2))
                {
                    if(isInteger(type3))
                    {
                        if(*dat2->f >= *(double *)dat3->i)
                        {
                            *dat1->i = 1;
                        }
                        else *dat1->i = 0;
                    }
                    else if(isDouble(type3))
                    {
                        if(*dat2->f >= *dat3->f)
                        {
                            *dat1->i = 1;
                        }
                        else *dat1->i = 0;
                    }
                    else return IFJ_ERR_INTERPRET;
                }
                else if(isString(type2))
                {
                    if(isString(type3))
                    {
                        if(strcmp(dat2->str->str,dat3->str->str) >= 0)
                        {
                            *dat1->i = 1;
                        }
                        else *dat1->i = 0;
                    }
                    else return IFJ_ERR_INTERPRET;
                }
                else
                {
                    return IFJ_ERR_INTERPRET;
                }
                break;

            /*
            add1 = 1 pokud add2 == add3, jinak add1 = 0
            */
            case I_EQUAL:
                dat1 = getDat(&CT,&BlStack->First,((BTree *) &I->add1));
                type1 = getType((BTree *) &I->add1);
                dat2 = getDat(&CT,&BlStack->First,((BTree *) &I->add2));
                type2 = getType((BTree *) &I->add2);
                dat3 = getDat(&CT,&BlStack->First,((BTree *) &I->add3));
                type3 = getType((BTree *) &I->add3);

                setType(I->add1,IINTEGER);

                if(isInteger(type2))
                {
                    if(isInteger(type3))
                    {
                        if(*dat2->i == *dat3->i)
                        {
                            *dat1->i = 1;
                        }
                        else *dat1->i = 0;
                    }
                    else if(isDouble(type3))
                    {
                        if(*(double *)dat2->i == *dat3->f)
                        {
                            *dat1->i = 1;
                        }
                        else *dat1->i = 0;
                    }
                    else return IFJ_ERR_INTERPRET;
                }
                else if(isDouble(type2))
                {
                    if(isInteger(type3))
                    {
                        if(*dat2->f == *(double *)dat3->i)
                        {
                            *dat1->i = 1;
                        }
                        else *dat1->i = 0;
                    }
                    else if(isDouble(type3))
                    {
                        if(*dat2->f == *dat3->f)
                        {
                            *dat1->i = 1;
                        }
                        else *dat1->i = 0;
                    }
                    else return IFJ_ERR_INTERPRET;
                }
                else if(isString(type2))
                {
                    if(isString(type3))
                    {
                        if(strcmp(dat2->str->str,dat3->str->str) == 0)
                        {
                            *dat1->i = 1;
                        }
                        else *dat1->i = 0;
                    }
                    else return IFJ_ERR_INTERPRET;
                }
                else
                {
                    return IFJ_ERR_INTERPRET;
                }
                break;

            /*
            add1 = 1 pokud add2 != add3, jinak add1 = 0
            */
            case I_NOTEQ:
                dat1 = getDat(&CT,&BlStack->First,((BTree *) &I->add1));
                type1 = getType((BTree *) &I->add1);
                dat2 = getDat(&CT,&BlStack->First,((BTree *) &I->add2));
                type2 = getType((BTree *) &I->add2);
                dat3 = getDat(&CT,&BlStack->First,((BTree *) &I->add3));
                type3 = getType((BTree *) &I->add3);

                setType(I->add1,IINTEGER);

                if(isInteger(type2))
                {
                    if(isInteger(type3))
                    {
                        if(*dat2->i != *dat3->i)
                        {
                            *dat1->i = 1;
                        }
                        else *dat1->i = 0;
                    }
                    else if(isDouble(type3))
                    {
                        if(*(double *)dat2->i != *dat3->f)
                        {
                            *dat1->i = 1;
                        }
                        else *dat1->i = 0;
                    }
                    else return IFJ_ERR_INTERPRET;
                }
                else if(isDouble(type2))
                {
                    if(isInteger(type3))
                    {
                        if(*dat2->f != *(double *)dat3->i)
                        {
                            *dat1->i = 1;
                        }
                        else *dat1->i = 0;
                    }
                    else if(isDouble(type3))
                    {
                        if(*dat2->f != *dat3->f)
                        {
                            *dat1->i = 1;
                        }
                        else *dat1->i = 0;
                    }
                    else return IFJ_ERR_INTERPRET;
                }
                else if(isString(type2))
                {
                    if(isString(type3))
                    {
                        if(strcmp(dat2->str->str,dat3->str->str) != 0)
                        {
                            *dat1->i = 1;
                        }
                        else *dat1->i = 0;
                    }
                    else return IFJ_ERR_INTERPRET;
                }
                else
                {
                    return IFJ_ERR_INTERPRET;
                }
                break;
            /////////////////////////////////////////////////////////////////////////////////////////////
            /*

            */
            case I_LENGHT:
                dat1 = getDat(&CT,&BlStack->First,((BTree *) &I->add1));
                type1 = getType((BTree *) &I->add1);
                dat3 = getDat(&CT,&BlStack->First,((BTree *) &I->add3));
                type3 = getType((BTree *) &I->add3);

                *dat3->i = 0;

               for(int i=0;dat1->str->str!='\0';i++)
                {
                  if (*dat1->str->str >= 'A' && *dat1->str->str <= 'Z')
                     *dat3->i++;
                  if (*dat1->str->str >= 'a' && *dat1->str->str <= 'z')
                     *dat3->i++;
                  if (*dat1->str->str >= '0' && *dat1->str->str <= '9')
                     *dat3->i++;
                }
                break;
            case I_SUBSTR:
                dat1 = getDat(&CT,&BlStack->First,((BTree *) &I->add1));
                type1 = getType((BTree *) &I->add1);
                dat2 = getDat(&CT,&BlStack->First,((BTree *) &I->add2));
                type2 = getType((BTree *) &I->add2);
                dat3 = getDat(&CT,&BlStack->First,((BTree *) &I->add3));
                type3 = getType((BTree *) &I->add3);

                for (int i = 0 ; i < *dat2->i ; i++)
                    {
                        dat1->str->str++;
                    }
                strcpy(dat3->str->str,dat1->str->str);
                strncpy (dat3->str->str, dat3->str->str , *dat3->i);
                *(*dat3->str->str+dat3->i) = '\0';
                break;
            case I_CONCAT:
                dat1 = getDat(&CT,&BlStack->First,((BTree *) &I->add1));
                type1 = getType((BTree *) &I->add1);
                dat2 = getDat(&CT,&BlStack->First,((BTree *) &I->add2));
                type2 = getType((BTree *) &I->add2);
                dat3 = getDat(&CT,&BlStack->First,((BTree *) &I->add3));
                type3 = getType((BTree *) &I->add3);

                dat3->str->length = dat1->str->length + dat2->str->length;
                // alokace
                if(dat3->str->length >= dat3->str->allocSize)
                {
                    if ((dat3->str->str = (char*) realloc(dat3->str->str, dat3->str->length + STR_LEN_INC)) == NULL)
                        return STR_ERROR;
                    dat3->str->allocSize = dat3->str->length + STR_LEN_INC;
                }

                concat(dat1->str->str,dat2->str->str,dat3->str->str);
                break;
            case I_FIND:
                dat1 = getDat(&CT,&BlStack->First,((BTree *) &I->add1));
                type1 = getType((BTree *) &I->add1);
                dat2 = getDat(&CT,&BlStack->First,((BTree *) &I->add2));
                type2 = getType((BTree *) &I->add2);
                dat3 = getDat(&CT,&BlStack->First,((BTree *) &I->add3));
                type3 = getType((BTree *) &I->add3);
                find(dat1->str->str,dat2->str->str,*dat3->i);
                break;
            case I_SORT:
                dat1 = getDat(&CT,&BlStack->First,((BTree *) &I->add1));
                type1 = getType((BTree *) &I->add1);
                dat2 = getDat(&CT,&BlStack->First,((BTree *) &I->add2));
                type2 = getType((BTree *) &I->add2);
                quicksort(dat1->str->str,dat2->str->str);
                break;
            /////////////////////////////////////////////////////////////////////
            /*
            null null null
            */
            case I_IF:
                zanoreniIf++;
                break;
            case I_IF_COND:
                dat1 = getDat(&CT,&BlStack->First,((BTree *) &pomIn->add1));
                podminkyIf[zanoreniIf] = *dat1->i;

                if(podminkyIf[zanoreniIf] == 0) //podminka neni splnena
                {
                    skipIf = 0;
                    while(!((nextI->Type == I_END_IF) && (skipIf == 0)))
                    {
                        if(nextI->Type == I_IF) skipIf ++;
                        if(nextI->Type == I_END_IF) skipIf --;
                        listNext(L);    // preskoci instrukce, ktere se nemaji vykonat
                        I = listGetData(L);
                        nextI = listGetNextData(L);
                    }
                }
                break;
            case I_END_IF:
                if(nextI->Type != I_ELSE) zanoreniIf--;
                break;
            case I_ELSE:
                if(podminkyIf[zanoreniIf] != 0)
                {
                    skipIf = 0;
                    while(!((nextI->Type == I_END_ELSE) && (skipIf == 0)))
                    {
                        if(nextI->Type == I_ELSE) skipIf ++;
                        if(nextI->Type == I_END_ELSE) skipIf --;
                        listNext(L);    // preskoci instrukce, ktere se nemaji vykonat
                        I = listGetData(L);
                        nextI = listGetNextData(L);
                    }
                }
                break;
            case I_END_ELSE:
                zanoreniIf--;
                break;
            case I_FOR:
                zanoreniFor++;
                break;
            case I_FOR_CHCK:
                instForChck = (L->active);
                break;
            case I_FOR_DIFF:
                instForDiff = (L->active);
                dat1 = getDat(&CT,&BlStack->First,((BTree *) &pomIn->add1));
                podminkaFor = *dat1->i;
                while(nextI->Type != I_FOR_COND)
                {
                    listNext(L);
                    I = listGetData(L);
                    nextI = listGetNextData(L);
                }
                break;
            case I_FOR_COND:
                if(diffJump == 1)
                {
                    diffJump = 0;
                    listGoto(L,instForChck);
                }
                else
                {
                    if(podminkaFor == 0)
                    {
                        skipIf = 0;
                        while(!((nextI->Type == I_END_FOR) && (skipIf == 0)))
                        {
                            if(nextI->Type == I_FOR) skipIf ++;
                            if(nextI->Type == I_END_FOR) skipIf --;
                            listNext(L);
                            I = listGetData(L);
                            nextI = listGetNextData(L);
                        }
                    }
                }
                break;
            case I_END_FOR:
                if(podminkaFor != 0)
                {
                    if(diffJump == 1) printf("TEST DIFF\n");
                    diffJump = 1;
                    listGoto(L,instForDiff);
                }
                else
                {
                    zanoreniFor--;
                }
                break;

            /*
            add1    adresa promenne kam se ma ulozit nactena hodnota
            */
            case I_READ:
                dat1 = getDat(&CT,&BlStack->First,((BTree *) &I->add1));
                type1 = getType((BTree *) &I->add1);

                if(isInteger(type1))
                {
                    scanf("%i",dat1->i);
                }
                else if(isDouble(type1))
                {
                    scanf("%g",dat1->f);
                }
                else if(isString(type1))
                {
                    strClear(dat1->str);
                    while((inChar = getchar()) != EOF)
                    {
                        if(strAddChar(dat1->str, inChar) != 0)
                        {
                          return IFJ_ERR_INTERPRET;
                        }
                    }
                }
                else return IFJ_ERR_INTERPRET;
                break;

            /*
            add1    adresa promenne odkud se vypisuje hodnota
            */
            case I_WRITE:
                dat1 = getDat(&CT,&BlStack->First,((BTree *) &I->add1));
                type1 = getType((BTree *) &I->add1);

                if(isInteger(type1))
                {
                    printf("%i\n",*dat1->i);
                }
                else if(isDouble(type1))
                {
                    printf("%g\n",*dat1->f);
                }
                else if(isString(type1))
                {
                    strPrint(dat1->str);
                }
                else return IFJ_ERR_INTERPRET;
                break;

            /*
            add1 dest
            add2 source
            */
            case I_ASSIGN:
                dat1 = getDat(&CT,&BlStack->First,((BTree *) &I->add1));
                type1 = getType((BTree *) &I->add1);
                dat2 = getDat(&CT,&BlStack->First,((BTree *) &I->add2));
                type2 = getType((BTree *) &I->add2);

                if(isInteger(type1))
                {
                    if(isInteger(type2))
                    {
                        *dat1->i = *dat2->i;
                    }
                    if(isDouble(type2))
                    {
                        *dat1->i = *(int *)dat2->f;
                    }
                    if(isString(type2)) return IFJ_ERR_INTERPRET;
                }
                else if(isDouble(type1))
                {
                    if(isInteger(type2))
                    {
                        *dat1->f = *(double *)dat2->i;
                    }
                    if(isDouble(type2))
                    {
                        *dat1->f = *dat2->f;
                    }
                    if(isString(type2)) return IFJ_ERR_INTERPRET;
                }
                else if(isString(type1))
                {
                    if(isString(type2))
                    {
                        strCopyString(dat1->str,dat1->str);
                    }
                    else return IFJ_ERR_INTERPRET;
                }
                break;
        }

        pomIn = I; //pamatujeme si predchozi ins pro podminky - zachova info pro else
        if(L->active->nextItem == NULL) return 0;
        listNext(L);
    }
}

void concat(char * s1,char * s2,char * vysledok)
{
int l1,l2;
l1=strlen(s1);
l2=strlen(s2);
l1=l1+l2;

while (l1>UCHAR_MAX)
{
    printf("Pridlhy retazec");
    return;
}
    while(*s1)
   {
      *vysledok = *s1;
      vysledok++;
      s1++;
   }

   while(*s2)
   {
      *vysledok = *s2;
      vysledok++;
      s2++;
   }
   *vysledok = '\0';
   s1--;
}

int main()
{
    interpret();
    return 0;
}
