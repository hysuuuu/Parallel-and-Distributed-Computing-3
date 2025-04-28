#include <stdio.h> //remove if not using.
#include "util.h"//implementing
#include <pthread.h>
#include <stdlib.h>  

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

typedef enum {
    ORDER_IJK,
    ORDER_IKJ,
    ORDER_JIK,
    ORDER_JKI,
    ORDER_KIJ,
    ORDER_KJI
} LoopOrder;

typedef struct{
    Mat *A;
    Mat *B;
    Mat *C;
    unsigned int start_row;
    unsigned int end_row;
    LoopOrder order;
} thread_data;

static void multiply_row(Mat *A, Mat *B, Mat *C, unsigned int row, LoopOrder order) {
    unsigned int N = A->n;
    switch (order) {
        case ORDER_IJK:
            for (unsigned int j = 0; j < N; j++) {
                double sum = 0;
                for (unsigned int k = 0; k < N; k++) {
                    sum += A->ptr[row * N + k] * B->ptr[k * N + j];
                }
                C->ptr[row * N + j] = sum;
            }
            break;
        case ORDER_IKJ:
            for (unsigned int k = 0; k < N; k++) {
                for (unsigned int j = 0; j < N; j++) {
                    C->ptr[row * N + j] += A->ptr[row * N + k] * B->ptr[k * N + j];
                }
            }
            break;
        case ORDER_JIK:
            for (unsigned int j = 0; j < N; j++) {
                for (unsigned int i = row; i <= row; i++) {
                    double sum = 0;
                    for (unsigned int k = 0; k < N; k++) {
                        sum += A->ptr[i * N + k] * B->ptr[k * N + j];
                    }
                    C->ptr[i * N + j] = sum;
                }
            }
            break;
        case ORDER_JKI:
            for (unsigned int j = 0; j < N; j++) {
                for (unsigned int k = 0; k < N; k++) {
                    C->ptr[row * N + j] += A->ptr[row * N + k] * B->ptr[k * N + j];
                }
            }
            break;
        case ORDER_KIJ:
            for (unsigned int k = 0; k < N; k++) {
                for (unsigned int i = row; i <= row; i++) {
                    for (unsigned int j = 0; j < N; j++) {
                        C->ptr[i * N + j] += A->ptr[i * N + k] * B->ptr[k * N + j];
                    }
                }
            }
            break;
        case ORDER_KJI:
            for (unsigned int k = 0; k < N; k++) {
                for (unsigned int j = 0; j < N; j++) {
                    C->ptr[row * N + j] += A->ptr[row * N + k] * B->ptr[k * N + j];
                }
            }
            break;
    }
}

// Matirx[i][j] = M->ptr[i * M->n + j]
void *mat_multiply_thread(void *arg) {
    thread_data *data = (thread_data *)arg;
    Mat *A = data->A;
    Mat *B = data->B;
    Mat *C = data->C;
    unsigned int start_row = data->start_row;
    unsigned int end_row = data->end_row;
    LoopOrder order = data->order;

    for (unsigned int i = start_row; i <= end_row; i++) {  
        multiply_row(A, B, C, i, order);
    }
    return NULL;
}

void mat_multiply(Mat *A, Mat *B, Mat *C, unsigned int threads) {
    char *e = getenv("LOOP_ORDER");
    LoopOrder order = e ? (LoopOrder)atoi(e) : ORDER_IJK;

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
        data[i].order = order;

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
    return;
}
