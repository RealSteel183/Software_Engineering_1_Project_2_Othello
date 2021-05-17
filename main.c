//Written by Luke Rickard

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "othello.h"


int main() {
    gamestate thegame;
    bool play_again = true;
    int skip_count = 0;
    int move_type; // return value from make_type: 1 is skip; 0 is piece placed
    FILE *fp;

    //file creation
    if ((fp = fopen("othello-results.txt", "a+")) == NULL) {
        fprintf(stderr,"Could not open log file.\n");
        return (-1);
    }

    //initialise game
    init_game(&thegame);

    while (play_again) {
        while (skip_count < 2) { // if we have back to back skips, game over!
            print_gamestate(stdout,&thegame);
            move_type = make_move(&thegame);
            // skip_count > 1 and move_type > 1, then done (skip_count = 2)
            //  otherwise, skip_count = move_type
            skip_count = skip_count * move_type + move_type;
        }
        print_gamestate(stdout,&thegame);
        print_gameover(stdout,&thegame);
        print_gameover(fp,&thegame);

        play_again = ask_to_play_again();
        if (play_again) {
            reinit_game(&thegame);
            skip_count=0;
        }
    }
    printf("Thanks for playing.\n");

    //close file
    fclose(fp);
    return 0;
}

//print board
void print_board(FILE *fp,gamestate *thegameptr) {
    // top row of ---'s
    fprintf(fp,"  ");
    for (int j = 0; j<BOARDSIZE; j++) {
        fprintf(fp," ---");
    }
    fprintf(fp,"\n");

    for (int i=0;i<BOARDSIZE;i++) {
        // row #
        fprintf(fp,"%d ",i+1);
        // and board states for this row
        for (int j = 0; j<BOARDSIZE; j++) {
            fprintf(fp,"| %c ",thegameptr->board[i][j]);
        }
        fprintf(fp,"|\n");
        // row of ---'s
        fprintf(fp,"  ");
        for (int j = 0; j<BOARDSIZE; j++) {
            fprintf(fp," ---");
        }
        fprintf(fp,"\n");
    }
    // bottom row a-h
    fprintf(fp,"  ");
    for (int j = 0; j<BOARDSIZE; j++) {
        fprintf(fp,"  %c ",'a'+j);
    }
    fprintf(fp,"\n");
}

//print board at the current gamestate
void print_gamestate(FILE *fp,gamestate *thegameptr) {
    //print the score at the top
    fprintf(fp,"Score: %s (BLACK) %d:%d %s (WHITE)\n",thegameptr->p1.name,thegameptr->p1.score,thegameptr->p2.score,thegameptr->p2.name);
    //print board
    print_board(fp,thegameptr);
}
//initialise game
int init_game(gamestate *thegameptr) {
    //set player colors
    thegameptr->p1.color = BLACK;
    thegameptr->p2.color = WHITE;
    thegameptr->playertomove = BLACK;

    printf("Enter name of Player 1 (Black): ");
    thegameptr->p1.name = (char *)malloc(MAX_SIZE_NAME*sizeof(char));
    scanf("%s",thegameptr->p1.name);

    printf("Enter name of Player 2 (White): ");
    thegameptr->p2.name = (char *)malloc(MAX_SIZE_NAME*sizeof(char));
    scanf("%s",thegameptr->p2.name);

    thegameptr->board = (char **)malloc(BOARDSIZE*sizeof(char *));
    for (int i=0;i<BOARDSIZE;i++) {
        thegameptr->board[i] = (char *)malloc(BOARDSIZE*sizeof(char));
    }
    reinit_game(thegameptr);
}

int reinit_game(gamestate *thegameptr) {
    //set scores and first move
    thegameptr->p1.score = 0;
    thegameptr->p2.score = 0;
    thegameptr->playertomove = BLACK;

    for (int i=0;i<BOARDSIZE;i++) {
        for (int j=0;j<BOARDSIZE;j++) {
            thegameptr->board[i][j]=' ';
        }
    }
    // fill the center 4 squares with the checker pattern of BW's
    thegameptr->board[BOARDSIZE/2][BOARDSIZE/2-1]='B';
    thegameptr->p1.score++;
    thegameptr->board[BOARDSIZE/2-1][BOARDSIZE/2]='B';
    thegameptr->p1.score++;
    thegameptr->board[BOARDSIZE/2-1][BOARDSIZE/2-1]='W';
    thegameptr->p2.score++;
    thegameptr->board[BOARDSIZE/2][BOARDSIZE/2]='W';
    thegameptr->p2.score++;
}

