//+------------------------------------------------------------------+
//|	Project name:        IFJ - Team project                          |
//| Filename:            precedence.c                                |
//| Author:                                               			 |
//|	Encoding:            UTF-8										 |
//|	Description:         Source of Precendenc Table.				 |
//+------------------------------------------------------------------+
#include "precedence.h"

//def pravidla -> zapsany prevracene
static int rule40[3] = {EXPR, EQUAL, EXPR};
static int rule41[3] = {EXPR, NOTEQUAL, EXPR};
static int rule42[3] = {EXPR, LESSEQUAL, EXPR};
static int rule43[3] = {EXPR, MOREEQUAL, EXPR};
static int rule44[3] = {EXPR, LESS, EXPR};
static int rule45[3] = {EXPR, MORE, EXPR};
static int rule46[3] = {EXPR, ADD, EXPR};
static int rule47[3] = {EXPR, SUB, EXPR};
static int rule48[3] = {EXPR, MUL, EXPR};
static int rule49[3] = {EXPR, DIV, EXPR};
static int rule50[3] = {IDF, 0, 0};
static int rule51[3] = {LBRACE, EXPR, RBRACE};
static int rule52[3] = {DATATYPE, 0, 0};

//precedentni tabluka
static int PTable[16][16];

//inicializace precedentnej tabulky
void PTableInit (int (*PTable)[16])
{
	PTable[EQUAL][ADD]= PTLESS;
	PTable[EQUAL][SUB]=	PTLESS;
	PTable[EQUAL][MUL]= PTLESS;
	PTable[EQUAL][DIV]= PTLESS;
	PTable[EQUAL][LBRACE]= PTLESS;
	PTable[EQUAL][DATATYPE]= PTLESS;
	PTable[EQUAL][IDF]= PTLESS;	
	PTable[NOTEQUAL][ADD]= PTLESS;
	PTable[NOTEQUAL][SUB]= PTLESS;
	PTable[NOTEQUAL][MUL]= PTLESS;
	PTable[NOTEQUAL][DIV]= PTLESS;
	PTable[NOTEQUAL][LBRACE]= PTLESS;
	PTable[NOTEQUAL][DATATYPE]= PTLESS;
	PTable[NOTEQUAL][IDF]= PTLESS;	
	PTable[LESSEQUAL][ADD]= PTLESS;
	PTable[LESSEQUAL][SUB]= PTLESS;
	PTable[LESSEQUAL][MUL]= PTLESS;
	PTable[LESSEQUAL][DIV]= PTLESS;
	PTable[LESSEQUAL][LBRACE]= PTLESS;
	PTable[LESSEQUAL][DATATYPE]= PTLESS;
	PTable[LESSEQUAL][IDF]= PTLESS;	
	PTable[MOREEQUAL][ADD]= PTLESS;
	PTable[MOREEQUAL][SUB]= PTLESS;
	PTable[MOREEQUAL][MUL]= PTLESS;
	PTable[MOREEQUAL][DIV]= PTLESS;
	PTable[MOREEQUAL][LBRACE]= PTLESS;
	PTable[MOREEQUAL][DATATYPE]= PTLESS;
	PTable[MOREEQUAL][IDF]= PTLESS;	
	PTable[LESS][ADD]= PTLESS;
	PTable[LESS][SUB]= PTLESS;
	PTable[LESS][MUL]= PTLESS;
	PTable[LESS][DIV]= PTLESS;
	PTable[LESS][LBRACE]= PTLESS;
	PTable[LESS][DATATYPE]= PTLESS;
	PTable[LESS][IDF]= PTLESS;	
	PTable[MORE][ADD]= PTLESS;
	PTable[MORE][SUB]= PTLESS;
	PTable[MORE][MUL]= PTLESS;
	PTable[MORE][DIV]= PTLESS;
	PTable[MORE][LBRACE]= PTLESS;
	PTable[MORE][DATATYPE]= PTLESS;
	PTable[MORE][IDF]= PTLESS;	
	PTable[ADD][MUL]= PTLESS;
	PTable[ADD][DIV]= PTLESS;
	PTable[ADD][LBRACE]= PTLESS;	
	PTable[ADD][DATATYPE]= PTLESS;
	PTable[ADD][IDF]= PTLESS;	
	PTable[SUB][MUL]= PTLESS;
	PTable[SUB][DIV]= PTLESS;
	PTable[SUB][LBRACE]= PTLESS;	
	PTable[SUB][DATATYPE]= PTLESS;
	PTable[SUB][IDF]= PTLESS;	
	PTable[MUL][LBRACE]=PTLESS;
	PTable[MUL][DATATYPE]=PTLESS;
	PTable[MUL][IDF]= PTLESS;	
	PTable[DIV][LBRACE]=PTLESS;
	PTable[DIV][DATATYPE]=PTLESS;
	PTable[DIV][IDF]= PTLESS;	
	PTable[LBRACE][EQUAL]= PTLESS;
	PTable[LBRACE][NOTEQUAL]= PTLESS;
	PTable[LBRACE][LESSEQUAL]= PTLESS;
	PTable[LBRACE][MOREEQUAL]= PTLESS;
	PTable[LBRACE][LESS]= PTLESS;
	PTable[LBRACE][MORE]= PTLESS;
	PTable[LBRACE][ADD]= PTLESS;
	PTable[LBRACE][SUB]= PTLESS;
	PTable[LBRACE][MUL]= PTLESS;
	PTable[LBRACE][DIV]= PTLESS;
	PTable[LBRACE][LBRACE]= PTLESS;
	PTable[LBRACE][RBRACE]= PTEQUAL;
	PTable[LBRACE][DATATYPE]=PTLESS;
	PTable[LBRACE][IDF]= PTLESS;
	PTable[LBRACE][TERMINAL]= PTNULL;	
	PTable[RBRACE][LBRACE]= PTNULL;
	PTable[RBRACE][DATATYPE]= PTNULL;	
	PTable[RBRACE][IDF]= PTNULL;	
	PTable[IDF][LBRACE]= PTNULL;
	PTable[IDF][DATATYPE] = PTNULL;
	PTable[IDF][IDF]= PTNULL;	
	PTable[DATATYPE][LBRACE]= PTNULL;
	PTable[DATATYPE][DATATYPE]= PTNULL;
	PTable[DATATYPE][IDF]= PTNULL;	
	PTable[TERMINAL][EQUAL]= PTLESS;
	PTable[TERMINAL][NOTEQUAL]= PTLESS;
	PTable[TERMINAL][LESSEQUAL]= PTLESS;
	PTable[TERMINAL][MOREEQUAL]= PTLESS;
	PTable[TERMINAL][LESS]= PTLESS;
	PTable[TERMINAL][MORE]= PTLESS;
	PTable[TERMINAL][ADD]= PTLESS;
	PTable[TERMINAL][SUB]= PTLESS;
	PTable[TERMINAL][MUL]= PTLESS;
	PTable[TERMINAL][DIV]= PTLESS;
	PTable[TERMINAL][LBRACE]= PTLESS;
	PTable[TERMINAL][RBRACE]= PTNULL;
	PTable[TERMINAL][IDF]= PTLESS;
	PTable[TERMINAL][DATATYPE]= PTLESS;
	PTable[TERMINAL][TERMINAL]= PTNULL;
		
}
/*
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

int RuleGenerator(int *rule)
{
	if (compareIntArrays(rule, rule40)) return 40;
	else if (compareIntArrays(rule, rule41)) return 41;
	else if (compareIntArrays(rule, rule42)) return 42;
	else if (compareIntArrays(rule, rule43)) return 43;
	else if (compareIntArrays(rule, rule44)) return 44;
	else if (compareIntArrays(rule, rule45)) return 45;
	else if (compareIntArrays(rule, rule46)) return 46;
	else if (compareIntArrays(rule, rule47)) return 47;
	else if (compareIntArrays(rule, rule48)) return 48;
	else if (compareIntArrays(rule, rule49)) return 49;
	else if (compareIntArrays(rule, rule50)) return 50;
	else if (compareIntArrays(rule, rule51)) return 51;
	else if (compareIntArrays(rule, rule52)) return 52;
	else  return -1;

}
*/
