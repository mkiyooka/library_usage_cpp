#include <cmath>
#include <iostream>
#include <vector>

#include <matplot/matplot.h>

int main() {
    std::cout << "=== Matplot++ Example ===\n";

    const size_t N = 200;
    auto x = matplot::linspace(0.0, 2.0 * matplot::pi, N);

    // sin / cos のプロット
    auto sin_y = matplot::transform(x, [](double v) { return std::sin(v); });
    auto cos_y = matplot::transform(x, [](double v) { return std::cos(v); });

    auto f = matplot::figure(true);
    f->size(800, 500);

    matplot::hold(matplot::on);
    matplot::plot(x, sin_y)->line_width(2).display_name("sin(x)");
    matplot::plot(x, cos_y)->line_width(2).line_style("--").display_name("cos(x)");
    matplot::hold(matplot::off);

    matplot::xlabel("x");
    matplot::ylabel("y");
    matplot::title("sin(x) and cos(x)");
    matplot::legend();
    matplot::grid(true);

    matplot::save("sin_cos.png");
    std::cout << "Saved: sin_cos.png\n";

    // ヒストグラム
    auto f2 = matplot::figure(true);
    f2->size(600, 400);

    // 正規分布サンプル（Box-Muller）
    std::vector<double> samples;
    samples.reserve(1000);
    for (size_t i = 0; i < 500; ++i) {
        double u1 = (static_cast<double>(i * 7 + 1) / 500.0);
        double u2 = (static_cast<double>(i * 13 + 1) / 500.0);
        double z0 = std::sqrt(-2.0 * std::log(u1)) * std::cos(2.0 * matplot::pi * u2);
        double z1 = std::sqrt(-2.0 * std::log(u1)) * std::sin(2.0 * matplot::pi * u2);
        samples.push_back(z0);
        samples.push_back(z1);
    }

    matplot::hist(samples, 30);
    matplot::title("Normal distribution histogram");
    matplot::xlabel("value");
    matplot::ylabel("count");
    matplot::save("histogram.png");
    std::cout << "Saved: histogram.png\n";

    return 0;
}
