#include <stdio.h>

#define BLACK_PLAYER_STRATEGY make_turn_ai
#define WHITE_PLAYER_STRATEGY make_turn_player

//Implementation of type Position
typedef struct turn_t {
    int row;
    int col;
    int is_pass;
} Turn;

Turn Turn_from_str(char f, char s)
{
    Position p = {
        .row = f - 'A',
        .col = f - '0'
    };
    return p;
}

//Implementation of type game

struct Game {
    Player white;
    Player black;

}

struct Player {
    int (*make_turn)(int a);
};

Position make_turn_ai(*Game game)
{
    return in * 2;
}

Position make_turn_player(int in)
{
    char r, c;
    return scanf("%c%c", &r, &c);
}

struct Player black = {
    .make_turn = BLACK_PLAYER_STRATEGY
};

struct Player white = {
    .make_turn = WHITE_PLAYER_STRATEGY
};

int main()
{
    int i;
    scanf("%d", &i);
    int ai_out = white.make_turn(i);
    int player_out = black.make_turn(i);
    printf("%d %d", ai_out, player_out);
    return 0;
}
