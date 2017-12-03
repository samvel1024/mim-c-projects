/**
 * @author Samvel Abrahamyan
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


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
#define WHITE_PLAYER_STRATEGY make_turn_ai
#define SIZE 8
#define WHITE (-1)
#define FREE  0
#define BLACK 1
#define PRINT_ON_PLAYER BLACK
#define TURN_NO_ERR 0
#define TURN_ERR_TERMINATE 1
#define TURN_ERR_INVALID 2

const char VIEW_MAP[] = {WHITE_VIEW, FREE_VIEW, BLACK_VIEW};


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


const Vector DIRECTIONS[] = {{.r=1, .c=0},   //Down
                             {.r=-1, .c=0},  //Up
                             {.r=0, .c=-1},  //Left
                             {.r=0, .c=1},   //Right
                             {.r=1, .c=1},   //DownRight
                             {.r=-1, .c=-1}, //UpLeft
                             {.r=1, .c=-1},  //DownLeft
                             {.r=-1, .c=1}}; //UpRight

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
      (*b)[i][j] = FREE;
    }
  }
  int mid = SIZE / 2;
  (*b)[mid - 1][mid - 1] = (*b)[mid][mid] = WHITE;
  (*b)[mid][mid - 1] = (*b)[mid - 1][mid] = BLACK;
  return b;
}

/**
 * @return the character used to represent the cell at (r, c) in the view
 */
