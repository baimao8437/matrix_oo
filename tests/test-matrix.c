#include "matrix.h"
#include "stopwatch.h"
#include <stdio.h>

#define TIME_UNIT "ms"

int main()
{
    watch_p timer = Stopwatch.create(TIME_UNIT);
    Matrix dst, m, n, fixed;

    MatrixProvider.assign(&m, (Mat4x4) {
        .values = {
            { 1, 2, 3, 4, },
            { 5, 6, 7, 8, },
            { 1, 2, 3, 4, },
            { 5, 6, 7, 8, },
        },
    });

    MatrixProvider.assign(&n, (Mat4x4) {
        .values = {
            { 1, 2, 3, 4, },
            { 5, 6, 7, 8, },
            { 1, 2, 3, 4, },
            { 5, 6, 7, 8, },
        },
    });

    Stopwatch.start(timer);
    MatrixProvider.mul(&dst, &m, &n);
    Stopwatch.stop(timer);

    MatrixProvider.assign(&fixed, (Mat4x4) {
        .values = {
            { 34,  44,  54,  64, },
            { 82, 108, 134, 160, },
            { 34,  44,  54,  64, },
            { 82, 108, 134, 160, },
        },
    });

    if (MatrixProvider.equal(&dst, &fixed)) {
        printf("time: %lf %s\n", Stopwatch.read(timer), TIME_UNIT);
        return 0;
    }
    return -1;
}
