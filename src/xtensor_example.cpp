#include <iostream>

#include <xtensor/xarray.hpp>
#include <xtensor/xio.hpp>
#include <xtensor/xmath.hpp>
#include <xtensor/xview.hpp>

static void BasicArrayOps() {
    std::cout << "=== Basic Array Operations ===\n";

    xt::xarray<double> a = {
        {1.0, 2.0, 3.0},
        {4.0, 5.0, 6.0}
    };
    xt::xarray<double> b = {
        {10.0, 20.0, 30.0},
        {40.0, 50.0, 60.0}
    };

    std::cout << "a =\n" << a << "\n";
    std::cout << "a + b =\n" << (a + b) << "\n";
    std::cout << "a * 2 =\n" << (a * 2.0) << "\n";
    std::cout << "sum(a) = " << xt::sum(a)() << "\n";
    std::cout << "mean(a) = " << xt::mean(a)() << "\n";
}

static void SlicingAndViews() {
    std::cout << "\n=== Slicing and Views ===\n";

    xt::xarray<int> arr = xt::arange<int>(12);
    arr.reshape({3, 4});
    std::cout << "arr (3x4):\n" << arr << "\n";

    auto row0 = xt::view(arr, 0, xt::all());
    std::cout << "row 0: " << row0 << "\n";

    auto col2 = xt::view(arr, xt::all(), 2);
    std::cout << "col 2: " << col2 << "\n";
}

static void MathOps() {
    std::cout << "\n=== Math Operations ===\n";

    xt::xarray<double> x = {0.0, 1.0, 2.0, 3.0};
    std::cout << "x = " << x << "\n";
    std::cout << "sin(x) = " << xt::sin(x) << "\n";
    std::cout << "exp(x) = " << xt::exp(x) << "\n";
    std::cout << "sqrt(x) = " << xt::sqrt(x) << "\n";
}

int main() {
    BasicArrayOps();
    SlicingAndViews();
    MathOps();
    return 0;
}
