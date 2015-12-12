/*string f1()
{
    return "42";
}
int main()
{
    string s;
    s = f1();
    cout << s << "\n";
    return 42;
}

string f1();*/



/* Program 2: Vypocet faktorialu (rekurzivne) */

int factorial(int n) // Definice funkce pro vypocet hodnoty faktorialu
{
int temp_result;
auto decremented_n = n - 1;
if (n < 2) {
return 1;
} else {
temp_result = factorial(decremented_n);

return n*temp_result;
}
}

int main()
{


int a; int vysl;
cout << "Zadejte cislo pro vypocet faktorialu: ";
cin >> a;
if (a < 0) {
cout << "Faktorial nelze spocitat!\n";
}
else {
vysl = factorial(a);
int pom=0;
auto neg = 0 - vysl;
cout << "Vysledek: " << vysl << " (zaporny: " << neg  << ")\n";
}
return 0;
}



/*string func( int p1, int p2, double p4 ) {
	string s = concat( "sgvdf", "asd" );
	return "gdgdfgf";
}

int main() {
	string x = func( 1, 3.4, 5.2 );

	if( x == "fvdgf" )
	{
	}else {}
		return 0;
}*/

/*int ackerman(int m, int n)
{
    if(m)
    {
        if(n)
        {
           // cout << "jyufyfjyfjyfhfh";
            int x = n - 1;
            int y = m - 1;

            int v = ackerman(m, x);
            int b = ackerman(y,v);
            return b;
        }
        else
        {
            m = m - 1;

            int x = ackerman(m, 1);
            return x;
        }

    }
    else
    {
        return n + 1;
    }

}

int main()
{
    for(int m = 0; m <= 4; m = m +1)
    {
        for(int n = 0; n < 5 - m; n = n +1)
        {
            int a = ackerman(m,n);

            cout << "A " << m << " " << n << " " << a << "\n";
        }
    }

    return 42;
}*/

/*int main()
{
string str1;
{ // vnoreny blok s lokalni promennou str2 a pristupem k str1
int x;
str1 = "Toto je nejaky text";
string str2;
str2 = concat(str1, ", ktery jeste trochu obohatime");
x = find(str2, "text");
cout << "Pozice retezce \"text\" v retezci str2: "
<< x << "\n";
cout << "Zadejte nejakou posloupnost vsech malych pismen a-h, "
<< "pricemz se pismena nesmeji v posloupnosti opakovat:";
}
cin >> str1;
str1 = sort(str1);
if (str1 != "abcdefgh")
{
for (auto s = str1; s != "abcdefgh"; s = s)
{
cout << "Spatne zadana posloupnost, zkuste znovu:";
cin >> str1;
s = sort(str1);
}
}
else {}
return 0;
}*/
/*int main(){
  string str="dbca";
  string st="c";
  int a=1;
  int b=2;
  string g="ssdfsfds";
  g=substr(str,a,b);
  cout << g;
  return 5;


}
*/




/*int incA(int b){
return b+1;
}

int incI(int a){
  a=5;
  return incA(a);
}
int main(){
  int h=5;
  int g=7;
  g=incI(h);
  cout << g;
  return 3;
}

*/


/*
int main(){
  string st="abcdefghijklmn";
  string s;
  s=substr(st,3,5);
  cout << s;
  return 0;
}

*/
/*
int main(){
  auto n=5;
  cout << n;
  return 5;

}*/

