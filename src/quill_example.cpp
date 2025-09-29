#include <thread>

#include <quill/Backend.h>
#include <quill/Frontend.h>
#include <quill/LogMacros.h>
#include <quill/Logger.h>
#include <quill/sinks/ConsoleSink.h>

int main() {
    // Quillバックエンドの開始
    quill::BackendOptions backend_options;
    quill::Backend::start(backend_options);

    // 独自フォーマットのデモ
    auto console_meta_sink = quill::Frontend::create_or_get_sink<quill::ConsoleSink>("meta_sink_1");
    auto console_main_sink = quill::Frontend::create_or_get_sink<quill::ConsoleSink>("sink_id_1");

    const auto pattern_formatter_meta = quill::PatternFormatterOptions{
        R"__([%(log_level_short_code)][%(logger)]{{"start":"%(time)",%(message)}})__", "%Y-%m-%d %H:%M:%S.%Qus",
        quill::Timezone::GmtTime
    };
    const auto pattern_formatter_main =
        quill::PatternFormatterOptions{"[%(log_level_short_code)][%(logger)]%(message)"};

    quill::Logger *logger_meta =
        quill::Frontend::create_or_get_logger("M", std::move(console_meta_sink), pattern_formatter_meta);
    quill::Logger *logger_main =
        quill::Frontend::create_or_get_logger("R", std::move(console_main_sink), pattern_formatter_main);

    // JSON形式の構造化ログ
    LOG_INFO(logger_meta, R"("message":{},"var2":{})", "\"start\"", 42);

    // 通常のログ
    LOG_INFO(logger_main, "information message");
    LOG_WARNING(logger_main, "warning message");
    LOG_CRITICAL(logger_main, "critical message");
    LOG_ERROR(logger_main, "error message");

    // JSON形式の終了ログ
    LOG_INFO(logger_meta, R"("message":{},"var2":{})", "\"finish\"", 43);

    // 少し待ってからアプリケーション終了（ログ出力完了のため）
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    return 0;
}
