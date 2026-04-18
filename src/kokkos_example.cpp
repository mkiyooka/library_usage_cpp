#include <cmath>
#include <iostream>

#include <Kokkos_Core.hpp>

int main(int argc, char *argv[]) {
    Kokkos::initialize(argc, argv);
    {
        std::cout << "=== Kokkos Example ===\n";
        std::cout << "Execution space: " << Kokkos::DefaultExecutionSpace::name() << "\n\n";

        constexpr int N = 1000;

        // 1D view (device array)
        Kokkos::View<double *> a("a", N);
        Kokkos::View<double *> b("b", N);
        Kokkos::View<double *> c("c", N);

        // 初期化: parallel_for
        Kokkos::parallel_for(
            "init_a", N, KOKKOS_LAMBDA(int i) {
                a(i) = static_cast<double>(i);
                b(i) = static_cast<double>(N - i);
            }
        );

        // 要素ごとの加算
        Kokkos::parallel_for("add", N, KOKKOS_LAMBDA(int i) { c(i) = a(i) + b(i); });

        // リダクション: sum
        double sum = 0.0;
        Kokkos::parallel_reduce("sum_c", N, KOKKOS_LAMBDA(int i, double &lsum) { lsum += c(i); }, sum);

        std::cout << "N = " << N << "\n";
        std::cout << "a[i] = i, b[i] = N-i, c[i] = a[i]+b[i] = N\n";
        std::cout << "sum(c) = " << sum << "  (expected: " << static_cast<double>(N) * N << ")\n";
    }
    Kokkos::finalize();
    return 0;
}
