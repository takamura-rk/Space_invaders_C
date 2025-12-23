#include <stdio.h>
#include "si.h"
#include "game.h"

char matrice[5][11]=
  {
    {1,1,1,1,1,1,1,1,1,1,1},
    {2,2,2,2,2,2,2,2,2,2,2},
    {2,2,2,2,2,2,2,2,2,2,2},
    {3,3,3,3,3,3,3,3,3,3,3},
    {3,3,3,3,3,3,3,3,3,3,3}
  };

/*
 * crée un pointeur vers la structure Si. On passe en paramètre
 * les dimension de la fenêtre et la taille de pixel
 */
Si *si_new(int window_width, int window_height, int pixel_size)
{
  Si *si;
  if (!si)
        return NULL;
  si=(Si *)malloc(sizeof(Si));
  si->window_width=window_width;
  si->window_height=window_height;
  si->pixel_size=pixel_size;
  si->nbr_players=1;
}

/*Détruit les ressources */
void si_del(Si *si)
{
  free(si);
}


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
/* renvoi le nombre de points en fonction de l’ennemi */
int si_get_points(Si_Type t)
{
  int p; 
  if(t==1){
    p=30;
  }
  if(t==2){
    p=20;
  }
  if(t==3){
    p=10;
  }
  return p;
}

/* renvoie le nombre de joueurs */
int si_get_nbr_players(Si *si)
{
  return si->nbr_players;
}

/* tank */
/*
* teste si le tank peut se d\’eplacer vers le haut. SI c’est possible,
* met à jour la coordonné y du tir da Tank.
*/
int si_tank_shoot_can_move_up(Si *si)
{
  if(si->tank.firing==0){
    return 0;
  }
  si->tank.shoot_y-=si->pixel_size;
  /*si le tire sort de l'écran on remet firing à 0*/
  if(si->tank.shoot_y<=0){
    si->tank.firing=0;
    return 1;
  }
  /*si le tire touche un ennemie on remet firing à 0*/
  if (si_invader_is_hit(si)==1)
    {
        si->tank.firing = 0;
        return 1;
    }
  return 1;
}

/* renvoie 1 si le tank est touch\’e, 0 sinon 
int si_tank_is_hit(Si *si)
{
  if(si->invaders.firing==0){
    return 0;
  }
 
  

  
  return 1;
  }*/

/* invaders */
/*
* Détermine dans quelle colonnnede la matrice, choisie aléatoirement,
* la bombe est lachée. Met à jour les coordonn\’ees de la bombe
* dans Invaders.

void si_invaders_get_column(Si *si){
 
  
    
}*/

/*
* Renvoie 1 si la bombe peut se déplacer vers le bas, 0 sinon.
* Met à jour la coordonnéee y de la bombe.
*/
int si_invaders_bomb_can_move_down(Si *si)
{
  if (si->invaders.firing==0){
    return 0;
  }
  si->invaders.bomb_y+=si->pixel_size;
  /*Si la bombe sort de l'écran remet invaders.firing à 0*/
  if(si->invaders.bomb_y>=si->window_height){
    si->invaders.firing=0;
    return 1;
  }
  /*si le tank est touché, remet invaders.firing à 0*/
  if(si_tank_is_hit(si)==1){
    si->invaders.firing=0;
    return 1;
  }

  return 1;
}

/*
* Renvoie 1 si les ennemis peuvent se déplacer vers la gauche, 0 sinon.
* Met à jour la position de la matrice.

int si_invaders_can_move_left(Si *si){

}*/


