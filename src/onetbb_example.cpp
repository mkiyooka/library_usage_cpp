#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

#include <tbb/parallel_for.h>
#include <tbb/parallel_reduce.h>
#include <tbb/parallel_sort.h>
#include <tbb/task_arena.h>

int main() {
    std::cout << "=== oneTBB Example ===\n\n";

    const int N = 1'000'000;
    std::vector<double> v(N);

    // parallel_for: インデックス範囲を並列に処理
    tbb::parallel_for(tbb::blocked_range<size_t>(0, N), [&](const tbb::blocked_range<size_t>& r) {
        for (size_t i = r.begin(); i < r.end(); ++i) {
            v[i] = static_cast<double>(i) * 0.000001;
        }
    });
    std::cout << "parallel_for: v[500000] = " << v[500000] << "\n";

    // parallel_reduce: 並列合計
    double sum = tbb::parallel_reduce(
        tbb::blocked_range<size_t>(0, N), 0.0,
        [&](const tbb::blocked_range<size_t>& r, double init) {
            for (size_t i = r.begin(); i < r.end(); ++i) {
                init += v[i];
            }
            return init;
        },
        std::plus<double>{});
    std::cout << "parallel_reduce sum = " << sum << " (expected ~500000)\n";

    // parallel_sort
    std::vector<int> nums(20);
    for (int i = 0; i < 20; ++i) nums[i] = 20 - i;
    tbb::parallel_sort(nums.begin(), nums.end());
    std::cout << "parallel_sort: ";
    for (int x : nums) std::cout << x << " ";
    std::cout << "\n";

    // task_arena: スレッド数制限
    tbb::task_arena arena(2);
    arena.execute([&] {
        std::cout << "task_arena(2) parallel_for done\n";
        tbb::parallel_for(0, 4, [](int i) { (void)i; });
    });

    return 0;
}
