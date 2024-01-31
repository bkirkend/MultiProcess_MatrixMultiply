//declare test dimensions
//col1 must equal row2 for multiplication to be valid
int row1 = 4;
int col1 = 3; 
int row2 = 3;
int col2 = 4;

int func1(int x, int y){return x ^ y;}
int func2(int x, int y){return x & y + x | y;}

// Function to initialize matrix1
int** initializeMatrix(int row, int col, int (*func)(int, int)) {
	int** matrix = malloc(sizeof(int*) * row);
	if(matrix == NULL){
		printf("Error matrix allocation\n");
		return NULL;
	}
    for (int i = 0; i < row; i++) {
		matrix[i] = malloc(sizeof(int) * col);	     
		if(matrix[i] == NULL){
			printf("Error matrix allocation\n");
			return NULL;
		}
    }

	for(int i = 0; i < row; i++){
		for(int j = 0; j < col; j++){
			matrix[i][j] = (*func)(i, j);
		}
	}	
	return matrix;
}

int** initializeMatrixNull(int row, int col) {
	int** matrix = malloc(sizeof(int*) * row);
	if(matrix == NULL){
		printf("Error matrix allocation\n");
		return NULL;
	}
    for (int i = 0; i < row; i++) {
		matrix[i] = malloc(sizeof(int) * col);	     
		if(matrix[i] == NULL){
			printf("Error matrix allocation\n");
			return NULL;
		}
    }
	return matrix;
}

void freeMatrix(int** matrix, int row, int col){
	for(int i = 0; i < row; i++){
		free(matrix[i]);
	}
	free(matrix);
}

void print_matrix(int row, int col, int** matrix){
	for(int i = 0; i < row; i++){
		for(int j = 0; j < col; j++){
			printf("%3d ", matrix[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}
