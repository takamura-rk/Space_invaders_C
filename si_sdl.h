#ifndef SI_SDL_H
#define SI_SDL_H

#include"game.h"

void si_display_sprite(Game *g, char *data, int rows, int cols, int x, int y);

/*tank*/
void si_tank_display(Game *g, int x, int y);
void si_tank_shoot_display(Game *g, int x, int y);
void si_tank_explode_display(Game *g, int frame, int x, int y);
void si_tank_set_position(Game *g);

/*ufo*/
void si_ufo_display(Game *g, int x, int y);

/*invader*/
void si_invader_display(Game *g, int type, int frame, int x, int y);
void si_invader_explode_display(Game *g, int x, int y);
void si_invaders_shoot_display(Game *g, int x , int y);
void si_invaders_display(Game *g, int x, int y);

/*alphanum*/
void si_text_display(Game *g, const char *text, int l, int c, int spacing);
int font_index_from_char(char c);
void si_str_display(Game *g, const char *str, int x, int y);
void si_alphanum_display(Game *g, char c, int x, int y);


#endif
