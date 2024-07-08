/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
    if (N == 32 && M == 32) {
        int i, j, y, t1, t2, t3, t4, t5, t6, t7, t8;
    for (j = 0; j + 8 <= M; j += 8) {
        for (i = 0; i + 8 <= N; i += 8){
            for (y = j; y < j + 8; ++y) {
                
                t1 = A[i][y];
                t2 = A[i + 1][y];
                t3 = A[i + 2][y];
                t4 = A[i + 3][y];
                t5 = A[i + 4][y];
                t6 = A[i + 5][y];
                t7 = A[i + 6][y];
                t8 = A[i + 7][y];

                B[y][i] = t1;
                B[y][i + 1] = t2;
                B[y][i + 2] = t3;
                B[y][i + 3] = t4;
                B[y][i + 4] = t5;
                B[y][i + 5] = t6;
                B[y][i + 6] = t7;
                B[y][i + 7] = t8;
                
            }
        }
    }
    } else {
         int i, j, y, t1, t2, t3, t4;
    for (j = 0; j + 8 <= M; j += 8) {
        for (i = 0; i + 8 <= N; i += 8){
            for (y = j; y < j + 8; ++y) {
                
                t1 = A[i][y];
                t2 = A[i + 1][y];
                t3 = A[i + 2][y];
                t4 = A[i + 3][y];

                B[y][i] = t1;
                B[y][i + 1] = t2;
                B[y][i + 2] = t3;
                B[y][i + 3] = t4;
                
            }

            for (y = j; y < j + 8; ++y) {
                
                t1 = A[i + 4][y];
                t2 = A[i + 5][y];
                t3 = A[i + 6][y];
                t4 = A[i + 7][y];

                B[y][i + 4] = t1;
                B[y][i + 5] = t2;
                B[y][i + 6] = t3;
                B[y][i + 7] = t4;
                
            }

        }
    }
    int tx, ty;
    for (ty = 0; ty < j; ++ty ) {
        for (tx = i; tx < N; ++tx) {
            t1 = A[tx][ty];
            B[ty][tx] = t1;
        }
    }
    for (ty = j; ty < M; ++ty ) {
        for (tx = 0; tx < N; ++tx) {
            t1 = A[tx][ty];
            B[ty][tx] = t1;
        }
    }
    }
}

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/**
 * using block trick
 */
char transBlock8_desc[] = "Block8 transpose";
void transBlock8(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp, x, y;
    for  (i = 0; i + 8 <= N; i += 8){
        for(j = 0; j + 8 <= M; j += 8) {
            for (y = j; y < j + 8; ++y) {
                for (x = i; x < i + 8; ++x) {
                    tmp = A[x][y];
                    B[y][x] = tmp;
                }
            }
        }
    }
    int tx, ty;
    for (ty = 0; ty < j; ++ty ) {
        for (tx = i; tx < N; ++tx) {
            tmp = A[tx][ty];
            B[ty][tx] = tmp;
        }
    }
    for (ty = j; ty < M; ++ty ) {
        for (tx = 0; tx < N; ++tx) {
            tmp = A[tx][ty];
            B[ty][tx] = tmp;
        }
    }

}


char transBlockopen_desc[] = "Blockopen transpose";
void transBlockopen(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, y, t1, t2, t3, t4, t5, t6, t7, t8;
    for (j = 0; j + 8 <= M; j += 8) {
        for (i = 0; i + 8 <= N; i += 8){
            for (y = j; y < j + 8; ++y) {
                
                t1 = A[i][y];
                t2 = A[i + 1][y];
                t3 = A[i + 2][y];
                t4 = A[i + 3][y];
                t5 = A[i + 4][y];
                t6 = A[i + 5][y];
                t7 = A[i + 6][y];
                t8 = A[i + 7][y];

                B[y][i] = t1;
                B[y][i + 1] = t2;
                B[y][i + 2] = t3;
                B[y][i + 3] = t4;
                B[y][i + 4] = t5;
                B[y][i + 5] = t6;
                B[y][i + 6] = t7;
                B[y][i + 7] = t8;
                
            }
        }
    }
    int tx, ty;
    for (ty = 0; ty < j; ++ty ) {
        for (tx = i; tx < N; ++tx) {
            t1 = A[tx][ty];
            B[ty][tx] = t1;
        }
    }
    for (ty = j; ty < M; ++ty ) {
        for (tx = 0; tx < N; ++tx) {
            t1 = A[tx][ty];
            B[ty][tx] = t1;
        }
    }

}

char transBlockopen64_desc[] = "Blockopen64 transpose";
void transBlockopen64(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, y, t1, t2, t3, t4;
    for (j = 0; j + 8 <= M; j += 8) {
        for (i = 0; i + 8 <= N; i += 8){
            for (y = j; y < j + 8; ++y) {
                
                t1 = A[i][y];
                t2 = A[i + 1][y];
                t3 = A[i + 2][y];
                t4 = A[i + 3][y];

                B[y][i] = t1;
                B[y][i + 1] = t2;
                B[y][i + 2] = t3;
                B[y][i + 3] = t4;
                
            }

            for (y = j; y < j + 8; ++y) {
                
                t1 = A[i + 4][y];
                t2 = A[i + 5][y];
                t3 = A[i + 6][y];
                t4 = A[i + 7][y];

                B[y][i + 4] = t1;
                B[y][i + 5] = t2;
                B[y][i + 6] = t3;
                B[y][i + 7] = t4;
                
            }

        }
    }
    int tx, ty;
    for (ty = 0; ty < j; ++ty ) {
        for (tx = i; tx < N; ++tx) {
            t1 = A[tx][ty];
            B[ty][tx] = t1;
        }
    }
    for (ty = j; ty < M; ++ty ) {
        for (tx = 0; tx < N; ++tx) {
            t1 = A[tx][ty];
            B[ty][tx] = t1;
        }
    }

}


/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */

    // registerTransFunction(trans, trans_desc); 
    // registerTransFunction(transBlock8, transBlock8_desc);
    // registerTransFunction(transBlockSplit, transBlockSplit_desc);
    registerTransFunction(transBlockopen, transBlockopen_desc);
    registerTransFunction(transBlockopen64, transBlockopen64_desc);

}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

