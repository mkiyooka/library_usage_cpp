#include <cmath>
#include <iostream>
#include <vector>

#include <xsimd/xsimd.hpp>

static void BasicSIMD() {
    std::cout << "=== Basic SIMD Operations ===\n";
    std::cout << "SIMD batch size (double): " << xsimd::batch<double>::size << "\n\n";

    using batch_t = xsimd::batch<double>;
    constexpr std::size_t N = batch_t::size;

    alignas(batch_t::arch_type::alignment()) double a_arr[N], b_arr[N], r_arr[N];
    for (std::size_t i = 0; i < N; ++i) {
        a_arr[i] = static_cast<double>(i + 1);
        b_arr[i] = static_cast<double>(N - i);
    }

    auto va = batch_t::load_aligned(a_arr);
    auto vb = batch_t::load_aligned(b_arr);
    auto vr = va * vb + va;
    vr.store_aligned(r_arr);

    std::cout << "a:       ";
    for (std::size_t i = 0; i < N; ++i) {
        std::cout << a_arr[i] << " ";
    }
    std::cout << "\nb:       ";
    for (std::size_t i = 0; i < N; ++i) {
        std::cout << b_arr[i] << " ";
    }
    std::cout << "\na*b+a:   ";
    for (std::size_t i = 0; i < N; ++i) {
        std::cout << r_arr[i] << " ";
    }
    std::cout << "\n";
}

static void VectorizedMath() {
    std::cout << "\n=== Vectorized Math (sin) ===\n";

    constexpr std::size_t N = 1024;
    std::vector<double> input(N), output(N);
    for (std::size_t i = 0; i < N; ++i) {
        input[i] = static_cast<double>(i) * 0.01;
    }

    using batch_t = xsimd::batch<double>;
    constexpr std::size_t step = batch_t::size;
    std::size_t i = 0;
    for (; i + step <= N; i += step) {
        auto v = batch_t::load_unaligned(&input[i]);
        auto r = xsimd::sin(v);
        r.store_unaligned(&output[i]);
    }
    for (; i < N; ++i) {
        output[i] = std::sin(input[i]);
    }

    std::cout << "sin(" << input[100] << ") = " << output[100] << "  (scalar: " << std::sin(input[100]) << ")\n";
}

int main() {
    BasicSIMD();
    VectorizedMath();
    return 0;
}
