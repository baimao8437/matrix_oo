#ifndef MATRIX_H_
#define MATRIX_H_

#include <stdbool.h>

/* predefined shortcut */
#define DECLARE_MATRIX(col, row) \
    typedef struct { float values[col][row]; } Mat ## col ## x ## row
DECLARE_MATRIX(3, 3);
DECLARE_MATRIX(4, 4);

typedef struct {
    int row, col;
    void *priv;
} Matrix;

typedef struct {
    const char *name;
    void (*assign)(Matrix *thiz, Mat4x4);
    bool (*equal)(const Matrix *l, const Matrix *r);
    bool (*mul)(Matrix *dst, const Matrix *l, const Matrix *r);
} MatrixAlgo;

#define REGISTER_ALGO(nameX)\
	MatrixAlgo MatrixAlgo_##nameX __attribute__((section("MatrixAlgo"))) = { \
		.name = #nameX, .assign = assign, .equal = equal, .mul = mul, \
	};

/* Available matrix providers */
extern MatrixAlgo __start_MatrixAlgo[], __stop_MatrixAlgo[];

#define MUL_IMPL_BEGIN __start_MatrixAlgo
#define MUL_IMPL_END __stop_MatrixAlgo

#endif /* MATRIX_H_ */
