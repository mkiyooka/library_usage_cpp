#include <cmath>
#include <iostream>

#include <Eigen/Dense>
#include <EigenRand/EigenRand>

int main() {
    std::cout << "=== EigenRand Example ===\n\n";

    Eigen::Rand::Vmt19937_64 urng{42};

    // 正規分布でランダム行列を生成
    Eigen::MatrixXf mat(4, 4);
    mat = Eigen::Rand::normalLike(mat, urng, 0.0f, 1.0f);
    std::cout << "Normal(0, 1) 4x4 matrix:\n" << mat << "\n\n";

    // 一様分布 [0, 1)
    Eigen::VectorXf vec(6);
    vec = Eigen::Rand::uniformRealLike(vec, urng);
    std::cout << "Uniform [0, 1) vector (6):\n" << vec.transpose() << "\n\n";

    // ベルヌーイ分布（確率0.3でtrue）
    Eigen::ArrayXf bern_f(8);
    bern_f = Eigen::Rand::bernoulli(bern_f, urng, 0.3);
    std::cout << "Bernoulli(p=0.3) samples (8):\n" << bern_f.transpose() << "\n\n";

    // 統計量の確認（大サンプル）
    Eigen::VectorXd large(10000);
    large = Eigen::Rand::normalLike(large, urng, 5.0, 2.0);
    double mean = large.mean();
    double std_dev = std::sqrt((large.array() - mean).square().mean());
    std::cout << "Normal(5, 2) n=10000:\n";
    std::cout << "  mean = " << mean << "  (expected: 5.0)\n";
    std::cout << "  std  = " << std_dev << "  (expected: 2.0)\n";

    return 0;
}
