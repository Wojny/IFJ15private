
              //asdfg
//identifikator
#define ID        0

//klicova slova
#define KBEGIN    1
#define KBOOLEAN  2
#define KDO       3
#define KELSE     4
#define KEND      5
#define KFALSE    6
#define KFIND     7
#define KFORWARD  8
#define KFUNCTION 9
#define KIF       10
#define KINTEGER  11
#define KREADLN   12
#define KREAL     13
#define KSORT     14
#define KSTRING   15
#define KTHEN     16
#define KTRUE     17
#define KVAR      18
#define KWHILE    19
#define KWRITE    20

#define SETZERO   21
#define READ      22
#define WRITE     23
#define WHILE     24
#define SETZERO   25
#define READ      26
#define WRITE     27
#define WHILE     28
#define WHILE     29
#define SETZERO   30
#define READ      31
#define WRITE     32
#define WHILE     33
#define SETZERO   34
#define READ      35

#define STR       36
#define INTEGER   37
#define REAL      38



//hlavicka funkce simulujici lexikalni analyzator
void setSourceFile(FILE *f);
int getNextToken(string *attr);
