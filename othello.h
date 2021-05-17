//
// Created by Luke Rickard on 07/05/2021.
//

#ifndef UNTITLED11_OTHELLO_H
#define UNTITLED11_OTHELLO_H

#define MAX_SIZE_NAME 80
#define BOARDSIZE 8
char rowchars[BOARDSIZE+1] = "12345678";
char colchars[BOARDSIZE+1] = "abcdefgh";

enum player_colors {BLACK,WHITE};

typedef struct player {
    char *name;
    int score;
    enum player_colors color;
} player;

typedef struct gamestate {
    player p1;
    player p2;
    char **board;
    enum player_colors playertomove;
} gamestate;

void print_board(FILE *fp,gamestate *thegameptr);
void print_gamestate(FILE *fp,gamestate *thegameptr);
int init_game(gamestate *thegameptr);
int reinit_game(gamestate *thegameptr);
int make_move(gamestate *thegameptr);
bool valid_move(gamestate *thegameptr,int i,int j);
bool valid_dir(gamestate *thegameptr,int i,int j,int idir,int jdir);
void print_gameover(FILE *fp,gamestate *thegameptr);
bool ask_to_play_again();

#endif //UNTITLED11_OTHELLO_H
