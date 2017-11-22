#include <stdio.h>
#include <stdlib.h>

#define BLACK_PLAYER_STRATEGY make_turn_ai
#define WHITE_PLAYER_STRATEGY make_turn_player

//Implementation of type Turn
typedef struct turn_t {
  int row;
  int col;
  int is_pass;
} Turn;

Turn Turn_from_str(char f, char s) {
  Turn p = {
    .row = f - 'A',
    .col = f - '0'
  };
  return p;
}

//Implementation of type game
struct people;

typedef struct {
  /* same as before */
  struct people *friends;
} Person;

typedef struct people {
  /* same as before */
} People;


struct player_t;

typedef struct {

  int board[][];
  int is_running;
  int loop_counter;
  struct player_t *white_player;
  struct player_t *black_player;


} Game;

void Game_loop(Game *game){
  game->black_player->make_turn;
}

typedef struct player_t {
  Turn (*make_turn)(Game *game, Turn last_turn);
} Player;


/*
 * Impl of computer player
 */
Turn make_turn_ai(Game *game, Turn last) {

}


/*
 * Impl of player from std input
 */
Turn make_turn_std_input(Game *game, Turn last) {
  char r, c;
  return Turn_from_str(r, c);
}

int main() {
  Player *black = malloc(sizeof(Player));
  black->make_turn = make_turn_ai;
  Player *white = malloc(sizeof(Player));
  white->make_turn = make_turn_std_input;
  Game *game = malloc(sizeof(Game));
  game->white_player = white;
  game->black_player = black;
  free(game);
  return 0;
}
