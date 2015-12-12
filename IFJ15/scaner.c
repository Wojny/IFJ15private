#include <stdio.h>
#include <stdlib.h>
//#include "str.c"
#include "scaner.h"
//#include "sym_table.h"


//--------------------------------------------------------SCANNER---------------------------



// promenna pro ulozeni vstupniho souboru
FILE *source;
int line=1;

void setSourceFile(FILE *f)
{
  source = f;
}



int getNextToken(string *attr){
// hlavni funkce lexikalniho analyzatoru
  int state = 0;// stav KA
  int c; // precteny znak



  int hex;
  //int num=0;
  strClear(attr);
  while (1==1){
    c=getc(source); // precte znak ze vstupniho souboru
    switch(state){
      case 0:  // Konecny Automat
        if((c==' ')||(c=='\n')||(c=='\r')||(c=='\t')){ // odstraneni bilych znaku
          if(c=='\n') line++;
          state=0;
        }
        else if (c=='/'){ // zacatek komentare nebo operace deleni
          state=1;
        }
        else if((c=='_')||((c>='a')&&(c<='z'))||((c>='A')&&(c<='Z'))){ // zacatek identifikatoru nebo klicoveho slova
          state=5;
          strAddChar(attr, c);
        }
        else if (c=='"'){ // zacatek retezce
          state=6;
        }
        else if(c=='0'){  // zacatek cisla zacinajici nulou
          state=8;
          //strAddChar(attr,c);
        }
        else if((c>='1')&&(c<='9')){ // zacatek cisla nezacinajici nulou
          state=9;
          strAddChar(attr,c);
        }
        else if(c=='<'){ // zacatek  < || <= || <<
          state=15;
        }
        else if(c=='>'){ // zacatek  > || >= || >>
          state=16;
        }
        else if(c=='!'){ // zacatek !=
          state=17;
        }
        else if(c=='='){ // zacatek  == || =
          state=18;
        }
        else if(c=='+'){ // operator +
          return PLUS;
        }
        else if(c=='-'){ // operator +
          return MINUS;
        }
        else if(c=='*'){ // operator +
          return TIMES;
        }
        else if(c==';'){ // strednik
          return SEMICOLON;
        }
        else if(c=='('){ // leva zavorka
          return LPARENTH;
        }
        else if(c==')'){ // prava zavorka
          return RPARENTH;
        }
        else if(c=='{'){ // zacatek bloku
          return LBRACKET;
        }
        else if(c=='}'){ // konec bloku
          return RBRACKET;
        }
        else if(c==','){ // konec bloku
          return COLON;
        }
        else if(c==EOF){ // konec souboru
          return END;
        }
        else{
          AddERR(line,IFJ_ERR_LEXICAL);
          return ERROR; // lexikalni chyba
        }


        break;
      //---------------------
      case 1:
        if(c=='/') state=2; //radkovy komentar
        else if(c=='*') state=3; //blokovy komentar
        else{
          ungetc(c, source);
          return DIVIDE;
        }
        break;
      case 2:
        if(c=='\n'){
          state=0; // konec radkoveho komentare
          line++;
        }
        break;
      case 3:
        if(c=='*') state=4; // kontrola */
        break;
      case 4:
        if(c=='/') state=0; // konec blokoveho komentare
        else state=3; // pokracovani blokoveho komentare
        break;
      case 5:
        if((c=='_')||((c>='a')&&(c<='z'))||((c>='A')&&(c<='Z'))||((c>='0')&&(c<='9'))){
          strAddChar(attr,c); // nacitani identifikatoru
        }
        else{ // konec identifikatoru
          ungetc(c, source);
          // kontrola zda nejde o klicove slovo
          if (strCmpConstStr(attr, "auto") == 0) return KAUTO;
	      else if (strCmpConstStr(attr, "cin") == 0) return KCIN;
	      else if (strCmpConstStr(attr, "cout") == 0) return KCOUT;
	      else if (strCmpConstStr(attr, "double") == 0) return KDOUBLE;
	      else if (strCmpConstStr(attr, "else") == 0) return KELSE;
	      else if (strCmpConstStr(attr, "for") == 0) return KFOR;
	      else if (strCmpConstStr(attr, "if") == 0) return KIF;
	      else if (strCmpConstStr(attr, "int") == 0) return KINTEGER;
	      else if (strCmpConstStr(attr, "return") == 0) return KRETURN;
	      else if (strCmpConstStr(attr, "string") == 0) return KSTRING;
	      else return ID;
        }

        break;
      case 6:
        if(c=='"') return CSTR; // konec retezce
        else if(c=='\n'){
            AddERR(line,IFJ_ERR_LEXICAL);
             return ERROR; // konec radku lexikalni chyba
        }
        else if (c=='\\') state=7; // escape sekvence
        else if(c>=' ') strAddChar(attr,c); // nacitani retezce
        else{
          AddERR(line,IFJ_ERR_LEXICAL);
          return ERROR;// lexikalni chyba
        }
        break;
      case 7:
        if(c=='"') strAddChar(attr,c);// vlozeni uvozovek do retezce
        else if(c=='n')strAddChar(attr,'\n');// vlozeni do retezce novy radek
        else if(c=='t')strAddChar(attr,'\t'); // vlozeni do retezce tabulator
        else if(c=='\\') strAddChar(attr,'\\'); // vlozeni do retezce zpetne lomitko
        else if(c=='x'){ //vlozeni do retezce znak definovany hexadecimalnim cislem ve tvaru \x00
          c=getc(source);
          if(((c>='0')&&(c<='9'))||((c>='a')&&(c<='f'))||((c>='A')&&(c<='F'))){ // nacitani hexadecimalniho cisla a konverze
            if((c>='a')&&(c<='f')) c=10+(c-'a');
            else if((c>='A')&&(c<='F')) c=10+(c-'A');
            else c=c-'0';
            hex=c*16;
            c=getc(source);
            if(!(((c>='0')&&(c<='9'))||((c>='a')&&(c<='f'))||((c>='A')&&(c<='F')))){
              AddERR(line,IFJ_ERR_LEXICAL);
              return ERROR;
            }
            if((c>='a')&&(c<='f')) c=10+(c-'a');
            else if((c>='A')&&(c<='F')) c=10+(c-'A');
            else c=c-'0';
            hex=hex+c;
            c=hex;
            strAddChar(attr,c);
          }
          else{
              AddERR(line,IFJ_ERR_LEXICAL);
            return ERROR;
          }
        }
        else{
          AddERR(line,IFJ_ERR_LEXICAL);
          return ERROR;
        }
        state=6;
        break;

      //-----------------------
      case 8:
        if(c=='0') state=8;// odstraneni nul na zacatku
        else if((c>='1')&&(c<='9')){ // nacteni nenuloveho cisla
          strAddChar(attr,c);
          state=9;
        }
        else if((c=='e')||(c=='E')){ // cislo s exponentem
          strAddChar(attr,'0');
          strAddChar(attr,c);
          state=12;
        }
        else if(c=='.'){ // desetinne cislo
          strAddChar(attr,'0');
          strAddChar(attr,c);
          state=10;
        }
        else{ // cele cislo 0
          strAddChar(attr,'0');
          ungetc(c, source);
          return CINTEGER;

        }
        break;
      case 9:
        if((c>='0')&&(c<='9')){// pokracovani nacitani cisla
          strAddChar(attr,c);
        }
        else if((c=='e')||(c=='E')){ // cislo s exponentem
          strAddChar(attr,c);
          state=12;
        }
        else if(c=='.'){ // desetinne cislo
          strAddChar(attr,c);
          state=10;
        }
        else{ // vraci se nactene cele cislo
          ungetc(c, source);
          return CINTEGER;

        }
        break;
      case 10:
        if((c>='0')&&(c<='9')){// nacteni cisla za desetinnou teckou
          strAddChar(attr,c);
          state=11;
        }
        else{
          AddERR(line,IFJ_ERR_LEXICAL);
          return ERROR;
        }
        break;
      case 11:
        if((c>='0')&&(c<='9')){ // nacteni posloupnosti cisel za desetinnou teckou
          strAddChar(attr,c);
        }
        else if((c=='e')||(c=='E')){ // desetinne cislo s exponentem
          strAddChar(attr,c);
          state=12;
        }
        else{
          ungetc(c, source);
          return CDOUBLE; //vraci nactene desetinne cislo
        }
        break;
      case 12:
        if((c=='-')||(c=='+')){// nacteni znamenka u E
          strAddChar(attr,c);
          state=13;
        }
        else if((c>='0')&&(c<='9')){ // nacteni cisla u E
          strAddChar(attr,c);
          state=14;
        }
        else{
          AddERR(line,IFJ_ERR_LEXICAL);
          return ERROR;
        }
        break;
      case 13:
        if((c>='0')&&(c<='9')){// nacteni cisla za znamenkem
          strAddChar(attr,c);
          state=14;
        }
        else{
          AddERR(line,IFJ_ERR_LEXICAL);
          return ERROR;
        }
        break;
      case 14:
        if((c>='0')&&(c<='9')){ // nacteni posloupnosti cisel za E
          strAddChar(attr,c);
        }
        else{
          ungetc(c, source);
          return CDOUBLE; // vraci realne cislo
        }
        break;

      case 15:
        if(c=='<'){
          return COUT; // operator vystupu
        }
        else if(c=='='){
          return LEQ;  // operator <=
        }
        else{
          ungetc(c, source);
          return LE;  // operator <
        }
        break;
      case 16:
        if(c=='>'){
          return CIN;  // operator >>
        }
        else if(c=='='){
          return GEQ;  // operator >=
        }
        else{
          ungetc(c, source);
          return GR; // operator >
        }
        break;
      case 17:
        if(c=='='){
          return NEQ; // operator !=
        }
        else{
          AddERR(line,IFJ_ERR_LEXICAL);
          return ERROR;
        }
        break;
      case 18:
        if(c=='='){ // operator ==
          return EQ;
        }
        else{
          ungetc(c, source);
          return ASSIGN; // operator prirazeni
        }
        break;

    }

  }

}
