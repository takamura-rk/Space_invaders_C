#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "si.h"

char matrice[5][11]=
  {
    {1,1,1,1,1,1,1,1,1,1,1},
    {2,2,2,2,2,2,2,2,2,2,2},
    {2,2,2,2,2,2,2,2,2,2,2},
    {3,3,3,3,3,3,3,3,3,3,3},
    {3,3,3,3,3,3,3,3,3,3,3}
  };

Si *si_new(int window_width, int window_height, int pixel_size)
{
  Si *si = (Si *)malloc(sizeof(Si));
  if (!si) return NULL;
  
  si->window_width=window_width;
  si->window_height=window_height;
  si->pixel_size=pixel_size;
  si->nbr_players=1;
  si->score_1 = 0;
  si->life_1 = 3; 
  si->score_2 = 0;
  si->life_2 = 3;
  si->wave = 1; 
  si->sound_shoot_flag = 0;
  si->sound_explode_flag = 0;
  si->sound_wave_flag = 0;
  
  /* On charge le fichier high score */
  si->score_highest = 0;
  FILE *f = fopen("highscore.txt", "r");
  if(f) {
      if(fscanf(f, "%d", &si->score_highest) != 1) si->score_highest = 0;
      fclose(f);
  }

  /* On initialise les données du tank, des invaders et de l'ufo */
  si->tank.x = (window_width / 2) - (13 * pixel_size) / 2;
  si->tank.firing = 0;
  si->tank.shoot_x=0;
  si->tank.shoot_y=0;
  si->tank.destroyed=0;
  si->tank.destroyed_count=0;

  si->invaders.x=pixel_size;
  si->invaders.y=pixel_size * 35; 
  si->invaders.direction=1;
  si->invaders.firing=0;
  si->invaders.bomb_x = 0;
  si->invaders.bomb_y = 0;
  
  si->ufo.active = 0;
  si->ufo.y = pixel_size * 35;
  si->ufo.spawn_counter = SDL_GetPerformanceCounter();
  si->ufo.move_counter = SDL_GetPerformanceCounter();
  si->ufo.next_spawn_allowed = SDL_GetPerformanceCounter();

  
  return si;
}

/* libération de mémoire */
void si_del(Si *si)
{
  free(si);
}
/*réinitalise l'état du jeu au lancement ou lors d'un redémarrage*/
void si_reset(Si *si)
{
  /*réinitialisation des informations du jeu*/
  si->score_1 = 0;
  si->life_1 = 3;
  si->wave = 1; 
  /*initialisation du tank*/
  si->tank.x = (si->window_width / 2) - (13 * si->pixel_size) / 2;
  si->tank.firing = 0;
  si->tank.destroyed = 0;
  /*initialisation des invaders*/
  si->invaders.x = si->pixel_size;
  si->invaders.y = si->pixel_size * 60;
  si->invaders.direction = 1;
  si->invaders.firing = 0;
  /*initialisation de l'ufo*/
  si->ufo.active = 0;
  si->ufo.spawn_counter = SDL_GetPerformanceCounter();
  si->ufo.move_counter = SDL_GetPerformanceCounter();
  si->ufo.next_spawn_allowed = SDL_GetPerformanceCounter();
  /*direction de déplacement aléatoire*/
  si->ufo.dir = (rand() % 2) ? 1 : -1;

  /*réinitialisation des effet sonnores*/
  si->sound_shoot_flag = 0;
  si->sound_explode_flag = 0;
  si->sound_wave_flag = 0;
  
  /*réinitalisation de la matrice des invaders*/
  for(int j=0; j<11; j++) matrice[0][j] = 1;//SQUID
  for(int j=0; j<11; j++) matrice[1][j] = 2;//CRAB
  for(int j=0; j<11; j++) matrice[2][j] = 2;//CRAB
  for(int j=0; j<11; j++) matrice[3][j] = 3;//OCTOPUS
  for(int j=0; j<11; j++) matrice[4][j] = 3;//OCTOPUS
}

/*réinitialisation de la matrice des invaders pour le lancement d'une nouvelle vague*/ 
void si_next_wave(Si *si)
{
  /* passage à la vague suivante */
  si->wave++;
  /*délenchement du sens de début de vague */
  si->sound_wave_flag = 1;
  /*réinitialisation de la position et de l'état des invaders*/
  si->invaders.x = si->pixel_size;
  si->invaders.y = si->pixel_size * 60;
  si->invaders.direction = 1;
  si->invaders.firing = 0;
  si->invaders.bomb_x = 0;
  si->invaders.bomb_y = 0;
  /* arrêt du tir du tank*/
  si->tank.firing = 0;
  /*réinitialisation de la matrice des invaders*/
  for(int j=0; j<11; j++) matrice[0][j] = 1;
  for(int j=0; j<11; j++) matrice[1][j] = 2;
  for(int j=0; j<11; j++) matrice[2][j] = 2;
  for(int j=0; j<11; j++) matrice[3][j] = 3;
  for(int j=0; j<11; j++) matrice[4][j] = 3;
}

