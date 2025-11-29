void render_update();
int create_window(void);

void draw_tank(int x, int y, int scale);
void draw_tank_shoot(int x, int y, int scale);
void draw_tank_explode(int frame, int x, int y, int scale);
void draw_invader(int type, int frame, int x, int y, int scale);
void draw_invader_explode(int x, int y, int scale);
void draw_ufo(int x, int y, int scale);
void draw_sprite(char *data, int rows, int cols, int x, int y, int scale);

