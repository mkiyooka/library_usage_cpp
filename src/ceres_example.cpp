#include <cmath>
#include <iostream>

#include <ceres/ceres.h>

// コスト関数: f(x) = (x - 3)^2
struct QuadraticCostFunctor {
    template <typename T>
    bool operator()(const T *const x, T *residual) const {
        residual[0] = x[0] - T(3.0);
        return true;
    }
};

// ロゼンブロック関数: f(x,y) = (1-x)^2 + 100*(y-x^2)^2
struct RosenbrockCostFunctor {
    template <typename T>
    bool operator()(const T *const x, const T *const y, T *residual) const {
        residual[0] = T(1.0) - x[0];
        residual[1] = T(10.0) * (y[0] - x[0] * x[0]);
        return true;
    }
};

static void SimpleMinimization() {
    std::cout << "=== Simple Quadratic Minimization ===\n";
    std::cout << "Minimize: f(x) = (x - 3)^2\n";

    double x = 0.0;
    ceres::Problem problem;
    problem.AddResidualBlock(
        new ceres::AutoDiffCostFunction<QuadraticCostFunctor, 1, 1>(new QuadraticCostFunctor), nullptr, &x
    );

    ceres::Solver::Options options;
    options.minimizer_progress_to_stdout = false;

    ceres::Solver::Summary summary;
    ceres::Solve(options, &problem, &summary);

    std::cout << "Initial x = 0.0\n";
    std::cout << "Final x   = " << x << "  (expected: 3.0)\n";
    std::cout << "Cost: " << summary.final_cost << "\n";
}

static void RosenbrockMinimization() {
    std::cout << "\n=== Rosenbrock Minimization ===\n";
    std::cout << "Minimize: f(x,y) = (1-x)^2 + 100*(y-x^2)^2\n";

    double x = -1.0, y = 1.0;

    ceres::Problem problem;
    problem.AddResidualBlock(
        new ceres::AutoDiffCostFunction<RosenbrockCostFunctor, 2, 1, 1>(new RosenbrockCostFunctor), nullptr, &x, &y
    );

    ceres::Solver::Options options;
    options.minimizer_progress_to_stdout = false;
    options.max_num_iterations = 200;

    ceres::Solver::Summary summary;
    ceres::Solve(options, &problem, &summary);

    std::cout << "Final: x=" << x << " y=" << y << "  (expected: x=1 y=1)\n";
    std::cout << "Cost: " << summary.final_cost << "\n";
}

int main() {
    SimpleMinimization();
    RosenbrockMinimization();
    return 0;
}
