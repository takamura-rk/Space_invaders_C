#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include "si_font.h"


typedef struct Si Si;


typedef struct
{
  SDL_Window *win;
  SDL_Renderer *ren;
  
  int window_width;
  int window_height;
  int pixel_size;
  int play_game;
  int paused;
  
  Si *si;
  
  Uint64 freq;
  Uint64 count_invaders;
  Uint64 count_shoot;
  
  char update;
  /* NOUVEAU : Animation des invaders */
  int invader_frame;
  
  /* NOUVEAU : Les Sons */
  Mix_Music *bg_music;      // Musique de fond (MP3)
  Mix_Chunk *sfx_shoot;     // Bruitage tir (WAV)
  Mix_Chunk *sfx_explode;   // Bruitage explosion (WAV)
  Mix_Chunk *sfx_wave;      // Bruitage vague (WAV)
  
} Game;

Game *game_new();
void game_update(Game *g);

void game_run(Game *g);
void game_del(Game *g);

#endif
