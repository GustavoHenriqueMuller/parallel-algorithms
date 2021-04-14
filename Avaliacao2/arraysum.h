#ifndef ARRAYSUM_H
#define ARRAYSUM_H

#include <math.h>

namespace arraysum {
    extern const long size;
    extern const int threadNum;

    void init();
    double serial();
    double omp();
    double pthreads();
    void* pthreadsInner(void* arg);
    void times();
}

#endif // VECTORSUM_H
