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


