/*The following program is an implementation of a Sudoku Solver in C.
*/
#include <stdlib.h>
#include <stdio.h>
#include <omp.h>


#define NELEMENTS 16
#define NBLOCKS 4


int guess_is_valid(int sudoku[][NELEMENTS], int row, int col, int num){
    /* check for sudoku validity 
     * check if num is already in the row
     * check if num is already in the column
     * check if num is already in the block */


    int rowStart = (row/NBLOCKS) * NBLOCKS;
    int colStart = (col/NBLOCKS) * NBLOCKS;
    int i, j;

    for(i=0; i<NELEMENTS; ++i){
        if (sudoku[row][i] == num) return 0;
        if (sudoku[i][col] == num) return 0;
        if (sudoku[rowStart + (i%NBLOCKS)][colStart + (i/NBLOCKS)] == num) return 0;
    }
    return 1;
}


int read_input_mat(char *name, int sudoku[][NELEMENTS]) {
    /* Reading sudoku from file */ 

    FILE *in_file;  /* file containing the input data */
    int i, j;

    in_file = fopen(name, "r");
    if (in_file == NULL) { /* Test for error */
        fprintf(stderr, "Error: Unable to find input file %s\n",name);
        exit (8);
    }
   
    for (i=0; i<NELEMENTS; i++){
        for (j=0; j<NELEMENTS; j++){
	        if (fscanf(in_file, "%d", &sudoku[i][j]) != 1) {
	            fprintf(stderr, "Failed to read matrix[%d][%d]\n", i, j);
		        exit(1);
	        }
	    }
    }
    fclose(in_file);
}


int read_input_line(char *name, int sudoku[][NELEMENTS]) {
    /* Reading sudoku from file */ 

    FILE *in_file;  /* file containing the input data */
    int i, j;

    in_file = fopen(name, "r");
    if (in_file == NULL) { /* Test for error */
        fprintf(stderr, "Error: Unable to find input file %s\n",name);
        exit (8);
    }
   
    for (i=0; i<NELEMENTS*NELEMENTS; i++){
        if (fscanf(in_file, "%d", &sudoku[i/NELEMENTS][i%NELEMENTS]) != 1) {
	        fprintf(stderr, "Failed to read matrix[%d][%d]\n", i, j);
            exit(1);
	    }
    }
    fclose(in_file);
}


void print_sudoku(int sudoku[][NELEMENTS], int is_finished){
    /* print sudoku to std output */
    int i, j;

    printf("\n+-----------+-----------+-----------+-----------+\n");
    for(i=1; i<NELEMENTS+1; ++i){
        for(j=1; j<NELEMENTS+1; ++j) printf("|%2d", sudoku[i-1][j-1]);
        printf("|\n");
        if (i%NBLOCKS == 0) printf("+-----------+-----------+-----------+-----------+\n");
    }
    if (is_finished) exit(0);
}

void copy_sudoku(int sudoku[][NELEMENTS], int new_sudoku[][NELEMENTS]){
    /* copy sudoku */
    int i, j;

    for (i=0;i<NELEMENTS;i++)
	    for (j=0;j<NELEMENTS;j++)
	        new_sudoku[i][j] = sudoku[i][j];
}	

void solve_sudoku(int sudoku[][NELEMENTS], int row, int column){
    
    int guess;
    
    // If the row number is greater than NELEMENTS than we have filled all cells. Sudoku is solved.
    if(row+1>NELEMENTS) print_sudoku(sudoku, 0);
    
    //Check if it is an empty field
    if(sudoku[row][column]!=0)
        // navigate to the next field of the board
        if(column+1<NELEMENTS) solve_sudoku(sudoku, row, column+1);
        else solve_sudoku(sudoku, row+1, 0);
    else
    {
        for(guess=1; guess<=NELEMENTS; guess++){
            if(guess_is_valid(sudoku,row, column, guess)){
                #pragma omp task firstprivate(row, column, guess)
                {
                    // make a copy of sudoku to let tasks be independent
                    // remember that sudoku is a pointer to an array
                    // so making sudoku firstprivate will not do the trick  
                    int new_sudoku[NELEMENTS][NELEMENTS];
                    copy_sudoku(sudoku, new_sudoku);

                    new_sudoku[row][column] = guess;
                    // navigate to the next field of the board
                    if(column+1<NELEMENTS) solve_sudoku(new_sudoku, row, column+1);
                    else solve_sudoku(new_sudoku, row+1, 0);
                }
            }
        }
        // backtracking 
        #pragma omp taskwait
        sudoku[row][column] = 0; 
    }
}


int main(int argc, char* argv[]){
    int row,column;
    int sudoku[NELEMENTS][NELEMENTS]; // store the sudoku data 
    
    read_input_line(argv[1], sudoku);
    print_sudoku(sudoku, 0);
    #pragma omp parallel
        #pragma omp single 
            solve_sudoku(sudoku, 0, 0);
}

