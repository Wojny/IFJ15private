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
int forEND=0;
bool ElseSwitch = false;
int assignment=0;
string *Identif;

int InitBD()//inicializace Depth Table
{
  if(((BDepth=malloc(sizeof (struct BD)))==NULL))
  	return -1;
  (BDepth)->BracketType=-1;
  (BDepth)->NextDepth=NULL;
  return 0;
}
void AddBD(int type)// pridam nove Depth
{
  if(BDepth==NULL)
  	InitBD();
  if(BDepth->BracketType==-1)
  {
    BDepth->BracketType=type;
    BDepth->NextDepth=NULL;
  }
  else
  {
    BracketDepth BDepthpom;
    if(((BDepthpom=malloc(sizeof (struct BD)))==NULL))
    	return;
    BDepthpom->BracketType=type;
    BDepthpom->NextDepth=BDepth;
    BDepth=BDepthpom;
  }

}

int getBD()//volam ked najdu token '}'
{
  BracketDepth BDepthpom;
  BDepthpom=BDepth;
  BDepth=BDepth->NextDepth;
  int pom=BDepthpom->BracketType;
  free(BDepthpom);
  return pom;
}

bool parserProlog(string *attr, int *type)
{
	if((*type == KSTRING) || (*type == KINTEGER) || (*type == KDOUBLE)){ // ked token je INT,STR alebo DOUBLE zavolam Initializ
        assignment=1;
		if(Initializ(attr, type) == false)
			return false;
    }
	else if(*type == ID) //ked token IDENTIFIKATOR
	{
		if((ActualAdd1=SearchBT(ActualFN, attr)) != NULL)//Existujuci Parameter
		{

            assignment=1;
			NextToken(attr, type);
			if(Param(attr, type) == false)
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

	if (strcmp(attr->str, "auto")==0)
	{
		assignment=0;

		NextToken(attr,type);
		if(*type == ID)
        {
			strCopyString(Identif,attr);
			NextToken(attr, type);
			if(Param(attr, type) == false)
			{
				res = false;
			}
			else
			{
				if(BTAddID(&ActualFN,Identif,ActualAdd2->type,ProgDepth,ProgKeys) == -1)//vratim typ a vytvorim Param
				{
					AddERR(line,IFJ_ERR_PROGRAM);
					res = false;
				}
				else
				{
					ProgKeys++;
					ActualAdd1 = SearchBT(ActualFN, Identif);
					CreateInst(I_ASSIGN,ActualAdd1,ActualAdd2,NULL,L);
				}
			}
		}
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
					if((ActualAdd1=SearchBT(ActualFN, attr)) != NULL)//Existujuci IDENTIFIKATOR
						CreateInst(I_READ,ActualAdd1,NULL,NULL,L);
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
		if(*type == CSTR || *type == CINTEGER || *type == CDOUBLE || *type == ID || *type==LPARENTH)
		{

            if((*type==ID)&&((SearchFN(GST->FunRoot, attr)) != NULL))
            {
		      if(FunctionDef(attr,type) == true)
				{
                    NextToken(attr, type);
                    if(*type==SEMICOLON){
                      res= true;
                    }
                    else
                      res= false;

                }
		    }
            else
            {
                sTree SynSemTree = syn_exp(*type,attr,ActualFN,newCTable,SEMICOLON);
                if(SynSemTree == NULL)
                    res = false;
                if(sem_sym(SynSemTree) != -1 ){
                    ActualAdd1=SynSemTree->node;
                    deleteSynTree(SynSemTree);
                }
                else
                    res = false;
            }
			CreateInst(I_RETURN,NULL,ActualAdd1,NULL,L);

		}
		else
		{
			AddERR(line,IFJ_ERR_SYNTAX);
			res = false;
		}

        assignment=0;
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
                    {
                        NextToken(attr, type);
                        if(*type==RPARENTH)
                          return true;
                        else return false;
                    }
                    else
                    {
                        if(SearchBT(ActualFN, attr) != NULL)
                        {
                            sTree SynSemTree = syn_exp(*type,attr,ActualFN,newCTable,RPARENTH);
                            if(SynSemTree == NULL)
                                res = false;
                            if(sem_sym(SynSemTree) != -1 ){
                                ActualAdd2=SynSemTree->node;
                                deleteSynTree(SynSemTree);
                            }
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
					if(sem_sym(SynSemTree) != -1 ){
						ActualAdd2=SynSemTree->node;
						deleteSynTree(SynSemTree);
					}
					else
						res = false;
                }
            }
			else
			{
                AddERR(line,IFJ_ERR_SYNTAX);
                res = false;
			}

			NextToken(attr, type);
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
		if(ElseSwitch == false)
		{
            AddERR(line,IFJ_ERR_SYNTAX);
            res = false;
        }
        else
            ElseSwitch = false;
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
            ProgDepth++;
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
			if(sem_sym(SynSemTree) != -1 ){
				ActualAdd2=SynSemTree->node;
			    deleteSynTree(SynSemTree);
			}
			CreateInst(I_FOR_DIFF,NULL,NULL,NULL,L);


			NextToken(attr,type);
			forEND=1;
			assignment=1;
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
        {
            CreateInst(I_END_IF,NULL,NULL,NULL,L);
            ElseSwitch = true;
        }

		else if(EndBracket == I_FOR)
			CreateInst(I_END_FOR,NULL,NULL,NULL,L);
		else if(EndBracket == I_ELSE)
			    CreateInst(I_END_ELSE,NULL,NULL,NULL,L);

		else if(EndBracket == I_FUNC){
			CreateInst(I_END_FUNC,NULL,NULL,NULL,L);
			ProgKeys=0;
		}
		else if(EndBracket == I_MAIN)
        {
            CreateInst(I_END_MAIN,NULL,NULL,NULL,L);
            ProgKeys=0;

        }

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
	int ParamCounter=1;
	int ParamType;
	bool res = true;
	FN pomFN;

	if (strcmp(attr->str,"length")==0)//vytvori instrukci  predem a uzivatelem deklarovanej funkci a priradi tam adresi
	{
        assignment=0;
		NextToken(attr,type);
		if( *type == LPARENTH)
		{
			if(IALFunctCall(attr, type, ParamCounter, CSTR) == true)//ActualAdd2 sa uklada aktualne spracovani parameter
			{
				if(ParamCounter == 1)
					CreateInst(I_LENGHT, ActualAdd2, NULL, ActualAdd1,L);
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
	else if (strcmp(attr->str,"substr")==0)
	{
		assignment=0;
		NextToken(attr,type);
		if( *type == LPARENTH)
		{
			for(ParamCounter =1; ParamCounter < 4;ParamCounter++)
			{
				switch(ParamCounter)
				{
					case 1:
						ParamType = CSTR;
						break;
					case 2:
						ParamType = CINTEGER;
						break;
					case 3:
						ParamType = CINTEGER;
						break;
				}
				if(IALFunctCall(attr, type, ParamCounter, ParamType) == false)
				{
					AddERR(line,IFJ_ERR_SYNTAX);
					res = false;
				}

			}
			CreateInst(I_SUBSTR, ActualAdd2, ActualAdd3, ActualAdd4,L);
			CreateInst(I_SUBSTR2, ActualAdd1, NULL, NULL,L);
		}
		else
		{
			AddERR(line,IFJ_ERR_SYNTAX);
			res = false;
		}
	}
	else if (strcmp(attr->str,"concat")==0)
	{
		assignment=0;
		NextToken(attr,type);
		if( *type == LPARENTH)
		{
			for(ParamCounter =1; ParamCounter < 3;ParamCounter++)
			{
				switch(ParamCounter)
				{
					case 1:
						ParamType = CSTR;
						break;
					case 2:
						ParamType = CSTR;
						break;
				}
				if(IALFunctCall(attr, type, ParamCounter, ParamType) == false)
				{
					AddERR(line,IFJ_ERR_SYNTAX);
					res = false;
				}
			}
			CreateInst(I_CONCAT, ActualAdd2, ActualAdd3, ActualAdd1,L);
		}
		else
		{
			AddERR(line,IFJ_ERR_SYNTAX);
			res = false;
		}
	}
	else if (strcmp(attr->str,"find")==0)
	{

		assignment=0;
		NextToken(attr,type);
		if( *type == LPARENTH)
		{
			for(ParamCounter =1; ParamCounter < 3; ParamCounter++)
			{
				switch(ParamCounter)
				{
					case 1:
						ParamType = CSTR;
						break;
					case 2:
						ParamType = CSTR;
						break;
				}
				if(IALFunctCall(attr, type, ParamCounter, ParamType) == false)
				{
					AddERR(line,IFJ_ERR_SYNTAX);
					res = false;
				}
			}
			CreateInst(I_FIND, ActualAdd2, ActualAdd3, ActualAdd1,L);
		}
		else
		{
			AddERR(line,IFJ_ERR_SYNTAX);
			res = false;
		}
	}
	else if (strcmp(attr->str,"sort")==0)
	{
		assignment=0;
		NextToken(attr,type);
		if( *type == LPARENTH)
		{
			if(IALFunctCall(attr, type, ParamCounter, CSTR) == true)
			{
				if(ParamCounter == 1)
					CreateInst(I_SORT, ActualAdd2, ActualAdd1, NULL,L);
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
	else if ((pomFN=SearchFN(GST->FunRoot, attr)) != NULL)
	{
		assignment=0;
		CreateInst(I_CREATE_BLOCK,pomFN->ident,NULL,NULL,L);
		if(FunctCall(attr,type) == false)
		{
			AddERR(line,IFJ_ERR_SYNTAX);
			res = false;
		}
		CreateInst(I_CALL, pomFN->ident, NULL, NULL,L);
		CreateInst(I_GET_RETURN, ActualAdd1, NULL, NULL,L);
	}
	else{

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

		if(isConstant(*type) == 1)
		{
			//if(ParamType != *type)
			if(isCompatible(ParamType,*type)==false)
			{
				AddERR(line,IFJ_ERR_SEMANTIC);
				res = false;
			}

			ActualAdd4=createConst(&newCTable,*type,attr);
		}
		else if((ActualAdd4=SearchBT(ActualFN, attr)) == NULL)
		{
			AddERR(line,IFJ_ERR_SYNTAX);
			res = false;
		}
		else
		{
			if(isCompatible(ParamType,ActualAdd4->type)==false)
			{

				AddERR(line,IFJ_ERR_SEMANTIC);
				res = false;
			}
		}
	}
    if(ParamCounter==1) ActualAdd2=ActualAdd4;
    else if(ParamCounter==2) ActualAdd3=ActualAdd4;

	if(NextToken(attr,type) == 0)
	{
	 	if((*type == RPARENTH )|| (*type == COLON))//NextToken musi byt "," alebo ")" inac ERROR
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

		while(*type != RPARENTH)//volam do okola kim ne narazim na konec funkcnich param
  		{
			int pomtype=*type;
			NextToken(attr,type);
			if((*type==RPARENTH)&&(pomtype==LPARENTH)) continue;
			if(*type == CSTR || *type == CINTEGER || *type == CDOUBLE || *type == ID)
			{
				if(isConstant(*type) == 1)
				{
					ActualAdd2=createConst(&newCTable,*type,attr);
					CreateInst(I_ASSIGNPARAM,NULL,ActualAdd2,NULL,L);
				}
				else if((ActualAdd2=SearchBT(ActualFN, attr)) == NULL)
				{
					AddERR(line,IFJ_ERR_SYNTAX);
					res = false;
				}
				else
					CreateInst(I_ASSIGNPARAM,NULL,ActualAdd2,NULL,L);
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
				{
                    NextToken(attr, type);
                    if(*type==SEMICOLON)
                      return true;

                    else
                      return false;
                }
				else
				{
					if(SearchBT(ActualFN, attr) != NULL)
					{
						sTree SynSemTree;
						if(forEND)
						{
						  SynSemTree = syn_exp(*type,attr,ActualFN,newCTable,RPARENTH);
						  forEND=0;
						  assignment=1;
						}
						else
							SynSemTree = syn_exp(*type,attr,ActualFN,newCTable,SEMICOLON);
						if(SynSemTree == NULL)
							return false;
						if(sem_sym(SynSemTree) != -1 ){
							ActualAdd2=SynSemTree->node;
							deleteSynTree(SynSemTree);
						}
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
				sTree SynSemTree;
				if(forEND)
				{
                    SynSemTree = syn_exp(*type,attr,ActualFN,newCTable,RPARENTH); //syn_exp zaciklenie
                    forEND=0;
                    assignment=1;
				}
				else
				  SynSemTree = syn_exp(*type,attr,ActualFN,newCTable,SEMICOLON); //syn_exp zaciklenie
				if(SynSemTree == NULL)
					return false;
				if(sem_sym(SynSemTree) != -1 ){
					ActualAdd2=SynSemTree->node;
					deleteSynTree(SynSemTree);
				}
				else
					res = false;
			}
		}
		else
		{
			AddERR(line,IFJ_ERR_SYNTAX);
			res = false;
		}
    if(assignment==1)
		{
		    CreateInst(I_ASSIGN,ActualAdd1,ActualAdd2,NULL,L);
		    assignment=0;
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
						if(*type == ID)
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
						if(*type == ID)
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
        {
			CreateInst(I_MAIN, NULL, NULL, NULL,L);
			addFunInst(&ActualFN, L->last);
			CreateInst(I_CREATE_BLOCK, ActualFN->ident, NULL, NULL,L);
        }
		else{
			CreateInst(I_FUNC, NULL, NULL, NULL,L);
			addFunInst(&ActualFN, L->last);

		}

		if(NextToken(attr,type) == 0 && (*type == LBRACKET || *type == SEMICOLON || *type == END))
		{
			if(*type == SEMICOLON)
			{
				if(BTDelete(&ActualFN->BTroot,ProgDepth,&ActualFN->tempSTable) == -1)
		        {
        	        AddERR(line,IFJ_ERR_PROGRAM);
			        res=false;
			    }
				ProgDepth--;
				ProgKeys=0;

			}
			else
			{
                setFunDefined(ActualFN);
				if(strcmp(Identif->str,"main")==0)
					AddBD(I_MAIN);
				else
					AddBD(I_FUNC);
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
		AddERR(line,IFJ_ERR_PROGRAM);
		res = false;
	}
	return res;
}

bool Initializ(string *attr, int *type)
{

	bool res = true;
	//pri deklaraci KINT ...   //CINT ked je cislo //IINTEGER typ promeny == identifikator
	switch (*type)
	{
		case KSTRING:
			NextToken(attr, type);
			if(*type == ID) //Skontrolujeme ze je to IDENTIFIKATOR, ked nie tak ERROR
			{
				strCopyString(Identif,attr);
				if(strcmp(attr->str,"main")==0)//IDENTIFIKATOR je main
				{
					NextToken(attr, type);
					if(*type == LPARENTH)
					{
						if(MainSwitch == false)//ked exituje uz main tak vraci ERROR
						{
							MainSwitch = true;
							if(FunctionParams(attr,type,KSTRING,Identif) == false)
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
						if(SearchFN(GST->FunRoot, Identif) == NULL)//kontrolujem ze bol uz vytvoren tato funkce alebo ne
						{
							if(FunctionParams(attr,type,KSTRING,Identif) == false)
							{
								AddERR(line,IFJ_ERR_SYNTAX);
								res = false;
							}
						}
						else
						{
							if(FunctionParams(attr,type,KSTRING,Identif))
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

						if(BTAddID(&ActualFN,Identif,KSTRING,ProgDepth,ProgKeys) == -1)//ked IDENTIFIKATOR je uspesne ukladan tak zvysim ProgKeys
						{
							AddERR(line,IFJ_ERR_PROGRAM);
							res = false;
						}
						else
							ProgKeys++;

					}
					else if(*type == ASSIGN)//IDNTIFIKATOR je meno premenne a dalsi token je "="
					{

						if(BTAddID(&ActualFN,Identif,KSTRING,ProgDepth,ProgKeys) == -1)//ked IDENTIFIKATOR je uspesne ukladan tak zvysim ProgKeys
						{

							AddERR(line,IFJ_ERR_PROGRAM);
							res = false;
						}
						else
						{
							ActualAdd1 = SearchBT(ActualFN, Identif);//pridat do IINTEGER a IDOUBLE
							ProgKeys++;
						}
						if(Param(attr, type) == false)
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
				AddERR(line,IFJ_ERR_SYNTAX);
				res = false;
			}
			break;
		case KINTEGER:

			NextToken(attr, type);
			if(*type == ID)
			{
				strCopyString(Identif,attr);
				if(strcmp(attr->str,"main") == 0)//IDENTIFIKATOR je main depth=0 must be
				{
					NextToken(attr, type);
					if(*type == LPARENTH)
					{
						if(MainSwitch == false)//ked exituje uz main tak vraci ERROR
						{
							MainSwitch = true;
							if(FunctionParams(attr,type,KINTEGER,Identif) == false)
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
						if(SearchFN(GST->FunRoot, Identif) == NULL)//kontrolujem ze bol uz vytvoren tato funkce alebo ne
						{

							if(FunctionParams(attr,type,KINTEGER,Identif) == false)
							{
								AddERR(line,IFJ_ERR_SYNTAX);
								res = false;
							}
						}
						else
						{
							if(FunctionParams(attr,type,KINTEGER,Identif))
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
						if(BTAddID(&ActualFN,Identif,KINTEGER,ProgDepth,ProgKeys) == -1)//ked IDENTIFIKATOR je uspesne ukladan tak zvysim ProgKeys
						{
							AddERR(line,IFJ_ERR_PROGRAM);
							res = false;
						}
						else
							ProgKeys++;
					}
					else if(*type == ASSIGN)//IDNTIFIKATOR je meno premenne a dalsi token je "="
					{



						if(BTAddID(&ActualFN,Identif,KINTEGER,ProgDepth,ProgKeys) == -1)//ked IDENTIFIKATOR je uspesne ukladan tak zvysim ProgKeys
						{
							AddERR(line,IFJ_ERR_PROGRAM);
							res = false;
						}
						else
						{
							ActualAdd1 = SearchBT(ActualFN, Identif);//pridat do IINTEGER a IDOUBLE
							ProgKeys++;
						}
						if(Param(attr, type) == false)
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
				AddERR(line,IFJ_ERR_SYNTAX);
				res = false;
			}
			break;
		case KDOUBLE:
			NextToken(attr, type);
			if(*type == ID)
			{
				strCopyString(Identif,attr);
				if(strcmp(attr->str,"main") == 0)//IDENTIFIKATOR je main
				{
					NextToken(attr, type);
					if(*type == LPARENTH)
					{
						if(MainSwitch == false)//ked exituje uz main tak vraci ERROR
						{
							MainSwitch = true;
							if(FunctionParams(attr,type,KDOUBLE,Identif) == false)
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
						if(SearchFN(GST->FunRoot, Identif) == NULL)//kontrolujem ze bol uz vytvoren tato funkce alebo ne
						{
							if(FunctionParams(attr,type,KDOUBLE,Identif) == false)
							{
								AddERR(line,IFJ_ERR_SYNTAX);
								res = false;
							}
						}
						else
						{
							if(FunctionParams(attr,type,KDOUBLE,Identif))
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
						if(BTAddID(&ActualFN,Identif,KDOUBLE,ProgDepth,ProgKeys) == -1)//ked IDENTIFIKATOR je uspesne ukladan tak zvysim ProgKeys
						{
							AddERR(line,IFJ_ERR_PROGRAM);
							res = false;
						}
						else
							ProgKeys++;
					}
					else if(*type == ASSIGN)//IDNTIFIKATOR je meno premenne a dalsi token je "="
					{
						if(BTAddID(&ActualFN,Identif,KDOUBLE,ProgDepth,ProgKeys) == -1)//ked IDENTIFIKATOR je uspesne ukladan tak zvysim ProgKeys
						{
							AddERR(line,IFJ_ERR_PROGRAM);
							res = false;
						}
						else
						{
							ActualAdd1 = SearchBT(ActualFN, Identif);//pridat do IINTEGER a IDOUBLE
							ProgKeys++;
						}
						if(Param(attr, type) == false)
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
    initstr0(&Identif);
    int *type;
    if(((type=malloc(sizeof(int)))==NULL)) return false; // allokace type
    if(((L=malloc(sizeof(tList)))==NULL)) return false; // allokace L list
    listInit(L);

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
			bool asd = true;
			asd= parserProlog(attr,type);
			NextToken(attr,type);
			if(ElseSwitch == true && strcmp(attr->str,"else")==1)
            {
                AddERR(line,IFJ_ERR_SYNTAX);
                ElseSwitch = false;
            }
			if (asd == false){
			    free(attr);
                free(type);
                free(BDepth);
                return false;
			}
			/*printf(" %d \n",*type );//vypis chyb
			if (asd == false)
               	printf("ahoj ");
            else
                printf("cau ");*/
		}
		free(attr);
		free(type);
		free(BDepth);
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
