
#pragma once

#include "includes.h"
#include <math.h>

#define g_array_get(a, i) g_array_index((a), double, (i))
#define g_array_set(a, i, v) {\
    double t = (v);\
    g_array_index((a), double, (i)) = t;\
}
#define g_array_ref(a, i) g_array_index((a), double, (i))

#ifdef DEBUG
#define debug_print_array(a) {\
    if (n < 100) {\
        fprintf(stderr, #a":\n");\
        for (int i = 0; i <= n; ++i) {\
            fprintf(stderr, "%.6lf, ", g_array_get((a), i));\
        }\
        fprintf(stderr, "\n");\
    }\
}
#else
#define debug_print_array(a) 
#endif

/**
 * Describes linear system used for solution of boundary-value 
 * problem of the form:
 *   y'' = f(y),
 *   x in interval [start, end]
 *   y(start) = y_start,
 *   y(end)   = y_end
 *
 * a, b, c - arrays of length (n + 1) that hold 
 * three diagonals of the system's matrix
 * f - array of length (n + 1) holding the right-hand side 
 * of the system
 * */
typedef struct {
    GArray *a, *b, *c;
    GArray *f;
    size_t n;
    double y_start, y_end;
} model_s;

typedef double (*model_function_p)(double);

/**
 * a = f(start),
 * b = f(end),
 * start, end - interval of interest,
 * n - interval is divided into (n + 1) points;
 *      for correct parallelization, n should be equal
 *      2^k for some k
 * f - function f in the rhs of the equation (see comment above model_s)
 */
typedef struct {
    double a, b;
    double start, end;
    int n;
    model_function_p f;
} model_create_params_s;

void model_params_print(model_create_params_s params);

/**
 * Constructs model given model params
 */
model_s* model_create_with_params(model_create_params_s params);

#define model_create(...) model_create_with_params(\
        (model_create_params_s) {\
            .a = 0.0, \
            .b = 0.0, \
            .start = 0.0, \
            .end = 1.0, \
            .n = 1024, \
            .f = NULL, \
            __VA_ARGS__\
        })

void model_free(model_s *model);