/* Calcule les dimensions de la matrice des invaders encore en vie */
static int invaders_alive(int *min_row, int *max_row, int *min_col, int *max_col)
{
    int found = 0;
    /*valeurs initiales pour trouver les extrémités */
    int rmin = 5, rmax = -1, cmin = 11, cmax = -1;
    /*parcours de la matrice*/
    for (int r = 0; r < 5; r++) {
        for (int c = 0; c < 11; c++) {
	  /*on teste si un invaders est présent ou non*/
	  if (matrice[r][c] != 0) {
                found = 1;
		/*mise à jour des extrémités des lignes*/
                if (r < rmin) rmin = r;
                if (r > rmax) rmax = r;
		/*mise à jour des extrémités des colonnes*/
                if (c < cmin) cmin = c;
                if (c > cmax) cmax = c;
            }
        }
    }

    /*il ne reste plus d'invaders*/
    if (!found)
      return 0;
    /*on renvoie les nouvelles extrémités de la zone occupée par les invaders*/
    *min_row = rmin; *max_row = rmax;
    *min_col = cmin; *max_col = cmax;
    return 1;
}
/*renvoie 1 si les invaders on atteint ou dépassé le tank et 0 sinon */
int si_check_collision_bottom(Si *si)
{
    int min_row, max_row, min_col, max_col;
    /*on vérifie s'il reste des invaders non détruit et on récupère leurs limite*/
    if (!invaders_alive(&min_row, &max_row, &min_col, &max_col))
        return 0; // plus d'invaders

    int inv_h = 8 * si->pixel_size;//hauteur d'un invader
    int gap_y = 3 * si->pixel_size;// espacement vertical

    /* bas des envahisseurs */
    int invaders_bottom =
        si->invaders.y + max_row * (inv_h + gap_y) + inv_h;
    /*position du heut du tank*/
    int tank_top = si->window_height - (16 * si->pixel_size);
    
    return invaders_bottom >= tank_top;
}
/* renvoie le nombre d’ennemis restant dans la matrice */
int si_matrix_count(void)
{
  int ennemis=0;
  for(int i=0;i<5;i++) {
      for(int j=0;j<11;j++) {
	  if (matrice[i][j]!=0) ennemis+=1;
	}
    }
  return ennemis;
}
/*compare le score actuel avec le meilleur score enregistré et met à jour le meilleur score si un nouveau est atteint */
void si_check_highscore(Si *si)
{
  /*on vérifie si le score actuel dépasse le meilleur score déjà enrendistré*/
  if (si->score_1 > si->score_highest) {
    si->score_highest = si->score_1;// on met à jour le meilleur score
    FILE *f = fopen("highscore.txt", "w");// on ouvre le fichier de sauvgarde du meilleur score
        if(f) {
	  /*on écrit le nouveau meilleur score */
            fprintf(f, "%d", si->score_highest);
            fclose(f);
        }
    }
}

char *si_get_matrix(void)
{
  return &matrice[0][0];
}
int si_get_nbr_players(Si *si)
{
  return si->nbr_players;
}

int si_get_points(Si_Type t)
{
  if(t==1) return 30; 
  if(t==2) return 20; 
  if(t==3) return 10; 
  return 0;
}


/* renvoie le nombre de points que l'Ufo donne s'il est touché et 0 sinon */
int si_ufo_is_hit(Si *si)
{
  /*si l'Ufo n'est pas sur l'écran il ne peut pas être touché */
  if (!si->ufo.active)
    return 0;
  /*si le tank ne tire pas l'ufo ne peut pas être touché*/
  if (!si->tank.firing)
      return 0;

    int ps = si->pixel_size;

    /* dimensions et coordonnées du rectangle de l' UFO  */
    int ufo_w = 16 * ps;
    int ufo_h = 8 * ps;  
    int ufo_x = si->ufo.x;
    int ufo_y = si->ufo.y;
    
    /* dimensions et coordonnées du rectangle du tir du tank */
    int shot_x = si->tank.shoot_x;
    int shot_y = si->tank.shoot_y;
    int shot_w = 1 * ps;
    int shot_h = 8 * ps;

    /* On test l'intersection des rectangles */
    int collision =
        (shot_x < ufo_x + ufo_w) &&
        (shot_x + shot_w > ufo_x) &&
        (shot_y < ufo_y + ufo_h) &&
        (shot_y + shot_h > ufo_y);

    if (!collision)
      return 0;

    /* touché : on retire l'UFO et on stoppe le tir */
    si->ufo.active = 0;
    si->tank.firing = 0;

    /* points UFO (50/100/150/300) */
    int points_table[15] = {50,100,150,300};
    int points = points_table[rand() % 4];
    si->score_1 += points;
    si_check_highscore(si);
    /* on joue un son d'explosion  */
    si->sound_explode_flag = 1;

    return points;
}

