#include <stdio.h>
#include <omp.h>
#include <math.h>
#include <time.h>
#include <errno.h>
#include <stdlib.h>

int msleep(long msec)
{
    struct timespec ts;
    int res;

    if (msec < 0)
    {
        errno = EINVAL;
        return -1;
    }

    ts.tv_sec = msec / 1000;
    ts.tv_nsec = (msec % 1000) * 1000000;

    do {
        res = nanosleep(&ts, &ts);
    } while (res && errno == EINTR);

    return res;
}

double epsilon = 0.00001;
double iteration_step = 0.0001;

double cpuSecond()
{
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    return ((double)ts.tv_sec + (double)ts.tv_nsec * 1.e-9);
}

void parallel_dot(double* A, double* b, double* c, int n, int m) {
    #pragma omp parallel
    {
    #pragma omp for
    for (int i = 0; i < n; i++) {
        c[i] = 0;
        for (int j = 0; j < m; j++)
            c[i] += A[i * n + j] * b[j];
    }
    } 
}

void serial_dot(double* A, double* b, double* c, int n, int m) {
    for (int i = 0; i < n; i++) {
        c[i] = 0;
        for (int j = 0; j < m; j++)
            c[i] += A[i * n + j] * b[j];
    }
}

double vector_length(double* vec, int n) {
    double vec_length_sum = 0;
    for (int i = 0; i < n; i++)
        vec_length_sum += vec[i] * vec[i];
    return sqrt(vec_length_sum);
}

void simple_iteration_method_first_realization(double* A, double* x, double* b, int n) {
    double* xn = (double*)malloc(sizeof(double) * n);
    double* x_offset = (double*)malloc(sizeof(double) * n);
    double convergence_coeff = 1;
    
    double b_length = vector_length(b, n);
    double t = cpuSecond();
    while (convergence_coeff > epsilon) {
        parallel_dot(A, x, xn, n, n);
        double x_offset_length = 0;
        for (int i = 0; i < n; i++) {
            x_offset[i] = (xn[i] - b[i]);
            x[i] = x[i] - iteration_step * x_offset[i];
            x_offset_length += x_offset[i] * x_offset[i];
        }
        convergence_coeff = sqrt(x_offset_length) / b_length;
    }
    t = cpuSecond() - t;
    printf("%.12f\n", t);
    free(xn);
    free(x_offset);
}

void simple_iteration_method_second_realization(double* A, double* x, double* b, int n) {
    double* xn = (double*)malloc(sizeof(double) * n);
    double* x_offset = (double*)malloc(sizeof(double) * n);
    double* minimize_vector = (double*)malloc(sizeof(double) * n);
    double convergence_coeff = 1;
    double b_length = vector_length(b, n);
    double t = cpuSecond();
    while (convergence_coeff > epsilon) {
        //parallel_dot(A, x, xn, n, n);
        double x_offset_length = 0;
        #pragma omp parallel
        {
        #pragma omp for
        for (int i = 0; i < n; i++) {
            xn[i] = 0;
            for (int j = 0; j < n; j++)
                xn[i] += A[i * n + j] * x[j];
            x_offset[i] = (xn[i] - b[i]);
            minimize_vector[i] = iteration_step * x_offset[i];
            x_offset_length += x_offset[i] * x_offset[i];
        }
        }
        for (int i = 0; i < n; i++) 
            x[i] = x[i] - minimize_vector[i];
        
        convergence_coeff = sqrt(x_offset_length) / b_length;
        
    }
    t = cpuSecond() - t;
    printf("%.12f\n", t);
    free(xn);
    free(x_offset);
}

int main() {
    int n = 10000;
    double* A = (double*)malloc(sizeof(double) * n * n);
    double* b = (double*)malloc(sizeof(double) * n);
    double* x = (double*)malloc(sizeof(double) * n);
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n;j ++) {
            if (i == j) A[i * n + j] = 2.0;
            else A[i * n + j] = 1.0;
        }
        x[i] = 0;
    }
    for (int i = 0; i < n; i++)
        b[i] = n + 1;
    
    // for (int i = 0; i < n; i++) {
    //     for (int j = 0; j < n; j++) {
    //         printf("%lfx%d ", A[i*n+j], j + 1);
    //         if (j != n - 1)
    //             printf("+ ");
    //     }
    //     printf("= %lf\n", b[i]);
    // }
    simple_iteration_method_first_realization(A, x, b, n);
    for (int i = 0; i < n; i++) {
        x[i] = 0;
    }
    simple_iteration_method_second_realization(A, x, b, n);
    
    //for (int i = 0; i < n; i++)
    //    printf("x%d = %lf\n", i + 1, x[i]);
    
    return 0;
}