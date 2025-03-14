#include <stdbool.h>
#include <libgen.h>  // Für dirname
#include <string.h>
#include <unistd.h>  // Für readlink
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "puzzle.h"
#include "dfs.h"
#include "pruning_flood_fill.h"
//Basic Data: Field is 7x7
/*     0  1  2  3  4  5  6
 * 0   J  F  M  A  M  J  X
 * 1   J  A  S  O  N  D  X
 * 2   1  2  3  4  5  6  7
 * 3   8  9 10 11 12 13 14
 * 4  15 16 17 18 19 20 21
 * 5  22 23 24 25 26 27 28
 * 6  29 30 31  X  X  X  X
 */

void print_puzzle_a_day() {
        // Define the matrix directly in the function
        char matrix[7][7] = {
            {'J', 'F', 'M', 'A', 'M', 'J', 'X'},
            {'J', 'A', 'S', 'O', 'N', 'D', 'X'},
            {  1 ,  2 ,  3 ,  4 ,  5 ,  6 ,  7 },
            {  8 ,  9 , 10 , 11 , 12 , 13 , 14 },
            { 15 , 16 , 17 , 18 , 19 , 20 , 21 },
            { 22 , 23 , 24 , 25 , 26 , 27 , 28 },
            { 29 , 30 , 31 , 'X', 'X', 'X', 'X'}
        };
        // Print the matrix
        for (int i = 0; i < 7; i++) {
            for (int j = 0; j < 7; j++) {
                if (matrix[i][j] == 'X') {
                    printf("  %c ", matrix[i][j]); // Print 'X' as is
                } else if (matrix[i][j] >= 'A' && matrix[i][j] <= 'Z') {
                    printf("  %c ", matrix[i][j]); // Print letters
                } else {
                    printf(" %2d ", matrix[i][j]); // Print numbers
                }
            }
            printf("\n");
        }
    }