/*
* teste si le tir du  tank peut se déplacer vers le haut. SI c’est possible,
* met à jour la coordonné y du tir da Tank.
*/
int si_tank_shoot_can_move_up(Si *si, int *hit_row, int *hit_col)
{
  /* si le tank ne tire pas il n'y a pas de mise à jour */
  if(si->tank.firing==0) return 0;
  
  si->tank.shoot_y -= (si->pixel_size * 2); //déplacement vérticale du tir ver le haut
  /*si le tir atteint le haut de l'écran on arréte le tir */
  if(si->tank.shoot_y <= 10 * si->pixel_size)
    {
      si->tank.firing=0;
      return 1;
    }
  
  /*vérifie s'il y a une collision avec un invader ou l'ufo le tir disparaît */
  if (si_invader_is_hit(si, hit_row, hit_col) || si_ufo_is_hit(si))
    {
      si->tank.firing = 0;
      return 1;
    }
  /*le tir continue son déplacement*/
  return 1;
}

/* renvoie 1 si le tank est touché, 0 sinon */
int si_tank_is_hit(Si *si)
{
  /*si les invaders ne tirent pas le tank ne peut pas être touché*/
  if(si->invaders.firing == 0) return 0;

  /*coordonées et dimonsions du rectangle de la bombe des invaders*/
  int bx = si->invaders.bomb_x;
  int by = si->invaders.bomb_y;
  int bw = 5 * si->pixel_size;
  int bh = 8 * si->pixel_size;

  /*coordonnées et dimonsions du rectangle du  tank*/
  int tx = si->tank.x;
  int ty = si->window_height - (16 * si->pixel_size); 
  int tw = 13 * si->pixel_size;
  int th = 8 * si->pixel_size;

  /*test de collision avec l'intersection des 2 rectangles */
  if (bx < tx + tw && bx + bw > tx && by < ty + th && by + bh > ty)
    {
      si->tank.destroyed = 1; //le tank est touché donc il est mit sur l'état de déstruction
      si->invaders.firing = 0;//arrêt du tir de l'invader
      si->sound_explode_flag = 1;// déclenchement du son de l'explosion
      return 1;
    }
  /*aucune collision détecté*/
  return 0;
}

/*
* teste si le tank peut se d\’eplacer vers le haut. SI c’est possible,
* met à jour la coordonné y du tir da Tank.
*/
void si_invaders_get_column(Si *si){
  int i;
  int col = rand() % 11;// sélection aléatoire d'une colonne parmi les 11 colonnes d'invaders
  int lar_inv=12 * si->pixel_size;//largeur d'un invader
  int gap_x=2 * si->pixel_size;//espacement horizontal
  int lar_bomb=5 * si->pixel_size;//largeur de la bombe
  int haut_inv=8 * si->pixel_size;//hauteur d'un invader
  int gap_y=3 * si->pixel_size;//hauteur d'un envahisseur
  /*parcourir la colonne choisi de bas en haut pour trouver l'invader le plus bas encore en vie */
  for(i=4;i>=0;i--) {
      if(matrice[i][col]!=0) {
	/*calcul de la position horizontale et verticale de la bombe centré par rapport à l'invader */
          si->invaders.bomb_x = si->invaders.x + col * (lar_inv + gap_x) + (lar_inv - lar_bomb)/2;
          si->invaders.bomb_y = si->invaders.y + i * (haut_inv + gap_y) + haut_inv;
          return;
      }
  }
  /*dans le cas où y a pas d'inveder sur cette colonne on ne tire pas*/
  si->invaders.firing=0; 
}

/*
* Détermine dans quelle colonnnede la matrice, choisie aléatoirement,
* la bombe est lachée. Met à jour les coordonn\’ees de la bombe
* dans Invaders.
*/
int si_invaders_bomb_can_move_down(Si *si)
{
  /* si aucun invader n'est entrain de tirer il n'y a rien à déplacer*/
  if (si->invaders.firing==0) return 0;
  
  si->invaders.bomb_y += si->pixel_size;// déplacement vertical de la bombe ennemi

  /*si la bombe sort de l'écran par le bas firing est remis à 0 */
  if(si->invaders.bomb_y >= si->window_height){
    si->invaders.firing=0;
    return 1;
  }

  /*test de collision entre le rectangle du tank et celui de la bombe*/
  if(si_tank_is_hit(si)==1)
    return 1;
  return 1;// la bombe continue sont déplacement 
}

