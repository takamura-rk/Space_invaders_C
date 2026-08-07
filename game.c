#include <time.h>
#include <stdio.h> 

#include "game.h"
#include "si_sdl.h"
#include "si.h"
#include "si_menu.h"
#include "si_font.h"

Game *game_new(void)
{
  Game *g=malloc(sizeof *g);
  if (!g) return NULL;
  
  /* Initialisation de g */
  g->win=NULL;
  g->ren=NULL;
  g->window_width=546-6;
  g->window_height=720;
  g->pixel_size=3;
  g->play_game=0;
  g->paused = 0; /* Pas de pause au début */
  g->si = si_new(g->window_width,g->window_height,g->pixel_size);
  g->freq = SDL_GetPerformanceFrequency();
  g->count_invaders=0;
  g->count_shoot=0;
  g->update=1;
  g->invader_frame = 0;
  
  /* Init SDL Video et Audio */
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) { 
      SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_Init error: %s\n", SDL_GetError());
      free(g); return NULL; 
  }

  /* NOUVEAU : Init Mixer (Audio) */
  if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
      printf("Erreur SDL_Mixer: %s\n", Mix_GetError());
  }

  /* Chargement des sons */
  g->bg_music = Mix_LoadMUS("music.mp3");
  g->sfx_shoot = Mix_LoadWAV("shoot.wav");
  g->sfx_explode = Mix_LoadWAV("explosion.wav");
  g->sfx_wave = Mix_LoadWAV("wave.wav");

  /* Lancer la musique en boucle (-1) */
  if(g->bg_music) Mix_PlayMusic(g->bg_music, -1);

  /* ... Reste de la création fenêtre ... */
  SDL_DisplayMode dm;
  SDL_GetCurrentDisplayMode(0, &dm);
  int x = dm.w/2 -273 +3; 
  int y = g->window_height/2 -360; 
  
  g->win = SDL_CreateWindow("Space Invaders", x, y, g->window_width, g->window_height, SDL_WINDOW_SHOWN);
  if (!g->win) return NULL; 

  g->ren= SDL_CreateRenderer(g->win, -1, SDL_RENDERER_ACCELERATED);
  if (!g->ren) return NULL;

  srand(time(NULL));
  return g;
}

void game_del(Game *g)
{
  if (!g) return;
  
  /* Nettoyage Audio */
  if(g->sfx_shoot) Mix_FreeChunk(g->sfx_shoot);
  if(g->sfx_explode) Mix_FreeChunk(g->sfx_explode);
  if(g->sfx_wave) Mix_FreeChunk(g->sfx_wave);
  if(g->bg_music) Mix_FreeMusic(g->bg_music);
  Mix_CloseAudio();

  if (g->ren) SDL_DestroyRenderer(g->ren);
  if (g->win) SDL_DestroyWindow(g->win);
  if (g->si) si_del(g->si);
  SDL_Quit();
  free(g);
}