int welcome_message(int *d, int *m, int ask) {
    int mode=0;
    printf("Welcome to Puzzle-a-Day Solver by NinjaMax\n");
    printf("This solver gives a valid Solution for DragonFjords Puzzle-A-Day\n");
    print_puzzle_a_day();
    printf("Please insert Date you want to have a Solution for (DD.MM.) \n");
    int check;
    if (ask==1) {
        scanf("%2d.%2d.%n",d ,m, &check);
        if (*d==-1) {
            mode = 1;
            printf("Going from 1.1. to ...?\n");
            scanf("%2d.%2d.%n",d ,m, &check);
            check = 6;
	    while((*d>31 || *m>12 || *d<1 || *m<1)) {
            if (check!=6)
                fprintf(stderr,"Unknown Date Format. Please enter like DD.MM.\n");
            if (*d>31 || *m>12 || *d<1 || *m<1 )
                fprintf(stderr,"Invalid Dates. Please enter Date that exists\n");
            while (getchar() != '\n'){}
            scanf("%2d.%2d.%n",d, m, &check);
            
        }
        }
        else{
            mode = 0;
        while(check!=6 ||(*d>31 || *m>12 || *d<1 || *m<1)) {
            if (check!=6)
                fprintf(stderr,"Unknown Date Format. Please enter like DD.MM.\n");
            if (*d>31 || *m>12 || *d<1 || *m<1 )
                fprintf(stderr,"Invalid Dates. Please enter Date that exists\n");
            while (getchar() != '\n'){}
            scanf("%2d.%2d.%n",d, m, &check);
        }
    }
    } else {
        *m = 9; *d = 6;
    }
    printf("Finding a Solution for %d/%d \n\n", *d, *m);
    return mode;
}
void place_date(int d, int m, int field[FIELD_DIM][FIELD_DIM]) {
    int block = -9;
    //PLACE MONTH
    if (m>=1 && m<=6)
        field[m-1][0] = block;
    if (m>=7 && m<=12)
        field[m-7][1] = block;
    //PLACE DAY
    if (d >= 1 && d <= 7)
        field[d - 1][2] = block;
    else if (d >= 8 && d <= 14)
        field[d - 8][3] = block;
    else if (d >= 15 && d <= 21)
        field[d - 15][4] = block;
    else if (d >= 22 && d <= 28)
        field[d - 22][5] = block;
    else if (d >= 29 && d <= 31)
        field[d - 29][6] = block;
    //PLACE BARRIERS
    field[6][0] = field[6][1] = -8;
    field[3][6] = field[4][6] = field[5][6] = field[6][6]=-8;
}
int run_algorithm(int d, int m, char* filename, int algorithm, float *pt){
    int checks;
    int field[FIELD_DIM][FIELD_DIM];
    piece pieces[8]; //es existieren 8 verschiedene Teile
    load_pieces(pieces, filename);    //lade alle teile in dass array pieces
    initialise_field_zeros(field); //setzte das Board auf Null
    place_date(d, m, field);
    switch (algorithm) {
        case 1: {
            checks = solve_DFS(field, pieces);
            break;
        }
        case 2: {
            checks = solve_flood_fill(field, pieces, pt);
            break;
        }
    }
    return checks;
}
void print_results(int c1, int c2, float t1, float t2, int w1, int w2, int wc1, int wc2, float pt) {
    printf("------------TIME-------------------------------RECURSION-CALLS-------WINS:TIME-------WINS:CALLS\n");
    printf("NORMAL:    |%8.5f                            | %-12d   |   %-10d    |   %-10d\n", t1, c1, w1, wc1);
    printf("FLOOD_FILL:|%8.5f(of which pruning: %6.5f) | %-12d   |   %-10d    |   %-10d\n", t2, pt,  c2, w2, wc2);
    printf("-----------------------------------------------------------------------------------------------\n\n");

    if (c1 < c2)
        printf("NORMAL is faster by %-12d Recursion-Calls\n"
               "It is %-12.2f times faster than FLOOD_FILL\n", c2 - c1, (float)c2 / c1);
    else
        printf("FLOOD_FILL Heuristic is faster by %d Recursion-Calls\n"
               "It is %f times faster than NORMAL\n", c1 - c2, (float)c1 / c2);
}
void compare_algorithms(int d, int m, char* filename) {
    int w1,w2; //wins_time: Which Algorithm was better how many times?
    int c1,c2; //calls: Times the main recursion function was called
    float t1,t2; //clock: Time of each Algorithm per Solution
    int wc1, wc2; //wins_checks: Which Algorithm was better how many times?
    float pt; //pruning_time: Time needed for Pruning checks in each Solution
    c1=c2=0; t1=t2=0.0; w1=0,w2=0; wc1 = wc2=0; pt=0;

    clock_t tic = clock(); c1 = run_algorithm(d, m, filename, 1, &pt); clock_t toc = clock();
    t1 = (double)(toc - tic) / CLOCKS_PER_SEC;

    tic = clock(); c2 = run_algorithm(d, m, filename, 2, &pt); toc = clock();
    t2 = (double)(toc - tic) / CLOCKS_PER_SEC;
    printf("For Date: %d, Month: %d \nWe have compared a Normal and Improved Solving Algorithm\n", d, m);
    if (t1<t2)w1++;
    else if (t1>t2)w2++;
    if (c1<c2)wc1++;
    else if (c1>c2)wc2++;
    print_results(c1, c2, t1, t2, w1, w2, wc1, wc2, pt);
}
void compare_algorithms_in_range(int d, int m, char* filename) {
    int w1,w2; //wins: Which Algorithm was better how many times?
    int c1,c2; //calls: Times the main recursion function was called
    float t1,t2; //clock: Time of each Algorithm per Solution
    int wc1, wc2; //wins_checks: Which Algorithm was better how many times?
    float pt; //pruning_time: Time needed for Pruning checks in each Solution
    c1=c2=0; t1=t2=0.0; w1=0,w2=0; wc1 = wc2=0; pt=0;
    int i = 1;
    int j = 1;
    while (j < m || (j == m && i <= d)) {
                printf("Date: %d, Month: %d for NORMAL \n", i, j);
                clock_t tic = clock(); c1 += run_algorithm(i, j, filename, 1, &pt); clock_t toc = clock();
                t1 += (double)(toc - tic) / CLOCKS_PER_SEC;
                printf("Date: %d, Month: %d for FLOOD_FILL \n", i, j);
                tic = clock(); c2 += run_algorithm(i, j, filename, 2, &pt); toc = clock();
                t2 += (double)(toc - tic) / CLOCKS_PER_SEC;
                if (t1<t2)w1++;
                else if (t1>t2)w2++;
                if (c1<c2)wc1++;
                else if (c1>c2)wc2++;
        i++;
        if (i > 31) {  // Reset day and increment month
            i = 1;
            j++;
        }
    }
    printf("For Dates 01/01 - %d/%d \n", d, m);
    print_results(c1, c2, t1, t2, w1, w2, wc1, wc2, pt);
}

int main(void) {
    int day, month;
    char* filename = "data/pieces.txt";
    int mode = welcome_message(&day, &month, 1);
    if (mode == 0)
        compare_algorithms(day, month, filename);
    else if (mode == 1)
        compare_algorithms_in_range(day,month,filename);
    return 0;
}