/*
* Renvoie 1 si les ennemis peuvent se déplacer vers la gauche, 0 sinon.
* Met à jour la position de la matrice.
*/
int si_invaders_can_move_left(Si *si)
{
  /*si les invaders vont à droite on fait rien*/
    if (si->invaders.direction != -1) return 0;

    int min_row, max_row, min_col, max_col;
    /* s'il reste des envahisseurs vivants on récupère leurs limites*/
    if (!invaders_alive(&min_row, &max_row, &min_col, &max_col))
        return 0;

    int inv_w = 12 * si->pixel_size;//largeur des invaders
    int gap_x = 2 * si->pixel_size;//espacement entre les invaders

    /* colonne la plus à gauche encore vivante */
    int left_x = si->invaders.x + min_col * (inv_w + gap_x);

    if (left_x - si->pixel_size >= 0) {
        si->invaders.x -= si->pixel_size;//déplacement à droite 
        return 1;
    }

    /* on descend et on change de sens */
    si->invaders.y += (8 + 3) * si->pixel_size;// descente verticale
    si->invaders.direction = +1;// inversion du sens de déplacement 
    return 1;// mouvement effcetué
}


/*
* Renvoie 1 si les ennemis peuvent se déplacer vers la droite, 0 sinon.
* Met à jour la position de la matrice.
*/
int si_invaders_can_move_right(Si *si)
{
  /*si les invaders vont à gauche on fait rien*/
    if (si->invaders.direction != 1) return 0;

    int min_row, max_row, min_col, max_col;

   /* s'il reste des envahisseurs vivants on récupère leurs limites*/
    if (!invaders_alive(&min_row, &max_row, &min_col, &max_col))
      return 0;

    int inv_w = 12 * si->pixel_size;//largeur des invaders
    int gap_x = 2 * si->pixel_size;//espacement entre les invaders

    /* colonne la plus à droite  encore vivante */
    int right_x = si->invaders.x + max_col * (inv_w + gap_x) + inv_w;

    /*si cette colonne ne dépasse pas la largeur de la fenêtre on avance*/
    if (right_x + si->pixel_size <= si->window_width)
      {
	si->invaders.x += si->pixel_size;//déplacement à droite  
	return 1; 
      }

    /* on descend et on change de sens */
    si->invaders.y += 8 * si->pixel_size;// descente verticale
    si->invaders.direction = -1;// inversion du sens de déplacement 
    return 1; // mouvement effcetué
}

/*
* Renvoie 1 si un ennemi est touché par le tir du tank, 0 sinon.
* Met à jour la matrice.
*/
int si_invader_is_hit(Si *si, int *hit_row, int *hit_col)
{
  /*si le tank ne tire pas, pas de collision possible donc on renvoie 0*/
   if(si->tank.firing == 0)
     return 0;

   /*dimension et espacement des invaders */
   int inv_w = 12 * si->pixel_size;
   int inv_h = 8 * si->pixel_size;
   int gap_x = 2 * si->pixel_size;
   int gap_y = 3 * si->pixel_size;
   /*dimensions du rectangle du tir du tank*/
   int sx = si->tank.shoot_x;
   int sy = si->tank.shoot_y;
   int sw = 1 * si->pixel_size;
   int sh = 8 * si->pixel_size;

   /* parcours de la matrice des ennemis pour testé s'il y a une intersection des réctangles */
   for(int row = 0; row < 5; ++row) {
       for(int col = 0; col < 11; ++col) {
	 /*si un nnemi à déjà été détruis on l'ignore*/
           if (matrice[row][col] == 0)
	     continue;
	   /* calcul des coordonées de l'invader sur l'écran*/
           int ax = si->invaders.x + col * (inv_w + gap_x);
           int ay = si->invaders.y + row * (inv_h + gap_y);

           if (sx < ax + inv_w && sx + sw > ax &&
               sy < ay + inv_h && sy + sh > ay)
	     {
	       si->score_1 += si_get_points(matrice[row][col]);//incrémentation du score
               matrice[row][col] = 0;// supression de l'invader touché
               si_check_highscore(si);// vérification et mise à jour du meilleur score
               si->sound_explode_flag = 1;// déclenchement du bruit de l'explosion

	       if (hit_row)
		 *hit_row = row;
	       if (hit_col)
		 *hit_col = col;
               return 1;
	     }
       }
   }
   return 0;// aucune collision détecté
}

