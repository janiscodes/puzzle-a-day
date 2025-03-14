#include "dfs.h"
#include <stdio.h>

int solve_rec_DFS(int x, int y, int field[FIELD_DIM][FIELD_DIM], piece pieces[PIECE_NUM], int pieces_free[PIECE_NUM]) {
    if (x == FIELD_DIM) { // Move to the next row
        x = 0;
        y++;
    }
    if (sum_pieces_free(pieces_free)==0) { //if all pieces are taken
        return 1;
    }
    if (field[x][y] != 0) //skip occupied cells
        return solve_rec_DFS(x + 1, y, field, pieces, pieces_free);
    for (int i = 0; i < PIECE_NUM; i++) { //for every piece
        if (pieces_free[i] == 1) { //if pieces[i] is not taken
            piece p = pieces[i]; //take piece
            for (int k = 0; k < 2; k++) { //for normal and mirror piece
                for (int j = 0; j < 4; j++) { //for every 4 orientations
                    p = rotate_piece_90(p, j); //rotate j times to ensure we try every orientation
                    for (int l = 0; l <= (x>=PIECE_DIM? PIECE_DIM : PIECE_DIM-x); l++) { //try placing the pieces along x-l up until x
                        valid_checks++;
                        if (is_valid_move(field, p, x-l, y)) { //is move valid
                            pieces_free[i] = 0; //set piece as placed
                            place_piece(field, p, x-l, y); //place piece
                            if (solve_rec_DFS(x+1, y, field, pieces, pieces_free) == 1) {
                                return 1; //go down in recursion
                            }
                            pieces_free[i] = 1;  //reverse the picking of piece
                            remove_piece(field, p, x-l, y); //remove earlier placed piece
                        }
                    }
                }
                p = mirror_piece(p);
            }
        }
    }
    return 0;
}
int solve_DFS(int field[FIELD_DIM][FIELD_DIM], piece pieces[PIECE_NUM]) {
    valid_checks = 0;
    int x = 0, y = 0;
    int pieces_free[PIECE_NUM];
    for(int i=0; i<PIECE_NUM; i++){pieces_free[i]=1;}

    solve_rec_DFS(x,y,field,pieces,pieces_free);

    printf("NORMAL SOLUTION: \n");
    print_field(FIELD_DIM, field);
    return valid_checks;
}


