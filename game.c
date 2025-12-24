#include "game.h"
#include "si_sdl.h"
#include "si.h"
#include "si_menu.h"

Game *game_new(void)
{
  //INITIALISATION DE LA STRUCTURE g
  Game *g=malloc(sizeof *g);
  if (!g)
    return NULL;
  
  g->win=NULL;
  g->ren=NULL;
  g->window_width=546-6;
  g->window_height=720;
  g->pixel_size=3;
  g->play_game=0;
  g->si = NULL;
  g->freq=1;
  g->count_invaders=0;
  g->count_shoot=0;
  g->update=1;

  
  // SDL_Event events; (ligne plus tres utile)
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
      SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_Init error: %s\n", SDL_GetError());
      free(g);
      return NULL;
    }

  SDL_DisplayMode dm;
  SDL_GetCurrentDisplayMode(0, &dm);
  printf("Résolution : %d x %d\n", dm.w,dm.h);
    
  int x = dm.w/2 -273 +3; /* coord. x du coin haut gauche de la fenêtre */
  int y = dm.h/2 -360; /* coord. y du coin haut gauche de la fenêtre */
  g->window_width = 546-6; /* largeur de la fenêtre */
  g->window_height = 720; /* hauteur de la fenêtre */
  g->si = si_new(g->window_width, g->window_height, g->pixel_size);
  if (!g->si)
    {
      SDL_Quit();
      free(g);
      return NULL;
    }
  
  // création de la fenêtre
  g->win = SDL_CreateWindow("Space Invaders", x, y, g->window_width, g->window_height, SDL_WINDOW_SHOWN);
  if (g->win == NULL)
    {
      SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "error : %s\n", SDL_GetError());
      si_del(g->si);
      SDL_Quit();
      free(g);
      return NULL;
    }
  // création du renderer
  g->ren= SDL_CreateRenderer(g->win, -1, SDL_RENDERER_ACCELERATED);
  if (g->ren == NULL)
    {
      SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "error : %s\n", SDL_GetError());
      SDL_DestroyWindow(g->win);
      si_del(g->si);
      SDL_Quit();
      free(g);
      return NULL;
    }

  return g;
}

void game_del(Game *g)
{
  if (!g) return;
  if (g->ren) SDL_DestroyRenderer(g->ren);
  if (g->win) SDL_DestroyWindow(g->win);
  if (g->si) si_del(g->si);

  SDL_Quit();
  free(g);
}

void game_run(Game *g)
{
    if (!g) return;

    SDL_Event events;
    int running = 1;

    while (running) {
        while (SDL_PollEvent(&events)) {
            switch (events.type) {
            case SDL_QUIT:
                running = 0;
                break;
            case SDL_KEYDOWN:
                if (events.key.keysym.sym == SDLK_q) running = 0;
                break;
            }
        }

        if (g->update) {
            game_update(g);
        }
    }
}

void game_update(Game *g)
{
  /* FOND NOIR */
  SDL_SetRenderDrawColor(g->ren, 0, 0, 0, 255);
  SDL_RenderClear(g->ren);
  /* ON DESSINE TOUT EN BLANC ENSUITE */
  SDL_SetRenderDrawColor(g->ren, 255, 255, 255, 255);

  if (!g->play_game)
    {
      menu(g);
    }
  else if (g->si->life_1 == 0)
    {
      game_over(g);
    }
  else
    {
      /* Afficher les ennemis (matrice) */
      si_invaders_display(g, g->si->invaders.x, g->si->invaders.y);

      /* Si les ennemis tirent, afficher la bombe */
      if (g->si->invaders.firing)
	si_shoot_display(g, g->si->invaders.bomb_x, g->si->invaders.bomb_y);

      /* Afficher le tank (il est sur la dernière ligne de caractères) */
      si_tank_display(g, g->si->tank.x, (29 * 8 * g->pixel_size));

      /* Si le tank tire, afficher le tir */
      if (g->si->tank.firing)
	si_tank_shoot_display(g, g->si->tank.shoot_x, g->si->tank.shoot_y);
    }
  
  SDL_RenderPresent(g->ren);

  g->update = 0;
}
