//+------------------------------------------------------------------+
//|	Project name:        IFJ - Team project                          |
//| Filename:            precedence.h                                |
//| Author:                                              			 |
//|	Encoding:            UTF-8										 |
//|	Description:         Source of Precendenc Table.				 |
//+------------------------------------------------------------------+
#ifndef PRECEDENCE_H
#define PRECEDENCE_H

#include <stdbool.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "str.h"
#include "ial.h"
#include "scanner.h"
#include "error.h"
#include "parser.h"


#define PTMORE 0	// znak '>' v precedencni tabulce
#define PTLESS 1	// znak '<' v precedencni tabulce
#define PTEQUAL 2 	// znak '=' v precedencni tabulce
#define PTNULL -1	// zadny zaznam v precedencni tabulce
#define PTINIT_SIZE 20	//inicializacni velikost zasobniku

enum entries	{
	EQUAL = 1,	//== (1)
	NOTEQUAL,	//!= (2)
	LESSEQUAL,	//<= (3)
	MOREEQUAL,	//>= (4)
	LESS,		//< (5)
	MORE,		//> (6)	
	ADD,			//+ (7)
	SUB,			//- (8)
	MUL,			//* (9)
	DIV,			/// (10)	
	LBRACE,		//( (11)
	RBRACE,		//) (12)
	IDF, //variableid (13)	
	DATATYPE,	//STRING || DOUBLE || INTEGER (14)
	TERMINAL,	//konecny operator (15)
	EXPR,		//neterminal <expression> (16)	
};

typedef struct {
	int entry;
	tVarValue *expr;
}tData;

typedef struct {
	tData **arr;
	int top;
	int stackSize;
}tStack;

typedef struct PointerTrash {
	tVarValue **arr;
	int size;
	int top;
}*tPointTrashPtr;

#endif 
