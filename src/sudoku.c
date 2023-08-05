#include "sudoku.h"
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#define DEBUG


/* Return true iff g is completeed correctly so far  .
else false */
/*static int sudoku_is_correct_so_far(Grid_T g){
	int i, j, x, y, k, l, kstart, lstart, value;

	for(i = 0; i < 9; i++){
		for(j = 0; j < 9; j++){

			
			if( g.cell[i][j].choices[0] == 0 ) continue;

			value = g.cell[i][j].choices[0];
			
			for(y = 0; y < 9; y++){
				if( y != j && g.cell[i][y].choices[0] == value) return 0;
			}
			
			for(x = 0; x < 9; x++){
				if( x != i && g.cell[x][j].choices[0] == value) return 0;
			}
			
			x = i % 3;
			y = j % 3;
			kstart = i - x;
			lstart = j - y;
			for(k = kstart; k < kstart + 3; k++){
				for(l = lstart; l < lstart + 3; l++){
					if(k != i && l != j && g.cell[k][l].choices[0] == value) return 0;
				}
			}
		}
	}

	return 1;
}
*/



/*return a clone of g*/
static Grid_T sudoku_clone(Grid_T g){
	Grid_T clone;
	int i,j,k;

	clone.unique = g.unique;
	for(i = 0; i < 9; i++){
		for(j = 0; j <9; j++){
			clone.cell[i][j].count = g.cell[i][j].count;
			for(k = 0; k < 10; k++){
				clone.cell[i][j].choices[k] = g.cell[i][j].choices[k];
			}
		}
	}
	return clone;
}


/*read sudoky from stdin in format:
number space and new line every 9 number
and return a grid_t with it*/
Grid_T sudoku_read(void){
	int i, j, c;
	int v[9][9];
	Grid_T g;

	for(i = 0; i < 9; i++){
		for(j = 0; j < 9; j++){
			if(scanf("%d", &c) == 1){
				v[i][j] = c;
			}
			else {
				fprintf(stderr, "stray at %d,%d", i, j);
				exit(EXIT_FAILURE);
			}	
		}
	}

	g = grid_init(g, v);
/*do we want that blocked or alloed?*/
/*
	if(!sudoku_is_correct_so_far(g)){
		fprintf(stderr, "The sudoku read is not valid.\n");
		exit(-2);
	}
*/

	return g;
}

/*prints the sudoku given(g) at s in the format:
number space and new line every 9 number*/
void sudoku_print(FILE *s, Grid_T g){
	int i,j;
	
	for(i = 0; i < 9; i++){
		for(j = 0; j < 9; j++){
			printf("%d ", g.cell[i][j].choices[0]);		
			if(j == 8)printf("\n");
		}
	}
	
	return ;	
}

/* Return true iff g is complete and filled in correctly.
else false */
int sudoku_is_correct(Grid_T g){
	int i, j, x, y, k, l, kstart, lstart, value;

	for(i = 0; i < 9; i++){
		for(j = 0; j < 9; j++){

			/*if there is a unsined chell*/
			if( g.cell[i][j].choices[0] == 0 ) return 0;

			value = g.cell[i][j].choices[0];
			/*check this value in it's row*/
			for(y = 0; y < 9; y++){
				if( y != j && g.cell[i][y].choices[0] == value) return 0;
			}
			/*check the value in it's column*/
			for(x = 0; x < 9; x++){
				if( x != i && g.cell[x][j].choices[0] == value) return 0;
			}
			/*check value in it's 3x3 square*/

			/*finding the position of the cell in the 3x3*/ 
			x = i % 3;
			y = j % 3;
			kstart = i - x;
			lstart = j - y;
			for(k = kstart; k < kstart + 3; k++){
				for(l = lstart; l < lstart + 3; l++){
					if(k != i && l != j && g.cell[k][l].choices[0] == value) return 0;
				}
			}
		}
	}

	return 1;
}


#ifdef DEBUG
/* Print all row, col, sub-puzzle errors/conflicts found in puzzle g */
void sudoku_print_errors(Grid_T g){
	int i, j, x, y, k, l, kstart, lstart, value;

	for(i = 0; i < 9; i++){
		for(j = 0; j < 9; j++){

			/*if there is a unsined chell*/
			if( g.cell[i][j].choices[0] == 0 ){
			 	printf("error in %d,%d it has no value\n", i+1, j+1);
				continue;
			}
			value = g.cell[i][j].choices[0];
			/*check this value in it's row*/
			for(y = 0; y < 9; y++){
				if( y != j && g.cell[i][y].choices[0] == value) 
				printf("error in %d,%d it incorectly has value %d, same as %d,%d\n", i+1, y+1, value, i+1, j+1);
			}
			/*check the value in it's column*/
			for(x = 0; x < 9; x++){
				if( x != i && g.cell[x][j].choices[0] == value)
				 printf("error in %d,%d it incorectly has value %d, same as %d,%d\n", x+1, j+1, value, i+1, j+1);
			}
			/*check value in it's 3x3 square*/

			/*finding the position of the cell in the 3x3*/ 
			x = i % 3;
			y = j % 3;
			kstart = i - x;
			lstart = j - y;
			for(k = kstart; k < kstart + 3; k++){
				for(l = lstart; l < lstart + 3; l++){
					if(k != i && l != j && g.cell[k][l].choices[0] == value) 
					printf("error in %d,%d it incorectly has value %d, same as %d,%d\n", k+1, l+1, value, i, j);
				}
			}
		}
	}

	return ;
}
#endif

