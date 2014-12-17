
#include "includes.h"
#include "model.h"
#include "solver.h"

void print_results(GArray *y, model_create_params_s params)
{
    double interval = params.end - params.start;
    double h = interval / params.n;
    for (int i = 0; i <= params.n; ++i) {
        double x = i * h;
        double v = g_array_get(y, i);
        printf("%.6lf %.6lf\n", x, v);
    }
}

double f_exp(double x) 
{
    return exp(x);
}

int main(int argc, char **argv) 
{
    int num_processes = 1;
    if (argc > 1) {
        num_processes = atoi(argv[1]);
    }

    model_create_params_s model_params = {
        .a = 1.0,
        .b = 1.0,
        .start = 0.0,
        .end = 1.0,
        .n = 8,
        .f = f_exp
    };
    model_s *model = model_create_with_params(model_params);
    GArray *y = solve_model(model, num_processes);

    print_results(y, model_params);

    g_array_free(y, TRUE);
    model_free(model);
}
