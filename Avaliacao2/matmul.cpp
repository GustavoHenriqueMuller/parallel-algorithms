#include <iostream>
#include <omp.h>
#include <pthread.h>
#include "matmul.h"

using namespace std;

namespace matmul {
    const long size = 2000;
    const int threadNum = 4;
    const long aRows = size, aCols = size;
    const long bRows = size, bCols = size;
    const long rRows = aRows, rCols = bCols;

    int a[aRows][aCols];
    int b[bRows][bCols];
    int res[rRows][rCols];
}

void matmul::init() {
    // Initializes A matrix
    for(long i = 0; i < aRows; i++) {
        for(long j = 0; j < aCols; j++) {
            a[i][j] = 1;
        }
    }

    // Initializes B matrix
    for(long i = 0; i < bRows; i++) {
        for(long j = 0; j < bCols; j++) {
            b[i][j] = 2;
        }
    }

    // Initializes output matrix
    for(long i = 0; i < rRows; i++) {
        for(long j = 0; j < rCols; j++) {
            res[i][j] = 0;
        }
    }
}

double matmul::serial() {
    double start = omp_get_wtime();
    init();

    // Multiplies AxB
    for(long i = 0; i < aRows; i++) {
        for(long j = 0; j < bCols; j++) {
            for(long k = 0; k < aCols; k++) {
                res[i][j] += a[i][k] * b[k][j];
            }
        }
    }

    return omp_get_wtime() - start;
}

double matmul::omp() {
    double start = omp_get_wtime();
    init();

    // Multiplies AxB
    #pragma omp parallel for num_threads(threadNum)
    for(long i = 0; i < aRows; i++) {
        for(long j = 0; j < bCols; j++) {
            for(long k = 0; k < aCols; k++) {
                res[i][j] += a[i][k] * b[k][j];
            }
        }
    }

    return omp_get_wtime() - start;
}

double matmul::pthreads() {
    double start = omp_get_wtime();
    init();

    // Creates and executes threads
    pthread_t threads[threadNum];

    for(int i = 0; i < threadNum; i++) {
        // Creates new buffer
        int* buffer = new int;
        *buffer = i;

        pthread_create(&(threads[i]), NULL, pthreadsInner, buffer);
    }

    // Joins all threads
    for(int i = 0; i < threadNum; i++) {
        pthread_join(threads[i], 0);
    }

    return omp_get_wtime() - start;
}

void* matmul::pthreadsInner(void* arg) {
    int id = *((int*)arg);
    long blocksize = aRows/threadNum;
    long start = id * blocksize;
    long end = start + blocksize;
    long remainder = size - blocksize * threadNum;

    // Deallocates buffer created in for loop
    delete arg;

    // Multiplies AxB
    for(long i = start; i < end; i++) {
        for(long j = 0; j < bCols; j++) {
            for(long k = 0; k < aCols; k++) {
                res[i][j] += a[i][k] * b[k][j];
            }
        }
    }

    // Deals with remaining positions
    if(id == 0) {
        while(remainder > 0) {
            start = (size - remainder);

            for(long i = start; i < aRows; i++) {
                for(long j = 0; j < bCols; j++) {
                    for(long k = 0; k < aCols; k++) {
                        res[i][j] += a[i][k] * b[k][j];
                    }
                }
            }

            remainder--;
        }
    }
}

void matmul::times() {
    // Prints times on all execution modes
    double serialTime = serial();
    double ompTime = omp();
    double pthreadsTime = pthreads();

    cout << "_____________________________________________________________________" << endl << endl;
    cout << "MATRIX MULTIPLICATION (" << size << "x" << size << " elements, " << threadNum << " threads)" << endl;
    cout << "Serial: " << serialTime << "s" << endl;
    cout << "OMP: " << ompTime << "s" << endl;
    cout << "Pthreads: " << pthreadsTime << "s" << endl;
    cout << "_____________________________________________________________________" << endl;
}
