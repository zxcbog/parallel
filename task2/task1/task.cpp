#include <iostream>
#include <omp.h>
#include <chrono>
#include <vector>

int thread_number = 0;

// linear product
void matrix_vector_product(std::vector<double>& a, std::vector<double>& b, std::vector<double>& c)
{
    for (int i = 0; i < c.size(); i++) {
        c[i] = 0.0;
        for (int j = 0; j < b.size(); j++)
            c[i] += a[i * b.size() + j] * b[j];
    }   
}
// parallel product
void matrix_vector_product_omp(std::vector<double>& a, std::vector<double>& b, std::vector<double>& c)
{
    #pragma omp parallel num_threads(thread_number)
    {
        int nthreads = omp_get_num_threads(); // number of threads
        int threadid = omp_get_thread_num(); // current thread id
        int items_per_thread = c.size() / nthreads; // number of lines that can be given to one thread
        int lb = threadid * items_per_thread; // the first line of current thread
        int ub = (threadid == nthreads - 1) ? (c.size() - 1) : (lb + items_per_thread - 1); // the last line of the current thread
        for (int i = lb; i <= ub; i++) {
            c[i] = 0.0;
            for (int j = 0; j < b.size(); j++)
                c[i] += a[i * b.size() + j] * b[j];
        }
    }
}

void run_tests(int m, int n, void (*func)(std::vector<double>&, std::vector<double>&, std::vector<double>&))
{
    std::vector<double> a, b, c;
    a = std::vector<double>(m * n); // matrix of m x n size
    b = std::vector<double>(n); // vector of n size
    c = std::vector<double>(m); // out vector of m size
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++)
            a[i * n + j] = i + j;
    }
    for (int j = 0; j < n; j++)
        b[j] = j;
    const auto start{std::chrono::steady_clock::now()};
    func(a, b, c);
    const auto end{std::chrono::steady_clock::now()};
    const std::chrono::duration<double> elapsed_seconds{end - start};
    std::cout << "Elapsed time: " << elapsed_seconds.count() << "sec.\n";
}

int main(int argc, char* argv[]) 
{
    if (argc != 4) {
        std::cout << "Wrong parameters! You need to pass the M and N sizes of matrix and number of threads!";
        return -1;
    }
    int m, n;
    m = std::stoi(argv[1]);
    n = std::stoi(argv[2]);
    thread_number = std::stoi(argv[3]);
    std::cout << "Matrix-vector product (c[m] = a[m, n] * b[n]; m = " << m << ", n = " << n << ")\n";
    std::cout << "Memory used: " << int64_t(((m * n + m + n) * sizeof(double)) >> 20) << " MiB\n";
    std::cout << "Linear product test:\n";
    run_tests(m, n, matrix_vector_product); // test linear product
    std::cout << "Parallel product test:\n";
    run_tests(m, n, matrix_vector_product_omp); // test parallel product
    return 0;
}