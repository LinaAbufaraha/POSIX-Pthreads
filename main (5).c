//part 4

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <pthread.h>

#define MATRIX_SIZE 100
#define NUM_THREADS 2


pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int work_done = 0;


struct ThreadData {
    int (*matrix3)[MATRIX_SIZE][MATRIX_SIZE];
    const int (*matrix1)[MATRIX_SIZE][MATRIX_SIZE];
    const int (*matrix2)[MATRIX_SIZE][MATRIX_SIZE];
    int start;
    int end;
};

void* multiplyMatrices(void* arg) {
     struct ThreadData* data =  (struct ThreadData*)arg;

    for (int i = data->start; i < data->end; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            data->matrix3[0][i][j] = 0;
            for (int k = 0; k < MATRIX_SIZE; k++) {
                data->matrix3[0][i][j] += data->matrix1[0][i][k] * data->matrix2[0][k][j];
            }
        }
    }

    pthread_mutex_lock(&mutex);
    work_done = 1;
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
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

void printMatrix(int matrix[MATRIX_SIZE][MATRIX_SIZE]) {
    printf("\nThe result matrix3:\n");
    int i, j;
    for (i = 0; i < MATRIX_SIZE; i++) {
        for (j = 0; j < MATRIX_SIZE; j++) {
            printf("%d\t", matrix[i][j]);
        }
        printf("\n");
    }
}

int main(int argc, char *argv[])
{

    int student_Number[] = {1, 2, 1, 1, 9, 6, 8};
    int studentNumber_birthYear[] = {2, 4, 2, 7, 5, 7, 1, 9, 0, 4};

    int matrix1[MATRIX_SIZE][MATRIX_SIZE] = {0};
    int matrix2[MATRIX_SIZE][MATRIX_SIZE] = {0};
    int matrix3[MATRIX_SIZE][MATRIX_SIZE] = {0};

    creatMatrixs(matrix1, matrix2, student_Number,studentNumber_birthYear);

    pthread_t th[NUM_THREADS];
    struct ThreadData threadData[NUM_THREADS];

      pthread_attr_t attr;
      pthread_attr_init(&attr);
      pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED); // Set threads as detached


       clock_t start_time = clock();
      int i;
      for ( i = 0; i < NUM_THREADS; i++) {  // Divide the work between threads
        threadData[i].start = i * (MATRIX_SIZE / NUM_THREADS);
        threadData[i].end =(i == NUM_THREADS - 1) ? MATRIX_SIZE : ((i + 1) * (MATRIX_SIZE / NUM_THREADS) );
        //Any remaining rows will be covered, ensuring that all rows are processed
        threadData[i].matrix1 = &matrix1;
        threadData[i].matrix2 = &matrix2;
        threadData[i].matrix3 = &matrix3;

         if(pthread_create(&th[i], &attr ,multiplyMatrices, (void*)&threadData[i]) !=0){
         perror ("Error crieat thread\n");
         }
    }


     // Main thread waits for threads to finish
    pthread_mutex_lock(&mutex);
    while (!work_done) {
        pthread_cond_wait(&cond, &mutex);
    }
    pthread_mutex_unlock(&mutex);

    printMatrix(matrix3);
    pthread_attr_destroy(&attr);  // Destroy the thread attribute

     clock_t end_time = clock();
     double execution_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    double throughput=(1/execution_time);

    printf("Execution time to multiply matrices: %f secondss\n", execution_time);
    printf("Throughput : %f secondss\n", throughput);

    return 0;
}
