#include <iostream>

#include <taskflow/algorithm/for_each.hpp>
#include <taskflow/taskflow.hpp>

int main() {
    std::cout << "=== Taskflow Example ===\n\n";

    tf::Executor executor;
    tf::Taskflow taskflow("example");

    // 基本的なDAGタスクグラフ
    auto [A, B, C, D] = taskflow.emplace(
        [] { std::cout << "Task A\n"; },
        [] { std::cout << "Task B\n"; },
        [] { std::cout << "Task C\n"; },
        [] { std::cout << "Task D (depends on B, C)\n"; });

    A.name("A");
    B.name("B");
    C.name("C");
    D.name("D");

    A.precede(B, C);  // A → B, A → C
    D.succeed(B, C);  // B,C → D

    std::cout << "-- Simple DAG --\n";
    executor.run(taskflow).wait();

    // parallel_for: データ並列
    tf::Taskflow pf_flow;
    std::vector<int> data(8, 0);
    pf_flow.for_each_index(0, 8, 1, [&](int i) { data[i] = i * i; });
    executor.run(pf_flow).wait();
    std::cout << "\nparallel squares: ";
    for (int x : data) std::cout << x << " ";
    std::cout << "\n";

    // 条件付き実行 (condition task)
    tf::Taskflow cond_flow;
    int counter = 0;
    auto init = cond_flow.emplace([&] { counter = 0; }).name("init");
    auto cond = cond_flow.emplace([&]() -> int {
                               ++counter;
                               return counter < 3 ? 0 : 1;  // 0=ループ, 1=終了
                           }).name("cond");
    auto loop_body = cond_flow.emplace([&] {
                                  std::cout << "  loop iteration " << counter << "\n";
                              }).name("body");
    auto done = cond_flow.emplace([] { std::cout << "  done\n"; }).name("done");

    init.precede(cond);
    cond.precede(loop_body, done);
    loop_body.precede(cond);

    std::cout << "\nCondition task (loop 3x):\n";
    executor.run(cond_flow).wait();

    return 0;
}
