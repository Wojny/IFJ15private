//+------------------------------------------------------------------+
//|	Project name:        IFJ - Team project                          |
//| Filename:            parser.c                                    |
//| Author:              Dávid Polák                                 |
//|	Encoding:            UTF-8										 |
//|	Description:         Source of Syntax analyzer.					 |
//+------------------------------------------------------------------+

#include "parser.h"


bool ReturnExist = false;
bool MainSwitch = false;
int ProgDepth = 0;
int ProgKeys=0;
string *Str0;
int line=0;
void initstr0(string **str)
{
    if(((*str=malloc(sizeof(string)))==NULL)) return ;
	strInit(*str);
	strAddChar(*str,'0');
}

int InitBD(){ //inicializace Depth Table
  if(((BDepth=malloc(sizeof (struct BD)))==NULL)) return -1;
  (BDepth)->BracketType=-1;
  (BDepth)->NextDepth=NULL;
  //BRDepth=NULL;
  return 0;
}
void AddBD(int type){// pridam nove Depth
  if(BDepth->BracketType==-1){
    BDepth->BracketType=type;
    BDepth->NextDepth=NULL;

  }
  else{
    BracketDepth BDepthpom;
    if(((BDepthpom=malloc(sizeof (struct BD)))==NULL)) return;
    BDepthpom->BracketType=type;
    BDepthpom->NextDepth=BDepth;
    BDepth=BDepthpom;


  }

}

int getBD(){ //volam ked najdu token '}'
  BracketDepth BDepthpom;
  BDepthpom=BDepth;
  BDepth=BDepth->NextDepth;
  int pom=BDepthpom->BracketType;
  free(BDepthpom);
  return pom;


}
//Return ProgKeys 0  and others from 1 to n end reset at end of function
//funct return add to 0xfunc1


bool parserProlog(string *attr, int *type)
{
	//Funkce Find(string s,string search),
	//Substr(string s , int i, int n),
	//concat(string s1, string s2),
	//Sort(string s),
	//Length(string s)
	//printf("aaaaaaa\n");
	if((*type == KSTRING) || (*type == KINTEGER) || (*type == KDOUBLE)){ // ked token je INT,STR alebo DOUBLE zavolam Initializ
		if(Initializ(attr, type) == false)
			return false;
		printf("neco");
    }
	else if(*type == ID) //ked token IDENTIFIKATOR
	{
		if(SearchBT(GST->FunRoot, attr) != NULL)//Existujuci Parameter
		{
			NextToken(attr, type);
			if(Param(attr, type) == false)
				return false;
		}
		else if(SearchFN(GST->FunRoot, attr) != NULL)//Existujuci Funkce
		{
			if(FunctionDef(attr,type) == false)
				return false;
		}
		else
		{
			AddERR(line,IFJ_ERR_SYNTAX);
			return false;
		}
	}
	else if (Statement(attr,type) == true) //ked je to Statement zavolam funkci
	{
		return true;
	}
	else //inom pripade je to ERROR
	{
		AddERR(line,IFJ_ERR_SYNTAX);
		return false;
	}
	return true;
}

