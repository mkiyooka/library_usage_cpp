#define ARMA_DONT_USE_WRAPPER
#include <iostream>

#include <armadillo>

static void BasicOps() {
    std::cout << "=== Basic Matrix Operations ===\n";

    arma::mat A = {
        {1, 2,  3},
        {4, 5,  6},
        {7, 8, 10}
    };
    arma::vec b = {1, 2, 3};

    std::cout << "A:\n" << A;
    std::cout << "b: " << b.t();
    std::cout << "A * b = " << (A * b).t();
    std::cout << "det(A) = " << arma::det(A) << "\n";
    std::cout << "trace(A) = " << arma::trace(A) << "\n";
}

static void LinearSolve() {
    std::cout << "\n=== Linear Solve (Ax = b) ===\n";

    arma::mat A = {
        { 2,  1, -1},
        {-3, -1,  2},
        {-2,  1,  2}
    };
    arma::vec b = {8, -11, -3};
    arma::vec x = arma::solve(A, b);

    std::cout << "x = " << x.t();
    std::cout << "residual: " << arma::norm(A * x - b) << "\n";
}

static void EigenDecomposition() {
    std::cout << "\n=== Eigenvalue Decomposition ===\n";

    arma::mat M = {
        {4, 1},
        {2, 3}
    };
    arma::cx_vec eigval;
    arma::cx_mat eigvec;
    arma::eig_gen(eigval, eigvec, M);

    std::cout << "Eigenvalues:  " << arma::real(eigval).t();
    std::cout << "Eigenvectors:\n" << arma::real(eigvec);
}

static void StatOps() {
    std::cout << "\n=== Statistics ===\n";

    arma::vec v = {3.0, 1.0, 4.0, 1.0, 5.0, 9.0, 2.0, 6.0};
    std::cout << "data: " << v.t();
    std::cout << "mean = " << arma::mean(v) << "\n";
    std::cout << "stddev = " << arma::stddev(v) << "\n";
    std::cout << "min = " << arma::min(v) << ", max = " << arma::max(v) << "\n";
}

int main() {
    BasicOps();
    LinearSolve();
    EigenDecomposition();
    StatOps();
    return 0;
}
