#include "game.h"
#include "si_sdl.h"
#include "si.h"
#include "si_menu.h"
#include "si_font.h"

Game *game_new(void)
{
  //INITIALISATION DE LA STRUCTURE g
  Game *g=malloc(sizeof *g);
  if (!g)
    return NULL;
  
  g->win=NULL;
  g->ren=NULL;
  g->window_width=546-6;/* largeur de la fenêtre */
  g->window_height=720;/* hauteur de la fenêtre */
  g->pixel_size=3;
  g->play_game=0;
  g->si = si_new(g->window_width,g->window_height,g->pixel_size);
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
 
    
  int x = dm.w/2 -273 +3; /* coord. x du coin haut gauche de la fenêtre */
  int y = g->window_height/2 -360; /* coord. y du coin haut gauche de la fenêtre */
 

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
  int running ;
  running = 1;
  


  while (running)
    {
      SDL_Event events;
      Uint64 c_shoot;
      Uint64 c_invaders;
      while (SDL_PollEvent(&events))
	{
	  switch (events.type)
	    {
	    case SDL_QUIT:
	      running = 0; //On ferme la fenetre
	      break;
	    case SDL_KEYDOWN:
	      {
		switch (events.key.keysym.sym)
		  {
		  case SDLK_q:
		    {
		      running = 0; //On ferme la fenetre
		      break;
		    }
		  case SDLK_ESCAPE:
		    {
		      running = 0;
		      break;
		    }
		  case SDLK_SPACE:
		    {
		      /* si le jeux n'est pas lancer on lance et on met à jour */
		      if (!g->play_game)
			{
			  g->play_game = 1;   
			  g->update = 1;    
			}
		      /* si le jeux est déjà lancer et qu'un tire n'est pas en cour on lance le tire et on met à jour les coordnnées */
		      else if(!g->si->tank.firing)
			{
			  int tank_width;
			  
			  g->si->tank.firing = 1;

			  si_font_tank_get(&tank_width);
			  g->si->tank.shoot_x = g->si->tank.x + g->pixel_size*tank_width/2;
			  g->si->tank.shoot_y= g->si->window_height - 2 * 8 * g->si->pixel_size;
			  g->update = 1;
			
			}
		    }
		  }
		
		break; 
	      }
	    case SDL_MOUSEMOTION:
	      {
		g->si->tank.x = events.motion.x;
		si_tank_set_position(g);
		g->update = 1;
		break;
	      }
	    }
	  
	}
      if (g->play_game==0)
	{
	  g->update = 1;
	}
      else
	{
	  c_invaders = SDL_GetPerformanceCounter();
	  c_shoot = SDL_GetPerformanceCounter();

	  /* animation lente pour les ennemis */
	  if ((float)(c_invaders- g->count_invaders) / g->freq > 0.1)
	    {
	      if (g->si->tank.destroyed)
		{
		  g->si->tank.destroyed_count++;
		  if(g->si->tank.destroyed_count++ >= 6)
		    {
		      g->si->tank.destroyed=0;
		      g->si->tank.destroyed_count=0;
		      g->si->life_1--;
		      
			
		    }
		  g->update = 1;
		  
		}
	      else
		{
		  if(g->si->invaders.direction==1)
		    {
		      if(si_invaders_can_move_right(g->si))
			{
			  g->update=1;
			}
		    }
		  else
		    {
		      if(si_invaders_can_move_left(g->si))
			{
			  g->update = 1;
			}
		    }
		}
	      g->count_invaders = c_invaders;
	    }

	  /* animation rapide pour les 2 tirs */
	  if ((float)(c_shoot- g->count_shoot) / g->freq > 0.005)
	    {
	      /* si le tank tire ... */
	      if (g->si->tank.firing)
		{
		  if (si_tank_shoot_can_move_up(g->si))
		    {
		      g->update = 1;
		    }
		}
	      /* si les ennemis ne tire pas ... */
	      if (!g->si->invaders.firing)
		{
		  /* on trouve une colonne aléatoirement */
		  si_invaders_get_column(g->si);
		  /* et on dit que les ennemi ont tiré */
		  g->si->invaders.firing = 1;
		}

	      /* si les ennemis ont tirés */
	      if (g->si->invaders.firing)
		{
		  /*
		   * si la bombe peut descendre, on la déplace et
		   * on met à jour
		   */
		  if (si_invaders_bomb_can_move_down(g->si))
		    {
		      g->update = 1;
		    }
		}
	      g->count_shoot = c_shoot;
	    }
	  
	}
      if(g->update)
	{
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
  si_text_display(g,"SCORE<1> HI-score score<2>", 0, 0, 6);
  si_text_display(g,"0000", 2, 2, 6);
  si_text_display(g,"0000", 2, 11, 6);
  si_text_display(g,"0000", 2, 20, 6);
  
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
      si_invaders_display(g, 30, 120);
      if(g->si->invaders.firing)
	si_invaders_shoot_display(g,g->si->invaders.bomb_x,g->si->invaders.bomb_y);

      int tank_y = g->window_height - 2*(8 * g->pixel_size);
      si_tank_display(g, g->si->tank.x, tank_y);
      if(g->si->tank.firing)
	si_tank_shoot_display(g,g->si->tank.shoot_x,g->si->tank.shoot_y);
    }
  SDL_RenderPresent(g->ren);

  g->update = 0;
}
