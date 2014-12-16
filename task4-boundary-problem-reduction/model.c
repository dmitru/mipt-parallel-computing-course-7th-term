
#include "includes.h"
#include "model.h"

#define g_array_get(a, i) g_array_index((a), double, (i))
#define g_array_set(a, i, v) {\
    double t = (v);\
    g_array_index((a), double, (i)) = t;\
}
#define g_array_ref(a, i) g_array_index((a), double, (i))

model_s* model_create_with_params(model_create_params_s params)
{
#ifdef DEBUG
    model_params_print(params);
#endif 
    model_s *model = malloc(sizeof(model_s));

    int n = params.n;
    model->n = n; 
    model->a = g_array_new(FALSE, FALSE, sizeof(double));
    g_array_set_size(model->a, n + 1);
    model->b = g_array_new(FALSE, FALSE, sizeof(double));
    g_array_set_size(model->b, n + 1);
    model->c = g_array_new(FALSE, FALSE, sizeof(double));
    g_array_set_size(model->c, n + 1);
    model->f = g_array_new(FALSE, FALSE, sizeof(double));
    g_array_set_size(model->f, n + 1);
   
    g_array_ref(model->a, 0) = 1.0;
    g_array_ref(model->a, n) = 1.0;

    g_array_ref(model->f, 0) = params.a;
    g_array_ref(model->f, n) = params.b;

    double interval = params.end - params.start;
    double h = interval / n;

#pragma omp parallel for
    for (int i = 1; i < n; ++i) {
        g_array_ref(model->a, i) = -2.0;
        g_array_ref(model->b, i) = 1.0;
        g_array_ref(model->c, i) = 1.0;

        double x_prev = params.start + h * (i - 1);
        double x_curr = params.start + h * i;
        double x_next = params.start + h * (i + 1);

        double f_prev = (*params.f)(x_prev);
        double f_curr = (*params.f)(x_curr);
        double f_next = (*params.f)(x_next);
        
        g_array_ref(model->f, i) = h*h / 12.0 * (f_prev + f_next + 10.0 * f_curr);
    }

#ifdef DEBUG
    if (n < 100) {
        fprintf(stderr, "f:\n");
        for (int i = 0; i <= n; ++i) {
            fprintf(stderr, "%.6lf, ", g_array_get(model->f, i));
        }
        fprintf(stderr, "\n");
    }
#endif

    return model;
}

void model_free(model_s *model) 
{
    g_array_free(model->a, TRUE);
    g_array_free(model->b, TRUE);
    g_array_free(model->c, TRUE);
    g_array_free(model->f, TRUE);
    free(model);
}

void model_params_print(model_create_params_s params)
{
    fprintf(stderr, "params: \n");
    fprintf(stderr, "\tn = %u\n", (unsigned) params.n);
    fprintf(stderr, "\ta = %.6lf\n\tb = %.6lf\n\tstart = %.6lf\n\tend = %.6lf\n", 
            params.a, params.b, params.start, params.end);
}


double f_exp(double y)
{
    return exp(-y);
}

