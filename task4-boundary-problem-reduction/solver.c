
#include "solver.h"

GArray* solve_model(model_s *model, int num_processes)
{
    GArray *result = g_array_new(FALSE, TRUE, sizeof(double));
    g_array_set_size(result, model->n + 1);
    return result;
}
