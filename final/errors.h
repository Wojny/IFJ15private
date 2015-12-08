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


#define IFJ_ERR_NOERROR 0
#define IFJ_ERR_LEXICAL 1
#define	IFJ_ERR_SYNTAX 2
#define	IFJ_ERR_SEMANTIC 3
#define	IFJ_ERR_INTERPRET 4
#define	IFJ_ERR_PROGRAM 5

#define IFJ_ERR_UNKNOWN -1

extern int ErrFlag;
extern int ErrCounter;
extern int ReallocCounter;


typedef struct chyba
{
int LineNumber;
int ErrorType;
}TErr;
TErr *ERRList;
void PrintERRItem(TErr *p_temp);
int MakeERRL(void);
void AddERR(int ERRLine, int ERRType);
void PrintERR(void);
void FreeERR(void);
#endif
