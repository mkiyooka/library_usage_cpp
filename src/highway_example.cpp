#include <cmath>
#include <iostream>
#include <vector>

#include <hwy/highway.h>

namespace hn = hwy::HWY_NAMESPACE;

static void ScaleAdd(const float *input, float scale, float offset, float *output, size_t n) {
    const hn::ScalableTag<float> d;
    const size_t step = hn::Lanes(d);

    const auto vscale = hn::Set(d, scale);
    const auto voffset = hn::Set(d, offset);

    size_t i = 0;
    for (; i + step <= n; i += step) {
        auto v = hn::LoadU(d, input + i);
        auto r = hn::MulAdd(v, vscale, voffset);
        hn::StoreU(r, d, output + i);
    }
    for (; i < n; ++i) {
        output[i] = input[i] * scale + offset;
    }
}

static float DotProduct(const float *a, const float *b, size_t n) {
    const hn::ScalableTag<float> d;
    const size_t step = hn::Lanes(d);

    auto vsum = hn::Zero(d);
    size_t i = 0;
    for (; i + step <= n; i += step) {
        auto va = hn::LoadU(d, a + i);
        auto vb = hn::LoadU(d, b + i);
        vsum = hn::MulAdd(va, vb, vsum);
    }
    float result = hn::ReduceSum(d, vsum);
    for (; i < n; ++i) {
        result += a[i] * b[i];
    }
    return result;
}

int main() {
    std::cout << "=== Highway SIMD Example ===\n";

    constexpr size_t N = 8;
    float input[N] = {1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f};
    float output[N] = {};

    ScaleAdd(input, 2.0f, 1.0f, output, N);
    std::cout << "input * 2 + 1:\n";
    for (size_t i = 0; i < N; ++i) {
        std::cout << "  " << input[i] << " -> " << output[i] << "\n";
    }

    float a[N] = {1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f};
    float b[N] = {8.f, 7.f, 6.f, 5.f, 4.f, 3.f, 2.f, 1.f};
    float dot = DotProduct(a, b, N);
    std::cout << "\ndot(a, b) = " << dot << "  (expected: 120)\n";

    return 0;
}
