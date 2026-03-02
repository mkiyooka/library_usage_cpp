#define ANKERL_NANOBENCH_IMPLEMENT
#include <nanobench.h>

#include <chrono>
#include <filesystem>
#include <string>

#include <quill/Backend.h>
#include <quill/Frontend.h>
#include <quill/LogMacros.h>
#include <quill/Logger.h>
#include <quill/sinks/FileSink.h>

#include <spdlog/async.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>

namespace {

constexpr const char* kSpdlogSyncFile  = "/tmp/bench_spdlog_sync.log";
constexpr const char* kSpdlogAsyncFile = "/tmp/bench_spdlog_async.log";
constexpr const char* kQuillFile       = "/tmp/bench_quill.log";

constexpr int kBatchSize = 1000;

} // namespace

int main() {
    // ──────────────────────────────────────────────────────────────
    // spdlog セットアップ
    // ──────────────────────────────────────────────────────────────
    auto spdlog_sync = spdlog::basic_logger_mt("spdlog_sync", kSpdlogSyncFile);
    spdlog_sync->set_level(spdlog::level::info);
    spdlog_sync->set_pattern("%v");

    spdlog::init_thread_pool(8192, 1);
    auto spdlog_async =
        spdlog::basic_logger_mt<spdlog::async_factory>("spdlog_async", kSpdlogAsyncFile);
    spdlog_async->set_level(spdlog::level::info);
    spdlog_async->set_pattern("%v");

    spdlog_sync->info("warmup");
    spdlog_sync->flush();
    spdlog_async->info("warmup");
    spdlog_async->flush();

    // ──────────────────────────────────────────────────────────────
    // quill セットアップ
    // ──────────────────────────────────────────────────────────────
    quill::BackendOptions backend_options;
    backend_options.sleep_duration = std::chrono::nanoseconds{0};
    quill::Backend::start(backend_options);

    auto quill_file_sink = quill::Frontend::create_or_get_sink<quill::FileSink>(
        kQuillFile,
        []() {
            quill::FileSinkConfig cfg;
            cfg.set_open_mode('w');
            return cfg;
        }(),
        quill::FileEventNotifier{}
    );
    quill::Logger* quill_logger =
        quill::Frontend::create_or_get_logger("bench", std::move(quill_file_sink));
    quill_logger->set_log_level(quill::LogLevel::Info);

    LOG_INFO(quill_logger, "warmup");
    quill_logger->flush_log();

    // ──────────────────────────────────────────────────────────────
    // ベンチマーク
    // ──────────────────────────────────────────────────────────────
    ankerl::nanobench::Bench bench;
    bench.title("Logger Benchmark").unit("log call").warmup(200);

    // ════════════════════════════════════════════════════════════════
    // セクション1: レイテンシ — 直接呼び出し（ベースライン）
    //   sync : 呼び出しスレッドがI/O完了まで待機
    //   async: エンキュー + flush（バックエンド完了まで待機）
    // ════════════════════════════════════════════════════════════════

    bench.minEpochIterations(100000);
    bench.run("spdlog  sync  [latency:direct] 1 msg", [&] {
        spdlog_sync->info("Benchmark message {}", 42);
    });

    bench.minEpochIterations(1000);
    bench.run("spdlog  async [latency:direct] 1 msg (enqueue + flush)", [&] {
        spdlog_async->info("Benchmark message {}", 42);
        spdlog_async->flush();
    });

    bench.minEpochIterations(1000);
    bench.run("quill   async [latency:direct] 1 msg (enqueue + flush)", [&] {
        LOG_INFO(quill_logger, "Benchmark message {}", 42);
        quill_logger->flush_log();
    });

    // ════════════════════════════════════════════════════════════════
    // セクション2: スループット — 直接呼び出し（バッチN件→flush）
    // ════════════════════════════════════════════════════════════════

    bench.batch(kBatchSize).minEpochIterations(100);
    bench.run(
        "spdlog  sync  [throughput:direct] batch " + std::to_string(kBatchSize) + " msgs + flush",
        [&] {
            for (int i = 0; i < kBatchSize; ++i) {
                spdlog_sync->info("Batch message {}", i);
            }
            spdlog_sync->flush();
        }
    );

    bench.minEpochIterations(3000);
    bench.run(
        "spdlog  async [throughput:direct] batch " + std::to_string(kBatchSize) + " msgs + flush",
        [&] {
            for (int i = 0; i < kBatchSize; ++i) {
                spdlog_async->info("Batch message {}", i);
            }
            spdlog_async->flush();
        }
    );

    bench.run(
        "quill   async [throughput:direct] batch " + std::to_string(kBatchSize) + " msgs + flush",
        [&] {
            for (int i = 0; i < kBatchSize; ++i) {
                LOG_INFO(quill_logger, "Batch message {}", i);
            }
            quill_logger->flush_log();
        }
    );

    quill_logger->flush_log();
    spdlog::drop_all();

    for (const char* f : {kSpdlogSyncFile, kSpdlogAsyncFile, kQuillFile}) {
        std::filesystem::remove(std::filesystem::path{f});
    }

    return 0;
}
