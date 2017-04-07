#include "matrix.h"
#include "stopwatch.h"
#include <stdio.h>

#define TIME_UNIT "ms"

int main()
{
    FILE *output = fopen("time.txt", "w+a");
    Matrix dst, m, n, fixed;
    watch_p timer = Stopwatch.create(TIME_UNIT);

    for (MatrixAlgo *p = MUL_IMPL_BEGIN; p < MUL_IMPL_END; p++) {
        fprintf(output, "%s ", p->name);

        p->assign(&m, (Mat4x4) {
            .values = {
                { 1, 2, 3, 4, },
                { 5, 6, 7, 8, },
                { 1, 2, 3, 4, },
                { 5, 6, 7, 8, },
            },
        });

        p->assign(&n, (Mat4x4) {
            .values = {
                { 1, 2, 3, 4, },
                { 5, 6, 7, 8, },
                { 1, 2, 3, 4, },
                { 5, 6, 7, 8, },
            },
        });
        for (int i = 0; i < REPEAT ; i++) {
            Stopwatch.restart(timer);
            p->mul(&dst, &m, &n);
            Stopwatch.stop(timer);

            p->assign(&fixed, (Mat4x4) {
                .values = {
                    { 34,  44,  54,  64, },
                    { 82, 108, 134, 160, },
                    { 34,  44,  54,  64, },
                    { 82, 108, 134, 160, },
                },
            });

            if (p->equal(&dst, &fixed)) {
                fprintf(output, "%lf ", Stopwatch.read(timer));
            }
        }
        fprintf(output, "\n");
    }
    Stopwatch.destroy(timer);
    fclose(output);
    return 0;
}
