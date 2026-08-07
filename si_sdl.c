#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "si_sdl.h"
#include "si_font.h"

#include "si.h"

extern char *si_get_matrix(void);

extern char si_font_tank[8][13];
extern char si_font_ufo[8][16];
extern char si_font_invaders[3][2][8][12];
extern char si_font_invader_explode[8][13];
extern char si_font_alphanum[43][8][5];
extern char si_font_tank_explode[2][8][16];
extern char si_font_tank_shoot[8][1];
extern char si_font_invader_shoot[8][5];

SDL_Window *win = NULL;
SDL_Renderer *ren = NULL;
int update = 1;

/* fonction qui à un caractère associe son index dans
   si_font_alphanum définie dans si_font.c */
int font_index_from_char(char c) 
{
  /* on convertit les minuscules en majuscules */
  if (c >= 'a' && c <= 'z')
    c = c - 'a' + 'A';
    
  if (c >= 'A' && c <= 'Z') return c - 'A';
  if (c >= '0' && c <= '9') return 26 + (c - '0');
  if (c == ' ')  return 36;
  if (c == '-')  return 37;
  if (c == '<')  return 38;
  if (c == '>')  return 39;
  if (c == '=')  return 40;
  if (c == '?')  return 41;
  if (c == '*') return 42;
  /* si on a ecrit un caractère non défini on affiche un espace */
  return 36;
}

void si_display_sprite(Game *g, char *data, int rows, int cols, int x, int y)
{
  for (int w = 0; w < rows; w++) {
    for (int h = 0; h < cols; h++) {
      if (data[w * cols + h]) {
	SDL_Rect pixel = {
	  x + h * g->pixel_size,
	  y + w * g->pixel_size,
	  g->pixel_size,
	  g->pixel_size
	};
	SDL_RenderFillRect(g->ren, &pixel);
      }
    }
  }
}

/* Fonction qui affiche une phrase à l'écran */
void si_text_display(Game *g, const char *text, int ligne, int colonne, int spacing)
{
    /* 1. On calcule la position de départ (Haut-Gauche) */
    /* On suppose qu'une case de la grille fait 7 unités de large (5px de lettre + 2px d'espace) */
    /* Et 8 unités de haut (hauteur de la lettre) */
    int x = colonne * (7 * g->pixel_size); 
    int y = ligne   * (8 * g->pixel_size);

    /* 2. On parcourt le texte lettre par lettre */
    for (int i = 0; text[i] != '\0'; i++) 
    {
        /* On récupère l'index du caractère dans notre tableau de police */
        int index = font_index_from_char(text[i]);

        /* On affiche le sprite de la lettre */
        /* 8 = hauteur du sprite, 5 = largeur du sprite */
        si_display_sprite(g, &si_font_alphanum[index][0][0], 8, 5, x, y);

        /* 3. On déplace le curseur X vers la droite pour la prochaine lettre */
        /* Décalage = (Largeur de la lettre * taille du pixel) + espacement supplémentaire */
        x += (5 * g->pixel_size) + spacing;
    }
}
void si_tank_display(Game *g, int x, int y)
{
  si_display_sprite(g, &si_font_tank[0][0], 8, 13, x, y);
}
void si_tank_shoot_display(Game *g, int x, int y)
{
  si_display_sprite(g, &si_font_tank_shoot[0][0], 8, 1, x, y);
}

void si_tank_explode_display(Game *g, int frame, int x, int y)
{
  si_display_sprite(g, &si_font_tank_explode[frame][0][0], 8, 16, x, y);
}

void si_tank_set_position(Game *g)
{
    int tank_width;

    si_font_tank_get(&tank_width);

    if (g->si->tank.x < 0)
        g->si->tank.x = 0;

    if (g->si->tank.x >
        g->si->window_width - tank_width * g->si->pixel_size)
    {
        g->si->tank.x =
            g->si->window_width - tank_width * g->si->pixel_size;
    }
}


void si_invader_display(Game *g, int type, int frame, int x, int y)
{
  si_display_sprite(g, &si_font_invaders[type][frame][0][0], 8, 12, x, y);
}
void si_invader_explode_display(Game *g, int x, int y)
{
  si_display_sprite(g, &si_font_invader_explode[0][0], 8, 13, x, y);
}
void si_ufo_display(Game *g, int x, int y)
{
  si_display_sprite(g, &si_font_ufo[0][0], 8, 16, x, y);
}
void si_invaders_shoot_display(Game *g, int x, int y)
{
  si_display_sprite(g, &si_font_invader_shoot[0][0],8,5,x,y);
}

void si_invaders_display(Game *g, int x0, int y0)
{
    char *m = si_get_matrix();

    /* Dimensions d’un sprite invader : 8 lignes x 12 colonnes */
    const int w = 12 * g->pixel_size;
    const int h = 8  * g->pixel_size;
    /* Espacement entre chacun des invaders */
    const int gap_x = 2 * g->pixel_size;
    const int gap_y = 5 * g->pixel_size;

    for (int row = 0; row < 5; ++row) {
        for (int col = 0; col < 11; ++col) {

            int v = m[row * 11 + col];
            if (v == 0) continue;
            //  1,2,3 dans la matrice d'ennemis -> type 0,1,2 pour si_invader_display
            int type = 0;
            if (v == 1)
	      type = 0;
            else if (v == 2)
	      type = 1;
            else if (v == 3)
	      type = 2;
            else continue; /* valeur inattendue */

            int x = x0 + col * (w + gap_x);
            int y = y0 + row * (h + gap_y);

            si_invader_display(g, type, g->invader_frame, x, y);
        }
    }
}
