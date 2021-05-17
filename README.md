# Software_Engineering_1_Project_2_Othello
The Board was implimented as a 8x8 2 dimensional array of which the lines in between each option were printed afterwards. Each postion on the board ([i][j]) could take a B, W or be empty.
The 2 players were structs  that contained their name, their score and what color they were.
Each gamestate had 2 players (from struct player) (p1 & p2),a board , and which players turn it was to move.
At the beginning of the game the player names were asked and then allocated to which player they were (either p1 or p2). Then the intial board was created and then printed to the console. With the score at the top from the function printboard.
Then it gives the player the option to make a move.Vaild synatax must be input otherwise it will just ask again(if it isnt on the board cant be played) .The player enters a move. It checks first that the square isnt already taken by an opposing color . It then scans the entire board finding all vaild moves that are avaible by check all the directions you can make moves and checking to find your own color(even if your oppenent is there). If you go there is changes all the oppsing colors into your color in that direction andadds to your score
This is repeated until the 2 skips occur one after another.
Then the end result is added to a file
