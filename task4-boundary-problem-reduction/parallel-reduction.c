
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
    return exp(-x);
}

int main(int argc, char **argv) 
{
    int num_processes = 1;
    if (argc > 2) {
        num_processes = atoi(argv[2]);
    }

    int number_of_points = 1024;
    if (argc > 1) {
        number_of_points = atoi(argv[1]);
    }

    double b = 1.0;
    if (argc > 3) {
        b = atof(argv[3]);
    }

    omp_set_num_threads(num_processes);

    model_create_params_s model_params = {
        .a = 1.0,
        .b = b,
        .start = 0.0,
        .end = 1.0,
        .n = number_of_points,
        .f = f_exp
    };
    model_s *model = model_create_with_params(model_params);

    double timeElapsedMs;
    GArray *y = NULL;
    MEASURE_TIME_MS(timeElapsedMs,
        y = solve_model(model, num_processes);
    );

    fprintf(stderr, "%.6f\n", timeElapsedMs);
    print_results(y, model_params);

    g_array_free(y, TRUE);
    model_free(model);
}
