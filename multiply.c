#include <stdio.h> //remove if not using.
#include "util.h"//implementing
#include <pthread.h>

// Functions defined with the modifier 'static' are only visible to other
// functions in this file. They cannot be called from outside (for example,
// from main.c). Use them to organize your code. Remember that in C, you cannot
// use a function until you declare it, so declare all your utility functions
// above the point where you use them.
//
// Maintain the mat_multiply function as lean as possible because we are
// measuring their speed. Unless you are debugging, do not print anything on
// them, that consumes precious time.

// Remember to set the correct values for C->m, and C->n after doing the
// multiplication (this matters if the matrices are not square)

// Thread data structure for dividing the outer i-loop across threads.
// Each thread computes matrix multiplication for rows [start_row, end_row].
typedef struct{
    Mat *A;
    Mat *B;
    Mat *C;
    unsigned int start_row;
    unsigned int end_row;
} thread_data;

// Thread worker function:
// Computes rows from start_row to end_row (inclusive) of matrix C using the ijk ordering.
// Matirx[i][j] = M->ptr[i * M->n + j]
void *mat_multiply_thread(void *arg) {
    thread_data *data = (thread_data *)arg;
    Mat *A = data->A;
    Mat *B = data->B;
    Mat *C = data->C;
    unsigned int start_row = data->start_row;
    unsigned int end_row = data->end_row;

    for (unsigned int i = start_row; i <= end_row; i++) {
        for (unsigned int j = 0; j < B->n; j++) {
            double sum = 0;
            for (unsigned int k = 0; k < A->n; k++) {
                sum += A->ptr[i * A->n + k] * B->ptr[k * B->n + j];
            }
            C->ptr[i * C->n + j] = sum;
        }        
    }
    return NULL;
}

// Multi-threaded matrix multiplication:
// Unfolds the outer 'i' loop across threads.
void mat_multiply(Mat *A, Mat *B, Mat *C, unsigned int threads) {
    pthread_t tid[threads];
    thread_data data[threads];

    unsigned int rows_per_thread = A->m / threads;
    unsigned int extra_rows = A->m % threads;

    unsigned int start_row = 0;
    for (unsigned int i = 0; i < threads; i++) {
        data[i].A = A;
        data[i].B = B;
        data[i].C = C;
        data[i].start_row = start_row;
        data[i].end_row = start_row + rows_per_thread - 1;

        if (extra_rows > 0) {
            data[i].end_row++;
            extra_rows--;
        }
        start_row = data[i].end_row + 1;        
    }

    for (unsigned int i = 0; i < threads; i++) {
        pthread_create(&tid[i], NULL, mat_multiply_thread, &data[i]);
    }
    for (unsigned int i = 0; i < threads; i++) {
        pthread_join(tid[i], NULL);
    }

    // Set dimensions of the result matrix C
    C->m = A->m;
    C->n = B->n;

    return;
}
