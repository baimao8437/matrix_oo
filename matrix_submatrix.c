#include "matrix.h"
#include <stdlib.h>
#include <string.h>
#include <immintrin.h>

struct matrix_priv {
    int values[4][4];
};

#define PRIV(x) \
    ((struct matrix_priv *) ((x)->priv))

static void assign(Matrix *thiz, Mat4x4 data)
{
    /* FIXME: don't hardcode row & col */
    thiz->row = thiz->col = 4;

    thiz->priv = malloc(4 * 4 * sizeof(float));
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            PRIV(thiz)->values[i][j] = data.values[i][j];
}

static const float epsilon = 1 / 10000.0;

static bool equal(const Matrix *l, const Matrix *r)
{
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (PRIV(l)->values[i][j] + epsilon < PRIV(r)->values[i][j] ||
                    PRIV(r)->values[i][j] + epsilon < PRIV(l)->values[i][j])
                return false;
    return true;
}

static bool mul(Matrix *dst, const Matrix *l, const Matrix *r)
{
    dst->priv = malloc(4 * 4 * sizeof(float));
    int src1_h = 4;
    int src2_w = 4;
    int src2_h = 4;
    memset(PRIV(dst)->values, 0, sizeof(int) * src1_h * src2_w);

    for (int i = 0; i < src1_h; i += 4) {
        for (int j = 0; j < src2_w; j += 4) {
            for (int k = 0; k < src2_h; k += 4) {
                for (int i2 = 0; i2 < 4; ++i2) {
                    for (int j2 = 0; j2 < 4; ++j2) {
                        for (int k2 = 0; k2 < 4; ++k2) {
                            PRIV(dst)->values[i + i2][j + j2] += PRIV(l)->values[i + i2][k + k2] *
                                                                 PRIV(r)->values[k + k2][j + j2];
                        }
                    }
                }
            }
        }
    }
    return true;
}

REGISTER_ALGO(submatrix)
