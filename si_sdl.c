#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "si_sdl.h"
#include "si_font.h"
#include "game.h"
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

/* fonction qui affiche une phrase */
void si_text_display(Game *g,const char *text, int l, int c, int spacing)
{
  int i = 0;
  int x = c *7*3;
  int y = l *8*3;
  while (text[i] != '\0') {   // tant qu'on n'est pas à la fin de la phrase
    int index = font_index_from_char(text[i]);
    si_display_sprite(g, &si_font_alphanum[index][0][0], 8, 5, x, y);
    x += 5 * g->pixel_size + spacing; /* se déplace vers la droite pour la prochaine lettre */
    i++;
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
void si_shoot_display(Game *g, int x, int y)
{
  si_display_sprite(g, &si_font_invader_shoot[0][0],8,5,x,y);
}

void si_invaders_display(Game *g, int x0, int y0)
{
    /* Matrice 5x11 aplatie */
    char *m = si_get_matrix();

    /* Dimensions d’un sprite invader : 8 lignes x 12 colonnes */
    const int SPR_W = 12 * g->pixel_size;
    const int SPR_H = 8  * g->pixel_size;

    /* Espacement entre ennemis (tu peux ajuster) */
    const int GAP_X = 4 * g->pixel_size;
    const int GAP_Y = 4 * g->pixel_size;

    for (int row = 0; row < 5; ++row) {
        for (int col = 0; col < 11; ++col) {

            int v = m[row * 11 + col];
            if (v == 0) continue; /* ennemi détruit */
            //  1,2,3 dans ta matrice -> type 0,1,2 pour si_invader_display
            int type = 0;
            if (v == 1)
	      type = 0;
            else if (v == 2)
	      type = 1;
            else if (v == 3)
	      type = 2;
            else continue; /* valeur inattendue */

            int x = x0 + col * (SPR_W + GAP_X);
            int y = y0 + row * (SPR_H + GAP_Y);

            /* ta fonction déjà existante */
            si_invader_display(g, type, 0, x, y);
        }
    }
}
