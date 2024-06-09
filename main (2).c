#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define MATRIX_SIZE 100

void multiplyMatrices(int matrix3[MATRIX_SIZE][MATRIX_SIZE], const int matrix1[MATRIX_SIZE][MATRIX_SIZE], const int matrix2[MATRIX_SIZE][MATRIX_SIZE]) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            matrix3[i][j] = 0;
            for (int k = 0; k < MATRIX_SIZE; k++) {
                matrix3[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }
}

void creatMatrixs(int matrix1[MATRIX_SIZE][MATRIX_SIZE],int matrix2[MATRIX_SIZE][MATRIX_SIZE], int student_Number[], int studentNumber_birthYear[]) {
    // Fill the matrix with the repeating student_Number and studentNumber_birthYear
    for (int i = 0; i < MATRIX_SIZE; ++i) {
        for (int j = 0; j < MATRIX_SIZE; ++j) {
            matrix1[i][j] = student_Number[j % 7];
            matrix2[i][j] = studentNumber_birthYear[j % 10];
        }
    }
}


void printMateix(int matrix3[MATRIX_SIZE][MATRIX_SIZE]){

      printf("\nThe result matrix3:\n");
      int i,j;
      for(i=0; i < MATRIX_SIZE;i++){
        for(j=0; j < MATRIX_SIZE;j++){
           printf("%d\t",matrix3[i][j]);
        }
        printf("\n");
      }

}

int main()
{
    // Define the repeating number
    int student_Number[] = {1, 2, 1, 1, 9, 6, 8};
    int studentNumber_birthYear[]={2,4,2,7,5,7,1,9,0,4};

    int matrix1[MATRIX_SIZE][MATRIX_SIZE]={0};
    int matrix2[MATRIX_SIZE][MATRIX_SIZE]={0};
    int matrix3[MATRIX_SIZE][MATRIX_SIZE]={0};

   creatMatrixs(matrix1, matrix2, student_Number,studentNumber_birthYear);

    clock_t start_time = clock();

     multiplyMatrices(matrix3, matrix1, matrix2);

    printMateix(matrix3);

   clock_t end_time = clock();
    double execution_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    double throughput=(1/execution_time);

    printf("Execution time to multiply matrices: %f secondss\n", execution_time);
    printf("Throughput : %f secondss\n", throughput);

    return 0;
}
