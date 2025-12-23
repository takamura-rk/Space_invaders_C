#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "si_sdl.h"
#include "si_font.h"


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

void si_display_sprite(char *data, int rows, int cols, int x, int y, int scale)
{
  for (int w = 0; w < rows; w++) {
    for (int h = 0; h < cols; h++) {
      if (data[w * cols + h]) {
	SDL_Rect pixel = {
	  x + h * scale,
	  y + w * scale,
	  scale,
	  scale
	};
	SDL_RenderFillRect(ren, &pixel);
      }
    }
  }
}

/* fonction qui affiche une phrase */
void si_text_display(const char *text, int l, int c, int scale, int spacing)
{
  int i = 0;
  int x = c *7*3;
  int y = l *8*3;
  while (text[i] != '\0') {   // tant qu'on n'est pas à la fin de la phrase
    int index = font_index_from_char(text[i]);
    si_display_sprite(&si_font_alphanum[index][0][0], 8, 5, x, y, scale);
    x += 5 * scale + spacing; /* se déplace vers la droite pour la prochaine lettre */
    i++;
  }
}
void si_tank_display(int x, int y, int scale)
{
  si_display_sprite(&si_font_tank[0][0], 8, 13, x, y, scale);
}
void si_tank_shoot_display(int x, int y, int scale)
{
  si_display_sprite(&si_font_tank_shoot[0][0], 8, 1, x, y, scale);
}

void si_tank_explode_display(int frame, int x, int y, int scale)
{
  si_display_sprite(&si_font_tank_explode[frame][0][0], 8, 16, x, y, scale);
}
void si_invader_display(int type, int frame, int x, int y, int scale)
{
  si_display_sprite(&si_font_invaders[type][frame][0][0], 8, 12, x, y, scale);
}
void si_invader_explode_display(int x, int y, int scale)
{
  si_display_sprite(&si_font_invader_explode[0][0], 8, 13, x, y, scale);
}
void si_ufo_display(int x, int y, int scale)
{
  si_display_sprite(&si_font_ufo[0][0], 8, 16, x, y, scale);
}
void si_shoot_display(int x, int y, int scale)
{
  si_display_sprite(&si_font_invader_shoot[0][0],8,5,x,y,scale);
}