bool Statement(string *attr, int *type)
{
	bool res = true;
	string Identif;
	//strInit(&Identif);
	if (strcmp(attr->str, "auto")==0)
	{
		//int typ;
		NextToken(attr,type);
		if(*type == ID)
        {
			if(SearchBT(GST->FunRoot, attr) == NULL)//Neexistujuci IDENTIFIKATOR
			{
				strCopyString(&Identif,attr);
				NextToken(attr, type);
				if(Param(attr, type) == false)
					res = false;
				else
				{

					if(BTAddID(&ActualFN,&Identif,ActualAdd2->type,ProgDepth,ProgKeys) == -1)//vratim typ a vytvorim Param
					{
							AddERR(line,IFJ_ERR_PROGRAM);
							res = false;
					}
					else
					{
						ProgKeys++;
						ActualAdd1 = SearchBT(GST->FunRoot, &Identif);
						CreateInst(I_ASSIGN,ActualAdd1,ActualAdd2,NULL,L);
					}
				}
			}
			else
			{
				AddERR(line,IFJ_ERR_SYNTAX);
				res = false;
			}
		}	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! ked param existuje nepretypuje sa ? !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		return res;
	}
	else if (strcmp(attr->str, "cout")==0)//READ
	{
		while(*type != SEMICOLON)// kim nenajde konec ";"
		{
			NextToken(attr,type);
			if(*type == COUT)// ked je to "<<"
			{
				NextToken(attr,type);
				if(*type == CSTR || *type == CINTEGER || *type == CDOUBLE || *type == ID)
				{
					if(isConstant(*type) == 1)
						ActualAdd1=createConst(&newCTable,*type,attr);
					else if((ActualAdd1=SearchBT(ActualFN, attr)) == NULL)
					{
						AddERR(line,IFJ_ERR_SYNTAX);
						res = false;
					}
				}
				CreateInst(I_WRITE,ActualAdd1,NULL,NULL,L);
			}
			else if(*type == SEMICOLON)
				break;
			else
			{
				AddERR(line,IFJ_ERR_SYNTAX);
				res = false;
			}
			if( *type == END)
				break;
		}
		return res;
	}
	else if (strcmp(attr->str,"cin")==0)//FUNKCE WRITE
	{
		while(*type != SEMICOLON)// not strcmp but return only TYPE of it
		{
			NextToken(attr,type);
			if(*type == CIN)// ked je to ">>"
			{				
				NextToken(attr,type);
				if(*type == ID)
				{
					if((ActualAdd1=SearchBT(GST->FunRoot, attr)) != NULL)//Existujuci IDENTIFIKATOR
						CreateInst(I_READ,attr,NULL,NULL,L);
					else
					{
						AddERR(line,IFJ_ERR_SYNTAX);
						res = false;
					}
				}
				else
				{
					AddERR(line,IFJ_ERR_SYNTAX);
					res = false;
				}
			}
			else if(*type == SEMICOLON)
				break;
			else
			{
				AddERR(line,IFJ_ERR_SYNTAX);
				res = false;
			}
			if( *type == END)
				break;
		}
		return res;
	}
	else if (strcmp(attr->str,"return")==0)//FUNKCE RETURN
	{
		ReturnExist = true;
		NextToken(attr,type);
		if(*type == CSTR || *type == CINTEGER || *type == CDOUBLE || *type == ID)
		{
			if(*type == ID)
			{
				if((ActualAdd1=SearchBT(GST->FunRoot, attr)) == NULL)//Existujuci IDENTIFIKATOR
				{
					AddERR(line,IFJ_ERR_SYNTAX);
					res = false;
				}
			}
			else
				ActualAdd1=createConst(&newCTable,*type,attr);

			CreateInst(I_RETURN,Str0,ActualAdd1,NULL,L);
		}
		else
		{
			AddERR(line,IFJ_ERR_SYNTAX);
			res = false;
		}

		NextToken(attr,type);
		if(*type != SEMICOLON )
		{
			AddERR(line,IFJ_ERR_SYNTAX);
			res = false;
		}
		return res;
	}
	else if (strcmp(attr->str,"if")==0)//FUNKCE IF
	{
		NextToken(attr, type);
		//control if is function
		if(*type == LPARENTH)
		{
			CreateInst(I_IF,NULL,NULL,NULL,L);
			NextToken(attr, type);
            if(*type == CSTR || *type == CINTEGER || *type == CDOUBLE || *type == ID || *type == LPARENTH)
            {

                if(*type == ID)
                {
                    if(FunctionDef(attr,type) == true)
                        return true;
                    else
                    {
                        if(SearchBT(GST->FunRoot, attr) != NULL)
                        {
                            sTree SynSemTree = syn_exp(*type,attr,ActualFN,newCTable,RPARENTH);
                            if(SynSemTree == NULL)
                                res = false;
                            if(sem_sym(SynSemTree) != -1 )
                                ActualAdd2=SynSemTree->node;
                            else
                                res = false;
                        }
                        else
                        {
                            AddERR(line,IFJ_ERR_SYNTAX);
                            res = false;
                        }
                    }

                }
                else
                {
                    sTree SynSemTree = syn_exp(*type,attr,ActualFN,newCTable,RPARENTH);
					if(SynSemTree == NULL)
						return false;
					if(sem_sym(SynSemTree) != -1 )
						ActualAdd2=SynSemTree->node;
					else
						res = false;
                }
            }
			else
			{
                AddERR(line,IFJ_ERR_SYNTAX);
                res = false;
			}
			//!!!!!!!!!!!!!!!!!!!!!!!!!1asi potrebuje Nexttoken aby bol testovan lebo syn_exp prebere to do ')' !!!!!!!!!!!!!!!!!!!!!!!!!! 
			if(*type == LBRACKET)
			{
				CreateInst(I_IF_COND,NULL,NULL,NULL,L);
				AddBD(I_IF);
				ProgDepth++;
			}
			else
			{
				AddERR(line,IFJ_ERR_SYNTAX);
				res = false;
			}

		}
		else
		{
			AddERR(line,IFJ_ERR_SYNTAX);
			res = false;
		}
		return res;
	}
	else if(strcmp(attr->str,"else")==0)//FUNKCE ELSE
	{
		NextToken(attr,type);
		if(*type == LBRACKET)
		{
			CreateInst(I_ELSE,NULL,NULL,NULL,L);
			AddBD(I_ELSE);
			ProgDepth++;
		}
		else
		{
			AddERR(line,IFJ_ERR_SYNTAX);
			res = false;
		}
		return res;
	}
	else if (strcmp(attr->str,"for")==0)//FUNKCE FOR
	{
		NextToken(attr,type);
		if(*type == LPARENTH)
		{

			CreateInst(I_FOR,NULL,NULL,NULL,L);
			NextToken(attr,type);
			if(parserProlog(attr,type) == false)//for( int a ; ; )
			{
				AddERR(line,IFJ_ERR_SYNTAX);
				res = false;
			}
			CreateInst(I_FOR_CHCK,NULL,NULL,NULL,L);


			NextToken(attr,type);
			sTree SynSemTree = syn_exp(*type,attr,ActualFN,newCTable,SEMICOLON);//for( ; a < 8 ; )
			if(SynSemTree == NULL)
				res = false;
			if(sem_sym(SynSemTree) != -1 )
				ActualAdd2=SynSemTree->node;
			CreateInst(I_FOR_DIFF,NULL,NULL,NULL,L);


			NextToken(attr,type);
			if(parserProlog(attr,type) == false)//for( ; ; a = a + 1 )
			{
				AddERR(line,IFJ_ERR_SYNTAX);
				res = false;
			}

			NextToken(attr,type); // 'for(;;) {' -> '{'
			if(*type == LBRACKET)
			{
				CreateInst(I_FOR_COND,NULL,NULL,NULL,L);
				AddBD(I_FOR);
				ProgDepth++;
			}
			else
			{
				AddERR(line,IFJ_ERR_SYNTAX);
				res = false;
			}

		}
		else
		{
			AddERR(line,IFJ_ERR_SYNTAX);
			res = false;
		}
		return res;
	}
	else if (*type == LBRACKET) // ked je to blok '{'
	{
		AddBD(200);
		ProgDepth++;
		return res;
	}
	else if (*type == RBRACKET)
	{

		if(BTDelete(&ActualFN->BTroot,ProgDepth,&ActualFN->tempSTable) == -1)
			AddERR(line,IFJ_ERR_PROGRAM);
		int EndBracket = getBD();
		if(EndBracket == I_IF)
			CreateInst(I_END_IF,NULL,NULL,NULL,L);
		else if(EndBracket == I_FOR)
			CreateInst(I_END_FOR,NULL,NULL,NULL,L);
		else if(EndBracket == I_ELSE)
			CreateInst(I_END_ELSE,NULL,NULL,NULL,L);
		else if(EndBracket == I_FUNC)
			CreateInst(I_END_FUNC,NULL,NULL,NULL,L);
		else if(EndBracket == I_MAIN)
			CreateInst(I_END_MAIN,NULL,NULL,NULL,L);

		ProgDepth--;
		if(ProgDepth == 0)
		{
			if(ReturnExist == false)
				AddERR(line,8);
			ProgKeys = 0;
			ReturnExist = false;
		}
		return res;
	}
	else
	{
		AddERR(line,IFJ_ERR_SYNTAX);
		return false;
	}

}

