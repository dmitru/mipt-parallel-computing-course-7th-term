
#include "solver.h"
#include "model.h"
#include "includes.h"

#define ref(a, i) g_array_ref((a), (i))
#define swap(a, b) { void *t = (b); (b) = (a); (a) = t; }

GArray* solve_model(model_s *model, int num_processes)
{
    int n = model->n;

    GArray *x = g_array_new(FALSE, TRUE, sizeof(double));
    g_array_set_size(x, n + 1);
    
    GArray *a_buffers[2], *b_buffers[2], *c_buffers[2], *f_buffers[2];
    for (int i = 0; i < 2; ++i) {
        a_buffers[i] = g_array_new(FALSE, TRUE, sizeof(double));
        g_array_set_size(a_buffers[i], n + 1);
        b_buffers[i] = g_array_new(FALSE, TRUE, sizeof(double));
        g_array_set_size(b_buffers[i], n + 1);
        c_buffers[i] = g_array_new(FALSE, TRUE, sizeof(double));
        g_array_set_size(c_buffers[i], n + 1);
        f_buffers[i] = g_array_new(FALSE, TRUE, sizeof(double));
        g_array_set_size(f_buffers[i], n + 1);
    }

    GArray *a = a_buffers[0],
           *b = b_buffers[0],
           *c = c_buffers[0],
           *f = f_buffers[0];

    GArray *a_next = a_buffers[1],
           *b_next = b_buffers[1],
           *c_next = c_buffers[1],
           *f_next = f_buffers[1];

    // 1. Copy the arrays a, b, c, f from model to local arrays a, b, c, f
    for (int i = 0; i <= n; i++) {
        g_array_ref(a, i) = g_array_ref(model->a, i);
        g_array_ref(b, i) = g_array_ref(model->b, i);
        g_array_ref(c, i) = g_array_ref(model->c, i);
        g_array_ref(f, i) = g_array_ref(model->f, i);
        g_array_ref(a_next, i) = g_array_ref(model->a, i);
        g_array_ref(b_next, i) = g_array_ref(model->b, i);
        g_array_ref(c_next, i) = g_array_ref(model->c, i);
        g_array_ref(f_next, i) = g_array_ref(model->f, i);
    }

    // 2. Set boundary values for the solution
    ref(x, 0) = model->y_start;
    ref(x, n) = model->y_end;
    
    debug_print_array(a);
    debug_print_array(b);
    debug_print_array(c);
    debug_print_array(f);


    // 3. Run forward steps of the algorithm
    int number_of_unknowns = n - 1;
    int level = 1;
    int stride = 1;
    while (number_of_unknowns > 1) {

        #ifdef DEBUG
            fprintf(stderr, "forward step %d, stride is %d, number of unknowns %d\n", level, stride * 2, number_of_unknowns);
        #endif

        // TODO: stride that should be parallelized
        for (int i = stride * 2; i < n; i += stride * 2) {
            #ifdef DEBUG
                fprintf(stderr, "\ti = %d\n", i);
            #endif
            double alpha = -ref(a, i) / ref(c, i - stride);
            double beta =  -ref(b, i) / ref(c, i + stride);

            double new_a = alpha * ref(a, i - stride);
            double new_b =  beta * ref(b, i + stride);
            double new_c = alpha * ref(b, i - stride) + ref(c, i) + beta * ref(a, i + stride);
            double new_f = alpha * ref(f, i - stride) + ref(f, i) + beta * ref(f, i + stride);

            ref(a_next, i) = new_a;
            ref(b_next, i) = new_b;
            ref(c_next, i) = new_c;
            ref(f_next, i) = new_f;
        }

        stride *= 2;
        level++;
        number_of_unknowns = (number_of_unknowns + 1) / 2 - 1;

        swap(a, a_next);
        swap(b, b_next);
        swap(c, c_next);
        swap(f, f_next);
    }

    debug_print_array(a);
    debug_print_array(b);
    debug_print_array(c);
    debug_print_array(f);

    //swap(a, a_next);
    //swap(b, b_next);
    //swap(c, c_next);
    //swap(f, f_next);

    // 4. Run backward steps of the algorithm
    level--;
    while (stride >= 1) {
        #ifdef DEBUG
            fprintf(stderr, "backward step %d, stride %d\n", level, stride);
        #endif

        // Parallelize this step
        for (int i = stride; i < n; i += 2 * stride) {
            #ifdef DEBUG
                fprintf(stderr, "\ti = %d\n", i);
            #endif
            double new_x = (ref(f, i) - ref(a, i) * ref(x, i - stride) - ref(b, i) * ref(x, i + stride)) / ref(c, i);
            ref(x, i) = new_x;
        }
        stride /= 2;
        level--;
    }

    // TODO: free buffers

    return x;
}
