#include <iostream>

#include <Eigen/Dense>

static void BasicMatrixOps() {
    std::cout << "=== Basic Matrix Operations ===\n";

    Eigen::Matrix3d A;
    A << 1, 2, 3, 4, 5, 6, 7, 8, 10;

    Eigen::Vector3d b(1, 2, 3);

    std::cout << "Matrix A:\n" << A << "\n\n";
    std::cout << "Vector b: " << b.transpose() << "\n\n";
    std::cout << "A * b = " << (A * b).transpose() << "\n";
    std::cout << "A.transpose():\n" << A.transpose() << "\n\n";
    std::cout << "det(A) = " << A.determinant() << "\n";
    std::cout << "trace(A) = " << A.trace() << "\n";
}

static void LinearSolve() {
    std::cout << "\n=== Linear System Solve (Ax = b) ===\n";

    Eigen::Matrix3d A;
    A << 2, 1, -1, -3, -1, 2, -2, 1, 2;

    Eigen::Vector3d b(8, -11, -3);
    Eigen::Vector3d x = A.lu().solve(b);

    std::cout << "x = " << x.transpose() << "  (expected: 2 3 -1)\n";
    std::cout << "residual: " << (A * x - b).norm() << "\n";
}

static void EigenDecomposition() {
    std::cout << "\n=== Eigenvalue Decomposition ===\n";

    Eigen::Matrix2d M;
    M << 4, 1, 2, 3;

    Eigen::EigenSolver<Eigen::Matrix2d> solver(M);
    std::cout << "Eigenvalues:  " << solver.eigenvalues().real().transpose() << "\n";
    std::cout << "Eigenvectors:\n" << solver.eigenvectors().real() << "\n";
}

static void SVD() {
    std::cout << "\n=== SVD ===\n";

    Eigen::MatrixXd A(3, 2);
    A << 1, 2, 3, 4, 5, 6;

    Eigen::JacobiSVD<Eigen::MatrixXd> svd(A, Eigen::ComputeThinU | Eigen::ComputeThinV);
    std::cout << "Singular values: " << svd.singularValues().transpose() << "\n";
}

int main() {
    BasicMatrixOps();
    LinearSolve();
    EigenDecomposition();
    SVD();
    return 0;
}