void game_run(Game *g)
{
  int running = 1;
  g->count_invaders = SDL_GetPerformanceCounter();
  g->count_shoot = SDL_GetPerformanceCounter();

  while (running)
    {
      SDL_Event events;
      Uint64 current_time = SDL_GetPerformanceCounter();

      while (SDL_PollEvent(&events))
	{
	  switch (events.type)
	    {
	    case SDL_QUIT:
	      running = 0; 
	      break;
	    case SDL_KEYDOWN:
	      {
		switch (events.key.keysym.sym)
		  {
		  case SDLK_q:
		  case SDLK_ESCAPE:
		    running = 0;
		    break;
            
		    /* la touche s met le jeu en pause */
		  case SDLK_s:
		    if (g->play_game) {
		      g->paused = !g->paused; /* on inverse l'état  */
		      g->update = 1; /* on met à jour pour afficher le texte PAUSE */
                    
		      /*on met la musique en pause aussi */
		      if(g->paused) Mix_PauseMusic();
		      else Mix_ResumeMusic();
		    }
		    break;

		  case SDLK_SPACE:
		    {
		      /* on interdit les actions si PAUSE est activé */
		      if (g->paused) break; 
		      /*si une partie n'est pas en cours ou que le joueur n'as plus de vie on relance la partie */
		      if (!g->play_game || g->si->life_1 <= 0)
			{
			  si_reset(g->si);//réinitialisation du jeu
			  g->play_game = 1;// on démarre la partie
			  g->update = 1;// on met à jour l'affichage 
			}
		      /*sinon si le tank n'est pas en train de tirer on déclenche le tir  */
		      else if(!g->si->tank.firing)
			{
			  int tank_width;
			  g->si->tank.firing = 1;// déclenchement du tir 
			  si_font_tank_get(&tank_width);// on récupére la largeur du tank
			  /*on calcule et on initialise la position du tir*/
			  g->si->tank.shoot_x = g->si->tank.x + g->pixel_size*tank_width/2;
			  g->si->tank.shoot_y= g->si->window_height - 2 * 8 * g->si->pixel_size;
                  
			  /* on déclenche le son du tir */
			  if(g->sfx_shoot) Mix_PlayChannel(-1, g->sfx_shoot, 0);
                  
			  g->update = 1;
			}
		      break;
		    }
		  }
		break; 
	      }
	    case SDL_MOUSEMOTION: //si la souris est déplacée
	      {
		//on vérifie que la partie est en cours que le jeux n'est pas en pause et que le joeur posséde au moin une vie
		if (!g->paused && g->play_game && g->si->life_1 > 0) {
		   //on met à jour la position du tanks avec la position hotizontale de la souris
		  g->si->tank.x = events.motion.x;
		  si_tank_set_position(g);// fonction qui met à jour la position du tank à l'écran
		  g->update = 1;
		}
		break;
	      }
	    }
	}

      /* Si le jeu est en PAUSE, on met à jour l'affichage */
      if (g->paused) {
	if (g->update) game_update(g);
	SDL_Delay(20);// limitation de la boucle 
	continue; /* On saute tout le reste de la boucle */
      }
      /* s'il n'y a pas de partie en cours */
      if (g->play_game==0)
	{
	  g->update = 1; 
	  SDL_Delay(20); 
	}
      else
	{
	  /*on vérifie si la vague courante est fini ou pas*/
	  if (si_matrix_count() == 0)
	    {
	      SDL_Delay(500); 
	      si_next_wave(g->si); // on initialise la nouvelle vague 
	      g->update = 1;
	    }

	  /* VERIFICATION DES SONS PROVENANT DE SI.C */
	  if (g->si->sound_explode_flag) {
            if(g->sfx_explode)
	      Mix_PlayChannel(-1, g->sfx_explode, 0);
            g->si->sound_explode_flag = 0; /* réinitialisaton du flag */
	  }
	  if (g->si->sound_wave_flag) {
            if(g->sfx_wave) Mix_PlayChannel(-1, g->sfx_wave, 0);
            g->si->sound_wave_flag = 0; /* Reset flag */
	  }

	  /* Accélération de la vitesse des invaders en fonction de la vague */
	  double wave_speed = 0.5 - ((g->si->wave) * 0.1);
	  if (wave_speed < 0.05)
	    wave_speed = 0.05;//on limite la vitesse
	  /*interval entre les déplacement des invaders*/
	  double dt_invaders = (double)(current_time - g->count_invaders) / g->freq;
	  /*si le délai est écoulé on déplace les invaders*/
	  if (dt_invaders > wave_speed) 
	    {
	      /*on gère la destruction du tank*/
	      if (g->si->tank.destroyed)
		{
		  g->si->tank.destroyed_count++;
		  if(g->si->tank.destroyed_count++ >= 6)
		    {
		      g->si->tank.destroyed=0;
		      g->si->tank.destroyed_count=0;
		      /*on décrémente le nombre de vie restant s'il est strictement positif */
		      if(g->si->life_1 > 0)
			g->si->life_1--; 
		    }
		  g->update = 1;
		}
	      else if (g->si->life_1 > 0) 
		{
		  int moved = 0;
		  /*déplacement horizentale des invaders en fonctin de la direction actuel */
		  if(g->si->invaders.direction==1)
		    {
		      if(si_invaders_can_move_right(g->si))
			moved=1;
		    }
		  else
		    {
		      if(si_invaders_can_move_left(g->si))
			moved=1;
		    }
		  /*un déplacement à eu lieu*/
		  if(moved) {
		    g->invader_frame ^= 1; /*on alterne les sprites des invaders entre 0 et 1 */
		    g->update = 1;
		    /*on vérifie s'il y a eu une collision avec le bas de l'écran*/
		    if (si_check_collision_bottom(g->si))
		      g->si->life_1 = 0; //le joueur perd immédiatement 
		  }
		}
	      /*on met à jour le compteur de temps*/
	      g->count_invaders = current_time;
	    }

	  double dt_shoot = (double)(current_time - g->count_shoot) / g->freq;
	  if (dt_shoot > 0.02)
	    {

	      if (g->si->tank.firing)
		{
		  int hit_row = -1, hit_col = -1;
		  if (si_tank_shoot_can_move_up(g->si, &hit_row, &hit_col))
		    {
		      /* si un invader a été touché */
		      if (hit_row != -1 && hit_col != -1)
			{
			  int inv_w = 12 * g->pixel_size;
			  int inv_h = 8  * g->pixel_size;
			  int gap_x = 2  * g->pixel_size;
			  int gap_y = 5  * g->pixel_size;

			  int ex = g->si->invaders.x + hit_col * (inv_w + gap_x);
			  int ey = g->si->invaders.y + hit_row * (inv_h + gap_y);

			  /* On redessine l’écran normal */
			  game_update(g);

			  /* On affiche l’explosion par-dessus, puis on freeze un court instant */
			  si_invader_explode_display(g, ex, ey);
			  SDL_RenderPresent(g->ren);
			  SDL_Delay(100); 
			  g->update = 1;
			}
		    }
		}
		  
	      /* si les ennemis ne tirent pas */
	      if (!g->si->invaders.firing && g->si->life_1 > 0)
		{
		  if (rand() % 30 == 0) {
		    si_invaders_get_column(g->si);
		    g->si->invaders.firing = 1;
		  }
		}
	      /* si les ennemis ont tirés */
	      if (g->si->invaders.firing)
		if (si_invaders_bomb_can_move_down(g->si)) g->update = 1;
             
	      g->count_shoot = current_time;
	    }

	  /* APPARITION DE L'UFO */
	  Uint64 now = SDL_GetPerformanceCounter();
	  /* temps écoulé depuis le dernier test */
	  double dt = (double)(now - g->si->ufo.spawn_counter) / g->freq;

	  /* on attend 15 secondes avant d'essayer de spawn un ufo,
	     puis on tente un spawn chaque seconde
	   */
	  if (!g->si->ufo.active && now >= g->si->ufo.next_spawn_allowed && dt > 1.0) {
	    g->si->ufo.spawn_counter = now; // reset timer des tests

	    if ((rand() % 100) < 20) { // 20% de chance d'apparaitre
	      g->si->ufo.active = 1;
	      g->si->ufo.y = 35 * g->pixel_size;
	      g->si->ufo.dir = (rand() % 2) ? 1 : -1;

	      int ufo_w = 16 * g->pixel_size;
	      g->si->ufo.x = (g->si->ufo.dir == 1) ? -ufo_w : g->window_width;

	      /* on définit le prochain spawn autorisé dans 15 secondes */
	      g->si->ufo.next_spawn_allowed = now + (Uint64)(15 * g->freq);

	      g->update = 1;
	    }
	  }

	  /* DEPLACEMENT DE L'UFO */
	  double dt_ufo = (double)(current_time - g->si->ufo.move_counter) / g->freq;

	  if (g->si->ufo.active && dt_ufo > 0.04) { // 25 fois/sec
	    /*on met à jour le compteur de temps*/
	    g->si->ufo.move_counter = current_time;

	    /*on déplace l'ufo selon sa diréction */
	    g->si->ufo.x += g->si->ufo.dir * (g->pixel_size * 1);

	    int ufo_w = 16 * g->pixel_size;
	    /*on désactive l'ufo lorsqu'il sort de l'écran*/
	    if (g->si->ufo.x > g->window_width + ufo_w || g->si->ufo.x < -ufo_w) {
	      g->si->ufo.active = 0;
	    }

	    g->update = 1; // onmet à jour l'affichage 
	  }


	}
    
      if(g->update) game_update(g);
    }
}
  
