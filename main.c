#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#define MODE_DEBUG

#ifdef MODE_DEBUG
#define ENABLE_LOG
#endif


#ifdef ENABLE_LOG
#define log printf
#else
#define log(...)
#endif
#define UNUSED(x) (void)(x)


#define SIZE 8
#define BLACK_PLAYER_STRATEGY make_turn_player
#define WHITE_PLAYER_STRATEGY make_turn_player

#define FREE  0
#define BLACK 1
#define WHITE (-1)

#ifdef MODE_DEBUG
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

typedef int Board[SIZE][SIZE];

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

typedef struct vec_t {
  int r;
  int c;
} Vector;


Vector Vector_add(Vector a, Vector b) {
  Vector c = {
    .c = a.c + b.c,
    .r = a.r + b.r
  };
  return c;
}

Vector Vector_scale(Vector v, int k) {
  Vector vec = {
    .r = k * v.r,
    .c = k * v.c
  };
  return vec;
}

bool Vector_is_in_bounds(Vector v) {
  return 0 <= v.c && v.c < SIZE && 0 <= v.r && v.r < SIZE;
}

bool Vector_equals(Vector a, Vector b) {
  return a.r == b.r && a.c == b.c;
}

typedef struct turn_t {
  Vector pos;
  bool pass;
} Turn;


Turn make_turn_ai(Board *b, int color) {
  UNUSED(b);
  UNUSED(color);
  Turn t = {.pass = true};
  return t;
}

Turn make_turn_player(Board *b, int color) {
  UNUSED(b);
  UNUSED(color);
  char first, second;
  do {
    scanf("%c", &first);
  }while(isspace(first));
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
  if (!Vector_is_in_bounds(t.pos)){
    log("Entered position is out of bounds\n");
    exit(1);
  }
  return t;
}

Turn (*make_turn_black)(Board *b, int color) = BLACK_PLAYER_STRATEGY;

Turn (*make_turn_white)(Board *b, int color) = WHITE_PLAYER_STRATEGY;


int flip_pieces_by_direction(Board *b, Turn t, int color, Vector dir) {
  int flip_count = 0;
  Vector last_same_color = t.pos;
  for (Vector pos = Vector_add(t.pos, dir); Vector_is_in_bounds(pos); pos = Vector_add(pos, dir)) {
    if (*b[pos.r][pos.c] == color) {
      last_same_color = pos;
    }
  }
  for (Vector pos = t.pos;
       Vector_is_in_bounds(pos) && !Vector_equals(pos, last_same_color); pos = Vector_add(pos, dir)) {
    if (*b[pos.r][pos.c] == -color) {
      ++flip_count;
      *b[pos.r][pos.c] = color;
    }
  }
  log("Flipped %d pieces in direction [%d, %d] from [%d, %d]\n", flip_count, dir.r, dir.c, t.pos.r, t.pos.c);
  return flip_count;
}


int flip_pieces(Board *b, Turn t, int color) {
  Vector dirs[] = {{.r=1, .c=0},   //Down
                   {.r=0, .c=1},   //Right
                   {.r=1, .c=1},   //DownRight
                   {.r=1, .c=-1}}; //DownLeft
  int flip_count = 0;
  for (int i = 0; i < 4; ++i) {
    flip_count += flip_pieces_by_direction(b, t, color, dirs[i]);
    flip_count += flip_pieces_by_direction(b, t, color, Vector_scale(dirs[i], -1));
  }
  if (flip_count == 0) {
    log("Wrong input, no flipped pieces");
    exit(1);
  }
  *b[t.pos.r][t.pos.c] = color;
  return flip_count;
}

void start_reversi() {
  bool game_over = false;
  Board *board = Board_new();
  int curr_player_color = WHITE;
  while (!game_over) {
    Board_print(board);
    log("\n%s %s\n", (curr_player_color == WHITE) ? "White" : "Black", "player's turn");
    Turn t = (curr_player_color == WHITE) ? (*make_turn_white)(board, WHITE) : (*make_turn_black)(board, BLACK);
    if (!t.pass)
      flip_pieces(board, t, curr_player_color);
    curr_player_color *= -1;
  }
  free(board);
}

int main() {
  start_reversi();
  return 0;
}