char Board_map_to_view(Board *b, int r, int c) {
  return VIEW_MAP[(*b)[r][c] + 1];
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

bool Board_has_neighbouring_piece(Board *b, int r, int c) {
  Vector start = {.r = r, .c = c};
  for (int i = 0; i < (int) (sizeof(DIRECTIONS) / sizeof(Vector)); ++i) {
    Vector v = Vector_add(start, DIRECTIONS[i]);
    if (Vector_is_in_bounds(v) && (*b)[v.r][v.c] != FREE)
      return true;
  }
  return false;
}

/**
 * Represents a turn made by the player, the pass property is going to be true
 * if the player decided not to perform a turn, otherwise pos property represents
 * the cell on which the player puts their piece
 */
typedef struct turn_t {
  Vector pos;
  bool pass;
  int err_status;
} Turn;


/**
 * Count number of flips
 */
int Reversi_count_flips(Board *b, Vector start, int color);


/**
 * Implementation of computer player
 */
Turn make_turn_ai(Board *b, int color) {
  int max_flips = 0;
  Turn turn = {.pass = true};
  for (int r = 0; r < SIZE; ++r) {
    for (int c = 0; c < SIZE; ++c) {
      if ((*b)[r][c] != FREE || !Board_has_neighbouring_piece(b, r, c))
        continue;
      Vector pos = {.c = c, .r = r};
      int flips = Reversi_count_flips(b, pos, color);
      if (max_flips < flips) {
        max_flips = flips;
        turn.pos = pos;
      }
    }
  }
  if (max_flips > 0)
    turn.pass = false;
  log("Playing turn [%d, %d, pass=%d] by AI player\n", turn.pos.r, turn.pos.c, turn.pass);
  return turn;
}

/**
 * Implementation of player through stdin
 */
Turn make_turn_player(Board *b, int color) {
  UNUSED(b);
  UNUSED(color);
  int first = getchar();
  if (first == EOF) {
    Turn t = {.err_status = TURN_ERR_TERMINATE};
    return t;
  }
  if (first == '\n') {
    Turn t = {.err_status = TURN_ERR_INVALID};
    return t;
  }
  int second = getchar();
  if (first == '=' && second == '\n') {
    Turn t = {.pass = true, .err_status=TURN_NO_ERR};
    return t;
  } else if ('a' <= first && first <= 'h' && '1' <= second && second <= '8' && getchar() == '\n') {
    Turn t = {
      .pass = false,
      .pos = {
        .r = second - '1',
        .c = first - 'a'
      },
      .err_status = TURN_NO_ERR
    };
    return t;
  } else {
    while (getchar() != '\n');
    Turn t = {
      .err_status = TURN_ERR_INVALID
    };
    return t;
  }
}

//Initialization of strategies for white and black players
Turn (*make_turn_black)(Board *b, int color) = BLACK_PLAYER_STRATEGY;

Turn (*make_turn_white)(Board *b, int color) = WHITE_PLAYER_STRATEGY;

/**
 * @param dir the direction vector by which we perform the search
 * @return the number of pieces flipped on single direction
 */
int Reversi_traverse_flips_by_direction(Board *b, Vector start, int color, Vector dir,
                                        void (*on_found)(Board *b, int searched_color, Vector pos)) {
  int flip_count = 0;
  Vector first = Vector_add(start, dir);
  for (; Vector_is_in_bounds(first) && (*b)[first.r][first.c] == -color;
         first = Vector_add(first, dir));
  if (Vector_is_in_bounds(first) && (*b)[first.r][first.c] == color) {
    for (Vector pos = Vector_add(start, dir); !Vector_equals(pos, first); pos = Vector_add(pos, dir)) {
      ++flip_count;
      (*on_found)(b, color, pos);
    }
  }
  log("Counted %d pieces to be flipped in direction [%d, %d] from [%d, %d]\n", flip_count, dir.r, dir.c, start.r,
      start.c);
  return flip_count;
}

/**
 * @param b the board
 * @param start position of the piece
 * @param color the color of the new piece
 * @param on_found callback which is called on every matched piece
 * @return
 */
int Reversi_traverse_flips(Board *b, Vector start, int color,
                           void (*on_found)(Board *b, int searched_color, Vector pos)) {
  int flip_count = 0;
  for (int i = 0; i < (int) (sizeof(DIRECTIONS) / sizeof(Vector)); ++i) {
    flip_count += Reversi_traverse_flips_by_direction(b, start, color, DIRECTIONS[i], on_found);
  }
  return flip_count;
}


/**
 * Callback empty function that is passed to only count flips
 */
void do_nothing(Board *b, int color, Vector pos) {
  UNUSED(b);
  UNUSED(color);
  UNUSED(pos);
}

/**
 * Callback function that is passed to flip the pieces
 */
void Reversi_flip_piece(Board *b, int color, Vector pos) {
  (*b)[pos.r][pos.c] = color;
}

int Reversi_count_flips(Board *b, Vector start, int color) {
  return Reversi_traverse_flips(b, start, color, do_nothing);
}

/**
 * Count the number of valid turns left by specified color
 */
bool Reversi_has_valid_turns(Board *b, bool color) {
  for (int r = 0; r < SIZE; ++r) {
    for (int c = 0; c < SIZE; ++c) {
      if ((*b)[r][c] != FREE || !Board_has_neighbouring_piece(b, r, c))
        continue;
      Vector pos = {.c = c, .r = r};
      int flips = Reversi_count_flips(b, pos, color);
      if (flips > 0) {
        return true;
      }
    }
  }
  return false;
}

void Reversi_start() {
  bool game_over = false;
  Board *board = Board_new();
  int curr_player_color = BLACK;
  int white_score = 2;
  int black_score = 2;
  while (!game_over) {
    if (PRINT_ON_PLAYER == curr_player_color)
      Board_print(board);
    log("\n%s %s\n", (curr_player_color == WHITE) ? "White" : "Black", "player's turn");
    Turn t = (curr_player_color == WHITE) ? (*make_turn_white)(board, WHITE) : (*make_turn_black)(board, BLACK);
    int turn_err = t.err_status;
    if (!t.pass && !turn_err) {

      bool has_neigh = Board_has_neighbouring_piece(board, t.pos.r, t.pos.c);
      bool is_free = (*board)[t.pos.r][t.pos.c] == FREE;
      int flip_count = has_neigh && is_free ? Reversi_traverse_flips(board, t.pos, curr_player_color, Reversi_flip_piece) : 0;

      (*(curr_player_color == WHITE ? &white_score : &black_score)) += flip_count;
      (*(curr_player_color == WHITE ? &black_score : &white_score)) -= flip_count;

      if (flip_count == 0)
        turn_err = TURN_ERR_INVALID;
      else {
        (*board)[t.pos.r][t.pos.c] = curr_player_color;
        ++(*(curr_player_color == WHITE ? &white_score : &black_score));
      }
    }
    //Two possible determination strategies for game over, not mentioned exactly in problem description
    //game_over = (white_score + black_score) == SIZE * SIZE;
    //game_over = !Reversi_has_valid_turns(board, WHITE) && !Reversi_has_valid_turns(board, BLACK);
    switch (turn_err) {
      case TURN_ERR_TERMINATE:
        game_over = true;
        break;
      case TURN_ERR_INVALID:
        printf("? ");
        break;
      case TURN_NO_ERR:
        if (t.pass) printf("= ");
        else printf("%c%c ", t.pos.c + 'a', t.pos.r + '1');
        curr_player_color *= -1;
        break;
      default:
        exit(1);
    }
    if ((turn_err == TURN_NO_ERR || turn_err == TURN_ERR_INVALID) && PRINT_ON_PLAYER == curr_player_color) {
      printf("%d\n", black_score - white_score);
    }
  }
  free(board);
}

int main() {
  Reversi_start();
  return 0;
}