void game_update(Game *g)
{
  /*effacement de l'écran avec un fond noir */
  SDL_SetRenderDrawColor(g->ren, 0, 0, 0, 255);
  SDL_RenderClear(g->ren);
  /*on utilise la couleur blanche pour le rendu des sprites et du texte */
  SDL_SetRenderDrawColor(g->ren, 255, 255, 255, 255);

  /* Si le jeu est en PAUSE, on affiche juste PAUSE au milieu */
  if (g->paused) {
    si_text_display(g, "PAUSE", 15, 11, 6);
    SDL_RenderPresent(g->ren);
    g->update = 0;
    return;
  }
  /*affichage des score et de la numéro de la vague*/
  char score_text[20]; 
  char hiscore_text[20];
  char wave_text[20];
  
  /*affichage de la banière */
  si_text_display(g,"SCORE<1> HI-score score<2>", 0, 0, 6);//l'en-tête des scores
  // affichage du score du joueur
  sprintf(score_text, "%04d", g->si->score_1); 
  si_text_display(g, score_text, 2, 2, 6); 
  // affichage du meilleur score
  sprintf(hiscore_text, "%04d", g->si->score_highest);
  si_text_display(g, hiscore_text, 2, 11, 6);
  
  si_text_display(g,"0000", 2, 20, 6);//score du joueur 2
  
  if (!g->play_game) { menu(g); }
  else if (g->si->life_1 <= 0) { game_over(g); }
  else
    {
      /* Affichage des invaders et leurs tirs */
      si_invaders_display(g, g->si->invaders.x, g->si->invaders.y);
      if(g->si->invaders.firing)
        si_invaders_shoot_display(g,g->si->invaders.bomb_x,g->si->invaders.bomb_y);
      /* Affichage de l'UFO  */
      if (g->si->ufo.active) {
	si_ufo_display(g, g->si->ufo.x, g->si->ufo.y);
      }
      /* Affichage du tank */
      int tank_y = g->window_height - 2*(8 * g->pixel_size);
      if (g->si->tank.destroyed)
	{
	  
	  int frame;
	  /*animation de l'explosion du tank*/
	  if (g->si->tank.destroyed_count < 2)
	    {
	    frame = 0;
	    si_tank_explode_display(g, frame, g->si->tank.x,tank_y);
	    }
	  else if (g->si->tank.destroyed_count <4)
	    {
	    frame = 1;
	    si_tank_explode_display(g, frame, g->si->tank.x,tank_y);
	    }
	  else
	    {} // (le tank disparait a la fin de l'explosion)
	}
      else
	{
	  si_tank_display(g, g->si->tank.x, tank_y);// affichage du tank
	  /*affichage du tir du tank*/
	  if(g->si->tank.firing)
	    si_tank_shoot_display(g,g->si->tank.shoot_x,g->si->tank.shoot_y);
	}
      /* Bas de l'écran */
      sprintf(wave_text, "WAVE %d", g->si->wave);
      si_text_display(g, wave_text, 29, 0, 6); 
      // affichage des vie restantes
      int life_icon_y = g->window_height - (8 * g->pixel_size) - 5; 
      int start_x = g->window_width - 50; 
      int spacing = 15 * g->pixel_size;  
      for (int i = 1; i < g->si->life_1; i++) {
	si_tank_display(g, start_x - (i * spacing), life_icon_y + 3 * g->pixel_size);
      }
    }

  
  SDL_RenderPresent(g->ren);// affichage finale de la frame
  g->update = 0;
}
