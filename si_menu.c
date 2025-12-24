#include "si_sdl.h"
#include "si_menu.h"
#include <SDL2/SDL.h>

void menu(Game *g)
{
  // ON DESSINE TOUT EN BLANC ENSUITE 
  SDL_SetRenderDrawColor(g->ren, 255, 255, 255, 255);
  
  si_text_display(g,"SCORE<1> HI-score score<2>", 0, 0, 6);
  si_text_display(g,"0000", 2, 2, 6);
  si_text_display(g,"0000", 2, 11, 6);
  si_text_display(g,"0000", 2, 20, 6);
  si_text_display(g,"PLAY", 7, 11, 6);
  si_text_display(g,"SPACE  INVADERS", 9, 6, 6);
  si_text_display(g,"*SCORE ADVANCE TABLE*", 12, 2, 6);
  si_text_display(g,"? MYSTERY", 14, 9, 6);
  si_ufo_display(g,110, 340);
  si_text_display(g,"=30 POINTS", 16, 8, 6);
  si_invader_display(g,0, 0, 118, 386);
  si_text_display(g,"=20 POINTS", 18, 8, 6);
  si_invader_display(g,1, 0, 118, 435);
  si_text_display(g,"=10 POINTS", 20, 8, 6);
  si_invader_display(g,2, 0, 118, 482);
  si_text_display(g,"CREDIT", 29, 9, 6);


  si_tank_display(g,300, 550);
  si_tank_shoot_display(g,300, 510);
  si_tank_explode_display(g,0, 350, 550);
  si_tank_explode_display(g,1, 400, 550);
    
  si_invader_display(g, 0, 1, 0, 200);
  si_invader_display(g, 0, 0, 40, 200);
  si_invader_display(g, 1, 0, 80, 200);
  si_invader_display(g, 1, 1, 120, 200);
  si_invader_display(g, 2, 0, 160, 200);
  si_invader_display(g, 2, 1, 200, 200);
  si_invader_explode_display(g,240, 200);
  si_ufo_display(g,150, 500);
  si_shoot_display(g,120,240);

  //copie du renderer dans la fenêtre
  return;
}

void game_over(Game *g)
{
  return;
}

