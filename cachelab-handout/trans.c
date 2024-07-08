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
    if (N == 32 && M == 32)
    {
        int i, j, y, t1, t2, t3, t4, t5, t6, t7, t8;
        for (j = 0; j + 8 <= M; j += 8)
        {
            for (i = 0; i + 8 <= N; i += 8)
            {
                if (i == j && i + 8 < N)
                {
                    for (y = j; y < j + 8; ++y)
                    {

                        t1 = A[i][y];
                        t2 = A[i + 1][y];
                        t3 = A[i + 2][y];
                        t4 = A[i + 3][y];
                        t5 = A[i + 4][y];
                        t6 = A[i + 5][y];
                        t7 = A[i + 6][y];
                        t8 = A[i + 7][y];

                        B[y][i + 8] = t1;
                        B[y][i + 9] = t2;
                        B[y][i + 10] = t3;
                        B[y][i + 11] = t4;
                        B[y][i + 12] = t5;
                        B[y][i + 13] = t6;
                        B[y][i + 14] = t7;
                        B[y][i + 15] = t8;
                    }
                    for (y = j; y < j + 8; ++y)
                    {

                        t1 = B[y][i + 8];
                        t2 = B[y][i + 9];
                        t3 = B[y][i + 10];
                        t4 = B[y][i + 11];
                        t5 = B[y][i + 12];
                        t6 = B[y][i + 13];
                        t7 = B[y][i + 14];
                        t8 = B[y][i + 15];

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
                else
                {
                    for (y = j; y < j + 8; ++y)
                    {

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
        }
    }
    else if (M == 64 && N == 64)
    {
        int i, j, y, t1, t2, t3, t4;
        for (j = 0; j + 8 <= M; j += 8)
        {
            for (i = 0; i + 8 <= N; i += 8)
            {
                if (i == j && i + 16 < N && j + 16 < M)
                {
                    // diagnal line except the last diagnal block

                    // using the next block and next next block
                    // upper-left quarter
                    for (y = j; y < j + 4; ++y)
                    {

                        t1 = A[i][y];
                        t2 = A[i + 1][y];
                        t3 = A[i + 2][y];
                        t4 = A[i + 3][y];

                        B[y][i + 8] = t1;
                        B[y][i + 9] = t2;
                        B[y][i + 10] = t3;
                        B[y][i + 11] = t4;
                    }
                    // upper-right quarter
                    for (y = j + 4; y < j + 8; ++y)
                    {

                        t1 = A[i][y];
                        t2 = A[i + 1][y];
                        t3 = A[i + 2][y];
                        t4 = A[i + 3][y];

                        B[y - 4][i + 16] = t1;
                        B[y - 4][i + 17] = t2;
                        B[y - 4][i + 18] = t3;
                        B[y - 4][i + 19] = t4;
                    }

                    // get back
                    for (y = j; y < j + 4; ++y)
                    {

                        t1 = B[y][i + 8];
                        t2 = B[y][i + 9];
                        t3 = B[y][i + 10];
                        t4 = B[y][i + 11];

                        B[y][i] = t1;
                        B[y][i + 1] = t2;
                        B[y][i + 2] = t3;
                        B[y][i + 3] = t4;
                    }
                    for (y = j + 4; y < j + 8; ++y)
                    {

                        t1 = B[y - 4][i + 16];
                        t2 = B[y - 4][i + 17];
                        t3 = B[y - 4][i + 18];
                        t4 = B[y - 4][i + 19];

                        B[y][i] = t1;
                        B[y][i + 1] = t2;
                        B[y][i + 2] = t3;
                        B[y][i + 3] = t4;
                    }

                    // lower-left quarter
                    for (y = j; y < j + 4; ++y)
                    {

                        t1 = A[i + 4][y];
                        t2 = A[i + 5][y];
                        t3 = A[i + 6][y];
                        t4 = A[i + 7][y];

                        B[y][i + 12] = t1;
                        B[y][i + 13] = t2;
                        B[y][i + 14] = t3;
                        B[y][i + 15] = t4;
                    }
                    // lower-right quarter
                    for (y = j + 4; y < j + 8; ++y)
                    {

                        t1 = A[i + 4][y];
                        t2 = A[i + 5][y];
                        t3 = A[i + 6][y];
                        t4 = A[i + 7][y];

                        B[y - 4][i + 20] = t1;
                        B[y - 4][i + 21] = t2;
                        B[y - 4][i + 22] = t3;
                        B[y - 4][i + 23] = t4;
                    }

                    // get back
                    for (y = j; y < j + 4; ++y)
                    {

                        t1 = B[y][i + 12];
                        t2 = B[y][i + 13];
                        t3 = B[y][i + 14];
                        t4 = B[y][i + 15];

                        B[y][i + 4] = t1;
                        B[y][i + 5] = t2;
                        B[y][i + 6] = t3;
                        B[y][i + 7] = t4;
                    }
                    for (y = j + 4; y < j + 8; ++y)
                    {

                        t1 = B[y - 4][i + 20];
                        t2 = B[y - 4][i + 21];
                        t3 = B[y - 4][i + 22];
                        t4 = B[y - 4][i + 23];

                        B[y][i + 4] = t1;
                        B[y][i + 5] = t2;
                        B[y][i + 6] = t3;
                        B[y][i + 7] = t4;
                    }
                }
                else
                {
                    for (y = j; y < j + 4; ++y)
                    {

                        t1 = A[i][y];
                        t2 = A[i + 1][y];
                        t3 = A[i + 2][y];
                        t4 = A[i + 3][y];

                        B[y][i] = t1;
                        B[y][i + 1] = t2;
                        B[y][i + 2] = t3;
                        B[y][i + 3] = t4;
                    }

                    for (y = j + 4; y < j + 8; ++y)
                    {

                        t1 = A[i][y];
                        t2 = A[i + 1][y];
                        t3 = A[i + 2][y];
                        t4 = A[i + 3][y];

                        B[y - 4][i + 4] = t1;
                        B[y - 4][i + 5] = t2;
                        B[y - 4][i + 6] = t3;
                        B[y - 4][i + 7] = t4;
                    }

                    for (y = j; y < j + 4; ++y)
                    {
                        t1 = B[y][i + 4];
                        t2 = B[y][i + 5];
                        t3 = B[y][i + 6];
                        t4 = B[y][i + 7];

                        B[y][i + 4] = A[i + 4][y];
                        B[y][i + 5] = A[i + 5][y];
                        B[y][i + 6] = A[i + 6][y];
                        B[y][i + 7] = A[i + 7][y];

                        B[y + 4][i] = t1;
                        B[y + 4][i + 1] = t2;
                        B[y + 4][i + 2] = t3;
                        B[y + 4][i + 3] = t4;
                    }

                    for (y = j + 4; y < j + 8; ++y)
                    {

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
        }
    }
    else if (M == 61 && N == 67)
    {
        int i, j, y, t1, t2, t3, t4;
        for (j = 0; j + 8 <= M; j += 8)
        {
            for (i = 0; i + 8 <= N; i += 8)
            {
                if (i == j && i + 16 < N && j + 16 < M)
                {
                    // diagnal line except the last diagnal block

                    // using the next block and next next block
                    // upper-left quarter
                    for (y = j; y < j + 4; ++y)
                    {

                        t1 = A[i][y];
                        t2 = A[i + 1][y];
                        t3 = A[i + 2][y];
                        t4 = A[i + 3][y];

                        B[y][i + 8] = t1;
                        B[y][i + 9] = t2;
                        B[y][i + 10] = t3;
                        B[y][i + 11] = t4;
                    }
                    // upper-right quarter
                    for (y = j + 4; y < j + 8; ++y)
                    {

                        t1 = A[i][y];
                        t2 = A[i + 1][y];
                        t3 = A[i + 2][y];
                        t4 = A[i + 3][y];

                        B[y - 4][i + 16] = t1;
                        B[y - 4][i + 17] = t2;
                        B[y - 4][i + 18] = t3;
                        B[y - 4][i + 19] = t4;
                    }

                    // get back
                    for (y = j; y < j + 4; ++y)
                    {

                        t1 = B[y][i + 8];
                        t2 = B[y][i + 9];
                        t3 = B[y][i + 10];
                        t4 = B[y][i + 11];

                        B[y][i] = t1;
                        B[y][i + 1] = t2;
                        B[y][i + 2] = t3;
                        B[y][i + 3] = t4;
                    }
                    for (y = j + 4; y < j + 8; ++y)
                    {

                        t1 = B[y - 4][i + 16];
                        t2 = B[y - 4][i + 17];
                        t3 = B[y - 4][i + 18];
                        t4 = B[y - 4][i + 19];

                        B[y][i] = t1;
                        B[y][i + 1] = t2;
                        B[y][i + 2] = t3;
                        B[y][i + 3] = t4;
                    }

                    // lower-left quarter
                    for (y = j; y < j + 4; ++y)
                    {

                        t1 = A[i + 4][y];
                        t2 = A[i + 5][y];
                        t3 = A[i + 6][y];
                        t4 = A[i + 7][y];

                        B[y][i + 12] = t1;
                        B[y][i + 13] = t2;
                        B[y][i + 14] = t3;
                        B[y][i + 15] = t4;
                    }
                    // lower-right quarter
                    for (y = j + 4; y < j + 8; ++y)
                    {

                        t1 = A[i + 4][y];
                        t2 = A[i + 5][y];
                        t3 = A[i + 6][y];
                        t4 = A[i + 7][y];

                        B[y - 4][i + 20] = t1;
                        B[y - 4][i + 21] = t2;
                        B[y - 4][i + 22] = t3;
                        B[y - 4][i + 23] = t4;
                    }

                    // get back
                    for (y = j; y < j + 4; ++y)
                    {

                        t1 = B[y][i + 12];
                        t2 = B[y][i + 13];
                        t3 = B[y][i + 14];
                        t4 = B[y][i + 15];

                        B[y][i + 4] = t1;
                        B[y][i + 5] = t2;
                        B[y][i + 6] = t3;
                        B[y][i + 7] = t4;
                    }
                    for (y = j + 4; y < j + 8; ++y)
                    {

                        t1 = B[y - 4][i + 20];
                        t2 = B[y - 4][i + 21];
                        t3 = B[y - 4][i + 22];
                        t4 = B[y - 4][i + 23];

                        B[y][i + 4] = t1;
                        B[y][i + 5] = t2;
                        B[y][i + 6] = t3;
                        B[y][i + 7] = t4;
                    }
                }
                else
                {
                    for (y = j; y < j + 4; ++y)
                    {

                        t1 = A[i][y];
                        t2 = A[i + 1][y];
                        t3 = A[i + 2][y];
                        t4 = A[i + 3][y];

                        B[y][i] = t1;
                        B[y][i + 1] = t2;
                        B[y][i + 2] = t3;
                        B[y][i + 3] = t4;
                    }

                    for (y = j + 4; y < j + 8; ++y)
                    {

                        t1 = A[i][y];
                        t2 = A[i + 1][y];
                        t3 = A[i + 2][y];
                        t4 = A[i + 3][y];

                        B[y - 4][i + 4] = t1;
                        B[y - 4][i + 5] = t2;
                        B[y - 4][i + 6] = t3;
                        B[y - 4][i + 7] = t4;
                    }

                    for (y = j; y < j + 4; ++y)
                    {
                        t1 = B[y][i + 4];
                        t2 = B[y][i + 5];
                        t3 = B[y][i + 6];
                        t4 = B[y][i + 7];

                        B[y][i + 4] = A[i + 4][y];
                        B[y][i + 5] = A[i + 5][y];
                        B[y][i + 6] = A[i + 6][y];
                        B[y][i + 7] = A[i + 7][y];

                        B[y + 4][i] = t1;
                        B[y + 4][i + 1] = t2;
                        B[y + 4][i + 2] = t3;
                        B[y + 4][i + 3] = t4;
                    }

                    for (y = j + 4; y < j + 8; ++y)
                    {

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
        }
        // i = 56 -> 67, j = 56 -> 61
        for (j = 0; j + 8 <= M; j += 8)
        {
            for (y = j; y < j + 8; ++y)
            {
                t1 = A[64][y];
                t2 = A[65][y];
                t3 = A[66][y];

                B[y][64] = t1;
                B[y][65] = t2;
                B[y][66] = t3;
            }
        }

        for (i = 0; i + 8 <= N; i += 8)
        {
            for (y = 56; y < M; ++y)
            {
                t1 = A[i][y];
                t2 = A[i + 1][y];
                t3 = A[i + 2][y];
                t4 = A[i + 3][y];

                B[y][i] = t1;
                B[y][i + 1] = t2;
                B[y][i + 2] = t3;
                B[y][i + 3] = t4;
            }
            for (y = 56; y < M; ++y)
            {
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

        // i = 64, j = 56
        for (i = 64; i < N; ++i)
        {
            for (j = 56; j < M; ++j)
            {
                t1 = A[i][j];
                B[j][i] = t1;
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
    for (i = 0; i + 8 <= N; i += 8)
    {
        for (j = 0; j + 8 <= M; j += 8)
        {
            for (y = j; y < j + 8; ++y)
            {
                for (x = i; x < i + 8; ++x)
                {
                    tmp = A[x][y];
                    B[y][x] = tmp;
                }
            }
        }
    }
    int tx, ty;
    for (ty = 0; ty < j; ++ty)
    {
        for (tx = i; tx < N; ++tx)
        {
            tmp = A[tx][ty];
            B[ty][tx] = tmp;
        }
    }
    for (ty = j; ty < M; ++ty)
    {
        for (tx = 0; tx < N; ++tx)
        {
            tmp = A[tx][ty];
            B[ty][tx] = tmp;
        }
    }
}

char transBlockopen_desc[] = "Blockopen transpose";
void transBlockopen(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, y, t1, t2, t3, t4, t5, t6, t7, t8;
    for (j = 0; j + 8 <= M; j += 8)
    {
        for (i = 0; i + 8 <= N; i += 8)
        {
            for (y = j; y < j + 8; ++y)
            {

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
    for (ty = 0; ty < j; ++ty)
    {
        for (tx = i; tx < N; ++tx)
        {
            t1 = A[tx][ty];
            B[ty][tx] = t1;
        }
    }
    for (ty = j; ty < M; ++ty)
    {
        for (tx = 0; tx < N; ++tx)
        {
            t1 = A[tx][ty];
            B[ty][tx] = t1;
        }
    }
}

char transBlockopen64_desc[] = "Blockopen64 transpose";
void transBlockopen64(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, y, t1, t2, t3, t4;
    for (j = 0; j + 8 <= M; j += 8)
    {
        for (i = 0; i + 8 <= N; i += 8)
        {
            for (y = j; y < j + 8; ++y)
            {

                t1 = A[i][y];
                t2 = A[i + 1][y];
                t3 = A[i + 2][y];
                t4 = A[i + 3][y];

                B[y][i] = t1;
                B[y][i + 1] = t2;
                B[y][i + 2] = t3;
                B[y][i + 3] = t4;
            }

            for (y = j; y < j + 8; ++y)
            {

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
    for (ty = 0; ty < j; ++ty)
    {
        for (tx = i; tx < N; ++tx)
        {
            t1 = A[tx][ty];
            B[ty][tx] = t1;
        }
    }
    for (ty = j; ty < M; ++ty)
    {
        for (tx = 0; tx < N; ++tx)
        {
            t1 = A[tx][ty];
            B[ty][tx] = t1;
        }
    }
}

/**
 * Sloves the confilct misses problem.
 * Borrow the next block that will be stored soon.
 */
char transBlockopen64Diagnal_desc[] = "Blockopen64Diagnal transpose";
void transBlockopen64Diagnal(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, y, t1, t2, t3, t4;
    for (j = 0; j + 8 <= M; j += 8)
    {
        for (i = 0; i + 8 <= N; i += 8)
        {
            if (i == j && i + 16 < N)
            {
                // diagnal line except the last diagnal block

                // using the next block and next next block
                // upper-left quarter
                for (y = j; y < j + 4; ++y)
                {

                    t1 = A[i][y];
                    t2 = A[i + 1][y];
                    t3 = A[i + 2][y];
                    t4 = A[i + 3][y];

                    B[y][i + 8] = t1;
                    B[y][i + 9] = t2;
                    B[y][i + 10] = t3;
                    B[y][i + 11] = t4;
                }
                // upper-right quarter
                for (y = j + 4; y < j + 8; ++y)
                {

                    t1 = A[i][y];
                    t2 = A[i + 1][y];
                    t3 = A[i + 2][y];
                    t4 = A[i + 3][y];

                    B[y - 4][i + 16] = t1;
                    B[y - 4][i + 17] = t2;
                    B[y - 4][i + 18] = t3;
                    B[y - 4][i + 19] = t4;
                }

                // get back
                for (y = j; y < j + 4; ++y)
                {

                    t1 = B[y][i + 8];
                    t2 = B[y][i + 9];
                    t3 = B[y][i + 10];
                    t4 = B[y][i + 11];

                    B[y][i] = t1;
                    B[y][i + 1] = t2;
                    B[y][i + 2] = t3;
                    B[y][i + 3] = t4;
                }
                for (y = j + 4; y < j + 8; ++y)
                {

                    t1 = B[y - 4][i + 16];
                    t2 = B[y - 4][i + 17];
                    t3 = B[y - 4][i + 18];
                    t4 = B[y - 4][i + 19];

                    B[y][i] = t1;
                    B[y][i + 1] = t2;
                    B[y][i + 2] = t3;
                    B[y][i + 3] = t4;
                }

                // lower-left quarter
                for (y = j; y < j + 4; ++y)
                {

                    t1 = A[i + 4][y];
                    t2 = A[i + 5][y];
                    t3 = A[i + 6][y];
                    t4 = A[i + 7][y];

                    B[y][i + 12] = t1;
                    B[y][i + 13] = t2;
                    B[y][i + 14] = t3;
                    B[y][i + 15] = t4;
                }
                // lower-right quarter
                for (y = j + 4; y < j + 8; ++y)
                {

                    t1 = A[i + 4][y];
                    t2 = A[i + 5][y];
                    t3 = A[i + 6][y];
                    t4 = A[i + 7][y];

                    B[y - 4][i + 20] = t1;
                    B[y - 4][i + 21] = t2;
                    B[y - 4][i + 22] = t3;
                    B[y - 4][i + 23] = t4;
                }

                // get back
                for (y = j; y < j + 4; ++y)
                {

                    t1 = B[y][i + 12];
                    t2 = B[y][i + 13];
                    t3 = B[y][i + 14];
                    t4 = B[y][i + 15];

                    B[y][i + 4] = t1;
                    B[y][i + 5] = t2;
                    B[y][i + 6] = t3;
                    B[y][i + 7] = t4;
                }
                for (y = j + 4; y < j + 8; ++y)
                {

                    t1 = B[y - 4][i + 20];
                    t2 = B[y - 4][i + 21];
                    t3 = B[y - 4][i + 22];
                    t4 = B[y - 4][i + 23];

                    B[y][i + 4] = t1;
                    B[y][i + 5] = t2;
                    B[y][i + 6] = t3;
                    B[y][i + 7] = t4;
                }
            }
            else
            {
                for (y = j; y < j + 8; ++y)
                {

                    t1 = A[i][y];
                    t2 = A[i + 1][y];
                    t3 = A[i + 2][y];
                    t4 = A[i + 3][y];

                    B[y][i] = t1;
                    B[y][i + 1] = t2;
                    B[y][i + 2] = t3;
                    B[y][i + 3] = t4;
                }

                for (y = j; y < j + 8; ++y)
                {

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
    }
    int tx, ty;
    for (ty = 0; ty < j; ++ty)
    {
        for (tx = i; tx < N; ++tx)
        {
            t1 = A[tx][ty];
            B[ty][tx] = t1;
        }
    }
    for (ty = j; ty < M; ++ty)
    {
        for (tx = 0; tx < N; ++tx)
        {
            t1 = A[tx][ty];
            B[ty][tx] = t1;
        }
    }
}

char transBlockopen64DiagnalS4_desc[] = "Blockopen64DiagnalS4 transpose";
void transBlockopen64DiagnalS4(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, y, t1, t2, t3, t4;
    for (j = 0; j + 8 <= M; j += 8)
    {
        for (i = 0; i + 8 <= N; i += 8)
        {
            if (i == j && i + 16 < N && j + 16 < M)
            {
                // diagnal line except the last diagnal block

                // using the next block and next next block
                // upper-left quarter
                for (y = j; y < j + 4; ++y)
                {

                    t1 = A[i][y];
                    t2 = A[i + 1][y];
                    t3 = A[i + 2][y];
                    t4 = A[i + 3][y];

                    B[y][i + 8] = t1;
                    B[y][i + 9] = t2;
                    B[y][i + 10] = t3;
                    B[y][i + 11] = t4;
                }
                // upper-right quarter
                for (y = j + 4; y < j + 8; ++y)
                {

                    t1 = A[i][y];
                    t2 = A[i + 1][y];
                    t3 = A[i + 2][y];
                    t4 = A[i + 3][y];

                    B[y - 4][i + 16] = t1;
                    B[y - 4][i + 17] = t2;
                    B[y - 4][i + 18] = t3;
                    B[y - 4][i + 19] = t4;
                }

                // get back
                for (y = j; y < j + 4; ++y)
                {

                    t1 = B[y][i + 8];
                    t2 = B[y][i + 9];
                    t3 = B[y][i + 10];
                    t4 = B[y][i + 11];

                    B[y][i] = t1;
                    B[y][i + 1] = t2;
                    B[y][i + 2] = t3;
                    B[y][i + 3] = t4;
                }
                for (y = j + 4; y < j + 8; ++y)
                {

                    t1 = B[y - 4][i + 16];
                    t2 = B[y - 4][i + 17];
                    t3 = B[y - 4][i + 18];
                    t4 = B[y - 4][i + 19];

                    B[y][i] = t1;
                    B[y][i + 1] = t2;
                    B[y][i + 2] = t3;
                    B[y][i + 3] = t4;
                }

                // lower-left quarter
                for (y = j; y < j + 4; ++y)
                {

                    t1 = A[i + 4][y];
                    t2 = A[i + 5][y];
                    t3 = A[i + 6][y];
                    t4 = A[i + 7][y];

                    B[y][i + 12] = t1;
                    B[y][i + 13] = t2;
                    B[y][i + 14] = t3;
                    B[y][i + 15] = t4;
                }
                // lower-right quarter
                for (y = j + 4; y < j + 8; ++y)
                {

                    t1 = A[i + 4][y];
                    t2 = A[i + 5][y];
                    t3 = A[i + 6][y];
                    t4 = A[i + 7][y];

                    B[y - 4][i + 20] = t1;
                    B[y - 4][i + 21] = t2;
                    B[y - 4][i + 22] = t3;
                    B[y - 4][i + 23] = t4;
                }

                // get back
                for (y = j; y < j + 4; ++y)
                {

                    t1 = B[y][i + 12];
                    t2 = B[y][i + 13];
                    t3 = B[y][i + 14];
                    t4 = B[y][i + 15];

                    B[y][i + 4] = t1;
                    B[y][i + 5] = t2;
                    B[y][i + 6] = t3;
                    B[y][i + 7] = t4;
                }
                for (y = j + 4; y < j + 8; ++y)
                {

                    t1 = B[y - 4][i + 20];
                    t2 = B[y - 4][i + 21];
                    t3 = B[y - 4][i + 22];
                    t4 = B[y - 4][i + 23];

                    B[y][i + 4] = t1;
                    B[y][i + 5] = t2;
                    B[y][i + 6] = t3;
                    B[y][i + 7] = t4;
                }
            }
            else
            {
                for (y = j; y < j + 4; ++y)
                {

                    t1 = A[i][y];
                    t2 = A[i + 1][y];
                    t3 = A[i + 2][y];
                    t4 = A[i + 3][y];

                    B[y][i] = t1;
                    B[y][i + 1] = t2;
                    B[y][i + 2] = t3;
                    B[y][i + 3] = t4;
                }

                for (y = j + 4; y < j + 8; ++y)
                {

                    t1 = A[i][y];
                    t2 = A[i + 1][y];
                    t3 = A[i + 2][y];
                    t4 = A[i + 3][y];

                    B[y - 4][i + 4] = t1;
                    B[y - 4][i + 5] = t2;
                    B[y - 4][i + 6] = t3;
                    B[y - 4][i + 7] = t4;
                }

                for (y = j; y < j + 4; ++y)
                {
                    t1 = B[y][i + 4];
                    t2 = B[y][i + 5];
                    t3 = B[y][i + 6];
                    t4 = B[y][i + 7];

                    B[y][i + 4] = A[i + 4][y];
                    B[y][i + 5] = A[i + 5][y];
                    B[y][i + 6] = A[i + 6][y];
                    B[y][i + 7] = A[i + 7][y];

                    B[y + 4][i] = t1;
                    B[y + 4][i + 1] = t2;
                    B[y + 4][i + 2] = t3;
                    B[y + 4][i + 3] = t4;
                }

                for (y = j + 4; y < j + 8; ++y)
                {

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
    }
    int tx, ty;
    for (ty = 0; ty < j; ++ty)
    {
        for (tx = i; tx < N; ++tx)
        {
            t1 = A[tx][ty];
            B[ty][tx] = t1;
        }
    }
    for (ty = j; ty < M; ++ty)
    {
        for (tx = 0; tx < N; ++tx)
        {
            t1 = A[tx][ty];
            B[ty][tx] = t1;
        }
    }
}

char trans6167_desc[] = "61*67";
void trans6167(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, y, t1, t2, t3, t4;
    for (j = 0; j + 8 <= 56; j += 8)
    {
        for (i = 0; i + 8 <= 64; i += 8)
        {
            if (i == j && i + 16 < 64 && j + 16 < 56)
            {
                // diagnal line except the last diagnal block

                // using the next block and next next block
                // upper-left quarter
                for (y = j; y < j + 4; ++y)
                {

                    t1 = A[i][y];
                    t2 = A[i + 1][y];
                    t3 = A[i + 2][y];
                    t4 = A[i + 3][y];

                    B[y][i + 8] = t1;
                    B[y][i + 9] = t2;
                    B[y][i + 10] = t3;
                    B[y][i + 11] = t4;
                }
                // upper-right quarter
                for (y = j + 4; y < j + 8; ++y)
                {

                    t1 = A[i][y];
                    t2 = A[i + 1][y];
                    t3 = A[i + 2][y];
                    t4 = A[i + 3][y];

                    B[y - 4][i + 16] = t1;
                    B[y - 4][i + 17] = t2;
                    B[y - 4][i + 18] = t3;
                    B[y - 4][i + 19] = t4;
                }

                // get back
                for (y = j; y < j + 4; ++y)
                {

                    t1 = B[y][i + 8];
                    t2 = B[y][i + 9];
                    t3 = B[y][i + 10];
                    t4 = B[y][i + 11];

                    B[y][i] = t1;
                    B[y][i + 1] = t2;
                    B[y][i + 2] = t3;
                    B[y][i + 3] = t4;
                }
                for (y = j + 4; y < j + 8; ++y)
                {

                    t1 = B[y - 4][i + 16];
                    t2 = B[y - 4][i + 17];
                    t3 = B[y - 4][i + 18];
                    t4 = B[y - 4][i + 19];

                    B[y][i] = t1;
                    B[y][i + 1] = t2;
                    B[y][i + 2] = t3;
                    B[y][i + 3] = t4;
                }

                // lower-left quarter
                for (y = j; y < j + 4; ++y)
                {

                    t1 = A[i + 4][y];
                    t2 = A[i + 5][y];
                    t3 = A[i + 6][y];
                    t4 = A[i + 7][y];

                    B[y][i + 12] = t1;
                    B[y][i + 13] = t2;
                    B[y][i + 14] = t3;
                    B[y][i + 15] = t4;
                }
                // lower-right quarter
                for (y = j + 4; y < j + 8; ++y)
                {

                    t1 = A[i + 4][y];
                    t2 = A[i + 5][y];
                    t3 = A[i + 6][y];
                    t4 = A[i + 7][y];

                    B[y - 4][i + 20] = t1;
                    B[y - 4][i + 21] = t2;
                    B[y - 4][i + 22] = t3;
                    B[y - 4][i + 23] = t4;
                }

                // get back
                for (y = j; y < j + 4; ++y)
                {

                    t1 = B[y][i + 12];
                    t2 = B[y][i + 13];
                    t3 = B[y][i + 14];
                    t4 = B[y][i + 15];

                    B[y][i + 4] = t1;
                    B[y][i + 5] = t2;
                    B[y][i + 6] = t3;
                    B[y][i + 7] = t4;
                }
                for (y = j + 4; y < j + 8; ++y)
                {

                    t1 = B[y - 4][i + 20];
                    t2 = B[y - 4][i + 21];
                    t3 = B[y - 4][i + 22];
                    t4 = B[y - 4][i + 23];

                    B[y][i + 4] = t1;
                    B[y][i + 5] = t2;
                    B[y][i + 6] = t3;
                    B[y][i + 7] = t4;
                }
            }
            else
            {
                for (y = j; y < j + 4; ++y)
                {

                    t1 = A[i][y];
                    t2 = A[i + 1][y];
                    t3 = A[i + 2][y];
                    t4 = A[i + 3][y];

                    B[y][i] = t1;
                    B[y][i + 1] = t2;
                    B[y][i + 2] = t3;
                    B[y][i + 3] = t4;
                }

                for (y = j + 4; y < j + 8; ++y)
                {

                    t1 = A[i][y];
                    t2 = A[i + 1][y];
                    t3 = A[i + 2][y];
                    t4 = A[i + 3][y];

                    B[y - 4][i + 4] = t1;
                    B[y - 4][i + 5] = t2;
                    B[y - 4][i + 6] = t3;
                    B[y - 4][i + 7] = t4;
                }

                for (y = j; y < j + 4; ++y)
                {
                    t1 = B[y][i + 4];
                    t2 = B[y][i + 5];
                    t3 = B[y][i + 6];
                    t4 = B[y][i + 7];

                    B[y][i + 4] = A[i + 4][y];
                    B[y][i + 5] = A[i + 5][y];
                    B[y][i + 6] = A[i + 6][y];
                    B[y][i + 7] = A[i + 7][y];

                    B[y + 4][i] = t1;
                    B[y + 4][i + 1] = t2;
                    B[y + 4][i + 2] = t3;
                    B[y + 4][i + 3] = t4;
                }

                for (y = j + 4; y < j + 8; ++y)
                {

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
    }
    // i = 56 -> 67, j = 56 -> 61
    for (j = 0; j + 8 <= M; j += 8)
    {
        for (y = j; y < j + 8; ++y)
        {
            t1 = A[64][y];
            t2 = A[65][y];
            t3 = A[66][y];

            B[y][64] = t1;
            B[y][65] = t2;
            B[y][66] = t3;
        }
    }

    for (i = 0; i + 8 <= N; i += 8)
    {
        for (y = 56; y < M; ++y)
        {
            t1 = A[i][y];
            t2 = A[i + 1][y];
            t3 = A[i + 2][y];
            t4 = A[i + 3][y];

            B[y][i] = t1;
            B[y][i + 1] = t2;
            B[y][i + 2] = t3;
            B[y][i + 3] = t4;
        }
        for (y = 56; y < M; ++y)
        {
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

    // i = 64, j = 56
    for (i = 64; i < N; ++i)
    {
        for (j = 56; j < M; ++j)
        {
            t1 = A[i][j];
            B[j][i] = t1;
        }
    }
}

char transBlockopen32Diagnal_desc[] = "Blockopen32Diagnal transpose";
void transBlockopen32Diagnal(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, y, t1, t2, t3, t4, t5, t6, t7, t8;
    for (j = 0; j + 8 <= M; j += 8)
    {
        for (i = 0; i + 8 <= N; i += 8)
        {
            if (i == j && i + 8 < N)
            {
                for (y = j; y < j + 8; ++y)
                {

                    t1 = A[i][y];
                    t2 = A[i + 1][y];
                    t3 = A[i + 2][y];
                    t4 = A[i + 3][y];
                    t5 = A[i + 4][y];
                    t6 = A[i + 5][y];
                    t7 = A[i + 6][y];
                    t8 = A[i + 7][y];

                    B[y][i + 8] = t1;
                    B[y][i + 9] = t2;
                    B[y][i + 10] = t3;
                    B[y][i + 11] = t4;
                    B[y][i + 12] = t5;
                    B[y][i + 13] = t6;
                    B[y][i + 14] = t7;
                    B[y][i + 15] = t8;
                }
                for (y = j; y < j + 8; ++y)
                {

                    t1 = B[y][i + 8];
                    t2 = B[y][i + 9];
                    t3 = B[y][i + 10];
                    t4 = B[y][i + 11];
                    t5 = B[y][i + 12];
                    t6 = B[y][i + 13];
                    t7 = B[y][i + 14];
                    t8 = B[y][i + 15];

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
            else
            {
                for (y = j; y < j + 8; ++y)
                {

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
    }
}

/*
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++)
    {
        for (j = 0; j < M; j++)
        {
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
    registerTransFunction(trans6167, trans6167_desc);

    // registerTransFunction(trans, trans_desc);
    // registerTransFunction(transBlockopen, transBlockopen_desc);
    // registerTransFunction(transBlockopen64, transBlockopen64_desc);
    // registerTransFunction(transBlockopen64Diagnal, transBlockopen64Diagnal_desc);
    // registerTransFunction(transBlockopen32Diagnal, transBlockopen32Diagnal_desc);
    // registerTransFunction(transBlockopen64DiagnalS4, transBlockopen64DiagnalS4_desc);
}

/*
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++)
    {
        for (j = 0; j < M; ++j)
        {
            if (A[i][j] != B[j][i])
            {
                return 0;
            }
        }
    }
    return 1;
}
