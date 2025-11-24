#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include "si_sdl.h"



static SDL_Window *win = NULL;
static SDL_Renderer *renderer = NULL;
static int update = 1;
static void update(int w, int h)
{
  /* sélection de la couleur blanche pour le renderer */
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  /* remplissage de la couleur pour le renderer */
  SDL_RenderClear(renderer);

  double width = ;
  double height = ;

  for (int i=0; i<w ; i++)
    {
      for (int j=0 ; j<h ; j++)
	{
	  /* coordonnées du pixel*/
	  double x = x_min + i*(width/w);
	  double y = y_min + j*(height/h);
	  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	  SDL_RenderDrawPoint(renderer, i, j );
	}
    }
  
  /* copie du renderer dans la fenêtre */
  SDL_RenderPresent(renderer);
  update = 0;
}

int main(int argc, char *argv[])
{
  SDL_Event events;
  int x; /* coord. x du coin haut gauche de la fenêtre */
  int y; /* coord. y du coin haut gauche de la fenêtre */
  int w; /* largeur de la fenêtre */
  int h; /* hauteur de la fenêtre */
  int running;
  
  int ret = 1;
  
  /* SDL doit être initialisé */
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
      SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "error : %s\n", SDL_GetError());
      goto err;
    }
  
  /* création de la fenêtre */
  x = 60;
  y = 60;
  w = 800;
  h = 600;
  win = SDL_CreateWindow("Fractale", x, y, w, h, SDL_WINDOW_SHOWN);
  
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
  running = 1;
  while (running)
    {
      while (SDL_PollEvent(&events))
	{
	  switch (events.type)
	    {
	      /*ciquer sur la croix ferme la fenêtre */
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
	fractal_update(w, h);
    }
  

  ret = 0;

  SDL_DestroyRenderer(renderer);
 destroy_window:
  SDL_DestroyWindow(win);
 sdl_quit:
  SDL_Quit();
 err:
  return ret;
  
  (void)argc;
  (void)argv;
}
