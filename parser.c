//+------------------------------------------------------------------+
//|	Project name:        IFJ - Team project                          |
//| Filename:            parser.c                                    |
//| Author:              Dávid Polák                                 |
//|	Encoding:            UTF-8										 |
//|	Description:         Source of Syntax analyzer.					 |
//+------------------------------------------------------------------+

#include "parser.h"

bool MainSwitch = false;
int ProgDepth = 0;
int ProgKeys=0; //Return ProgKeys 0  and others from 1 to n end reset at end of function 
//funct return add to 0xfunc1
FN ActualFN;
bool parserProlog() 
{
	//Funkce Find(string s,string search),
	//Substr(string s , int i, int n),
	//concat(string s1, string s2), 
	//Sort(string s), 
	//Length(string s)
	NextToken(attr,type);
	if(*type == KSTRING || *type == KINT || *type == KDOUBLE)
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
				string Identif = attr;
				NextToken(attr, type);
				if(Param(attr, type) == -1)
					return false;
				else
				{
					if(BTAddID(ActualFN,Identif,typ,ProgDepth,ProgKeys) == -1)//vratim typ   
						AddERR(line,IFJ_ERR_PROGRAM);
					else
						ProgKeys++;
				}
			}

	}
	else if (strcmp(attr->str, "cout"))
	{
		while(!strcmp(attr->str,";"))
		{
			NextToken(attr,type);
			if(strcmp(attr->str, "<<"))
			{
				NextToken(attr,type);
				if(*type == CSTRING || *type == CINTEGER || *type == CDOUBLE || *type == ID)
					CreateInst(I_COUT,attr,NULL,NULL);
				else
					AddERR(line,IFJ_ERR_SYNTAX);
			}
			else
			{
				AddERR(line,IFJ_ERR_SYNTAX);
			}

		}
		return true;
	}
	else if (strcmp(attr->str,"cin"))
	{
		while(!strcmp(attr->str,";"))
		{
			NextToken(attr,type);
			if(strcmp(attr->str, ">>"))
			{
				NextToken(attr,type);
				if(*type == ID)
				{
					if(SearchBT(GST->FunNode, attr) != NULL)//Existujuci IDENTIFIKATOR
					{
						CreateInst(I_CIN,attr,NULL,NULL);
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
	else if (strcmp(attr->str,"return"))
	{
		NextToken(attr,type);
		if(*type == CSTRING || *type == CINTEGER || *type == CDOUBLE || *type == ID)
		{
			if(*type == ID)
				{
					if(SearchBT(GST->FunNode, attr) != NULL)//Existujuci IDENTIFIKATOR
					{
						CreateInst(I_RETURN,attr,NULL,NULL);
						return true;
					}
					else
						AddERR(line,IFJ_ERR_SYNTAX);
				}
			CreateInst(I_RETURN,attr,NULL,NULL);
			return true;
		}
		else
			AddERR(line,IFJ_ERR_SYNTAX);
		NextToken(attr,type);
		if(!strcmp(attr->str,";"))
		{
			AddERR(line,IFJ_ERR_SYNTAX);
			return false;
		}
		else
			return true;
	}
	else if (strcmp(attr->str,"if"))
	{
		NextToken(attr,type);
		if(strcmp(attr->str,"(")
		{
			/*nexttoken
		if ( && nexttoken

			precedence

		if {  then ProgDepth++
		*/
			CreateInst(I_IF,attr,NULL,NULL);
			ProgDepth++;
		}
		else
			AddERR(line,IFJ_ERR_SYNTAX);
		
	}
	else if (strcmp(attr->str,"else"))
	{
		NextToken(attr,type);
		if(strcmp(attr->str,"{")
		{
			CreateInst(I_ELSE,attr,NULL,NULL);
			ProgDepth++;
		}		
		else
		{
			AddERR(line,IFJ_ERR_SYNTAX);
			return false;
		}
	}
	else if (strcmp(attr->str,"for"))
	{
		NextToken(attr,type);
		if(strcmp(attr->str,"(")
		{
			if(parserProlog() == false)
				AddERR(line,IFJ_ERR_SYNTAX);
			//call precedence
			if(parserProlog() == false)
				AddERR(line,IFJ_ERR_SYNTAX);
			
			NextToken(attr,type);
			if(strcmp(attr->str,"{")
			{
				CreateInst(I_ELSE,attr,NULL,NULL);
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
	else if (strcmp(attr->str,"{"))
	{
		ProgDepth++; 
	}	
	else if (strcmp(attr->str,"}"))
	{
		ProgDepth--; 
		if(ProgDepth == 0)
			ProgKeys = 0;		
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
		CreateInst(I_CALL, attr->str, NULL, NULL);
	}
	else
	{
		AddERR(line,IFJ_ERR_SYNTAX);	
		return false;
	}
}

bool Param(string *attr, int *type)//ked existujem parameter zavolame tuto funkci ked je to premenne tak zavola sa percendence inom pripade moze bit to funkce
{	
	if(strcmp(attr->str,"="))
	{
		NextToken(attr, type);
		//control if is function
		if(*type == CSTRING || *type == CINTEGER || *type == CDOUBLE || *type == ID || *type == LPARENTH)
			//call precedence
			/*
			ked narazim na konstantu createConst(newcTable,type,attr)  return Btree
			return typ nasledujuceho znaku
			*/
		else if(!FunctionDef(attr,type))
			return false;

	}
	else
	{
		AddERR(line,IFJ_ERR_SYNTAX);	
		return false;
	}	
}

FunctionParams(string *attr, int *type, int typFN, string Identif)
{
	if((ActualFN=GSTadd(GST, Identif, typFN)) != NULL)
	{
		ProgDepth++;
		if(BTAddID(ActualFN,Str0,typFN,ProgDepth,ProgKeys) == -1)//nadeklarujem hodnotu return
			AddERR(line,IFJ_ERR_PROGRAM);
		else
			ProgKeys++;
		while(!strcmp(attr->str,")"))//volam do okola kim ne narazim na konec funkcnich param
  		{									
			NextToken(attr,type);
			if(*type == ISTRING || *type == IINTEGER || *type == IDOUBLE || *type == RPARENTH)
				switch (*type)
				{
					case ISTRING:
						NextToken(attr,type);
						if(type == ID)
						{
							if(BTAddID(ActualFN,attr,ISTRING,ProgDepth,ProgKeys) == -1)//ked IDENTIFIKATOR je uspesne ukladan tak zvysim ProgKeys  
								AddERR(line,IFJ_ERR_PROGRAM);
							else
								ProgKeys++;
							if(NextToken(attr,type) == 0)
							{
							 	if(strcmp(attr->str,")") || strcmp(attr->str,","))//NextToken musi byt "," alebo ")" inac ERROR
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
					case IDOUBLE:
						NextToken(attr,type);
						if(type == ID)
						{
							if(BTAddID(ActualFN,attr,IDOUBLE,ProgDepth,ProgKeys) == -1)//ked IDENTIFIKATOR je uspesne ukladan tak zvysim ProgKeys  
								AddERR(line,IFJ_ERR_PROGRAM);
							else
								ProgKeys++;
							if(NextToken(attr,type) == 0)
							{
							 	if(strcmp(attr->str,")") || strcmp(attr->str,","))//NextToken musi byt "," alebo ")" inac ERROR
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
					case IINTEGER:
						NextToken(attr,type);
						if(type == ID)
						{
							if(BTAddID(ActualFN,attr,IINTEGER,ProgDepth,ProgKeys) == -1)//ked IDENTIFIKATOR je uspesne ukladan tak zvysim ProgKeys  
								AddERR(line,IFJ_ERR_PROGRAM);
							else
								ProgKeys++;
							if(NextToken(attr,type) == 0)
							{
							 	if(strcmp(attr->str,")") || strcmp(attr->str,","))//NextToken musi byt "," alebo ")" inac ERROR
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
			CreateInst(I_MAIN, Identif, NULL, NULL, NULL);
		else
			CreateInst(I_FUNC, Identif, NULL, NULL, NULL);
		while(1)//Bude vypisovat ERROR kim ne najde ; alebo {
		{
			if(NextToken(attr,type) == 0 && strcmp(attr->str,"{") || strcmp(attr->str,";"); 
			{
				if(strcmp(attr->str,";"))
					ProgDepth--;
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
		case ISTRING:
			NextToken(attr, type);
			if(*type == ID) //Skontrolujeme ze je to IDENTIFIKATOR, ked nie tak ERROR
			{
				Identif = attr->str;
				if(strcmp(attr->str,"main"))//IDENTIFIKATOR je main
				{
					NextToken(attr, type);
					if(strcmp(attr->str,"("))
					{
						if(MainSwitch == false)//ked exituje uz main tak vraci ERROR
						{
							MainSwitch = true;
							if(FunctionParams(attr,type,ISTRING,Identif))
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
					if(strcmp(attr->str,"("))//INDETIFIKATOR je meno Funkce
					{
						if(SearchFN(GST->FunRoot, Identif) == NULL)//kontrolujem ze bol uz vytvoren tato funkce alebo ne 
						{
							if(FunctionParams(attr,type,ISTRING,Identif))
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
					else if(strcmp(attr->str,";"))//IDNTIFIKATOR je meno premenne 
					{
						
						if(BTAddID(ActualFN,Identif,ISTRING,ProgDepth,ProgKeys) == -1)//ked IDENTIFIKATOR je uspesne ukladan tak zvysim ProgKeys  
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
					else if(strcmp(attr->str,"="))//IDNTIFIKATOR je meno premenne a dalsi token je "="
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
		case IINTEGER:
			NextToken(attr, type);
			if(*type == ID)
			{
				Identif = attr->str;
				if(strcmp(attr->str,"main"))//IDENTIFIKATOR je main
				{
					NextToken(attr, type);
					if(strcmp(attr->str,"("))
					{
						if(MainSwitch == false)//ked exituje uz main tak vraci ERROR
						{
							MainSwitch = true;
							if(FunctionParams(attr,type,IINTEGER,Identif))
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
					if(strcmp(attr->str,"("))//INDETIFIKATOR je meno Funkce
					{
						if(SearchFN(GST->FunRoot, Identif) == NULL)//kontrolujem ze bol uz vytvoren tato funkce alebo ne 
						{
							if(FunctionParams(attr,type,IINTEGER,Identif))
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
					else if(strcmp(attr->str,";"))//IDNTIFIKATOR je meno premenne 
					{
						if(BTAddID(ActualFN,Identif,IINTEGER,ProgDepth,ProgKeys) == -1)//ked IDENTIFIKATOR je uspesne ukladan tak zvysim ProgKeys  
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
					else if(strcmp(attr->str,"="))//IDNTIFIKATOR je meno premenne a dalsi token je "="
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
		case IDOUBLE:
			NextToken(attr, type);
			if(*type == ID)
			{
				Identif = attr->str;
				if(strcmp(attr->str,"main"))//IDENTIFIKATOR je main
				{
					NextToken(attr, type);
					if(strcmp(attr->str,"("))
					{
						if(MainSwitch == false)//ked exituje uz main tak vraci ERROR
						{
							MainSwitch = true;
							if(FunctionParams(attr,type,IDOUBLE,Identif))
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
					if(strcmp(attr->str,"("))//INDETIFIKATOR je meno Funkce
					{
						if(SearchFN(GST->FunRoot, Identif) == NULL)//kontrolujem ze bol uz vytvoren tato funkce alebo ne 
						{
							if(FunctionParams(attr,type,IDOUBLE,Identif))
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
					else if(strcmp(attr->str,";"))//IDNTIFIKATOR je meno premenne 
					{
						if(BTAddID(ActualFN,Identif,IDOUBLE,ProgDepth,ProgKeys) == -1)//ked IDENTIFIKATOR je uspesne ukladan tak zvysim ProgKeys  
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
					else if(strcmp(attr->str,"="))//IDNTIFIKATOR je meno premenne a dalsi token je "="
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
	else if(!parserProlog())
	{
		AddERR(line, IFJ_ERR_PROGRAM);
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
