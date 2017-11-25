/**
 * @author Samvel Abrahamyan
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>


#define MODE_DEBUG

#ifdef MODE_DEBUG
#define ENABLE_LOG
#define FREE_VIEW '-'
#define BLACK_VIEW 'B'
#define WHITE_VIEW 'W'
#define CELL_FORMAT " %c "
#else
#define FREE_VIEW '-'
#define BLACK_VIEW 'C'
#define WHITE_VIEW 'B'
#define CELL_FORMAT "%c"
#endif

#ifdef ENABLE_LOG
#define log printf
#else
#define log(...)
#endif
/**
 * Used to pass the required compilation flag -Wextra -Werror :)
 * in order to annotate unused function arguments
 */
#define UNUSED(x) (void)(x)


/**
 * Define strategy per player, to be able to change for debugging
 */
#define BLACK_PLAYER_STRATEGY make_turn_player
#define WHITE_PLAYER_STRATEGY make_turn_player

#define SIZE 8
#define FREE  0
#define BLACK 1
#define WHITE (-1)

/**
 * Definition of Game board and related procedures
 */
typedef int Board[SIZE][SIZE];

/**
 * @return newly created board of dimensions SIZE by SIZE and the initial game state
 */
Board *Board_new() {
  Board *b = malloc(sizeof(Board));
  for (int i = 0; i < SIZE; ++i) {
    for (int j = 0; j < SIZE; ++j) {
      *b[i][j] = FREE;
    }
  }
  *b[3][3] = *b[4][4] = WHITE;
  *b[4][3] = *b[3][4] = BLACK;
  return b;
}

/**
 * @return the character used to represent the cell at (r, c) in the view
 */
char Board_map_to_view(Board *b, int r, int c) {
  switch (*b[r][c]) {
    case WHITE:
      return WHITE_VIEW;
    case BLACK:
      return BLACK_VIEW;
    case FREE:
      return FREE_VIEW;
    default:
      exit(1);
  }
}

/**
 * Pretty print the board
 */
void Board_print(Board *b) {
  for (int i = 0; i < SIZE; ++i) {
    for (int j = 0; j < SIZE; ++j) {
      char v = Board_map_to_view(b, i, j);
      printf(CELL_FORMAT, v);
    }
    printf("%d\n", i + 1);
  }
  for (int c = 0; c < SIZE; ++c) {
    printf(CELL_FORMAT, 'a' + c);
  }
  printf("\n");
}

/**
 * Vector representing a (row, column) tuple in the board
 */
typedef struct vec_t {
  int r;
  int c;
} Vector;

/**
 * @return Vector c that equals to the vector sum a + b
 */
Vector Vector_add(Vector a, Vector b) {
  Vector c = {
    .c = a.c + b.c,
    .r = a.r + b.r
  };
  return c;
}

/**
 * @return scaled vector v by a scalar k
 */
Vector Vector_scale(Vector v, int k) {
  Vector vec = {
    .r = k * v.r,
    .c = k * v.c
  };
  return vec;
}

/**
 * @return if the vector points to a cell inside of game board
 */
bool Vector_is_in_bounds(Vector v) {
  return 0 <= v.c && v.c < SIZE && 0 <= v.r && v.r < SIZE;
}

/**
 * @return true if two vectors are pointing to the same cell
 */
bool Vector_equals(Vector a, Vector b) {
  return a.r == b.r && a.c == b.c;
}


/**
 * Represents a turn made by the player, the pass property is going to be true
 * if the player decided not to perform a turn, otherwise pos property represents
 * the cell on which the player puts their piece
 */
typedef struct turn_t {
  Vector pos;
  bool pass;
} Turn;


/**
 * Definition of a procedure that ai player depends on.
 * This procedure by itself does not perform any state mutation on the board,
 * instead it takes a callback which can be used to flip the piece if needed
 *
 * @param b the game board
 * @param start the proposed position of the turn
 * @param color the color of the player
 * @param callback a function that is called by the procedure if it finds a piece that is a candidate for being flipped if the turn is performed
 * @return the number of flips that will happen after the turn
 */
int Reversi_count_pieces(Board *b, Vector start, int color,
                         void (*callback)(Board *b, int searched_color, Vector pos));

