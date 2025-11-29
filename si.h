typedef struct Invaders Invaders;
typedef struct Tank Tank;
typedef struct Si Si;

struct Invaders
{
  int x;
  int y;
  int direction;
  int firing;
  int bomb_x;
  int bomb_y;
};
struct Tank
{
  int x;
  int firing;
  int shoot_x;
  int shoot_y;
  int destroyed;
  int destroyed_count;
};
struct Si
{
  int window_width;
  int window_height;
  int pixel_size;
  int nbr_players;
  int score_1;
  int life_1;
  int score_2;
  int life_2;
  int score_highest;
  Tank tank;
  Invaders invaders;
};
