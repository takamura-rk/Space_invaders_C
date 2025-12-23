#include "game.h"

Game *game_new(void)
{
  //INITIALISATION DE LA STRUCTURE g
  Game *g=malloc(sizeof *g);
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

  
  SDL_Event events;
  SDL_Init(SDL_INIT_VIDEO);
  SDL_DisplayMode dm;
  SDL_GetCurrentDisplayMode(0, &dm);
  printf("Résolution : %d x %d\n", dm.w,dm.h);
    
  int x = dm.w/2 -273 +3; /* coord. x du coin haut gauche de la fenêtre */
  int y = dm.h/2 -360; /* coord. y du coin haut gauche de la fenêtre */
  g->window_width = 546-6; /* largeur de la fenêtre */
  g->window_height = 720; /* hauteur de la fenêtre */

  
  /* SDL doit être initialisé */
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "error : %s\n", SDL_GetError());
  }
  
  /* création de la fenêtre */
  g->win = SDL_CreateWindow("Space Invaders", x, y, g->window_width, g->window_height, SDL_WINDOW_SHOWN);
  if (g->win == NULL) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "error : %s\n", SDL_GetError());
    SDL_Quit();
  }

  g->ren= SDL_CreateRenderer(g->win, -1, SDL_RENDERER_ACCELERATED);
  if (g->ren == NULL) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "error : %s\n", SDL_GetError());
    SDL_DestroyWindow(g->win);
  }

/*
  // boucle des évènements
  int running = 1;
  int ret = 1;
  
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
      game_update();
  }
  
  ret = 0;

  SDL_DestroyRenderer(ren);
 destroy_window:
  SDL_DestroyWindow(win);
 sdl_quit:
  SDL_Quit();
 err:
  return ret;
*/

  return g;
}

void game_update(Game *g)
{
  /* FOND NOIR */
  SDL_SetRenderDrawColor(g->ren, 0, 0, 0, 255);
  SDL_RenderClear(g->ren);
  /* ON DESSINE TOUT EN BLANC ENSUITE */
  SDL_SetRenderDrawColor(g->ren, 255, 255, 255, 255);
  
  si_text_display("SCORE<1> HI-score score<2>", 0, 0, 3, 6);
  si_text_display("0000", 2, 2, 3, 6);
  si_text_display("0000", 2, 11, 3, 6);
  si_text_display("0000", 2, 20, 3, 6);
  si_text_display("PLAY", 7, 11, 3, 6);
  si_text_display("SPACE  INVADERS", 9, 6, 3, 6);
  si_text_display("*SCORE ADVANCE TABLE*", 12, 2, 3, 6);
  si_text_display("? MYSTERY", 14, 9, 3, 6);
  si_ufo_display(110, 340, 3);
  si_text_display("=30 POINTS", 16, 8, 3, 6);
  si_invader_display(0, 0, 118, 386, 3);
  si_text_display("=20 POINTS", 18, 8, 3, 6);
  si_invader_display(1, 0, 118, 435, 3);
  si_text_display("=10 POINTS", 20, 8, 3, 6);
  si_invader_display(2, 0, 118, 482, 3);
  si_text_display("CREDIT", 29, 9, 3, 6);

  /*
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
  si_ufo_display(150, 500, 3);
  si_shoot_display(120,240,3);
  */
  /* copie du renderer dans la fenêtre */
  SDL_RenderPresent(g->ren);
  update = 0;
}
