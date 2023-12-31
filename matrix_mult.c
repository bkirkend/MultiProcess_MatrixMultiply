#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "matrix_def.c"

#define MAX(a, b) ((a) > (b) ? (a) : (b))

int main(){
	//matrix initialization from matrix def test file
	int** matrix1 = initializeMatrix(row1, col1, func1);
	if(matrix1 == NULL){
		return 6;
	}
	int ** matrix2 = initializeMatrix(row2, col2, func2);
	if(matrix2 == NULL){
		return 7;
	}
	
	if(row2 != col1){
		printf("Error! Matrix sizes invalid\n");
		return 1;
	}
	int numProcesses = MAX(row1, col2);

	//printf("numProcesses is %d\n", numProcesses);
	//requires one pipe for each element in output
	//main parent process reads from each pipe, children write to each pipe
	int pipes[numProcesses][2];
	for(int i = 0; i < numProcesses; i++){
		if(pipe(pipes[i]) == -1){
			printf("Error! Pipe creation failed\n");
			return 2;
		}
	}
	
	//assign each process to compute one row or column of result (whichever is larger)
	if(row1 > col2){ 
		// printf("processing over rows\n");
		// fflush(stdout);
		//process over rows
		for(int j = 0; j < numProcesses; j++){
			int pid = fork();
			if(pid == -1){
				printf("Error! Fork failure\n");
				return 3;
			}
			if(pid == 0){
				//close read end of pipes
				for(int k = 0; k < numProcesses; k++){
					close(pipes[k][0]);
				}
				//child process execution - iterating over a row
				int res_row[col2];
				int result;
				for(int i = 0; i < col2; i++){
					result = 0;
					for(int k = 0; k < row2; k++){
						result += matrix1[j][k] * matrix2[k][i];	
					}	
					res_row[i] = result;
				}	

				//write row to parent process
				if(write(pipes[j][1], res_row, sizeof(int) * col2) < 1){
					printf("Error! Writing to pipe\n");
					return 4;
				}

				//close write end of pipes
				for(int h = 0; h < numProcesses; h++){
					close(pipes[h][1]);
				}
				return 0;	
			}
		}
		//parent process execution

		//close write end of pipes
		for(int i = 0; i < numProcesses; i++){
			close(pipes[i][1]);
		}

		//wait for all child processes to finish
		for(int i = 0; i < numProcesses; i++){
			wait(NULL);
		}

		//read from pipes to build result
		int** result_matrix = initializeMatrixNull(row1, col2);
		for(int i = 0; i < numProcesses; i++){
			if(read(pipes[i][0], &(result_matrix[i]), sizeof(int) * col2) < 0){
				printf("Error! Reading from pipes\n");
				return 5;
			}
		}

		//close read end of pipes
		for(int i = 0; i < numProcesses; i++){
			close(pipes[i][0]);
		}
		print_matrix(row1, col2, result_matrix);

	} else {

		//process over columns
		// printf("processing over columns\n");
		// fflush(stdout);
		for(int j = 0; j < numProcesses; j++){
			int pid = fork();
			if(pid == -1){
				printf("Error! Fork failure\n");
				return 3;
			}
			if(pid == 0){
				//close read end of pipes
				for(int k = 0; k < numProcesses; k++){
					close(pipes[k][0]);
				} //child process execution - iterating over a column
				int res_col[row1];
				int result;
				for(int i = 0; i < row1; i++){
					result = 0;
					for(int k = 0; k < col1; k++){
						result += matrix1[i][k] * matrix2[k][j];	
					}	
					res_col[i] = result;
				}	

				//write row to parent process
				if(write(pipes[j][1], res_col, sizeof(int) * row1) < 1){
					printf("Error! Writing to pipe\n");
					return 4;
				}

				//close write end of pipes
				for(int h = 0; h < numProcesses; h++){
					close(pipes[h][1]);
				}
				return 0;	
			}
		}
		//parent process execution

		//close write end of pipes
		for(int i = 0; i < numProcesses; i++){
			close(pipes[i][1]);
		}

		//wait for all child processes to finish
		for(int i = 0; i < numProcesses; i++){
			wait(NULL);
		}

		//read from pipes to build result
		int** result_matrix = initializeMatrixNull(row1, col2);
		for(int i = 0; i < numProcesses; i++){
			for(int j = 0; j < row1; j++){
				if(read(pipes[i][0], &result_matrix[j][i], sizeof(int)) < 0){
					printf("Error! Reading from pipes\n");
					return 5;
				}
			}
		}

		//close read end of pipes
		for(int i = 0; i < numProcesses; i++){
			close(pipes[i][0]);
		}
		
		print_matrix(row1, col1, matrix1);
		print_matrix(row2, col2, matrix2);
		print_matrix(row1, col2, result_matrix);	

		freeMatrix(matrix1, row1, col1);
		freeMatrix(matrix2, row2, col2);
		freeMatrix(result_matrix, row1, col2);
	}

	return 0;	
}
