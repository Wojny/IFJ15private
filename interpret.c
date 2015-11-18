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

    *BlockStack BlStack;
    BlockStackInit(BlStack);

    // Pomocne promenne pro pracy s daty
    union Dat *dat1, *dat2, *dat3;
    int *type1, *type2, *type3;

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
        }

        listNext(L);
    }
}
