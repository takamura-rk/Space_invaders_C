#ifndef SI_H
#define SI_H

#include <SDL2/SDL.h>

/* type d'ennemi */
typedef enum
{
  SI_TYPE_SQUID = 1,
  SI_TYPE_CRAB,
  SI_TYPE_OCTOPUS,
}Si_Type;



typedef struct Invaders Invaders;
typedef struct Tank Tank;
typedef struct Ufo Ufo;
typedef struct Si Si;

struct Invaders
{
  int x;
  int y;
  int direction; /*+-1*/
  int firing;  /*is fire ?*/
  int bomb_x;
  int bomb_y;
};
struct Tank
{
  int x;
  int firing;
  int shoot_x;
  int shoot_y;
  int destroyed;
  int destroyed_count;
};
/* Structure UFO */
struct Ufo
{
  int active;
  int x;
  int y;
  int dir; /* +-1 ,va vers la gauche ou la droite */
  Uint64 spawn_counter;
  Uint64 move_counter;
  Uint64 next_spawn_allowed;
};
struct Si
{
  int window_width;
  int window_height;
  int pixel_size;
  int nbr_players;
  int score_1;
  int life_1;
  int score_2;
  int life_2;
  int score_highest;
  int wave;

 
  int sound_shoot_flag;// bruit de tire
  int sound_explode_flag;//bruit d'une explosion
  int sound_wave_flag;// bruit lors d'une nouvelle vague

  Tank tank;
  Invaders invaders;
  /* Ufo */
  Ufo ufo;
};

 /*
 * crée un pointeur vers la structure Si. On passe en paramètre
 * les dimension de la fenêtre et la taille de pixel
 */
 Si *si_new(int window_width, int window_height, int pixel_size);

void si_del(Si *si);

/*Détruit les ressources */
void si_sdl(Si *si);

/*renvoie la matrice */
char *si_get_matrix(void);

/*renvoi le nombre d'ennemis restant dans la matrice */
int si_matrix_count(void);

/*renvoi le nombre de point en fonction de l'ennemi*/
int si_get_points(Si_Type t);

/*tank*/

/*
 * teste si le tank peut se d\’eplacer vers le haut. SI c’est possible,
 * met à jour la coordonné y du tir da Tank.
 */
int si_tank_shoot_can_move_up(Si *si, int *hit_row, int *hit_col);

 /* renvoie 1 si le tank est touch\’e, 0 sinon */
 int si_tank_is_hit(Si *si);


 /* invaders */

 /*
 * Détermine dans quelle colonnnede la matrice, choisie aléatoirement,
 * la bombe est lachée. Met à jour les coordonn\’ees de la bombe
 * dans Invaders.
 */
 void si_invaders_get_column(Si *si);

 /*
 * Renvoie 1 si la bombe peut se déplacer vers le bas, 0 sinon.
 * Met à jour la coordonnéee y de la bombe.
 */
 int si_invaders_bomb_can_move_down(Si *si);

 /*
 * Renvoie 1 si les ennemis peuvent se déplacer vers la gauche, 0 sinon.
 * Met à jour la position de la matrice.
 */

int si_invaders_can_move_left(Si *si);

 /*
 * Renvoie 1 si les ennemis peuvent se déplacer vers la droite, 0 sinon.
 * Met à jour la position de la matrice.
 */

 int si_invaders_can_move_right(Si *si);

 /*
 * Renvoie 1 si un ennemi est touché par le tir du tank, 0 sinon.
 * Met à jour la matrice.
 */
int si_invader_is_hit(Si *si, int *hit_row, int *hit_col);

 /*
   renvoie les points gagnés si l'ufo est touché, sinon 0
 */
int si_ufo_is_hit(Si *si); 

/* Fonctions de gestion de partie */
void si_reset(Si *si);
void si_next_wave(Si *si);
int si_check_collision_bottom(Si *si); // Vérifie si les ennemis touchent le fond */

#endif 