bool FunctionDef(string *attr, int *type)
{
	int ParamCounter = 0;
	int ParamType;
	bool res = true;
	if (strcmp(attr->str,"lenght")==0)//vytvori instrukci  predem a uzivatelem deklarovanej funkci a priradi tam adresi
	{		
		NextToken(attr,type);
		if( *type == LPARENTH)
		{
			if(IALFunctCall(attr, type, &ParamCounter, CSTR) == true)//ActualAdd2 sa uklada aktualne spracovani parameter
			{
				if(ParamCounter == 1)
					CreateInst(I_LENGHT, NULL, NULL, NULL,L);
			}
			else
			{
				AddERR(line,IFJ_ERR_SYNTAX);
				res = false;
			}
		}
		else
		{
			AddERR(line,IFJ_ERR_SYNTAX);
			res = false;
		}//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! tomto stavu u vsech je len u konca ')' !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	}
	else if (strcmp(attr->str,"substr")==0)
	{
		NextToken(attr,type);
		if( *type == LPARENTH)
		{
			for(ParamCounter =0; ParamCounter < 3;)
			{
				switch(ParamCounter)
				{
					case 0:
						ParamType = CSTR;
						break;
					case 1:
						ParamType = CINTEGER;
						break;
					case 2:
						ParamType = CINTEGER;
						break;
				}
				if(IALFunctCall(attr, type, &ParamCounter, ParamType) == false)
				{
					AddERR(line,IFJ_ERR_SYNTAX);
					res = false;
				}

			}
			CreateInst(I_SUBSTR, NULL, NULL, NULL,L);//!!!!!!!!!!!!!!!!!!!!!!!!!!!!! adressi este nie su zavolane !!!!!!!!!!!!!!!!!!!!!!!!!!!
		}
		else
		{
			AddERR(line,IFJ_ERR_SYNTAX);
			res = false;
		}
	}
	else if (strcmp(attr->str,"concat")==0)
	{
		NextToken(attr,type);
		if( *type == LPARENTH)
		{
			for(ParamCounter =0; ParamCounter < 2;)
			{
				switch(ParamCounter)
				{
					case 0:
						ParamType = CSTR;
						break;
					case 1:
						ParamType = CSTR;
						break;
				}
				if(IALFunctCall(attr, type, &ParamCounter, ParamType) == false)
				{
					AddERR(line,IFJ_ERR_SYNTAX);
					res = false;
				}
			}
			CreateInst(I_CONCAT, attr, NULL, NULL,L);
		}
		else
		{
			AddERR(line,IFJ_ERR_SYNTAX);
			res = false;
		}
	}
	else if (strcmp(attr->str,"find")==0)
	{
		NextToken(attr,type);
		if( *type == LPARENTH)
		{
			for(ParamCounter =0; ParamCounter < 2;)
			{
				switch(ParamCounter)
				{
					case 0:
						ParamType = CSTR;
						break;
					case 1:
						ParamType = CSTR;
						break;
				}
				if(IALFunctCall(attr, type, &ParamCounter, ParamType) == false)
				{
					AddERR(line,IFJ_ERR_SYNTAX);
					res = false;
				}
			}
			CreateInst(I_FIND, attr, NULL, NULL,L);
		}
		else
		{
			AddERR(line,IFJ_ERR_SYNTAX);
			res = false;
		}

	}
	else if (strcmp(attr->str,"sort")==0)
	{
		ParamCounter =0;
		NextToken(attr,type);
		if( *type == LPARENTH)
		{
			if(IALFunctCall(attr, type, &ParamCounter, CSTR) == true)
			{
				if(ParamCounter == 1)
					CreateInst(I_SORT, attr, NULL, NULL,L);
			}
			else
			{
				AddERR(line,IFJ_ERR_SYNTAX);
				res = false;
			}
		}
		else
		{
			AddERR(line,IFJ_ERR_SYNTAX);
			res = false;
		}
	}
	else if (SearchFN(GST->FunRoot, attr) != NULL)
	{
		CreateInst(I_CREATE_BLOCK,attr,NULL,NULL,L);
		if(FunctCall(attr,type) == false)
		{
			AddERR(line,IFJ_ERR_SYNTAX);
			res = false;
		}
		CreateInst(I_CALL, NULL, NULL, NULL,L);
		CreateInst(I_GET_RETURN, ActualAdd2, NULL, NULL,L);
	}
	else
	{
		AddERR(line,IFJ_ERR_SYNTAX);
		res = false;
	}
	return res;
}

bool IALFunctCall(string *attr, int *type, int ParamCounter, int ParamType)
{
	bool res = true;
  	NextToken(attr,type);
	if(*type == CSTR || *type == CINTEGER || *type == CDOUBLE || *type == ID)
	{
		ParamCounter++;//vrati sa do FunctionDef uz ako zvyseni
		if(isConstant(*type) == 1)
		{
			if(ParamType != *type)
			{
				AddERR(line,IFJ_ERR_SEMANTIC);
				res = false;
			}

			ActualAdd2=createConst(&newCTable,*type,attr);
			CreateInst(I_ASSIGNPARAM,&ParamCounter,ActualAdd2,NULL,L);
		}
		else if((ActualAdd2=SearchBT(ActualFN, attr)) == NULL)
		{
			AddERR(line,IFJ_ERR_SYNTAX);
			res = false;
		}
		else
		{
			if(ParamType != ActualAdd2->type)
			{
				AddERR(line,IFJ_ERR_SEMANTIC);
				res = false;
			}
			CreateInst(I_ASSIGNPARAM,&ParamCounter,ActualAdd2,NULL,L);
		}
	}

	if(NextToken(attr,type) == 0)
	{
	 	if(*type == RPARENTH || *type == COLON)//NextToken musi byt "," alebo ")" inac ERROR
			return res;
		else
		{
			AddERR(line,IFJ_ERR_SYNTAX);
			res = false;
		}
	}
	else
	{
		AddERR(line,IFJ_ERR_PROGRAM);
		res = false;
	}
	return res;
}

bool FunctCall(string *attr, int *type)
{
	bool res = true;
	if(*type != LPARENTH)
		NextToken(attr,type);
	if(*type == LPARENTH)
	{
		int ParamNum = 0;
		while(*type != RPARENTH)//volam do okola kim ne narazim na konec funkcnich param
  		{
			ParamNum++;
			NextToken(attr,type);
			if(*type == CSTR || *type == CINTEGER || *type == CDOUBLE || *type == ID)
			{
				if(isConstant(*type) == 1)
				{
					ActualAdd2=createConst(&newCTable,*type,attr);
					CreateInst(I_ASSIGNPARAM,&ParamNum,ActualAdd2,NULL,L);
				}
				else if((ActualAdd2=SearchBT(ActualFN, attr)) == NULL)
				{
					AddERR(line,IFJ_ERR_SYNTAX);
					res = false;
				}
				else
					CreateInst(I_ASSIGNPARAM,&ParamNum,ActualAdd2,NULL,L);
			}
			else
			{
				AddERR(line,IFJ_ERR_PROGRAM);
				res = false;
			}

			if(NextToken(attr,type) == 0)
			{
			 	if( *type == END)
					break;
			 	if(*type == RPARENTH || *type == COLON)//NextToken musi byt "," alebo ")" inac ERROR
					continue;
				else
				{
					AddERR(line,IFJ_ERR_SYNTAX);
					res = false;
					continue;
				}
			}
			else
			{
				AddERR(line,IFJ_ERR_PROGRAM);
				res = false;
				continue;
			}

		}
	}
	else
	{
		AddERR(line,IFJ_ERR_SYNTAX);
		res = false;
	}
	return res;
}

bool Param(string *attr, int *type)//ked existujem parameter zavolame tuto funkci ked je to premenne tak zavola sa percendence inom pripade moze bit to funkce
{
	bool res = true;
	if(*type == ASSIGN)// '='
	{
		NextToken(attr, type);
		//control if is function
		if(*type == CSTR || *type == CINTEGER || *type == CDOUBLE || *type == ID || *type == LPARENTH)
		{
			if(*type == ID)
			{
				if(FunctionDef(attr,type) == true)
					return true;
				else
				{
					if(SearchBT(GST->FunRoot, attr) != NULL)
					{
						sTree SynSemTree = syn_exp(*type,attr,ActualFN,newCTable,SEMICOLON);
						if(SynSemTree == NULL)
							return false;
						if(sem_sym(SynSemTree) != -1 )
							ActualAdd2=SynSemTree->node;
						else
							res = false;


					}
					else
					{
						AddERR(line,IFJ_ERR_SYNTAX);
						res = false;
					}
				}
			}
			else //ked nie je ID tak moze byt CINT,CSTR,CDOUBLE alebo '('
			{				
				sTree SynSemTree = syn_exp(*type,attr,ActualFN,newCTable,SEMICOLON); //syn_exp zaciklenie
				if(SynSemTree == NULL)
					return false;
				if(sem_sym(SynSemTree) != -1 )
					ActualAdd2=SynSemTree->node;
				else
					res = false;
			}
		}
		else
		{
			AddERR(line,IFJ_ERR_SYNTAX);
			res = false;
		}

	}
	else
	{
		AddERR(line,IFJ_ERR_SYNTAX);
		res = false;
	}
	return res;
}

bool FunctionParams(string *attr, int *type, int typFN, string *Identif)
{
	bool res = true;
	if((ActualFN=GSTadd(&GST, Identif, typFN)) != NULL)
	{
		ProgDepth++;
		addFunInst(&ActualFN, L->last);
		if(BTAddID(&ActualFN,Str0,typFN,ProgDepth,ProgKeys) == -1)//Param Str0 je navratova hodnota funkce
		{
			AddERR(line,IFJ_ERR_PROGRAM);
			res = false;
		}
		else
			ProgKeys++;
		while(*type != RPARENTH)//volam do okola kim ne narazim na konec funkcnich param
  		{			
			NextToken(attr,type);
			if(*type == KSTRING || *type == KINTEGER || *type == KDOUBLE || *type == RPARENTH)
				switch (*type)
				{
					case KSTRING:
						NextToken(attr,type);
						if(type == ID)
						{
							if(BTAddID(&ActualFN,attr,KSTRING,ProgDepth,ProgKeys) == -1)//ked IDENTIFIKATOR je uspesne ukladan tak zvysim ProgKeys
							{
								AddERR(line,IFJ_ERR_PROGRAM);
								res = false;
							}
							else
							{
								addFunType(&ActualFN, KSTRING);
								ProgKeys++;
							}
							if(NextToken(attr,type) == 0)
							{
							 	if(*type == RPARENTH || *type == COLON)//NextToken musi byt "," alebo ")" inac ERROR
									break;
								else
								{
									AddERR(line,IFJ_ERR_SYNTAX);
									res = false;
									break;
								}
							}
							else
							{
								AddERR(line,IFJ_ERR_PROGRAM);
								res = false;
								break;
							}
						}
						else
						{
							AddERR(line,IFJ_ERR_SYNTAX);
							res = false;
							break;
						}
					case KDOUBLE:
						NextToken(attr,type);
						if(type == ID)
						{
							if(BTAddID(&ActualFN,attr,KDOUBLE,ProgDepth,ProgKeys) == -1)//ked IDENTIFIKATOR je uspesne ukladan tak zvysim ProgKeys
							{
								AddERR(line,IFJ_ERR_PROGRAM);
								res = false;
							}
							else
							{
								addFunType(&ActualFN, KDOUBLE);
								ProgKeys++;
							}
							if(NextToken(attr,type) == 0)
							{
							 	if(*type == RPARENTH || *type == COLON)//NextToken musi byt "," alebo ")" inac ERROR
									break;
								else
								{
									AddERR(line,IFJ_ERR_SYNTAX);
									res = false;
									break;
								}
							}
							else
							{
								AddERR(line,IFJ_ERR_PROGRAM);
								res = false;
								break;
							}
						}
						else
						{
							AddERR(line,IFJ_ERR_SYNTAX);
							res = false;
							break;
						}
					case KINTEGER:
						NextToken(attr,type);
						if(*type == ID)
						{
							if(BTAddID(&ActualFN,attr,KINTEGER,ProgDepth,ProgKeys) == -1)//ked IDENTIFIKATOR je uspesne ukladan tak zvysim ProgKeys
							{
								AddERR(line,IFJ_ERR_PROGRAM);
								res = false;
							}
							else
							{
								addFunType(&ActualFN, KINTEGER);
								ProgKeys++;
							}
							if(NextToken(attr,type) == 0)
							{							 	
							 	if(*type == RPARENTH || *type == COLON)//NextToken musi byt "," alebo ")" inac ERROR
									break;
								else
								{
									AddERR(line,IFJ_ERR_SYNTAX);
									res = false;
									break;
								}
							}
							else
							{
								AddERR(line,IFJ_ERR_PROGRAM);
								res = false;
								break;
							}
						}
						else
						{
							AddERR(line,IFJ_ERR_SYNTAX);
							res = false;
							break;
						}
					case RPARENTH:
						break;

				}
			else
			{
				AddERR(line,IFJ_ERR_SYNTAX);
				res = false;
			}
		if( *type == END)
			break;
		}
		if (strcmp(Identif->str,"main")==0)
			CreateInst(I_MAIN, NULL, NULL, NULL,L);
		else
			CreateInst(I_FUNC, Identif, NULL, NULL,L);

		while(1)//Bude vypisovat ERROR kim ne najde ; alebo {
		{
			if(NextToken(attr,type) == 0 && (*type == LBRACKET || *type == SEMICOLON || *type == END))
			{
				if(*type == SEMICOLON)
				{
					ProgDepth--;
					break;
				}
				else
				{

					if(strcmp(Identif->str,"main")==0)
						AddBD(I_MAIN);
					else
						AddBD(I_FUNC);
					break;
				}
			}
			else
			{
				AddERR(line,IFJ_ERR_SYNTAX);
				res = false;
			}
		}
	}
	else
	{
		AddERR(line,IFJ_ERR_PROGRAM);
		res = false;
	}	
	return res;
}

bool Initializ(string *attr, int *type)
{
	string Identif;	
	bool res = true;
	//pri deklaraci KINT ...   //CINT ked je cislo //IINTEGER typ promeny == identifikator
	switch (*type)
	{
		case KSTRING:
			NextToken(attr, type);
			if(*type == ID) //Skontrolujeme ze je to IDENTIFIKATOR, ked nie tak ERROR
			{
				strCopyString(&Identif,attr);
				if(strcmp(attr->str,"main")==0)//IDENTIFIKATOR je main
				{
					//printf("ahoj");
					NextToken(attr, type);
					if(*type == LPARENTH)
					{
						if(MainSwitch == false)//ked exituje uz main tak vraci ERROR
						{
							MainSwitch = true;
							if(FunctionParams(attr,type,KSTRING,&Identif) == false)
							{
								AddERR(line,IFJ_ERR_SYNTAX);
								res = false;
							}
						}
						else
						{
							AddERR(line,IFJ_ERR_SYNTAX);
							res = false;
						}
					}
					else//Spatne nadeklarovana main funkce
					{
						AddERR(line,IFJ_ERR_SYNTAX);
						res = false;
					}
				}
				else
				{
					NextToken(attr, type);
					if(*type == LPARENTH)//INDETIFIKATOR je meno Funkce
					{
						if(SearchFN(GST->FunRoot, &Identif) == NULL)//kontrolujem ze bol uz vytvoren tato funkce alebo ne
						{
							if(FunctionParams(attr,type,KSTRING,&Identif) == false)
							{
								AddERR(line,IFJ_ERR_SYNTAX);
								res = false;
							}
						}
						else
						{
							if(FunctionParams(attr,type,KSTRING,&Identif))
							{
								if(isParamEqual(GST,ActualFN) == 1)
								{
									if(isMultipleDefinedFun(GST,ActualFN)== 1)
									{
										AddERR(line,IFJ_ERR_SEMANTIC);
										res = false;
									}
								}
								else
								{
									AddERR(line,IFJ_ERR_SEMANTIC);
									res = false;
								}
							}
							else
							{
								AddERR(line,IFJ_ERR_SYNTAX);
								res = false;
							}
						}
					}
					else if(*type == SEMICOLON)//IDNTIFIKATOR je meno premenne
					{

						if(BTAddID(&ActualFN,&Identif,KSTRING,ProgDepth,ProgKeys) == -1)//ked IDENTIFIKATOR je uspesne ukladan tak zvysim ProgKeys
						{
							AddERR(line,IFJ_ERR_PROGRAM);
							res = false;
						}
						else
							ProgKeys++;

					}
					else if(*type == ASSIGN)//IDNTIFIKATOR je meno premenne a dalsi token je "="
					{
						if(BTAddID(&ActualFN,&Identif,KSTRING,ProgDepth,ProgKeys) == -1)//ked IDENTIFIKATOR je uspesne ukladan tak zvysim ProgKeys
						{
							AddERR(line,IFJ_ERR_PROGRAM);
							res = false;
						}
						else
						{
							ActualAdd1 = SearchBT(GST->FunRoot, &Identif);//pridat do IINTEGER a IDOUBLE
							ProgKeys++;
						}
						if(Param(attr, type) == false)
							res = false;
						else
							CreateInst(I_ASSIGN,ActualAdd1,ActualAdd2,NULL,L);


					}
					else
					{
						AddERR(line,IFJ_ERR_SYNTAX);
						res = false;
					}
				}
			}
			else
			{
				AddERR(line,IFJ_ERR_SYNTAX);
				res = false;
			}
			break;
		case KINTEGER:
			NextToken(attr, type);
			if(*type == ID)
			{				
				strCopyString(&Identif,attr);
				if(strcmp(attr->str,"main") == 0)//IDENTIFIKATOR je main depth=0 must be
				{
					NextToken(attr, type);
					if(*type == LPARENTH)
					{
						if(MainSwitch == false)//ked exituje uz main tak vraci ERROR
						{
							MainSwitch = true;
							if(FunctionParams(attr,type,KINTEGER,&Identif) == false)
							{
								AddERR(line,IFJ_ERR_SYNTAX);
								res = false;
							}
						}
						else
						{
							AddERR(line,IFJ_ERR_SYNTAX);
							res = false;
						}
					}
					else//Spatne nadeklarovana main funkce
					{
						AddERR(line,IFJ_ERR_SYNTAX);
						res = false;
					}
				}
				else
				{
					NextToken(attr, type);
					if(*type == LPARENTH)//INDETIFIKATOR je meno Funkce
					{
						
						if(SearchFN(GST->FunRoot, &Identif) == NULL)//kontrolujem ze bol uz vytvoren tato funkce alebo ne
						{
							if(FunctionParams(attr,type,KINTEGER,&Identif) == false)
							{								
								AddERR(line,IFJ_ERR_SYNTAX);
								res = false;
							}
						}
						else
						{
							if(FunctionParams(attr,type,KINTEGER,&Identif))
							{
								if(isParamEqual(GST,ActualFN) == 1)
								{
									if(isMultipleDefinedFun(GST,ActualFN)== 1)
									{
										AddERR(line,IFJ_ERR_SEMANTIC);
										res = false;
									}
								}
								else
								{
									AddERR(line,IFJ_ERR_SEMANTIC);
									res = false;
								}
							}
							else
							{
								AddERR(line,IFJ_ERR_SYNTAX);
								res = false;
							}
						}
					}
					else if(*type == SEMICOLON)//IDNTIFIKATOR je meno premenne
					{
						if(BTAddID(&ActualFN,&Identif,KINTEGER,ProgDepth,ProgKeys) == -1)//ked IDENTIFIKATOR je uspesne ukladan tak zvysim ProgKeys
						{
							AddERR(line,IFJ_ERR_PROGRAM);
							res = false;
						}
						else
							ProgKeys++;
					}
					else if(*type == ASSIGN)//IDNTIFIKATOR je meno premenne a dalsi token je "="
					{
						if(BTAddID(&ActualFN,&Identif,KINTEGER,ProgDepth,ProgKeys) == -1)//ked IDENTIFIKATOR je uspesne ukladan tak zvysim ProgKeys
						{
							AddERR(line,IFJ_ERR_PROGRAM);
							res = false;
						}
						else
						{
							ActualAdd1 = SearchBT(GST->FunRoot, &Identif);//pridat do IINTEGER a IDOUBLE
							ProgKeys++;
						}
						if(Param(attr, type) == false)
							res = false;
						else
							CreateInst(I_ASSIGN,ActualAdd1,ActualAdd2,NULL,L);


					}
					else
					{
						AddERR(line,IFJ_ERR_SYNTAX);
						res = false;						
					}
				}
			}
			else
			{
				AddERR(line,IFJ_ERR_SYNTAX);
				res = false;
			}
			break;
		case KDOUBLE:
			NextToken(attr, type);
			if(*type == ID)
			{
				strCopyString(&Identif,attr);
				if(strcmp(attr->str,"main") == 0)//IDENTIFIKATOR je main
				{
					NextToken(attr, type);
					if(*type == LPARENTH)
					{
						if(MainSwitch == false)//ked exituje uz main tak vraci ERROR
						{
							MainSwitch = true;
							if(FunctionParams(attr,type,KDOUBLE,&Identif) == false)
							{
								AddERR(line,IFJ_ERR_SYNTAX);
								res = false;
							}
						}
						else
						{
							AddERR(line,IFJ_ERR_SYNTAX);
							res = false;
						}
					}
					else//Spatne nadeklarovana main funkce
					{
						AddERR(line,IFJ_ERR_SYNTAX);
						res = false;
					}
				}
				else
				{
					NextToken(attr, type);
					if(*type == LPARENTH)//INDETIFIKATOR je meno Funkce
					{
						if(SearchFN(GST->FunRoot, &Identif) == NULL)//kontrolujem ze bol uz vytvoren tato funkce alebo ne
						{
							if(FunctionParams(attr,type,KDOUBLE,&Identif) == false)
							{
								AddERR(line,IFJ_ERR_SYNTAX);
								res = false;
							}
						}
						else
						{
							if(FunctionParams(attr,type,KDOUBLE,&Identif))
							{
								if(isParamEqual(GST,ActualFN) == 1)
								{
									if(isMultipleDefinedFun(GST,ActualFN)== 1)
									{
										AddERR(line,IFJ_ERR_SEMANTIC);
										res = false;
									}
								}
								else
								{
									AddERR(line,IFJ_ERR_SEMANTIC);
									res = false;
								}
							}
							else
							{
								AddERR(line,IFJ_ERR_SYNTAX);
								res = false;
							}
						}
					}
					else if(*type == SEMICOLON)//IDNTIFIKATOR je meno premenne
					{
						if(BTAddID(&ActualFN,&Identif,KDOUBLE,ProgDepth,ProgKeys) == -1)//ked IDENTIFIKATOR je uspesne ukladan tak zvysim ProgKeys
						{
							AddERR(line,IFJ_ERR_PROGRAM);
							res = false;
						}
						else
							ProgKeys++;


					}
					else if(*type == ASSIGN)//IDNTIFIKATOR je meno premenne a dalsi token je "="
					{
						if(BTAddID(&ActualFN,&Identif,KDOUBLE,ProgDepth,ProgKeys) == -1)//ked IDENTIFIKATOR je uspesne ukladan tak zvysim ProgKeys
						{
							AddERR(line,IFJ_ERR_PROGRAM);
							res = false;
						}
						else
						{
							ActualAdd1 = SearchBT(GST->FunRoot, &Identif);//pridat do IINTEGER a IDOUBLE
							ProgKeys++;
						}
						if(Param(attr, type) == false)
							res = false;
						else
							CreateInst(I_ASSIGN,ActualAdd1,ActualAdd2,NULL,L);
					}
					else
					{
						AddERR(line,IFJ_ERR_SYNTAX);
						res = false;
					}
				}
			}
			else
			{
				AddERR(line,IFJ_ERR_SYNTAX);
				res = false;
			}
			break;
	}
return res;
}

bool parser()
{
	bool res = true;
    string *attr;
    if(((attr=malloc(sizeof(string)))==NULL)) return false; // allokace attr
    strInit(attr);
    initstr0(&Str0);
    int *type;
    if(((type=malloc(sizeof(int)))==NULL)) return false; // allokace type
    if(((L=malloc(sizeof(tList)))==NULL)) return false; // allokace L list
    setL(L);
	if (GSTinit(&GST) == -1)	// inicializace GST tabulky
	{
		AddERR(line, IFJ_ERR_PROGRAM);
		res = false;
	}
	else if(constTableInit(&newCTable) == -1) // inicializace Constante tabulky
	{
		AddERR(line, IFJ_ERR_PROGRAM);
		res = false;
	}
	else if(InitBD() == -1)	//inicializace Depth Table
		AddERR(line, IFJ_ERR_PROGRAM);
	else
	{
		NextToken(attr,type); //prvy token
		while(*type != END) // while kym ne najde END_OF_FILE
		{
			//printf("aaaaaaaaaa\n");
			bool asd = true;
			asd= parserProlog(attr,type);
			NextToken(attr,type);
			printf(" %d \n",*type );
			if (asd == false)
               	printf("ahoj ");
            else
                printf("cau ");
		}

	//	res = false;
	}
	return res;

}

int NextToken(string *attr, int *type)
{
	*type = getNextToken(attr);
	if(*type == -1)
		return -1;
	else
		return 0;
}
