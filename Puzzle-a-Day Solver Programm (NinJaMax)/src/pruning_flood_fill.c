#include <stdio.h>
#include <time.h>
#include "pruning_flood_fill.h"

float pruning_time;
void flood_fill(int to_flood[FIELD_DIM][FIELD_DIM], int x, int y, int target, int visited) {
    if (x < 0 || x >= FIELD_DIM || y < 0 || y >= FIELD_DIM || to_flood[x][y]!=target) return;
    to_flood[x][y] = visited;
    flood_fill(to_flood,x-1, y, target, visited);
    flood_fill(to_flood, x + 1, y, target, visited);
    flood_fill(to_flood, x, y - 1, target, visited);
    flood_fill(to_flood, x, y + 1, target, visited);
}
int count_zero_regions(const int field[FIELD_DIM][FIELD_DIM]) {
    int to_flood[FIELD_DIM][FIELD_DIM];
    copy_field(to_flood, field);
    int target = 0;
    int visited = -100;
    int count = 0; //number of zero-regions found
    for (int i = 0; i < FIELD_DIM; i++) {
        for (int j = 0; j < FIELD_DIM; j++) {
            if (to_flood[i][j] == 0) {
                //if (count > 1)return count; //tested if makes difference in runtime (not significant)
                count++; //found zero region
                flood_fill(to_flood, i, j, target, visited); //run flood_filling
            }
        }
    }
    return count;
}
int solve_rec_flood(int x, int y, int field[FIELD_DIM][FIELD_DIM], piece pieces[PIECE_NUM], int pieces_free[PIECE_NUM]) {
    clock_t tic = clock();
    if (count_zero_regions(field) > 1) return 0; //more than one "zero-region"
    clock_t toc = clock();
    pruning_time = pruning_time + (double)(toc - tic) / CLOCKS_PER_SEC;
    if (x == FIELD_DIM) { //move to the next row
        x = 0;
        y++;
    }
    if (sum_pieces_free(pieces_free)==0) { //all pieces are taken
        return 1;
    }
    if (field[x][y] != 0) // Skip occupied cells
        return solve_rec_flood(x + 1, y, field, pieces, pieces_free);
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
                            if (solve_rec_flood(x+1, y, field, pieces, pieces_free) == 1) {
                                return 1;
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
int solve_flood_fill(int field[FIELD_DIM][FIELD_DIM], piece pieces[PIECE_NUM], float *pt) {
    valid_checks = 0;
    int x = 0, y = 0;
    pruning_time = *pt;
    int pieces_free[PIECE_NUM];
    for(int i=0; i<PIECE_NUM; i++){pieces_free[i]=1;}

    solve_rec_flood(x,y,field,pieces,pieces_free);

    printf("FLOOD_FILL SOLUTION: \n");
    print_field(FIELD_DIM, field);
    *pt = pruning_time;
    return valid_checks;

}



