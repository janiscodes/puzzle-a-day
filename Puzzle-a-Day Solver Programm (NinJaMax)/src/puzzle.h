#ifndef PUZZLE_H
#define PUZZLE_H
#include <stdbool.h>
extern int FIELD_DIM;
extern int PIECE_NUM;
extern const int PIECE_DIM;
int valid_checks;
struct piece {
    int cells[4][4];
    int number;
};
typedef struct piece piece;
int sum_pieces_free(int const pieces_free[PIECE_NUM]);
void copy_field(int dest[FIELD_DIM][FIELD_DIM], int const src[FIELD_DIM][FIELD_DIM]);
int solve_BFS(int d, int m, int field[FIELD_DIM][FIELD_DIM], piece pieces[PIECE_NUM]);
void load_pieces(piece pieces[], char* filename);
bool is_valid_move(int field[FIELD_DIM][FIELD_DIM], piece p, int pos_x ,int pos_y);
void print_field(int dim, int field[dim][dim]);
void initialise_field_zeros(int field[FIELD_DIM][FIELD_DIM]);
piece rotate_piece_90(const piece p, int d);
piece shift_piece(const piece p);
piece mirror_piece(const piece p);
void place_piece(int field[FIELD_DIM][FIELD_DIM] ,piece p, int pos_x ,int pos_y);
void remove_piece(int field[FIELD_DIM][FIELD_DIM] ,piece p, int pos_x ,int pos_y);
#endif //PUZZLE_H
