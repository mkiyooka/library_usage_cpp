#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

int main() {
    // デフォルトロガー（コンソール出力）の設定
    spdlog::set_level(spdlog::level::trace);
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e][%^%l%$][%n] %v");

    // デフォルトロガーを使った各ログレベルのデモ
    spdlog::trace("trace message");
    spdlog::debug("debug message");
    spdlog::info("information message");
    spdlog::warn("warning message");
    spdlog::error("error message");
    spdlog::critical("critical message");

    // 名前付きロガーの作成
    auto logger = spdlog::stdout_color_mt("app");
    logger->set_level(spdlog::level::debug);

    logger->debug("named logger: debug");
    logger->info("named logger: info with value {}", 42);
    logger->warn("named logger: warn");

    return 0;
}