/**
 * Empty procedure that is passed to Reversi_conut_pieces by the
 * ai player when searching for the optimal turn
 */
void do_nothing(Board *b, int color, Vector pos) {
  UNUSED(b);
  UNUSED(color);
  UNUSED(pos);
}

/**
 * Implementation of computer player
 */
Turn make_turn_ai(Board *b, int color) {
  UNUSED(b);
  UNUSED(color);
  Vector v = {.r = 1, .c = 2};
  Reversi_count_pieces(b, v, color, do_nothing);
  Turn t = {.pass = true};
  return t;
}

/**
 * Implementation of player through std input
 */
Turn make_turn_player(Board *b, int color) {
  UNUSED(b);
  UNUSED(color);
  char first, second;
  do {
    scanf("%c", &first);
  } while (isspace(first));
  log("Reading %c as first input char\n", first);
  if (first == '=') {
    Turn t = {.pass = true, .pos = {.c = 0, .r = 0}};
    return t;
  }
  scanf("%c", &second);
  log("Reading %c as second input char\n", second);
  Turn t = {
    .pass = false,
    .pos = {
      .r = second - '1',
      .c = first - 'a'
    }
  };
  if (!Vector_is_in_bounds(t.pos)) {
    log("Entered position is out of bounds\n");
    exit(1);
  }
  return t;
}

//Initialization of strategies for white and black players
Turn (*make_turn_black)(Board *b, int color) = BLACK_PLAYER_STRATEGY;
Turn (*make_turn_white)(Board *b, int color) = WHITE_PLAYER_STRATEGY;

/**
 *
 * @param dir the direction vector by which we perform the search
 * @return the number of pieces flipped on single direction
 */
int Reversi_count_pieces_by_direction(Board *b, Vector start, int color, Vector dir,
                                      void (*callback)(Board *b, int searched_color, Vector pos)) {
  int flip_count = 0;
  Vector last_same_color = start;
  for (Vector pos = start; Vector_is_in_bounds(pos); pos = Vector_add(pos, dir)) {
    if (*b[pos.r][pos.c] == color) {
      last_same_color = pos;
    }
  }
  for (Vector pos = start; !Vector_equals(pos, last_same_color); pos = Vector_add(pos, dir)) {
    if (*b[pos.r][pos.c] == -color) {
      ++flip_count;
      (*callback)(b, color, pos);
    }
  }
  log("Flipped %d pieces in direction [%d, %d] from [%d, %d]\n", flip_count, dir.r, dir.c, start.r, start.c);
  return flip_count;
}


int Reversi_count_pieces(Board *b, Vector start, int color,
                         void (*callback)(Board *b, int searched_color, Vector pos)) {
  Vector dirs[] = {{.r=1, .c=0},   //Down
                   {.r=0, .c=1},   //Right
                   {.r=1, .c=1},   //DownRight
                   {.r=1, .c=-1}}; //DownLeft
  int flip_count = 0;
  for (int i = 0; i < 4; ++i) {
    flip_count += Reversi_count_pieces_by_direction(b, start, color, dirs[i], callback);
    flip_count += Reversi_count_pieces_by_direction(b, start, color, Vector_scale(dirs[i], -1), callback);
  }
  return flip_count;
}

/**
 *
 */
void Reversi_flip_piece(Board *b, int color, Vector pos) {
  *b[pos.r][pos.c] = color;
}


void Reversi_start() {
  bool game_over = false;
  Board *board = Board_new();
  int curr_player_color = WHITE;
  while (!game_over) {
    Board_print(board);
    log("\n%s %s\n", (curr_player_color == WHITE) ? "White" : "Black", "player's turn");
    Turn t = (curr_player_color == WHITE) ? (*make_turn_white)(board, WHITE) : (*make_turn_black)(board, BLACK);
    if (!t.pass) {
      int flip_count = Reversi_count_pieces(board, t.pos, curr_player_color, Reversi_flip_piece);
      if (flip_count == 0) {
        log("No flipped piece, error");
        exit(1);
      }
      *board[t.pos.r][t.pos.c] = curr_player_color;
    }
    curr_player_color *= -1;
  }
  free(board);
}

int main() {
  Reversi_start();
  return 0;
}
