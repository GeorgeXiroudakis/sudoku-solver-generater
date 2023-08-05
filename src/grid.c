#include<stdio.h>
#include"grid.h"
#include<assert.h>


/*if count >0 decreases the count by 1
 * returns the updates g*/
static Grid_T grid_udate_counts(Grid_T g){
	int i, j, k, count;
	count = 0;

	for(i = 0; i < 9; i++){
		for(j = 0; j < 9; j++){
			count = 0;
			for(k = 1; k < 10; k++){
				if(g.cell[i][j].choices[0] != 0)continue;
				if(g.cell[i][j].choices[k] == 1)count++;
			}
			g.cell[i][j].count = count;
		}
	}
	return g;
}


/*init g with values from array v
 * eliminates choices accordingly
 * returns the updates g
 * sets unique flag to true*/
Grid_T grid_init(Grid_T g, int v[9][9]){
	int i,j,k,l,x,y,kstart,lstart;
	int value;

	for(i = 0; i < 9; i++){
		for(j = 0; j < 9; j++){
			if( (v[i][j] != 0) && (v[i][j] > 0 && v[i][j] < 10) ){
				/*assign the value and eliminate all the other choices
				 * as this is a given*/ 
				g.cell[i][j].choices[0] = v[i][j];
				for(k = 1; k <= 10; k++){
					g.cell[i][j].choices[k] = 0;
				}
				g.cell[i][j].count = 0;
			}else{
				/*leave all the options open*/
				g.cell[i][j].choices[0] = 0;
				for(k = 1; k <= 10; k++){
					g.cell[i][j].choices[k] = 1;
				}
				g.cell[i][j].count = 9;
			}		
		}	
	}
	
	/*now we eliminate all the cheeses forbidden by the sudoku rules*/
	for(i = 0; i < 9; i++){
		for(j = 0; j < 9; j++){
			if(g.cell[i][j].choices[0] != 0 ){
				value = g.cell[i][j].choices[0];
				/*eliminate this choice from is's row*/
				for(y = 0; y < 9; y++){
					g.cell[i][y].choices[value] = 0;
				}
				/*eliminate this choice from is's column*/
				for(x = 0; x < 9; x++){
					g.cell[x][j].choices[value] = 0;
				}
				/*eliminate this choice from is's 3x3 square*/

				/*finding the position of the cell in the 3x3*/ 
				x = i % 3;
				y = j % 3;
				kstart = i - x;
				lstart = j - y;
				for(k = kstart; k < kstart + 3; k++){
					for(l = lstart; l < lstart + 3; l++){
						g.cell[k][l].choices[value] = 0;
					}
				}
			}
		}
	}
	g = grid_udate_counts(g);
	/*assuming it is and it changes if needed*/ 
	g.unique = 1;	
	return g;
}


/*updates the g according to the c(choice) given
 * also updates the choices of the effected cells
 * assumes c.n != 0  */
Grid_T grid_update(Grid_T g, Choice_T c){
int value, x, y, k, l, kstart, lstart;

	if(c.n == 0)return g;

	g.cell[c.i][c.j].choices[0] = c.n; 
	

	/*grid_eleminate_choices(g, c.i, c.j);*/
	 
	if(g.cell[c.i][c.j].choices[0] == 0 )return g;

	value = g.cell[c.i][c.j].choices[0];
	/*eliminate this choice from is's row*/
	for(y = 0; y < 9; y++){
		g.cell[c.i][y].choices[value] = 0;
	}
	/*eliminate this choice from is's column*/
	for(x = 0; x < 9; x++){
		g.cell[x][c.j].choices[value] = 0;
	}
	/*eliminate this choice from is's 3x3 square*/

	/*finding the position of the cell in the 3x3*/ 
	x = c.i % 3;
	y = c.j % 3;
	kstart = c.i - x;
	lstart = c.j-y;
	for(k = kstart; k < kstart + 3; k++){
		for(l = lstart; l < lstart + 3; l++){
			g.cell[k][l].choices[value] = 0;
		}
	}		
		
	g = grid_udate_counts(g);
	return g;
}


/*returns the unique flag of g*/
int grid_unique(Grid_T g){return g.unique;}


/*returns a cell with a unique choice of g
 *if there is none it return (0,0,0)*/
Choice_T grid_exist_unique(Grid_T g){
	int i, j, k;
	Choice_T c;
	c.i = 0;
	c.j = 0;
	c.n = 0;

	for(i = 0; i < 9; i++){
		for(j = 0; j < 9; j++){
			if(g.cell[i][j].choices[0] != 0)continue;
			if(g.cell[i][j].count == 1){
				for(k = 1; k < 10; k++){
					if(g.cell[i][j].choices[k] == 1){
						c.n = k;
						break;
					}
				}
				c.i = i;
				c.j = j;
				return c;
			}
		}
	}
	
	return c;	
}


