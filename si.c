#include <stdio.h>

char matrice[5][11]=
{
  {1,1,1,1,1,1,1,1,1,1,1},
  {2,2,2,2,2,2,2,2,2,2,2},
  {2,2,2,2,2,2,2,2,2,2,2},
  {3,3,3,3,3,3,3,3,3,3,3},
  {3,3,3,3,3,3,3,3,3,3,3}
};

/* renvoie la matrice */
 char *si_get_matrix(void)
 {
   char *matrix=&matrice[0][0];
   return matrix;
 }
   
 /* renvoie le nombre d’ennemis restant dans la matrice */
 int si_matrix_count(void)
 {
   int ennemis=0;
   for(int i=0;i<5;i++)
     {
       for(int j=0;j<11;j++)
	 {
	   if (matrice[i][j]!=0)
	     {
	       ennemis+=1;
	     } 
	 }
     }
   return ennemis;
 }

/* renvoi le nombre de points en fonction de l’ennemi 
int si_get_points(Si_Type t)
{
  
}*/

/* renvoie le nombre de joueurs 
int si_get_nbr_players(Si *si)
{

}*/

int main()
{
  int a=si_matrix_count();
  printf("%d",a);
  return 0;
}
