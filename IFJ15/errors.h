//+------------------------------------------------------------------+
//|	Project name:        IFJ - Team project                          |
//| Filename:            errors.h                                    |
//| Author:              			                                 |
//|	Encoding:            UTF-8										 |
//|	Description:         Definition of error codes.					 |
//+------------------------------------------------------------------+
#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define IFJ_ERR_LEXICAL 1
#define	IFJ_ERR_SYNTAX 2
#define	IFJ_ERR_SEMANTIC 3
#define IFJ_ERR_SEM_ARIT 4
#define	IFJ_ERR_DATA 5
#define IFJ_ERR_SEM_OTHER 6
#define	IFJ_ERR_INTRP_IN 7
#define	IFJ_ERR_INTRP_INIT 8
#define	IFJ_ERR_INTRP_DIV 9
#define	IFJ_ERR_INTERPRET 10
#define	IFJ_ERR_PROGRAM 11

extern int ErrCounter;

typedef struct chyba
{
int LineNumber;
int ErrorType;
}TErr;
TErr *ERRList;
void PrintERRItem(TErr *temp);
int MakeERRL(void);
void AddERR(int ERRLine, int ERRType);
int PrintERR(void);
void FreeERR(void);
#endif
