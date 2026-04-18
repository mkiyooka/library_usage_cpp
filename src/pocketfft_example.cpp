#include <cmath>
#include <complex>
#include <iostream>
#include <vector>

#include <pocketfft_hdronly.h>

using namespace pocketfft;

static void FFTExample() {
    std::cout << "=== 1D FFT (Real -> Complex) ===\n";

    const size_t N = 8;
    std::vector<double> in(N);
    for (size_t i = 0; i < N; ++i) {
        in[i] = std::cos(2.0 * M_PI * static_cast<double>(i) / N);
    }

    std::vector<std::complex<double>> out(N / 2 + 1);
    shape_t shape{N};
    stride_t in_stride{sizeof(double)};
    stride_t out_stride{sizeof(std::complex<double>)};

    r2c(shape, in_stride, out_stride, {0}, FORWARD, in.data(), out.data(), 1.0);

    std::cout << "Input (cos wave, N=" << N << "):\n  ";
    for (double v : in) {
        std::cout << v << " ";
    }
    std::cout << "\nFFT output (magnitude):\n";
    for (size_t i = 0; i < out.size(); ++i) {
        std::cout << "  [" << i << "] " << std::abs(out[i]) << "\n";
    }
}

static void InverseFFT() {
    std::cout << "\n=== Round-trip FFT ===\n";

    const size_t N = 16;
    std::vector<double> original(N);
    for (size_t i = 0; i < N; ++i) {
        original[i] = static_cast<double>(i % 4);
    }

    std::vector<std::complex<double>> freq(N / 2 + 1);
    shape_t shape{N};
    stride_t rs{sizeof(double)};
    stride_t cs{sizeof(std::complex<double>)};

    r2c(shape, rs, cs, {0}, FORWARD, original.data(), freq.data(), 1.0);

    std::vector<double> recovered(N);
    c2r(shape, cs, rs, {0}, BACKWARD, freq.data(), recovered.data(), 1.0 / N);

    double max_err = 0.0;
    for (size_t i = 0; i < N; ++i) {
        max_err = std::max(max_err, std::abs(original[i] - recovered[i]));
    }
    std::cout << "Max round-trip error: " << max_err << "\n";
}

int main() {
    FFTExample();
    InverseFFT();
    return 0;
}