/*clears unique flag*/
Grid_T grid_clear_unique(Grid_T g){
	g.unique = 0;
	return g;
}


/*return value of cell given to choice c of g
 * the value is returned to the .n of the Choice_T it returns*/
Choice_T grid_read_value(Grid_T g, Choice_T c){
	c.n = g.cell[c.i][c.j].choices[0];
	return c;
}


#ifdef DEBUG
/*prints a cell of the grid (for debug purposes*/
void grid_cell_print(FILE *stream, Grid_T g, Choice_T c){
	int k;

	fprintf(stream, "cell[%d][%d] is assigned to the value %d\n", c.i, c.j, g.cell[c.i][c.j].choices[0]);
	fprintf(stream, "it can take %d possible values\n", g.cell[c.i][c.j].count);
	if(g.cell[c.i][c.j].count != 0){
		fprintf(stream, "which are: ");
		for(k = 1; k < 10; k++){
			if(g.cell[c.i][c.j].choices[k] == 1)fprintf(stream, "%d ", k);
		}
	}
	fprintf(stream, "\n\n");
		
}
#endif


/*iterates over choices in in cell given(t.i, t,j) from value = t.n + 1
 * if there is no valid choice in this cell it cheeks the next one
 * if no chell has valid choice return (0,0,0)*/
Choice_T grid_iterate(Grid_T g, Choice_T t){
	/*iterate to next choise*/
	t.n++;
	if(t.n > 9){
		t.n = 1;
		t.j += 1;
		if(t.j > 8){
			t.i += 1;
			t.j = 0;
		}
	}

	/*assert(t.n > 0 && t.n < 10);
	assert(t.j >= 0 && t.j <= 8);*/
	
	/*check the chell given*/
	for(; t.n < 10; t.n++){
		if(g.cell[t.i][t.j].choices[0] != 0)break;
		if(g.cell[t.i][t.j].choices[t.n] == 1){
			return t;	
		}
	}

	/*go to the next cell*/
	t.j += 1;
	if(t.j > 8){
		t.i += 1;
		t.j = 0;
	}
	
	/*iterate untill you find one*/
	for(; t.i < 9; t.i++){
		for(; t.j < 9; t.j++){
			for(t.n = 1; t.n < 10; t.n++){
				if(g.cell[t.i][t.j].choices[0] != 0)break;
				if(g.cell[t.i][t.j].choices[t.n] == 1){
					return t;	
				}
			}
		}
		t.j = 0;
	}

	/*didnt find one*/
	t.i = 0;
	t.j = 0;
	t.n = 0;
	return t;
}


/*
int main (void){
	int i;
	Grid_T g, g0;
	struct choice_s c;	
	int sudoku[9][9] = {
    {5, 3, 0, 0, 7, 0, 0, 0, 0},
    {6, 0, 0, 1, 9, 5, 0, 0, 0},
    {0, 9, 8, 0, 0, 0, 0, 6, 0},
    {8, 0, 0, 0, 6, 0, 0, 0, 3},
    {4, 0, 0, 8, 0, 3, 0, 0, 1},
    {7, 0, 0, 0, 2, 0, 0, 0, 6},
    {0, 6, 0, 0, 0, 0, 2, 8, 0},
    {0, 0, 0, 4, 1, 9, 0, 0, 5},
    {0, 0, 0, 0, 8, 0, 0, 7, 9}
    };
	int sudoku0[9][9] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0}
    };






	g = grid_init(g, sudoku); 
	g0 = grid_init(g0, sudoku0);

	c.i = 7;
	c.j = 6;
	grid_cell_print(stdout, g, c);
	
	printf("8,6 = %d\n", g.cell[8][6].choices[0]);
	for(i = 1; i < 10; i++){
		printf("%d) -> %d\n", i, g.cell[7][6].choices[i]);
	}

	c.i = 8;
	c.j = 6;
	c.n = 6;
	g = grid_update(g, c);
	
	printf("\n\n");
	printf("8,6 = %d\n", g.cell[8][6].choices[0]);
	for(i = 1; i < 10; i++){
		printf("%d) -> %d\n", i, g.cell[7][6].choices[i]);
	}
	
	c.i = 7;
	c.j = 6;
	grid_cell_print(stdout, g, c);

	c.i = 2;
	c.j = 3;
	grid_cell_print(stdout, g, c);

	c.i = 0;
	c.j = 0;
	c.n = 0;
	while (1){
		c = grid_iterate(g, c);
		printf("\n%d,%d -> %d\n", c.i, c.j, c.n);
		if(c.i == 0 && c.j == 0 && c.n == 0)break;
	}

	
	
	c.i = 8;
	c.j = 7;
	grid_cell_print(stdout, g, c);
	return 0;
}
*/
