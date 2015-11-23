#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <string.h>
//#include "interpret.h"
#include "ilist.h"
#include "sym_table.h"
#include "scaner.h"
#include "errors.h"
#include "str.h"


int interpret(tList *L, GSTable *G, constTable *CT)
{
    listFirst(L);
    tInstr *I;
    tInstr *pomIn;

    *BlockStack BlStack;
    BlockStackInit(BlStack);

    // Pomocne promenne pro pracy s daty
    union Dat *dat1, *dat2, *dat3;
    int *type1, *type2, *type3;
    int podminka = 0;
    int skipIf = 0;
    int zanoreniIf = 0;
    int podminkyIf[50];

    memset(podminkyIf, -1, sizeof(podminkyIf));

    while(1)
    {
        I = listGetData(L);

        switch(I->Type)
        {
            case I_ADD:
                dat1 = getData(CT,BlStack->First,I->add1);
                type1 = getType(I->add1);
                dat2 = getData(CT,BlStack->First,I->add2);
                type2 = getType(I->add2);
                dat3 = getData(CT,BlStack->First,I->add3);
                type3 = getType(I->add3);

                // Scitani pouze INT or DOUBLE
                if(((isInteger(type1) || isDouble(type1)) && (isInteger(type2) || isDouble(type2)))) {}
                else
                {
                    return IFJ_ERR_INTERPRET;
                }

                // Kontrola typu double a int cisel -> 1-2 double ma double vysledek, oba int maji int vysledek
                if(isDouble(type2))
                {
                    type1 = IDOUBLE;
                    if(isDouble(type3))
                    {
                        dat1->f = dat2->f + dat3->f;
                    }
                    else
                    {
                        dat1->f = dat2->f + (double *)dat3->i;
                    }
                }
                else
                {
                    if(isDouble(type3))
                    {
                        type1 = IDOUBLE;
                        dat1->f = (double *)dat2->i + dat3->f;
                    }
                    else
                    {
                        type1 = IINTEGER;
                        dat1->i = dat2->i + dat3->i;
                    }
                }
                break;

            case I_SUB:
                dat1 = getData(CT,BlStack->First,I->add1);
                type1 = getType(I->add1);
                dat2 = getData(CT,BlStack->First,I->add2);
                type2 = getType(I->add2);
                dat3 = getData(CT,BlStack->First,I->add3);
                type3 = getType(I->add3);

                // Odcitani pouze INT or DOUBLE
                if(((isInteger(type1) || isDouble(type1)) && (isInteger(type2) || isDouble(type2)))) {}
                else return IFJ_ERR_INTERPRET;

                // Kontrola typu double a int cisel -> 1-2 double ma double vysledek, oba int maji int vysledek
                if(isDouble(type2))
                {
                    type1 = IDOUBLE;
                    if(isDouble(type3))
                    {
                        dat1->f = dat2->f - dat3->f;
                    }
                    else
                    {
                        dat1->f = dat2->f - (double *)dat3->i;
                    }
                }
                else
                {
                    if(isDouble(type3))
                    {
                        type1 = IDOUBLE;
                        dat1->f = (double *)dat2->i - dat3->f;
                    }
                    else
                    {
                        type1 = IINTEGER;
                        dat1->i = dat2->i - dat3->i;
                    }
                }
                break;

            case I_MUL:
                dat1 = getData(CT,BlStack->First,I->add1);
                type1 = getType(I->add1);
                dat2 = getData(CT,BlStack->First,I->add2);
                type2 = getType(I->add2);
                dat3 = getData(CT,BlStack->First,I->add3);
                type3 = getType(I->add3);

                // Nasobeni pouze INT or DOUBLE
                if(((isInteger(type1) || isDouble(type1)) && (isInteger(type2) || isDouble(type2)))) {}
                else return IFJ_ERR_INTERPRET;

                if(isDouble(type2))
                {
                    type1 = IDOUBLE;
                    if(isDouble(type3))
                    {
                        dat1->f = dat2->f * dat3->f;
                    }
                    else
                    {
                        dat1->f = dat2->f * (double *)dat3->i;
                    }
                }
                else
                {
                    if(isDouble(type3))
                    {
                        type1 = IDOUBLE;
                        dat1->f = (double *)dat2->i * dat3->f;
                    }
                    else
                    {
                        type1 = IINTEGER;
                        dat1->i = dat2->i * dat3->i;
                    }
                }
                break;

            case I_DIV:
                dat1 = getData(CT,BlStack->First,I->add1);
                type1 = getType(I->add1);
                dat2 = getData(CT,BlStack->First,I->add2);
                type2 = getType(I->add2);
                dat3 = getData(CT,BlStack->First,I->add3);
                type3 = getType(I->add3);

                // Nasobeni pouze INT or DOUBLE
                if(((isInteger(type1) || isDouble(type1)) && (isInteger(type2) || isDouble(type2)))) {}
                else return IFJ_ERR_INTERPRET;

                // Deleni nulou
                if((isInteger(type3) && dat3->i == 0) || (isDouble(type3) && dat3->f == 0)) else return IFJ_ERR_INTERPRET;

                if(isDouble(type2))
                {
                    type1 = IDOUBLE;
                    if(isDouble(type3))
                    {
                        dat1->f = dat2->f / dat3->f;
                    }
                    else
                    {
                        dat1->f = dat2->f / (double *)dat3->i;
                    }
                }
                else
                {
                    if(isDouble(type3))
                    {
                        type1 = IDOUBLE;
                        dat1->f = (double *)dat2->i / dat3->f;
                    }
                    else
                    {
                        type1 = IINTEGER;
                        dat1->i = dat2->i / dat3->i;
                    }
                }
                break;

            case I_LESS:
                dat1 = getData(CT,BlStack->First,I->add1);
                type1 = getType(I->add1);
                dat2 = getData(CT,BlStack->First,I->add2);
                type2 = getType(I->add2);
                dat3 = getData(CT,BlStack->First,I->add3);
                type3 = getType(I->add3);

                type1 = IINTEGER;

                if(isInteger(type2))
                {
                    if(isInteger(type3))
                    {
                        if(dat2->i < dat3->i)
                        {
                            dat1->i = 1;
                        }
                        else dat1->i = 0;
                    }
                    else if(isDouble(type3))
                    {
                        if((double *)dat2->i < dat3->f)
                        {
                            dat1->i = 1;
                        }
                        else dat1->i = 0;
                    }
                    else return IFJ_ERR_INTERPRET;
                }
                else if(isDouble(type2))
                {
                    if(isInteger(type3))
                    {
                        if(dat2->f < (double *)dat3->i)
                        {
                            dat1->i = 1;
                        }
                        else dat1->i = 0;
                    }
                    else if(isDouble(type3))
                    {
                        if(dat2->f < dat3->f)
                        {
                            dat1->i = 1;
                        }
                        else dat1->i = 0;
                    }
                    else return IFJ_ERR_INTERPRET;
                }
                else if(isString(type2))
                {
                    if(isString(type3))
                    {
                        if(strcmp(dat2->str,dat3->str) < 0)
                        {
                            dat1->i = 1;
                        }
                        else dat1->i = 0;
                    }
                    else return IFJ_ERR_INTERPRET;
                }
                else
                {
                    return IFJ_ERR_INTERPRET;
                }
                break;

            case I_MORE:
                dat1 = getData(CT,BlStack->First,I->add1);
                type1 = getType(I->add1);
                dat2 = getData(CT,BlStack->First,I->add2);
                type2 = getType(I->add2);
                dat3 = getData(CT,BlStack->First,I->add3);
                type3 = getType(I->add3);

                type1 = IINTEGER;

                if(isInteger(type2))
                {
                    if(isInteger(type3))
                    {
                        if(dat2->i > dat3->i)
                        {
                            dat1->i = 1;
                        }
                        else dat1->i = 0;
                    }
                    else if(isDouble(type3))
                    {
                        if((double *)dat2->i > dat3->f)
                        {
                            dat1->i = 1;
                        }
                        else dat1->i = 0;
                    }
                    else return IFJ_ERR_INTERPRET;
                }
                else if(isDouble(type2))
                {
                    if(isInteger(type3))
                    {
                        if(dat2->f > (double *)dat3->i)
                        {
                            dat1->i = 1;
                        }
                        else dat1->i = 0;
                    }
                    else if(isDouble(type3))
                    {
                        if(dat2->f > dat3->f)
                        {
                            dat1->i = 1;
                        }
                        else dat1->i = 0;
                    }
                    else return IFJ_ERR_INTERPRET;
                }
                else if(isString(type2))
                {
                    if(isString(type3))
                    {
                        if(strcmp(dat2->str,dat3->str) > 0)
                        {
                            dat1->i = 1;
                        }
                        else dat1->i = 0;
                    }
                    else return IFJ_ERR_INTERPRET;
                }
                else
                {
                    return IFJ_ERR_INTERPRET;
                }
                break;

            case I_LESSEQ:
                dat1 = getData(CT,BlStack->First,I->add1);
                type1 = getType(I->add1);
                dat2 = getData(CT,BlStack->First,I->add2);
                type2 = getType(I->add2);
                dat3 = getData(CT,BlStack->First,I->add3);
                type3 = getType(I->add3);

                type1 = IINTEGER;

                if(isInteger(type2))
                {
                    if(isInteger(type3))
                    {
                        if(dat2->i <= dat3->i)
                        {
                            dat1->i = 1;
                        }
                        else dat1->i = 0;
                    }
                    else if(isDouble(type3))
                    {
                        if((double *)dat2->i <= dat3->f)
                        {
                            dat1->i = 1;
                        }
                        else dat1->i = 0;
                    }
                    else return IFJ_ERR_INTERPRET;
                }
                else if(isDouble(type2))
                {
                    if(isInteger(type3))
                    {
                        if(dat2->f <= (double *)dat3->i)
                        {
                            dat1->i = 1;
                        }
                        else dat1->i = 0;
                    }
                    else if(isDouble(type3))
                    {
                        if(dat2->f <= dat3->f)
                        {
                            dat1->i = 1;
                        }
                        else dat1->i = 0;
                    }
                    else return IFJ_ERR_INTERPRET;
                }
                else if(isString(type2))
                {
                    if(isString(type3))
                    {
                        if(strcmp(dat2->str,dat3->str) <= 0)
                        {
                            dat1->i = 1;
                        }
                        else dat1->i = 0;
                    }
                    else return IFJ_ERR_INTERPRET;
                }
                else
                {
                    return IFJ_ERR_INTERPRET;
                }
                break;

            case I_MOREEQ:
                dat1 = getData(CT,BlStack->First,I->add1);
                type1 = getType(I->add1);
                dat2 = getData(CT,BlStack->First,I->add2);
                type2 = getType(I->add2);
                dat3 = getData(CT,BlStack->First,I->add3);
                type3 = getType(I->add3);

                type1 = IINTEGER;

                if(isInteger(type2))
                {
                    if(isInteger(type3))
                    {
                        if(dat2->i >= dat3->i)
                        {
                            dat1->i = 1;
                        }
                        else dat1->i = 0;
                    }
                    else if(isDouble(type3))
                    {
                        if((double *)dat2->i >= dat3->f)
                        {
                            dat1->i = 1;
                        }
                        else dat1->i = 0;
                    }
                    else return IFJ_ERR_INTERPRET;
                }
                else if(isDouble(type2))
                {
                    if(isInteger(type3))
                    {
                        if(dat2->f >= (double *)dat3->i)
                        {
                            dat1->i = 1;
                        }
                        else dat1->i = 0;
                    }
                    else if(isDouble(type3))
                    {
                        if(dat2->f >= dat3->f)
                        {
                            dat1->i = 1;
                        }
                        else dat1->i = 0;
                    }
                    else return IFJ_ERR_INTERPRET;
                }
                else if(isString(type2))
                {
                    if(isString(type3))
                    {
                        if(strcmp(dat2->str,dat3->str) >= 0)
                        {
                            dat1->i = 1;
                        }
                        else dat1->i = 0;
                    }
                    else return IFJ_ERR_INTERPRET;
                }
                else
                {
                    return IFJ_ERR_INTERPRET;
                }
                break;

            case I_EQUAL:
                dat1 = getData(CT,BlStack->First,I->add1);
                type1 = getType(I->add1);
                dat2 = getData(CT,BlStack->First,I->add2);
                type2 = getType(I->add2);
                dat3 = getData(CT,BlStack->First,I->add3);
                type3 = getType(I->add3);

                type1 = IINTEGER;

                if(isInteger(type2))
                {
                    if(isInteger(type3))
                    {
                        if(dat2->i == dat3->i)
                        {
                            dat1->i = 1;
                        }
                        else dat1->i = 0;
                    }
                    else if(isDouble(type3))
                    {
                        if((double *)dat2->i == dat3->f)
                        {
                            dat1->i = 1;
                        }
                        else dat1->i = 0;
                    }
                    else return IFJ_ERR_INTERPRET;
                }
                else if(isDouble(type2))
                {
                    if(isInteger(type3))
                    {
                        if(dat2->f == (double *)dat3->i)
                        {
                            dat1->i = 1;
                        }
                        else dat1->i = 0;
                    }
                    else if(isDouble(type3))
                    {
                        if(dat2->f == dat3->f)
                        {
                            dat1->i = 1;
                        }
                        else dat1->i = 0;
                    }
                    else return IFJ_ERR_INTERPRET;
                }
                else if(isString(type2))
                {
                    if(isString(type3))
                    {
                        if(strcmp(dat2->str,dat3->str) == 0)
                        {
                            dat1->i = 1;
                        }
                        else dat1->i = 0;
                    }
                    else return IFJ_ERR_INTERPRET;
                }
                else
                {
                    return IFJ_ERR_INTERPRET;
                }
                break;

            case I_NOTEQ:
                dat1 = getData(CT,BlStack->First,I->add1);
                type1 = getType(I->add1);
                dat2 = getData(CT,BlStack->First,I->add2);
                type2 = getType(I->add2);
                dat3 = getData(CT,BlStack->First,I->add3);
                type3 = getType(I->add3);

                type1 = IINTEGER;

                if(isInteger(type2))
                {
                    if(isInteger(type3))
                    {
                        if(dat2->i != dat3->i)
                        {
                            dat1->i = 1;
                        }
                        else dat1->i = 0;
                    }
                    else if(isDouble(type3))
                    {
                        if((double *)dat2->i != dat3->f)
                        {
                            dat1->i = 1;
                        }
                        else dat1->i = 0;
                    }
                    else return IFJ_ERR_INTERPRET;
                }
                else if(isDouble(type2))
                {
                    if(isInteger(type3))
                    {
                        if(dat2->f != (double *)dat3->i)
                        {
                            dat1->i = 1;
                        }
                        else dat1->i = 0;
                    }
                    else if(isDouble(type3))
                    {
                        if(dat2->f != dat3->f)
                        {
                            dat1->i = 1;
                        }
                        else dat1->i = 0;
                    }
                    else return IFJ_ERR_INTERPRET;
                }
                else if(isString(type2))
                {
                    if(isString(type3))
                    {
                        if(strcmp(dat2->str,dat3->str) != 0)
                        {
                            dat1->i = 1;
                        }
                        else dat1->i = 0;
                    }
                    else return IFJ_ERR_INTERPRET;
                }
                else
                {
                    return IFJ_ERR_INTERPRET;
                }
                break;
            ////////////////////////////////////////
            case I_LENGHT:
                break;
            case I_SUBSTR:
                break;
            case I_CONCAT:
                break;
            case I_FIND:
                break;
            case I_SORT:
                break;
            ////////////////////////////////////////
            case I_IF:
                //if(L->active->nextItem->instr->Type == I_IF_COND)
                podminka = 1; // vyhodnocuje se podminka
                zanoreniIf++;
                break;
            case I_IF_COND:
                podminka = 0;
                podminkyIf[zanoreniIf] = pomIn->add1;
                if(podminkyIf[zanoreniIf] == 0) //podminka neni splnena
                {
                    while(!((L->active->nextItem->instr->Type == I_END_IF) && (skipIf == 0)))
                    {
                        if(L->active->nextItem->instr->Type == I_IF) skipIf ++;
                        if(L->active->nextItem->instr->Type == I_END_IF) skipIf --;
                        listNext(L);    // preskoci instrukce, ktere se nemaji vykonat
                    }
                }
                break;
            case I_END_IF:
                if(L->active->nextItem->instr->Type != I_ELSE) zanoreniIf--;
                break;
            case I_ELSE:
                if(podminkyIf[zanoreniIf] != 0)
                {
                    while(!((L->active->nextItem->instr->Type == I_END_ELSE) && (skipIf == 0)))
                    {
                        if(L->active->nextItem->instr->Type == I_ELSE) skipIf ++;
                        if(L->active->nextItem->instr->Type == I_END_ELSE) skipIf --;
                        listNext(L);    // preskoci instrukce, ktere se nemaji vykonat
                    }
                }
                break;
            case I_END_ELSE:
                zanoreniIf--;
                break;
            case I_FOR:

                break;
            case I_FOR_COND:

                break;
            case I_END_FOR:

                break;

                // Domluvit se
            case I_READ:
                break;
            case I_WRITE:
                break;
        }

        if(podminka == 1) pomIn = I; //pamatujeme si predchozi ins pro podminky - zachova info pro else

        listNext(L);
    }
}
