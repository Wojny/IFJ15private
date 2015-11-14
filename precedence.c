//+------------------------------------------------------------------+
//|	Project name:        IFJ - Team project                          |
//| Filename:            precedence.c                                |
//| Author:                                               			 |
//|	Encoding:            UTF-8										 |
//|	Description:         Source of Precendenc Table.				 |
//+------------------------------------------------------------------+
#include "precedence.h"

static int PTable[14][14];

void PTableInit (int (*PTable)[14])
{
	PTable[EQUAL][ADD]= PTLESS;
	PTable[EQUAL][SUB]=	PTLESS;
	PTable[EQUAL][MUL]= PTLESS;
	PTable[EQUAL][DIV]= PTLESS;
	PTable[EQUAL][DATATYPE]= PTLESS;
	PTable[EQUAL][IDF]= PTLESS;
	PTable[NOTEQUAL][ADD]= PTLESS;
	PTable[NOTEQUAL][SUB]= PTLESS;
	PTable[NOTEQUAL][MUL]= PTLESS;
	PTable[NOTEQUAL][DIV]= PTLESS;
	PTable[NOTEQUAL][DATATYPE]= PTLESS;
	PTable[NOTEQUAL][IDF]= PTLESS;
	PTable[LESSEQUAL][ADD]= PTLESS;
	PTable[LESSEQUAL][SUB]= PTLESS;
	PTable[LESSEQUAL][MUL]= PTLESS;
	PTable[LESSEQUAL][DIV]= PTLESS;
	PTable[LESSEQUAL][DATATYPE]= PTLESS;
	PTable[LESSEQUAL][IDF]= PTLESS;
	PTable[MOREEQUAL][ADD]= PTLESS;
	PTable[MOREEQUAL][SUB]= PTLESS;
	PTable[MOREEQUAL][MUL]= PTLESS;
	PTable[MOREEQUAL][DIV]= PTLESS;
	PTable[MOREEQUAL][DATATYPE]= PTLESS;
	PTable[MOREEQUAL][IDF]= PTLESS;
	PTable[LESS][ADD]= PTLESS;
	PTable[LESS][SUB]= PTLESS;
	PTable[LESS][MUL]= PTLESS;
	PTable[LESS][DIV]= PTLESS;
	PTable[LESS][DATATYPE]= PTLESS;
	PTable[LESS][IDF]= PTLESS;
	PTable[MORE][ADD]= PTLESS;
	PTable[MORE][SUB]= PTLESS;
	PTable[MORE][MUL]= PTLESS;
	PTable[MORE][DIV]= PTLESS;
	PTable[MORE][DATATYPE]= PTLESS;
	PTable[MORE][IDF]= PTLESS;
	PTable[ADD][MUL]= PTLESS;
	PTable[ADD][DIV]= PTLESS;
	PTable[ADD][LBRACE]= PTLESS;
	PTable[ADD][RBRACE]= PTLESS;
	PTable[ADD][DATATYPE]= PTLESS;
	PTable[ADD][IDF]= PTLESS;
	PTable[SUB][MUL]= PTLESS;
	PTable[SUB][DIV]= PTLESS;
	PTable[SUB][LBRACE]= PTLESS;
	PTable[SUB][RBRACE]= PTLESS;
	PTable[SUB][DATATYPE]= PTLESS;
	PTable[SUB][IDF]= PTLESS;
	PTable[MUL][LBRACE]=
	PTable[MUL][DATATYPE]=
	PTable[MUL][IDF]= PTLESS;
	PTable[DIV][LBRACE]=
	PTable[DIV][DATATYPE]=
	PTable[DIV][IDF]= PTLESS;
	PTable[LBRACE][RBRACE]= PTEQUAL;
	PTable[LBRACE][DATATYPE]=PTLESS;
	PTable[LBRACE][IDF]= PTLESS;
	PTable[RBRACE][DATATYPE]= PTLESS;
	PTable[RBRACE][LBRACE]= PTNULL;
	PTable[RBRACE][IDF]= PTNULL;
	PTable[IDF][DATATYPE] = PTNULL;
	PTable[IDF][IDF]= PTNULL;
	PTable[DATATYPE][LBRACE]= PTNULL;
	PTable[DATATYPE][DATATYPE]= PTNULL;
	PTable[DATATYPE][IDF]= PTNULL;
	PTable[TERMINAL][RBRACE]= PTNULL;
	PTable[TERMINAL][TERMINAL]= PTNULL;
	PTable[TERMINAL][IDF]= PTLESS;	
}

int GetTermType( char *value, int type)
{
	if (type == IDENTIFIER) return IDF;
	else if (type == STRING || type == DOUBLE || type == INTEGER) return DATATYPE;
	else if (!strcmp(value, "==")) return EQUAL;
	else if (!strcmp(value, "!=")) return NOTEQUAL;
	else if (!strcmp(value, "<=")) return LESSEQUAL;
	else if (!strcmp(value, ">=")) return MOREEQUAL;
	else if (!strcmp(value, "<")) return LESS;
	else if (!strcmp(value, ">")) return MORE;
	else if (!strcmp(value, "+")) return ADD;
	else if (!strcmp(value, "-")) return SUB;
	else if (!strcmp(value, "*")) return MUL;
	else if (!strcmp(value, "/")) return DIV;
	else if (!strcmp(value, "(")) return LBRACE;
	else if (!strcmp(value, ")")) return RBRACE;
	else return TERMINAL;
}

