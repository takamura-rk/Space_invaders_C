#include <stdio.h>
#include <SDL.h>
#include "si_font.h"
#include "si_sdl.h"

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
  running = 1;
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
