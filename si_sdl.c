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
SDL_Renderer *renderer = NULL;
int update = 1;

/* fonction qui à un caractère associe son index dans
   si_font_alphanum définie dans si_font.c */
static int font_index_from_char(char c) 
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
  if (c == '.')  return 40;
  if (c == '?')  return 41;
  if (c == '\'') return 42;
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
	SDL_RenderFillRect(renderer, &pixel);
      }
    }
  }
}

/* fonction qui affiche une phrase */
void si_text_display(const char *text, int l, int c, int scale, int spacing)
{
  int i = 0;
  int x = l *7*3;
  int y = c *8*3;
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

void render_update()
{
  /* FOND NOIR */
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);
  /* ON DESSINE TOUT EN BLANC ENSUITE */
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  
  si_text_display("SCORE<1> HI-score score<2>", 0, 0, 3, 6);

  si_tank_display(300, 550, 3);
  si_tank_shoot_display(300, 510, 3);
  si_tank_explode_display(0, 350, 550, 3);
  si_tank_explode_display(1, 400, 550, 3);
    
  si_invader_display(0, 1, 0, 200, 3);
  si_invader_display(0, 0, 40, 200, 3);
  si_invader_display(1, 0, 80, 200, 3);
  si_invader_display(1, 1, 120, 200, 3);
  si_invader_display(2, 0, 160, 200, 3);
  si_invader_display(2, 1, 200, 200, 3);
  si_invader_explode_display(240, 200, 3);
  si_ufo_display(150, 100, 3);
  si_shoot_display(120,240,3);

  /* copie du renderer dans la fenêtre */
  SDL_RenderPresent(renderer);
  update = 0;
}

int create_window(void)
{
  SDL_Event events;
  SDL_Init(SDL_INIT_VIDEO);
  SDL_DisplayMode dm;
  SDL_GetCurrentDisplayMode(0, &dm);
  printf("Résolution : %d x %d\n", dm.w,dm.h);
    
  int x = dm.w/2 -273 +3; /* coord. x du coin haut gauche de la fenêtre */
  int y = dm.h/2 -360; /* coord. y du coin haut gauche de la fenêtre */
  int w = 546-6; /* largeur de la fenêtre */
  int h = 720; /* hauteur de la fenêtre */
  int running = 1;
  int ret = 1;
  
  /* SDL doit être initialisé */
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "error : %s\n", SDL_GetError());
    goto err;
  }
  
  /* création de la fenêtre */
  win = SDL_CreateWindow("Space Invaders", x, y, w, h, SDL_WINDOW_SHOWN);
  if (win == NULL) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "error : %s\n", SDL_GetError());
    goto sdl_quit;
  }

  renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
  if (renderer == NULL) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "error : %s\n", SDL_GetError());
    goto destroy_window;
  }

  /* boucle des évènements */
  while (running) {
    while (SDL_PollEvent(&events)) {
      switch (events.type) {
      case SDL_QUIT:
	running = 0;
	break;
      case SDL_KEYDOWN:
	switch (events.key.keysym.sym) {
	case SDLK_q:
	  running = 0;
	  break;
	}
	break;
      }
    }

    if (update)
      render_update();
  }
  
  ret = 0;

  SDL_DestroyRenderer(renderer);
 destroy_window:
  SDL_DestroyWindow(win);
 sdl_quit:
  SDL_Quit();
 err:
  return ret;
}
