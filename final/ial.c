#include <string.h>
#include "scaner.c"

// podfunkcia funkcie Quiksort
void sort(char str[],int left,int right)
{   char tmp;
    if(left<right)
    {
        int pom = left;
        int i;
        for (i =left+1;i<right;i++)
        {
            if (str[i]>str[left])
            {
                pom++;
                tmp=str[pom];
                str[pom]=str[i];
                str[i]=tmp;

            }
        }
     tmp=str[pom];
     str[pom]=str[left];
     str[left]=tmp;

   sort(str,left,pom);
   sort(str,pom+1,right);
    }
}
// radiaca metoda podla algoritmu quiksort
void quicksort(char vstup[],char str[])
{   char tmp;
    strcpy(str,vstup);
    int left=0;
    int right=strlen(str);
    if(left<right)
    {
        int pom = left;
        int i;
        for ( i =left+1;i<right;i++)
        {
            if (str[i]>str[left])
            {
                pom++;
                tmp=str[pom];
                str[pom]=str[i];
                str[i]=tmp;

            }
        }
     tmp=str[pom];
     str[pom]=str[left];
     str[left]=tmp;

   sort(str,left,pom);
   sort(str,pom+1,right);
    }
}


// Booyer -Moorov algoritmus
void find(char *s, char *hladany,int pozice)
{

  if(strcmp(hladany,"")==0)
  {
    return;
  }
  int i,r_pozice = 0,h_pozice = 0,h_delka = 0,r_delka = 0;
  h_delka = strlen(hladany);
  r_delka = strlen(s);
  h_pozice = r_pozice = h_delka -1;
  char *index_posunu;
  index_posunu = malloc(sizeof(int)*100);
  memset(index_posunu, 0, 100);
  for(i = 0;i < (h_delka - 1);i++)
  {
    index_posunu[hladany[i]] = i +1;
      }
  while(h_pozice < h_delka && r_pozice < r_delka && h_pozice >= 0 && r_pozice >= 0)
  {
       if(s[r_pozice] != hladany[h_pozice])
    {
      if(index_posunu[s[r_pozice]] != 0)
      {
               r_pozice = r_pozice - index_posunu[s[r_pozice]];
      }
      r_pozice = r_pozice + h_delka +1;
      h_pozice = h_delka;
    }
  h_pozice--;
  r_pozice--;

  }
  r_pozice++;

  if(r_pozice < r_delka)
  {

    pozice= r_pozice;

  }else
  {
    free(index_posunu);
    pozice= -1;
  }
}