int make_move(gamestate *thegameptr) {
    char themovestring[80];
    bool valid_syntax = false;
    bool free_square = false;
    bool player_can_move_here[BOARDSIZE][BOARDSIZE];
    bool at_least_one_valid_move = false;

    for (int i = 0; i < BOARDSIZE; i++) {
        for (int j = 0; j < BOARDSIZE; j++) {
            player_can_move_here[i][j] = valid_move(thegameptr, i, j);
            if (player_can_move_here[i][j]) {
                at_least_one_valid_move = true;
            }
        }
    }

    printf("%s, it's your turn. ", (thegameptr->playertomove == BLACK) ? thegameptr->p1.name : thegameptr->p2.name);
    while ((!valid_syntax) || (!free_square)) {  // first check that is is a valid move in terms of syntax
        printf("Where would you like to play? (valid moves are");
        if (at_least_one_valid_move) {
            for (int i = 0; i < BOARDSIZE; i++) {
                for (int j = 0; j < BOARDSIZE; j++) {
                    if (player_can_move_here[i][j]) printf(" '%c%c'", rowchars[i], colchars[j]);
                }
            }
        } else {
            printf(" 'p' for pass");
        }
        printf("): ");
        scanf("%s", themovestring);

        valid_syntax = (themovestring[0] == 'p') || (((themovestring[0] >= '1') && (themovestring[0] <= '8')) &&
                                                     ((themovestring[1] >= 'a') && (themovestring[1] <= 'h')));
        if (!valid_syntax) {
            printf("Not valid move syntax. Please try again.\n");
            continue;
        }
        if (at_least_one_valid_move && (themovestring[0] == 'p')) {
            printf("You cannot pass. You must select a valid move. Please try again.\n");
            continue;
        }
        if (!at_least_one_valid_move && (themovestring[0] == 'p')) {
            thegameptr->playertomove = (thegameptr->playertomove == BLACK)?WHITE:BLACK;
            return(1);
        }
        if (!player_can_move_here[themovestring[0] - '1'][themovestring[1] - 'a']) {
            printf("Not a valid move. You must select a valid move. Please try again.\n");
            continue;
        }
        break;
    }

    // it's a valid move, make the move (ie, update the gamestate)
    int imove = themovestring[0] - '1';
    int jmove = themovestring[1] - 'a';
    char opponent_color = (thegameptr->playertomove == BLACK) ? 'W' : 'B';

    if (thegameptr->playertomove == BLACK) {
        thegameptr->board[imove][jmove] = 'B';
        thegameptr->p1.score++;
    } else {
        thegameptr->board[imove][jmove] = 'W';
        thegameptr->p2.score++;
    }
    for (int idir = -1; idir <= 1; idir++) {
        for (int jdir = -1; jdir <= 1; jdir++) {
            if ((idir == 0) && (jdir == 0)) continue; // this is not a direction to check
            if (valid_dir(thegameptr, imove, jmove, idir, jdir)) {
                int jump = 1;
                while (((imove+jump*idir)>=0)&&((imove+jump*idir)<BOARDSIZE)&&
                       ((jmove+jump*jdir)>=0)&&((jmove+jump*jdir)<BOARDSIZE)&&
                       (thegameptr->board[imove+jump*idir][jmove+jump*jdir]==opponent_color)) {
                    if (thegameptr->playertomove == BLACK) {
                        thegameptr->board[imove+jump*idir][jmove+jump*jdir] = 'B';
                        thegameptr->p1.score++;
                        thegameptr->p2.score--;

                    } else {
                        thegameptr->board[imove+jump*idir][jmove+jump*jdir] = 'W';
                        thegameptr->p1.score--;
                        thegameptr->p2.score++;
                    }
                    jump++;
                }
            }
        }
    }
    thegameptr->playertomove = (thegameptr->playertomove == BLACK)?WHITE:BLACK;
    return(0);
}

bool valid_move(gamestate *thegameptr,int i,int j) {
    // check syntax
    if ((i<0)||(i>=BOARDSIZE)||(j<0)||(j>=BOARDSIZE)) {
        printf(stderr,"valid_move called with non-allowable index into board (%d,%d).\n",i,j);
        return(false);
    }
    // check if square is free or not
    if (thegameptr->board[i][j]!=' ') { // if square is not free, return false
        return(false);
    }
    for (int idir=-1;idir<=1;idir++) {
        for (int jdir = -1; jdir <= 1; jdir++) {
            if ((idir == 0) && (jdir == 0)) continue; // this is not a direction to check
            if (valid_dir(thegameptr,i,j,idir,jdir)) return(true);
        }
    }
    return(false);
}

bool valid_dir(gamestate *thegameptr,int i,int j,int idir,int jdir) {
    char my_color = (thegameptr->playertomove == BLACK) ? 'B' : 'W';
    char opponent_color = (thegameptr->playertomove == BLACK) ? 'W' : 'B';
    int jump = 1;

    // check to see if opponent occupies this direction
    if ((i+jump*idir<0)||(i+jump*idir>=BOARDSIZE)||(j+jump*jdir<0)||(j+jump*jdir>=BOARDSIZE)) return false;
    if (thegameptr->board[i+idir][j+jdir]!=opponent_color) return(false);

    // game on! we need to keep checking in this direction to see if valid move
    while (1) {
        jump++;
        if ((i+jump*idir<0)||(i+jump*idir>=BOARDSIZE)||(j+jump*jdir<0)||(j+jump*jdir>=BOARDSIZE)) {
            return(false); // we never found myself, and are outside the board now...
        }
        if (thegameptr->board[i+jump*idir][j+jump*jdir]==my_color) {
            return(true); // we found myself, this is a valid move
        }
        if (thegameptr->board[i+jump*idir][j+jump*jdir]==opponent_color) {
            continue; // the opponent is here, continue the search for myself
        }
        return(false); // must be empty - so return false
    }
}


void print_gameover(FILE *stream,gamestate *thegameptr) {
    time_t t;   // not a primitive datatype
    time(&t);

    fprintf(stream,"The date/time is %s\n", ctime(&t));
    print_gamestate(stream,thegameptr);
    if (thegameptr->p1.score > thegameptr->p2.score) {
        fprintf(stream,"Player 1 (%s) wins!\n",thegameptr->p1.name);
    } else if (thegameptr->p2.score > thegameptr->p1.score) {
        fprintf(stream,"Player 2 (%s) wins!\n",thegameptr->p2.name);
    } else {
        fprintf(stream,"It's a tie!\n");
    }
}

bool ask_to_play_again() {
    char answer[80];
    printf("Would you like to play again? (y/n): ");
    scanf("%s",answer);
    return(answer[0]=='y');
}