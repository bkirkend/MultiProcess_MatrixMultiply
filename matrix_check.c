#include <stdio.h>
#include <stdlib.h>
#include "matrix_def.c"

int main() {
  int **matrix1 = initializeMatrix(row1, col1, func1);
  if (matrix1 == NULL) {
    return 6;
  }
  int **matrix2 = initializeMatrix(row2, col2, func2);
  if (matrix2 == NULL) {
    return 7;
  }

  if (row2 != col1) {
    printf("Error! Matrix sizes invalid\n");
    return 1;
  }

  int **result = initializeMatrixNull(row1, col2);
  int value;
  for (int i = 0; i < row1; i++) {
    for (int j = 0; j < col2; j++) {
      value = 0;
      for (int k = 0; k < row2;
           k++) { // loop for "dot product" (row2 must equal col1)
        value += matrix1[i][k] * matrix2[k][j];
      }
      result[i][j] = value;
    }
  }
  print_matrix(row1, col1, matrix1);
  print_matrix(row2, col2, matrix2);
  print_matrix(row1, col2, result);
  freeMatrix(matrix1, row1, col1);
  freeMatrix(matrix2, row2, col2);
  freeMatrix(result, row1, col2);
  return 0;
}
