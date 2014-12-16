
#include "includes.h"
#include "model.h"

model_s* model_create_with_params(model_create_params_s params)
{
#ifdef DEBUG
    model_params_print(params);
#endif 
    model_s *model = malloc(sizeof(model_s));

    model->n = params.n;
    model->a = g_array_new(FALSE, TRUE, sizeof(double));
    g_array_set_size(model->a, params.n + 1);
    
    model->b = g_array_new(FALSE, TRUE, sizeof(double));
    g_array_set_size(model->b, params.n + 1);
    
    model->c = g_array_new(FALSE, TRUE, sizeof(double));
    g_array_set_size(model->c, params.n + 1);
    
    model->f = g_array_new(FALSE, TRUE, sizeof(double));
    g_array_set_size(model->f, params.n + 1);

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

