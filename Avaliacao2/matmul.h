#ifndef MATMUL_H
#define MATMUL_H

#include <math.h>

namespace matmul {
    extern const long size;
    extern const int threadNum;
    extern const long aRows, aCols;
    extern const long bRows, bCols;
    extern const long rRows, rCols;

    void init();
    double serial();
    double omp();
    double pthreads();
    void* pthreadsInner(void* arg);
    void times();
}

#endif // MATMUL_H
