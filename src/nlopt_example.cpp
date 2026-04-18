#include <cmath>
#include <iostream>
#include <vector>

#include <nlopt.hpp>

// 最小化: f(x,y) = (x-2)^2 + (y-3)^2
static double QuadraticObj(const std::vector<double> &x, std::vector<double> &grad, void *) {
    if (!grad.empty()) {
        grad[0] = 2.0 * (x[0] - 2.0);
        grad[1] = 2.0 * (x[1] - 3.0);
    }
    return (x[0] - 2.0) * (x[0] - 2.0) + (x[1] - 3.0) * (x[1] - 3.0);
}

// 制約: g(x,y) = x^2 + y^2 - 4 <= 0 (単位円内)
static double CircleConstraint(const std::vector<double> &x, std::vector<double> &grad, void *) {
    if (!grad.empty()) {
        grad[0] = 2.0 * x[0];
        grad[1] = 2.0 * x[1];
    }
    return x[0] * x[0] + x[1] * x[1] - 4.0;
}

static void UnconstrainedOpt() {
    std::cout << "=== Unconstrained Optimization ===\n";
    std::cout << "Minimize: f(x,y) = (x-2)^2 + (y-3)^2\n";

    nlopt::opt opt(nlopt::LD_LBFGS, 2);
    opt.set_min_objective(QuadraticObj, nullptr);
    opt.set_xtol_rel(1e-8);

    std::vector<double> x = {0.0, 0.0};
    double fmin;
    opt.optimize(x, fmin);

    std::cout << "x = " << x[0] << ", y = " << x[1] << "  (expected: 2, 3)\n";
    std::cout << "f = " << fmin << "\n";
}

static void ConstrainedOpt() {
    std::cout << "\n=== Constrained Optimization ===\n";
    std::cout << "Minimize f(x,y) = (x-2)^2 + (y-3)^2\n";
    std::cout << "Subject to: x^2 + y^2 <= 4\n";

    nlopt::opt opt(nlopt::LD_MMA, 2);
    opt.set_min_objective(QuadraticObj, nullptr);
    opt.add_inequality_constraint(CircleConstraint, nullptr, 1e-8);
    opt.set_xtol_rel(1e-8);

    std::vector<double> x = {0.5, 0.5};
    double fmin;
    opt.optimize(x, fmin);

    double r = std::sqrt(x[0] * x[0] + x[1] * x[1]);
    std::cout << "x = " << x[0] << ", y = " << x[1] << "\n";
    std::cout << "r = " << r << "  (expected ~2.0, on circle boundary)\n";
    std::cout << "f = " << fmin << "\n";
}

int main() {
    UnconstrainedOpt();
    ConstrainedOpt();
    return 0;
}
