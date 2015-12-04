//hlavicka pro lexikalni analyzator



#define ERROR     -1
//identifikator
#define ID        0

//klicova slova
#define KAUTO     1
#define KCIN      2
#define KCOUT     3
#define KDOUBLE   4
#define KELSE     5
#define KFOR      6
#define KIF       7
#define KINTEGER  8
#define KRETURN   9
#define KSTRING   10

#define PLUS      11
#define MINUS     12
#define TIMES     13
#define DIVIDE    14
#define SEMICOLON 15
#define LPARENTH  16
#define RPARENTH  17
#define EQ        18
#define NEQ       19
#define LEQ       20
#define LE        21
#define GEQ       22
#define GR        23
#define ASSIGN    24
#define LBRACKET  25
#define RBRACKET  26


#define ISTR       27
#define IINTEGER   28
#define IDOUBLE    29
#define CSTR      30
#define CINTEGER  31
#define CDOUBLE   32

#define COUT      33
#define CIN       34

#define END       35
#define COLON     36



//hlavicka funkce simulujici lexikalni analyzator
//void setSourceFile(FILE *f);
//int getNextToken(string *attr);
