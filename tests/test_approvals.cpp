#include <sstream>
#include <vector>

// GTest integration と実装定義をここで行う（1TUのみ）
#define APPROVALS_GOOGLETEST
#include <ApprovalTests.v.10.13.0.hpp>

using namespace ApprovalTests;

TEST(ApprovalsTest, VectorOutput) {
    std::vector<int> v = {1, 2, 3, 4, 5};
    std::ostringstream oss;
    for (int x : v) {
        oss << x << "\n";
    }
    Approvals::verify(oss.str());
}

TEST(ApprovalsTest, CalculationResult) {
    std::ostringstream oss;
    for (int i = 1; i <= 5; ++i) {
        oss << i << " * " << i << " = " << i * i << "\n";
    }
    Approvals::verify(oss.str());
}
