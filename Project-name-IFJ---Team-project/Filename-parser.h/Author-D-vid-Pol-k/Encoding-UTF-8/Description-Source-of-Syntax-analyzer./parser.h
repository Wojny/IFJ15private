//+------------------------------------------------------------------+
//|	Project name:        IFJ - Team project                          |
//| Filename:            parser.h                                    |
//| Author:              Dávid Polák                                 |
//|	Encoding:            UTF-8										 |
//|	Description:         Source of Syntax analyzer.					 |
//+------------------------------------------------------------------+
#ifndef PARSER_H
#define PARSER_H

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <malloc.h>
#include "ial.h"
#include "errors.h"
#include "scanner.h"
#include "str.h"

GSTable GST;
constTable newCTable;
FN ActualFN;
BTree ActualAdd1;
BTree ActualAdd2;
BTree ActualAdd3;
tList *L;

typedef struct BD
{
	int BracketType;
	struct BD* NextDepth;
} *BracketDepth;
BracketDepth BDepth;
BracketDepth BRDepth;

bool parserProlog(string *attr, int *type);
bool Statement(string *attr, int *type);
bool FunctionDef(string *attr, int *type);
bool Param(string *attr, int *type);
bool FunctionParams(string *attr, int *type, int typFN, string Identif);
bool Initializ(string *attr, int *type);
bool parser();
int NextToken(string *attr, int *type);

#endif
