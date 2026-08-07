#include "si_sdl.h"
#include "si_menu.h"
#include "game.h"
#include <SDL2/SDL.h>

void menu(Game *g)
{  
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
}

void game_over(Game *g)
{
  si_text_display(g, "GAME OVER", 10, 8, 6);
  si_text_display(g, "PRESS SPACE", 15, 7, 6);
  si_text_display(g, "TO RESTART", 17, 8, 6);
}
