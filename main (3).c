//Part 2

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

#define MATRIX_SIZE 100
#define NUM_CHILDREN 2

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


int main(int argc, char* argv[] )
{

    int student_Number[] = {1, 2, 1, 1, 9, 6, 8};
    int studentNumber_birthYear[]={2,4,2,7,5,7,1,9,0,4};

    int matrix1[MATRIX_SIZE][MATRIX_SIZE]={0};
    int matrix2[MATRIX_SIZE][MATRIX_SIZE]={0};
    int matrix3[MATRIX_SIZE][MATRIX_SIZE]={0};

     creatMatrixs(matrix1, matrix2, student_Number,studentNumber_birthYear);


     //   Create pipes
    int fd[NUM_CHILDREN][2];

    for (int i = 0; i < NUM_CHILDREN; i++) {
        if (pipe(fd[i]) == -1) {
            printf("Error opening pipe %d!\n", i);
            return 1;
        }
    }
     clock_t start_time = clock();

    // Create children
    for (int i = 0; i < NUM_CHILDREN; i++) {
        pid_t id = fork();
        if (id == -1) {
            printf("Error with fork for child %d!\n", i);
            return 2;
        }

          /////////////// Code for the child process //////////////
        if (id == 0) {

            close(fd[i][0]); // Close read end

            multiplyMatrices(matrix3, matrix1, matrix2);

            if (write(fd[i][1], matrix3, sizeof(matrix3)) == -1) {
                printf("Error writing to pipe %d\n", i);
                return 3;
            }
            close(fd[i][1]); // Close write end

            exit(0); // Exit the child process
        }
    }

            //////////////// Code for the parent process////////////////
    for (int i = 0; i < NUM_CHILDREN; i++) {

        close(fd[i][1]); // Close write end for each child
        wait(NULL);     // wait for each child to finish

        if (read(fd[i][0], matrix3, sizeof(matrix3)) == -1) {
            printf("Error reading from pipe %d\n", i);
            return 4;
        }

        // Print the result (matrix3)
        printMateix(matrix3);

        close(fd[i][0]); // Close read end for each child
    }

    clock_t end_time = clock();
    double execution_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    double throughput=(1/execution_time);

    printf("Execution time to multiply matrices: %f secondss\n", execution_time);
    printf("Throughput : %f secondss\n", throughput);

    return 0;
}

