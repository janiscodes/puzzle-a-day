#include <stdio.h>
#include "puzzle.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
int FIELD_DIM = 7; //7x7 puzzle field
int PIECE_NUM;
int const PIECE_DIM = 4; //4x4 piece field

int sum_pieces_free(int const pieces_free[PIECE_NUM]) {
    int sum = 0;
    for (int i = 0; i < PIECE_NUM; i++) {
        if (pieces_free[i] != 0)sum++;
    }
    return sum;
}
void copy_field(int dest[FIELD_DIM][FIELD_DIM], int const src[FIELD_DIM][FIELD_DIM]) {
    for (int i = 0; i < FIELD_DIM; i++) {
        for (int j = 0; j < FIELD_DIM; j++) {
            dest[i][j] = src[i][j];
        }
    }
}
void load_pieces(piece pieces[], char* filename) {
    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        perror("Error opening file");
        return;
    }
    char *line = NULL;
    size_t linecap = 0;
    ssize_t linelen;
    int zeile = 0;
    int piece_index = -1; //so pieceindex++ makes first index 0
    for (zeile = 0; getline(&line, &linecap, f) !=-1; zeile++){
        if (line[0] == 'p') { //wenn getline einen block abgerabeitet hat setzte zeile zurück
            zeile = 0;
            piece_index++;
        }
        if (zeile>0){
            for (int i = 0; i < PIECE_DIM; i++) {
                pieces[piece_index].cells[i][zeile-1] = ((line[i] - '0')==0? 0:1 )* (piece_index+1) ;
                pieces[piece_index].number = piece_index+1;
            }
        }
    }
    PIECE_NUM = piece_index+1;
    free(line);
    fclose(f);
}
void initialise_field_zeros(int field[FIELD_DIM][FIELD_DIM]) {
    for (int i = 0; i < FIELD_DIM; i++) {
        for (int j = 0; j < FIELD_DIM; j++) {
            field[i][j] = 0;
        }
    }
}
void print_field(int dim, int field[dim][dim]) {
    for (int j = 0; j < dim; j++) {
        for (int i = 0; i < dim; i++) {
            if (field[i][j] == -8)
                printf("X    ");
            else if (field[i][j] > 999)
                printf("%d ", field[i][j]);
            else if (field[i][j] > 99)
                printf("%d  ", field[i][j]);
            else if (field[i][j] > 9 || field[i][j] < 0)
                printf("%d   ", field[i][j]);
            else
                printf("%d    ", field[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}
piece shift_left(piece p) {
    piece shifted = p;
    // Iterate over each row
    for (int j= 0; j < PIECE_DIM-1; j++) {
        for (int i = 0; i < PIECE_DIM; i++) {
            shifted.cells[j][i]= p.cells[j+1][i];
        }
    }
    for (int i = 0; i < PIECE_DIM; i++) {
        shifted.cells[PIECE_DIM-1][i] = 0;
    }

    // Return the updated piece
    return shifted;
}
piece shift_up(piece p) {
    piece shifted = p;
    // Iterate over each row
    for (int j= 0; j < PIECE_DIM-1; j++) {
        for (int i = 0; i < PIECE_DIM; i++) {
            shifted.cells[i][j]= p.cells[i][j+1];
        }
    }
    for (int i = 0; i < PIECE_DIM; i++) {
        shifted.cells[i][PIECE_DIM-1] = 0;
    }

    // Return the updated piece
    return shifted;
}
piece shift_piece(const piece p) {
    piece shifted_piece = p;
    for (int k = 0; k < PIECE_DIM/2; k++) {
        bool shift=true;
        //CASE 1 SHIFT UP?
        for (int i = 0; i < PIECE_DIM; i++) {
            if (shifted_piece.cells[i][0] != 0) {
                shift=false;
            }
        }
        if (shift) {
            shifted_piece = shift_up(shifted_piece);
        }
        //CASE 2 SHIFT LEFT?
        shift=true;
        for (int i = 0; i < PIECE_DIM; i++) {
            if (shifted_piece.cells[0][i] != 0) {
                shift=false;
            }
        }
        if (shift) {
            shifted_piece = shift_left(shifted_piece);
        }
    }
    return shifted_piece;
}
piece mirror_piece(piece p) {
        for (int i = 0; i < PIECE_DIM / 2; i++) {
            for (int j = 0; j < PIECE_DIM; j++) {
                // Swap rows: first with last, second with second-to-last, etc.
                int temp = p.cells[i][j];
                p.cells[i][j] = p.cells[PIECE_DIM - 1 - i][j];
                p.cells[PIECE_DIM - 1 - i][j] = temp;
            }
        }
    p=shift_piece(p);
    return p;
    }
piece rotate_piece_90(const piece p, int d) {
    piece rotated_piece = p; // Copy the original piece to preserve its data
    int n = PIECE_DIM;

    for (int f = 0; f < d; f++) {
        piece temp = rotated_piece; // Temporary copy for the current rotation
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                // Rotate 90 degrees clockwise
                rotated_piece.cells[j][n - 1 - i] = temp.cells[i][j];
            }
        }
    }
    rotated_piece = shift_piece(rotated_piece);
    return rotated_piece; // Return the rotated piece
}
bool is_valid_move(int field[FIELD_DIM][FIELD_DIM], piece p, int pos_x ,int pos_y) {
   //print_field(PIECE_DIM, p.cells);
    for (int i = pos_x; i < pos_x + PIECE_DIM; i++) {
        for (int j = pos_y; j < pos_y + PIECE_DIM; j++) {
            if (p.cells[i-pos_x][j-pos_y]!=0) {
                if (i >= FIELD_DIM || j >= FIELD_DIM || i < 0 || j < 0) {
                    return false;
                }
                if ((p.cells[i-pos_x][j-pos_y] + field[i][j] != p.cells[i-pos_x][j-pos_y])) {
                    return false;
                }
            }
        }
    }
    return true;
}
void place_piece(int field[FIELD_DIM][FIELD_DIM] ,piece p, int pos_x ,int pos_y) {
    for (int i = pos_x; i < pos_x + PIECE_DIM; i++) {
        for (int j = pos_y; j < pos_y + PIECE_DIM; j++) {
            if (p.cells[i-pos_x][j-pos_y]!=0)
                field[i][j] = p.cells[i-pos_x][j-pos_y];
        }
    }
   // printf("PL %d at %d %d \n", p.number, pos_x, pos_y);
   // print_field(FIELD_DIM,field);
}
void remove_piece(int field[FIELD_DIM][FIELD_DIM],piece p, int pos_x ,int pos_y) {
    for (int i = pos_x; i < pos_x + PIECE_DIM; i++) {
        for (int j = pos_y; j < pos_y + PIECE_DIM; j++) {
            if (p.cells[i-pos_x][j-pos_y]>0 && p.cells[i-pos_x][j-pos_y]<9)
                field[i][j] = 0;
        }
    }
   // printf("RM %d at %d %d \n", p.number, pos_x, pos_y);
   // print_field(FIELD_DIM,field);
}

