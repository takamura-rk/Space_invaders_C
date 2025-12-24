#include "game.h"

void si_tank_display(Game *g, int x, int y);
void si_tank_shoot_display(Game *g, int x, int y);
void si_tank_explode_display(Game *g, int frame, int x, int y);
void si_invader_display(Game *g, int type, int frame, int x, int y);
void si_invader_explode_display(Game *g, int x, int y);
void si_ufo_display(Game *g, int x, int y);
void si_shoot_display(Game *g, int x , int y);

void si_invaders_display(Game *g, int x, int y);

void si_display_sprite(Game *g, char *data, int rows, int cols, int x, int y);
void si_text_display(Game *g, const char *text, int l, int c, int spacing);
int font_index_from_char(char c);
