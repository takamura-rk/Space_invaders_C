#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include "si_sdl.h"



SDL_Window *win = NULL;
SDL_Renderer *renderer = NULL;
int update = 1;
void render_update(int w, int h)
{
  /* FOND NOIR */
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);
  /* ON DESSINE EN BLANC  ENSUITE*/
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  
  for (int i=0; i<w ; i++){
      for (int j=0 ; j<3 ; j++){
	  /* dessin du pixel*/
	  SDL_RenderDrawPoint(renderer, i, j );
	}
    }
  
  /* copie du renderer dans la fenêtre */
  SDL_RenderPresent(renderer);
  update = 0;
}

int create_window(void)
{
  SDL_Event events;
  int x=60; /* coord. x du coin haut gauche de la fenêtre */
  int y=60; /* coord. y du coin haut gauche de la fenêtre */
  int w=800; /* largeur de la fenêtre */
  int h=600; /* hauteur de la fenêtre */
  int running=1;
  int ret = 1;
  
  /* SDL doit être initialisé */
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
      SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "error : %s\n", SDL_GetError());
      goto err;
    }
  
  /* création de la fenêtre */
  win = SDL_CreateWindow("Space Invaders", x, y, w, h, SDL_WINDOW_SHOWN);
  
  if (win == NULL)
    {
      SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "error : %s\n", SDL_GetError());
      goto sdl_quit;
    }
  renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
  if (renderer == NULL)
    {
      SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "error : %s\n", SDL_GetError());
      goto destroy_window;
    }
  
  /* création des évènements */
  while (running)
    {
      while (SDL_PollEvent(&events))
	{
	  switch (events.type)
	    {
	      /*cliquer sur la croix ferme la fenêtre */
	    case SDL_QUIT:
	      {
		/* on sort de la boucle */
		running = 0;
		break;
	      }
	    case SDL_KEYDOWN:
	      {
		switch (events.key.keysym.sym)
		  {
		  case SDLK_q:
		    running = 0;
		    break;
		  }
		break;
	      }
	    }
	}

      if (update)
	render_update(w, h);
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
