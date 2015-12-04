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
void initstr0(string *str)
{
	strInit(str);
	strAddChar(str,'0');
}

int InitBD(struct BD *BRDepth){
  if(((BRDepth=malloc(sizeof (struct BD)))==NULL)) return -1;
  BRDepth->BracketType=-1;
  BRDepth->NextDepth=NULL;
  //BRDepth=NULL;
  return 0;
}
void AddBD(int type){
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

int getBD(){
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

	if(*type == KSTRING || *type == KINTEGER || *type == KDOUBLE)
		if(!Initializ(attr, type))
			return false;
	else if(*type == ID)
	{
		if(SearchBT(GST->FunNode, attr) != NULL)//Existujuci IDENTIFIKATOR
		{
			NextToken(attr, type);
			if(Param(attr, type) == -1)
				return false;
		}
		else if(SearchFN(GST->FunRoot, attr) != NULL)//Existujuci Funkce
		{
			//CreateInst(I_CALL, attr->str, NULL, NULL);
			if(!FunctionDef(attr,type))
				return false;
		}
		else
			{
				AddERR(line,IFJ_ERR_SYNTAX);
				return false;
			}
	}
	else if (!Statement(attr,type))
	{
		return true;
	}
	else
	{
		AddERR(line,IFJ_ERR_SYNTAX);
		return false;
	}
}

bool Statement(string *attr, int *type)
{
	if (strcmp(attr->str, "auto"))
	{
		int typ;
		NextToken(attr,type);
		if(*type == ID)
			if(SearchBT(GST->FunNode, attr) == NULL)//Existujuci IDENTIFIKATOR
			{
				string Identif = attr->str;
				NextToken(attr, type);
				if(Param(attr, type) == -1)
					return false;
				else
				{

					if(BTAddID(ActualFN,Identif,ActualAdd2->type,ProgDepth,ProgKeys) == -1)//vratim typ
					{
							AddERR(line,IFJ_ERR_PROGRAM);
							return false;
					}
					else
					{
						ProgKeys++;
						ActualAdd1 = SearchBT(GST->FunNode, Identif);
						CreateInst(I_ASSIGN,ActualAdd1,ActualAdd2,NULL);
						return true
					}
				}
			}

	}
	else if (strcmp(attr->str, "cout"))
	{
		while(*type != SEMICOLON)
		{
			NextToken(attr,type);
			if(*type == COUT)
			{
				NextToken(attr,type);
				if(*type == CSTRING || *type == CINTEGER || *type == CDOUBLE || *type == ID)
				{
					if(isConstant(*type) == 1)
						ActualAdd1=createConst(&newCTable,*type,attr);
					else if((ActualAdd1=SearchBT(ActualFN, attr)) == NULL)
						AddERR(line,IFJ_ERR_SYNTAX);

				}
				CreateInst(I_WRITE,ActualAdd1,NULL,NULL);
			}
			else
			{
				AddERR(line,IFJ_ERR_SYNTAX);
			}

		}
		return true;
	}
	else if (strcmp(attr->str,"cin"))//FUNKCE CIN
	{
		while(*type != SEMICOLON)// not strcmp but return only TYPE of it
		{
			NextToken(attr,type);
			if(*type == CIN)
			{
				NextToken(attr,type);
				if(*type == ID)
				{
					if((ActualAdd1=SearchBT(GST->FunNode, attr)) != NULL)//Existujuci IDENTIFIKATOR
					{
						CreateInst(I_READ,attr,NULL,NULL);
						return true;
					}
					else
						AddERR(line,IFJ_ERR_SYNTAX);
				}
				else
					AddERR(line,IFJ_ERR_SYNTAX);
			}
		}
		return true;
		//CreateInst(I_CIN,attr,NULL,NULL);
	}
	else if (strcmp(attr->str,"return"))//FUNKCE RETURN
	{
		ReturnExist = true;
		NextToken(attr,type);
		if(*type == CSTRING || *type == CINTEGER || *type == CDOUBLE || *type == ID)
		{
			if(*type == ID)
				{
					if((ActualAdd1=SearchBT(GST->FunNode, attr)) == NULL)//Existujuci IDENTIFIKATOR
						AddERR(line,IFJ_ERR_SYNTAX);
				}
			else
				ActualAdd1=createConst(&newCTable,*type,attr);

			CreateInst(I_RETURN,*Str0,ActualAdd1,NULL);
		}
		else
			AddERR(line,IFJ_ERR_SYNTAX);

		NextToken(attr,type);
		if(*type != SEMICOLON )
		{
			AddERR(line,IFJ_ERR_SYNTAX);
			return false;
		}
		else
			return true;
	}
	else if (strcmp(attr->str,"if"))//FUNKCE IF
	{
		NextToken(attr, type);
		//control if is function
		if(*type == LPARENTH)
		{
			CreateInst(I_IF,NULL,NULL,NULL);
			NextToken(attr, type);
            if(*type == CSTRING || *type == CINTEGER || *type == CDOUBLE || *type == ID || *type == LPARENTH)
            {

                if(*type == ID)
                {
                    if(!FunctionDef(attr,type))
                        return true;
                    else
                    {
                        if(SearchBT(GST->FunNode, attr) != NULL)
                        {
                            sTree SynSemTree = syn_exp(*type,attr,ActualFN,newCTable,RPARENTH);
                            if(SynSemTree == NULL)
                                return false;
                            if(sem_vyr(SynSemTree) != -1 )
                            {
                                ActualAdd2=SynSemTree->node;
                                //return true;
                            }
                            else
                                return false;
                        }
                        else
                            AddERR(line,IFJ_ERR_SYNTAX);
                    }

                }
                else
                {
                    sTree SynSemTree = syn_exp(*type,attr,ActualFN,newCTable,RPARENTH);
					if(SynSemTree == NULL)
						return false;
					if(sem_vyr(SynSemTree) != -1 )
					{
						ActualAdd2=SynSemTree->node;
						//return true;
					}
					else
						return false;
                }
            }
			else
			{
                AddERR(line,IFJ_ERR_SYNTAX);
                return false;
			}
			if(*type == LBRACKET)
			{
				CreateInst(I_IF_COND,NULL,NULL,NULL);
				AddBD(I_IF);
				ProgDepth++;
				return true;
			}
			else
			{
				AddERR(line,IFJ_ERR_SYNTAX);
				return false;
			}

		}
		else
			AddERR(line,IFJ_ERR_SYNTAX);





	}
	else if(strcmp(attr->str,"else"))//FUNKCE ELSE
	{
		NextToken(attr,type);
		if(*type == LBRACKET)
		{
			CreateInst(I_ELSE,NULL,NULL,NULL);
			AddBD(I_ELSE);
			ProgDepth++;
		}
		else
		{
			AddERR(line,IFJ_ERR_SYNTAX);
			return false;
		}
	}
	else if (strcmp(attr->str,"for"))//FUNKCE FOR
	{
		NextToken(attr,type);
		if(*type == LPARENTH)
		{

			CreateInstC(I_FOR,NULL,NULL,NULL);
			NextToken(attr,type);
			if(parserProlog(attr,type) == false)
				AddERR(line,IFJ_ERR_SYNTAX);
			CreateInst(I_FOR_CHCK,NULL,NULL,NULL);
			//call precedence
			NextToken(attr,type);
			sTree SynSemTree = syn_exp(*type,attr,ActualFN,newCTable,SEMICOLON);
						if(SynSemTree == NULL)
							return false;
						if(sem_vyr(SynSemTree) != -1 )
						{
							ActualAdd2=SynSemTree->node;

						}
			CreateInst(I_FOR_DIFF,NULL,NULL,NULL);

			NextToken(attr,type);
			if(parserProlog(attr,type) == false)
				AddERR(line,IFJ_ERR_SYNTAX);

			NextToken(attr,type);
			if(*type == LBRACKET)
			{
				CreateInst(I_FOR_COND,NULL,NULL,NULL);
				AddBD(I_FOR);
				ProgDepth++;
			}
			else
			{
				AddERR(line,IFJ_ERR_SYNTAX);
				return false;
			}

		}
		else
			AddERR(line,IFJ_ERR_SYNTAX);
		/*
			pred for-em add ProgDepth++
			(; -> error pozriet zadanie
			(int i; i<10, i++)
				i control if exist ked nie je definovane ked = zavolam Param
		*/
	}
	else if (*type == LBRACKET)
	{
		AddBD(200);
		ProgDepth++;
	}
	else if (*type == RBRACKET)
	{

		if(BTDelete(ActualFN->BTroot,ProgDepth,ActualFN->tempSTable) == -1)
			AddERR(line,IFJ_ERR_PROGRAM);
		int EndBracket = getBD();
		if(EndBracket == I_IF)
			CreateInst(I_END_IF,NULL,NULL,NULL);
		else if(EndBracket == I_FOR)
			CreateInst(I_END_FOR,NULL,NULL,NULL);
		else if(EndBracket == I_ELSE)
			CreateInst(I_END_ELSE,NULL,NULL,NULL);
		else if(EndBracket == I_FUNC)
			CreateInst(I_END_FUNC,NULL,NULL,NULL);
		else if(EndBracket == I_MAIN)
			CreateInst(I_END_MAIN,NULL,NULL,NULL);


		ProgDepth--;
		if(ProgDepth == 0)
		{
			if(ReturnExist == false)
				AddERR(leni,8)
			ProgKeys = 0;
			ReturnExist = false;
			CreateInst(I_END_FUNC,NULL,NULL,NULL);
		}

	}
	else
	{
		AddERR(line,IFJ_ERR_SYNTAX);
		return false;
	}
	return true;
}

bool FunctionDef(string *attr, int *type)
{
	if (strcmp(attr->str,"lenght"))//vytvori instrukci  predem a uzivatelem deklarovanej funkci a priradi tam adresi
	{
		CreateInst(I_LENGHT, attr->str, NULL, NULL);
	}
	else if (strcmp(attr->str,"substr"))
	{
		CreateInst(I_SUBSTR, attr->str, NULL, NULL);
	}
	else if (strcmp(attr->str,"concat"))
	{
		CreateInst(I_CONCAT, attr->str, NULL, NULL);
	}
	else if (strcmp(attr->str,"find"))
	{
		CreateInst(I_FIND, attr->str, NULL, NULL);
	}
	else if (strcmp(attr->str,"sort"))
	{
		CreateInst(I_SORT, attr->str, NULL, NULL);
	}
	else if (SearchFN(GST->FunRoot, attr) != NULL)
	{
		CreateInst(I_CREATE_BLOCK,attr,NULL,NULL);
		FunctCall(attr,type);
		CreateInst(I_CALL, NULL, NULL, NULL);
		CreateInst(I_GET_RETURN, ActualAdd2, NULL, NULL);
		return true;
	}
	else
	{
		AddERR(line,IFJ_ERR_SYNTAX);
		return false;
	}
}

bool IALFunctCall(string *attr, int *type)
{
	if(*type == LPARENTH)
	{
		int ParamNum = 0;
		while(*type != LPARENTH)//volam do okola kim ne narazim na konec funkcnich param
  		{

  		}
  	}
}

bool FunctCall(string *attr, int *type)
{
	if(*type != LPARENTH)
		NextToken(attr,type);
	if(*type == LPARENTH)
	{
		int ParamNum = 0;
		while(*type == LPARENTH)//volam do okola kim ne narazim na konec funkcnich param
  		{
			ParamNum++;
			NextToken(attr,type);
			if(*type == CSTRING || *type == CINTEGER || *type == CDOUBLE || *type == ID)
			{
				if(isConstant(*type) == 1)
				{
					ActualAdd2=createConst(&newCTable,*type,attr);
					CreateInst(I_ASSIGNPARAM,ParamNum,ActualAdd2,NULL);
				}
				else if((ActualAdd2=SearchBT(ActualFN, attr)) == NULL)
					AddERR(line,IFJ_ERR_SYNTAX);
				else
					CreateInst(I_ASSIGNPARAM,ParamNum,ActualAdd2,NULL);

			}

			if(NextToken(attr,type) == 0)
			{
			 	if(*type == LPARENTH || *type == COLON)//NextToken musi byt "," alebo ")" inac ERROR
					continue;
				else
				{
					AddERR(line,IFJ_ERR_SYNTAX);
					continue;
				}
			}
			else
			{
				AddERR(line,IFJ_ERR_PROGRAM);
				continue;
			}
		}
		return true;
	}
	else
	{
		AddERR(line,IFJ_ERR_SYNTAX);
		return false;
	}

}

bool Param(string *attr, int *type)//ked existujem parameter zavolame tuto funkci ked je to premenne tak zavola sa percendence inom pripade moze bit to funkce
{
	if(*type == ASSIGN)
	{
		NextToken(attr, type);
		//control if is function
		if(*type == CSTRING || *type == CINTEGER || *type == CDOUBLE || *type == ID || *type == LPARENTH)
		{
			if(*type == ID)
			{
				if(!FunctionDef(attr,type))
					return true;
				else
				{
					if(SearchBT(GST->FunNode, attr) != NULL)
					{
						sTree SynSemTree = syn_exp(*type,attr,ActualFN,newCTable,SEMICOLON);
						if(SynSemTree == NULL)
							return false;
						if(sem_vyr(SynSemTree) != -1 )
						{
							ActualAdd2=SynSemTree->node;
							return true;
						}
						else
							return false;


					}
					else
						AddERR(line,IFJ_ERR_SYNTAX);
				}

			}
			else
			{
				sTree SynSemTree = syn_exp(*type,attr,ActualFN,newCTable,SEMICOLON);
						if(SynSemTree == NULL)
							return false;
						if(sem_vyr(SynSemTree) != -1 )
						{
							ActualAdd2=SynSemTree->node;
							return true;
						}
						else
							return false;
			}
		}
		else
		{
			AddERR(line,IFJ_ERR_SYNTAX);
			return false;
		}

	}
	else
	{
		AddERR(line,IFJ_ERR_SYNTAX);
		return false;
	}
}

bool FunctionParams(string *attr, int *type, int typFN, string Identif)
{
	if((ActualFN=GSTadd(GST, Identif, typFN)) != NULL)
	{
		ProgDepth++;
		addFunInst(ActualFN, L->last);
		if(BTAddID(ActualFN,Str0,typFN,ProgDepth,ProgKeys) == -1)
			AddERR(line,IFJ_ERR_PROGRAM);
		else
			ProgKeys++;
		while(*type != LPARENTH)//volam do okola kim ne narazim na konec funkcnich param
  		{
			NextToken(attr,type);
			if(*type == KSTRING || *type == KINTEGER || *type == KDOUBLE || *type == RPARENTH)
				switch (*type)
				{
					case KSTRING:
						NextToken(attr,type);
						if(type == ID)
						{
							if(BTAddID(ActualFN,attr,KSTRING,ProgDepth,ProgKeys) == -1)//ked IDENTIFIKATOR je uspesne ukladan tak zvysim ProgKeys
								AddERR(line,IFJ_ERR_PROGRAM);
							else
							{
								addFunType(ActualFN, KSTRING);
								ProgKeys++;
							}
							if(NextToken(attr,type) == 0)
							{
							 	if(*type == LPARENTH || *type == COLON)//NextToken musi byt "," alebo ")" inac ERROR
									break;
								else
								{
								AddERR(line,IFJ_ERR_SYNTAX);
									break;
								}
							}
							else
							{
								AddERR(line,IFJ_ERR_PROGRAM);
								break;
							}
						}
						else
						{
							AddERR(line,IFJ_ERR_SYNTAX);
							break;
						}
					case KDOUBLE:
						NextToken(attr,type);
						if(type == ID)
						{
							if(BTAddID(ActualFN,attr,KDOUBLE,ProgDepth,ProgKeys) == -1)//ked IDENTIFIKATOR je uspesne ukladan tak zvysim ProgKeys
								AddERR(line,IFJ_ERR_PROGRAM);
							else
							{
								addFunType(ActualFN, KDOUBLE);
								ProgKeys++;
							}
							if(NextToken(attr,type) == 0)
							{
							 	if(*type == LPARENTH || *type == COLON)//NextToken musi byt "," alebo ")" inac ERROR
									break;
								else
								{
								AddERR(line,IFJ_ERR_SYNTAX);
									break;
								}
							}
							else
							{
								AddERR(line,IFJ_ERR_PROGRAM);
								break;
							}
						}
						else
						{
							AddERR(line,IFJ_ERR_SYNTAX);
							break;
						}
					case KINTEGER:
						NextToken(attr,type);
						if(type == ID)
						{
							if(BTAddID(ActualFN,attr,KINTEGER,ProgDepth,ProgKeys) == -1)//ked IDENTIFIKATOR je uspesne ukladan tak zvysim ProgKeys
								AddERR(line,IFJ_ERR_PROGRAM);
							else
							{
								addFunType(ActualFN, KINTEGER);
								ProgKeys++;
							}
							if(NextToken(attr,type) == 0)
							{
							 	if(*type == LPARENTH || *type == COLON)//NextToken musi byt "," alebo ")" inac ERROR
									break;
								else
								{
								AddERR(line,IFJ_ERR_SYNTAX);
									break;
								}
							}
							else
							{
								AddERR(line,IFJ_ERR_PROGRAM);
								break;
							}
						}
						else
						{
							AddERR(line,IFJ_ERR_SYNTAX);
							break;
						}
					case RPARENTH:
						break;

				}
			else
				AddERR(line,IFJ_ERR_SYNTAX);
		}
		if (strcmp(Identif,"main"))
			CreateInst(I_MAIN, NULL, NULL, NULL);
		else
			CreateInst(I_FUNC, Identif, NULL, NULL);
		while(1)//Bude vypisovat ERROR kim ne najde ; alebo {
		{
			if(NextToken(attr,type) == 0 && (*type == LBRACKET || *type == SEMICOLON);
			{
				if(*type == SEMICOLON)
					ProgDepth--;
				else
				{
					if(strcmp(Identif,"main"))
						AddBD(I_MAIN);
					else
						AddBD(I_FUNC);
				}

				return true;
			}
			else
				AddERR(line,IFJ_ERR_SYNTAX);
		}
	}
	else
	{
		AddERR(line,IFJ_ERR_PROGRAM);
		return false;
	}
}

bool Initializ(string *attr, int *type)
{
	string Identif;
	//pri deklaraci KINT ...   //CINT ked je cislo //IINTEGER typ promeny == identifikator
	switch (*type)
	{
		case KSTRING:
			NextToken(attr, type);
			if(*type == ID) //Skontrolujeme ze je to IDENTIFIKATOR, ked nie tak ERROR
			{
				Identif = attr->str;
				if(strcmp(attr->str,"main"))//IDENTIFIKATOR je main
				{
					NextToken(attr, type);
					if(*type == LPARENTH)
					{
						if(MainSwitch == false)//ked exituje uz main tak vraci ERROR
						{
							MainSwitch = true;
							if(FunctionParams(attr,type,KSTRING,Identif))
								return true;
							else
							{
								AddERR(line,IFJ_ERR_SYNTAX);
								return false;
							}
						}
						else
						{
							AddERR(line,IFJ_ERR_SYNTAX);
							return false;
						}
					}
					else//Spatne nadeklarovana main funkce
					{
						AddERR(line,IFJ_ERR_SYNTAX);
						return false;
					}
				}
				else
				{
					NextToken(attr, type);
					if(*type == LPARENTH)//INDETIFIKATOR je meno Funkce
					{
						if(SearchFN(GST->FunRoot, Identif) == NULL)//kontrolujem ze bol uz vytvoren tato funkce alebo ne
						{
							if(FunctionParams(attr,type,KSTRING,Identif))
								return true;// nasiel token "{" alebo ";"
							else
							{
								AddERR(line,IFJ_ERR_SYNTAX);
								return false;
							}
						}
						else
						{
							if(FunctionParams(attr,type,KSTRING,Identif))
							{
								if(isParamEqual(GST,ActualFN) == 1)
								{
									if(idMultipleDefinedFun(GST,ActualFN)== 1)
									{
										AddERR(line,IFJ_ERR_SEMANTIC);
										return false;
									}
									else
										return true;// nasiel token "{" alebo ";"
								}
								else
								{
									AddERR(line,IFJ_ERR_SEMANTIC);
									return false;
								}
							}
							else
							{
								AddERR(line,IFJ_ERR_SYNTAX);
								return false;
							}
						}
					}
					else if(*type == SEMICOLON)//IDNTIFIKATOR je meno premenne
					{

						if(BTAddID(ActualFN,Identif,KSTRING,ProgDepth,ProgKeys) == -1)//ked IDENTIFIKATOR je uspesne ukladan tak zvysim ProgKeys
						{
							AddERR(line,IFJ_ERR_PROGRAM);
							return false;
						}
						else
						{
							ProgKeys++;
							return true;
						}
					}
					else if(*type == ASSIGN)//IDNTIFIKATOR je meno premenne a dalsi token je "="
					{
						if(BTAddID(ActualFN,Identif,KSTRING,ProgDepth,ProgKeys) == -1)//ked IDENTIFIKATOR je uspesne ukladan tak zvysim ProgKeys
						{
							AddERR(line,IFJ_ERR_PROGRAM);
							return false;
						}
						else
						{
							ActualAdd1 = SearchBT(GST->FunNode, Identif);//pridat do IINTEGER a IDOUBLE
							ProgKeys++;
							return true;
						}
						if(Param(attr, type) == -1)
							return false;
						else
							{
								CreateInst(I_ASSIGN,ActualAdd1,ActualAdd2,NULL);
								return true;
							}
					}
					else
					{
						AddERR(line,IFJ_ERR_SYNTAX);
						return false;
					}
				}
			}
			else
			{
				AddERR(line,IFJ_ERR_SYNTAX);
				return false;
			}
			break;
		case KINTEGER:
			NextToken(attr, type);
			if(*type == ID)
			{
				Identif = attr->str;
				if(strcmp(attr->str,"main"))//IDENTIFIKATOR je main
				{
					NextToken(attr, type);
					if(*type == LPARENTH)
					{
						if(MainSwitch == false)//ked exituje uz main tak vraci ERROR
						{
							MainSwitch = true;
							if(FunctionParams(attr,type,KINTEGER,Identif))
								return true;
							else
							{
								AddERR(line,IFJ_ERR_SYNTAX);
								return false;
							}
						}
						else
						{
							AddERR(line,IFJ_ERR_SYNTAX);
							return false;
						}
					}
					else//Spatne nadeklarovana main funkce
					{
						AddERR(line,IFJ_ERR_SYNTAX);
						return false;
					}
				}
				else
				{
					NextToken(attr, type);
					if(*type == LPARENTH)//INDETIFIKATOR je meno Funkce
					{
						if(SearchFN(GST->FunRoot, Identif) == NULL)//kontrolujem ze bol uz vytvoren tato funkce alebo ne
						{
							if(FunctionParams(attr,type,KINTEGER,Identif))
								return true;// nasiel token "{" alebo ";"
							else
							{
								AddERR(line,IFJ_ERR_SYNTAX);
								return false;
							}
						}
						else
						{
							AddERR(line,IFJ_ERR_SYNTAX);
							//need to find duplicated functions end
						}
					}
					else if(*type == SEMICOLON)//IDNTIFIKATOR je meno premenne
					{
						if(BTAddID(ActualFN,Identif,KINTEGER,ProgDepth,ProgKeys) == -1)//ked IDENTIFIKATOR je uspesne ukladan tak zvysim ProgKeys
						{
							AddERR(line,IFJ_ERR_PROGRAM);
							return false;
						}
						else
						{
							ProgKeys++;
							return true;
						}
					}
					else if(*type == ASSIGN)//IDNTIFIKATOR je meno premenne a dalsi token je "="
					{
						if(Param(attr, type) == -1)
							return false;
						else
							return true;
					}
					else
					{
						AddERR(line,IFJ_ERR_SYNTAX);
						return false;
					}
				}
			}
			else
			{
				AddERR(line,IFJ_ERR_SYNTAX);
				return false;
			}
			break;
		case KDOUBLE:
			NextToken(attr, type);
			if(*type == ID)
			{
				Identif = attr->str;
				if(strcmp(attr->str,"main"))//IDENTIFIKATOR je main
				{
					NextToken(attr, type);
					if(*type == LPARENTH)
					{
						if(MainSwitch == false)//ked exituje uz main tak vraci ERROR
						{
							MainSwitch = true;
							if(FunctionParams(attr,type,KDOUBLE,Identif))
								return true;
							else
							{
								AddERR(line,IFJ_ERR_SYNTAX);
								return false;
							}
						}
						else
						{
							AddERR(line,IFJ_ERR_SYNTAX);
							return false;
						}
					}
					else//Spatne nadeklarovana main funkce
					{
						AddERR(line,IFJ_ERR_SYNTAX);
						return false;
					}
				}
				else
				{
					NextToken(attr, type);
					if(*type == LPARENTH)//INDETIFIKATOR je meno Funkce
					{
						if(SearchFN(GST->FunRoot, Identif) == NULL)//kontrolujem ze bol uz vytvoren tato funkce alebo ne
						{
							if(FunctionParams(attr,type,KDOUBLE,Identif))
								return true;// nasiel token "{" alebo ";"
							else
							{
								AddERR(line,IFJ_ERR_SYNTAX);
								return false;
							}
						}
						else
						{
							AddERR(line,IFJ_ERR_SYNTAX);
							//need to find duplicated functions end
						}
					}
					else if(*type == SEMICOLON)//IDNTIFIKATOR je meno premenne
					{
						if(BTAddID(ActualFN,Identif,KDOUBLE,ProgDepth,ProgKeys) == -1)//ked IDENTIFIKATOR je uspesne ukladan tak zvysim ProgKeys
						{
							AddERR(line,IFJ_ERR_PROGRAM);
							return false;
						}
						else
						{
							ProgKeys++;
							return true;
						}
					}
					else if(*type == ASSIGN)//IDNTIFIKATOR je meno premenne a dalsi token je "="
					{
						if(Param(attr, type) == -1)//zavolam funkci param ktory pomaha za "=" zpracovat IDENTIFI.
							return false;
						else
							return true;
					}
					else
					{
						AddERR(line,IFJ_ERR_SYNTAX);
						return false;
					}
				}
			}
			else
			{
				AddERR(line,IFJ_ERR_SYNTAX);
				return false;
			}
			break;
	}

}

bool parser()
{
	bool res = true;

	if (GSTinit(&GST) == -1)
	{
		AddERR(line, IFJ_ERR_PROGRAM);
		res = false;
	}
	else if(constTableInit(&newCTable) == -1)
	{
		AddERR(line, IFJ_ERR_PROGRAM);
		res = false;
	}
	else if(InitBD(BRDepth) == -1)
		AddERR(line, IFJ_ERR_PROGRAM);
	else //if(!parserProlog())//while
	{
		NextToken(attr,type);
		while(*type != END)
		{
			parserProlog(attr,type);
			NextToken(attr,type);
		}

		res = false;
	}
	return res;

}

int NextToken(string *attr, int *type)
{
	*type = getNextToken(attr);
	if(type == -1)
		return -1;
	else
		return 0;
}
