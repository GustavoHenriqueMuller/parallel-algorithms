#include <iostream>
#include <omp.h>
#include <pthread.h>
#include "arraysum.h"

using namespace std;

namespace arraysum {
    pthread_mutex_t em = PTHREAD_MUTEX_INITIALIZER;
    const long size = 160000000;
    const int threadNum = 4;
    double a[size];
    double res;
}

void arraysum::init() {
    // Initializes array
    for(long i = 0; i < size; i++) {
        a[i] = i;
    }

    // Initializes output
    res = 0;
}

double arraysum::serial() {
    double start = omp_get_wtime();
    init();

    // Sums
    for(long i = 0; i < size; i++) {
        res += a[i];
    }

    return omp_get_wtime() - start;
}

double arraysum::omp() {
    double start = omp_get_wtime();
    init();

    // Sums
    #pragma omp parallel for num_threads(threadNum) reduction (+:res)
    for(long i = 0; i < size; i++) {
        res += a[i];
    }

    return omp_get_wtime() - start;
}

double arraysum::pthreads() {
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

void* arraysum::pthreadsInner(void* arg) {
    int id = *((int*)arg);
    long blocksize = size/threadNum;
    long start = id * blocksize;
    long end = start + blocksize;
    long remainder = size - blocksize * threadNum;
    double localSum = 0;

    // Deallocates buffer created in for loop
    delete arg;

    // Sums
    for(long i = start; i < end; i++) {
        localSum += a[i];
    }

    // Adds local sum to output
    pthread_mutex_lock(&em);
    res += localSum;
    pthread_mutex_unlock(&em);

    // Deals with remaining positions
    if(id == 0) {
        while(remainder > 0) {
            start = (size - remainder);

            for(long i = start; i < size; i++) {
                res += a[i];
            }

            remainder--;
        }
    }
}

void arraysum::times() {
    // Prints times on all execution modes
    double serialTime = serial();
    double ompTime = omp();
    double pthreadsTime = pthreads();

    cout << "_____________________________________________________________________" << endl << endl;
    cout << "ARRAY SUM (" << size << " elements, " << threadNum << " threads)" << endl;
    cout << "Serial: " << serialTime << "s" << endl;
    cout << "OMP: " << ompTime << "s" << endl;
    cout << "Pthreads: " << pthreadsTime << "s" << endl;
    cout << "_____________________________________________________________________" << endl;
}
