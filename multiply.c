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

typedef struct{
    Mat *A;
    Mat *B;
    Mat *C;
    unsigned int start_row;
    unsigned int end_row;
} thread_data;

// threads: {2, 4, 8, 16}.
void mat_multiply(Mat *A, Mat *B, Mat *C, unsigned int threads) {
    
    return;
}
