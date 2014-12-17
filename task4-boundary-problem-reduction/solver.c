
#include "solver.h"
#include "model.h"
#include "includes.h"

#define ref(a, i) g_array_ref((a), (i))
#define swap(a, b) { void *t = (b); (b) = (a); (a) = t; }

#define NUM_OF_TASKS 10

GArray* solve_model(model_s *model, int num_processes)
{
    const int n = model->n;

    GArray *x = g_array_new(FALSE, TRUE, sizeof(double));
    g_array_set_size(x, n + 1);

    const int depth = ceil(log2(n));

    GArray *a_buffers[depth],
           *b_buffers[depth], 
           *c_buffers[depth],
           *f_buffers[depth];
    for (int level = 0; level < depth; ++level) {
        a_buffers[level] = g_array_new(FALSE, FALSE, sizeof(double));
        g_array_set_size(a_buffers[level], n + 1);
        b_buffers[level] = g_array_new(FALSE, FALSE, sizeof(double));
        g_array_set_size(b_buffers[level], n + 1);
        c_buffers[level] = g_array_new(FALSE, FALSE, sizeof(double));
        g_array_set_size(c_buffers[level], n + 1);
        f_buffers[level] = g_array_new(FALSE, FALSE, sizeof(double));
        g_array_set_size(f_buffers[level], n + 1);
    }

    GArray **a = a_buffers,
           **b = b_buffers,
           **c = c_buffers,
           **f = f_buffers;

    // 1. Copy the arrays a, b, c, f from model to local arrays a, b, c, f
    for (int i = 0; i <= n; i++) {
        g_array_ref(*a, i) = g_array_ref(model->a, i);
        g_array_ref(*b, i) = g_array_ref(model->b, i);
        g_array_ref(*c, i) = g_array_ref(model->c, i);
        g_array_ref(*f, i) = g_array_ref(model->f, i);
    }

    // 2. Set boundary values for the solution
    ref(x, 0) = model->y_start;
    ref(x, n) = model->y_end;
    
    debug_print_array(*a);
    debug_print_array(*b);
    debug_print_array(*c);
    debug_print_array(*f);


    // 3. Run forward steps of the algorithm
    int number_of_unknowns = n - 1;
    int level = 0;
    int stride = 1;

    #pragma omp parallel 
    while (number_of_unknowns > 1) {

        #ifdef DEBUG
            #pragma omp master
            fprintf(stderr, "forward step %d, stride is %d, number of unknowns %d\n", level, stride, number_of_unknowns);
        #endif

        // TODO: for-loop that should be parallelized
        const int loop_stride = stride * 2;
        #pragma omp for 
        for (int i = stride * 2; i < n; i += loop_stride) {
            #ifdef DEBUG
                #pragma omp master
                fprintf(stderr, "\ti = %d\n", i);
            #endif
            for (int k = 0; k < NUM_OF_TASKS; ++k) {
                double alpha = -ref(*b, i) / ref(*a, i - stride);
                double beta =  -ref(*c, i) / ref(*a, i + stride);

                double new_b = alpha * ref(*b, i - stride);
                double new_c =  beta * ref(*c, i + stride);
                double new_a = alpha * ref(*c, i - stride) + ref(*a, i) + beta * ref(*b, i + stride);
                double new_f = alpha * ref(*f, i - stride) + ref(*f, i) + beta * ref(*f, i + stride);

                ref(*(a + 1), i) = new_a;
                ref(*(b + 1), i) = new_b;
                ref(*(c + 1), i) = new_c;
                ref(*(f + 1), i) = new_f;
            }

            //double t = 0.0;
            //for (int k = 0; k < 10000; k++) {
            //    t = sin(k * i);
            //}
        }

        #pragma omp master
        {
            stride *= 2;
            level++;
            number_of_unknowns = (number_of_unknowns + 1) / 2 - 1;
            
            a++, b++, c++, f++;
            
            debug_print_array(*a);
            debug_print_array(*b);
            debug_print_array(*c);
            debug_print_array(*f);
        }

        #pragma omp barrier
    }

    // 4. Run backward steps of the algorithm
    level--;

    #pragma omp parallel 
    while (stride >= 1) {
        #ifdef DEBUG
            #pragma omp master
            fprintf(stderr, "backward step %d, stride %d\n", level, stride);
        #endif

        // TODO: Parallelize this step
        const int loop_stride = 2 * stride;
        #pragma omp for
        for (int i = stride; i < n; i += loop_stride) {
            #ifdef DEBUG
                fprintf(stderr, "\ti = %d\n", i);
            #endif
            for (int k = 0; k < NUM_OF_TASKS; ++k) {
                double new_x = (ref(*f, i) - ref(*b, i) * ref(x, i - stride) - ref(*c, i) * ref(x, i + stride)) / ref(*a, i);
                ref(x, i) = new_x;
            }
        }

        #pragma omp master 
        {
            stride /= 2;
            level--;
            a--, b--, c--, f--;
        }
        #pragma omp barrier
    }

    // TODO: free buffers
    for (int i = 0; i < depth; ++i) {
        g_array_free(a_buffers[i], TRUE);
        g_array_free(b_buffers[i], TRUE);
        g_array_free(c_buffers[i], TRUE);
        g_array_free(f_buffers[i], TRUE);
    }

    return x;
}
