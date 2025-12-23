/*#include "si_sdl.h"
#include "si_menu.h"
#include <SDL2/SDL.h>

void menu(Game *g)
{
  // ON DESSINE TOUT EN BLANC ENSUITE 
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  
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

  //copie du renderer dans la fenêtre
  return;
}

void game_over(Gaùe *g)
{
  return;
}
*/