/*generates a new sudoku with nelts non 0 chells
and if unique == 1 the sudoku will have a unique 
choice in each step
(the sudokus come from known correct sudokus with 
rundom chells 0ed)*/
Grid_T sudoku_generate(int nelts, int unique){
	Grid_T g;
	int rand_i, rand_j, num_to_eleminate;
	int i,j;
	int sudoku[9][9];

	int unique_sudoku[9][9] = {
        {5, 3, 4, 6, 7, 8, 9, 1, 2},
        {6, 7, 2, 1, 9, 5, 3, 4, 8},
        {1, 9, 8, 3, 4, 2, 5, 6, 7},
        {8, 5, 9, 7, 6, 1, 4, 2, 3},
        {4, 2, 6, 8, 5, 3, 7, 9, 1},
        {7, 1, 3, 9, 2, 4, 8, 5, 6},
        {9, 6, 1, 5, 3, 7, 2, 8, 4},
        {2, 8, 7, 4, 1, 9, 6, 3, 5},
        {3, 4, 5, 2, 8, 6, 1, 7, 9}
    };

	int non_unique_sudoku[9][9] = {
		{1, 2, 3, 4, 5, 6, 7, 8, 9},
		{4, 5, 6, 7, 8, 9, 1, 2, 3},
		{7, 8, 9, 1, 2, 3, 4, 5, 6},
		{2, 1, 4, 3, 6, 5, 8, 9, 7},
		{3, 6, 5, 8, 9, 7, 2, 1, 4},
		{8, 9, 7, 2, 1, 4, 3, 6, 5},
		{5, 3, 1, 6, 4, 2, 9, 7, 8},
		{6, 4, 2, 9, 7, 8, 5, 3, 1},
		{9, 7, 8, 5, 3, 1, 6, 4, 2}
	};

	for(i = 0; i < 9; i++){
		for(j = 0; j < 9; j++){
			sudoku[i][j] = unique ? unique_sudoku[i][j] : non_unique_sudoku[i][j];
		}
	}

	srand(getpid());
	num_to_eleminate = 81 - nelts;

	for(; num_to_eleminate > 0; num_to_eleminate--){
		do{
			rand_i = rand() % 9;
			rand_j = rand() % 9;
		}while(sudoku[rand_i][rand_j] == 0);

		sudoku[rand_i][rand_j] = 0;
	}

	return grid_init(g, sudoku);
}


/* Sovle g and return the solution; if g has multiple solutions, return one of them.
firt call it will c.i = 0, c.j = 0 and c.n = 0*/	
Grid_T sudoku_solve(Grid_T g, Choice_T c){
	Grid_T sudoku;
	Grid_T tempG;
	Choice_T tempC;
	Choice_T rc;

	rc.i = c.i;
	rc.j = c.j;
	rc.n = c.n;

	sudoku = sudoku_clone(g);

	sudoku = grid_update(sudoku, rc);


	/*tring to solve the sudoku as if its unique*/
	c = grid_exist_unique(sudoku);
	while(c.i != 0 || c.j != 0 || c.n != 0){
		sudoku = grid_update(sudoku, c);

		c = grid_exist_unique(sudoku);
	}

	/*if the unique changes have been made
	and the sudoku is correct the sudoku was unique
	and it is now solved*/
	if(sudoku_is_correct(sudoku))return sudoku;
	/*if its not completes then it must not be unique
	so we start or continiue solving it as sach*/
	grid_clear_unique(sudoku);

	tempC = grid_iterate(sudoku, rc);
	if(tempC.n != 0 ){
		tempG = sudoku_solve(sudoku, tempC);
		if(sudoku_is_correct(tempG)){
			tempG.unique = 0;
			return tempG;
		}else{
			sudoku.cell[tempC.i][tempC.j].choices[tempC.n] = 0;
			return sudoku_solve(sudoku, rc);
		}
	}

	return sudoku;
}



int main(int argc, char **argv){
	Grid_T g;
	Choice_T c;
	
	int opt;
	int c_flag, g_flag, nelts , u_flag;

	c.i = 0;
	c.j = 0;
	c.n = 0;

	c_flag = 0;
	g_flag = 0;
	nelts = 0;
	u_flag = 0;

	while((opt = getopt(argc, argv, "cg:u")) != -1 ){
		switch(opt){
			case 'c':
				c_flag = 1;
				break;
			case 'g':
				g_flag = 1;
				nelts = atoi(optarg);
				break;
			case 'u':
				u_flag = 1;
				break;
			default:
				fprintf(stderr, "error: unknown flag give\n");
				exit(-3);
				break;
		}
	}

	/*if no flag given*/
	if(c_flag == 0 && g_flag == 0 && u_flag == 0){
		g = sudoku_read();
		sudoku_print(stderr, g);
		g = sudoku_solve(g, c);
		if(!sudoku_is_correct(g))fprintf(stderr, "the sudoku doen't have a solution\n");
		else fprintf(stderr, g.unique ? "the sudoku given is unique\n" : "the sudoku given is not unique\n");
		sudoku_print(stdout, g);

		return 0;
	}

	/*only c flag given*/
	if(c_flag == 1 && g_flag == 0 && u_flag == 0){
		g = sudoku_read();
		sudoku_print(stderr, g);
		if(sudoku_is_correct(g))fprintf(stderr, "the sudoku given is correct\n");
		else{
			fprintf(stderr, "the sudoku given is NOT correct\n");
			sudoku_print_errors(g); /*this might need to be printed in stderr fun the function doent take a file parameter*/
		}

		return 0;
	}

	/*g falg given and optional u*/
	if(c_flag == 0 && g_flag == 1){
		g = sudoku_generate(nelts, u_flag);
		sudoku_print(stdout, g);

		return 0;
	}

	printf("invalid flag selection\n");
	exit(-4);

	return 0;
}
