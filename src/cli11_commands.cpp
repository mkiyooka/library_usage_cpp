#include "option_parsing/cli11_commands.hpp"
#include <iostream>
#include <string>

namespace cli11_example {

void SetupCallbackCommands(CLI::App &app) {
    // processサブコマンド (コールバック方式)
    auto *process_cmd = app.add_subcommand("process", "ファイル処理");

    static std::string input;
    static std::string output;
    static bool verbose = false;

    process_cmd->add_option("-i,--input", input, "入力ファイル")->required();
    process_cmd->add_option("-o,--output", output, "出力ファイル")->required();
    process_cmd->add_flag("-v,--verbose", verbose, "詳細出力");

    process_cmd->callback([]() {
        std::cout << "Process: " << input << " -> " << output;
        if (verbose) {
            std::cout << " (verbose)";
        }
        std::cout << "\n";
    });

    // serverサブコマンド (コールバック方式)
    auto *server_cmd = app.add_subcommand("server", "サーバー起動");

    static int port = 8080;

    server_cmd->add_option("-p", port, "ポート番号");
    server_cmd->callback([]() { std::cout << "Server started on port " << port << "\n"; });
}

CLI::App *SetupManualCommands(CLI::App &app) {
    // buildサブコマンド (手動実行方式)
    auto *build_cmd = app.add_subcommand("build", "ビルド実行");

    static std::string type = "Release";
    static int jobs = 1;

    build_cmd->add_option("--type", type, "ビルドタイプ");
    build_cmd->add_option("-j", jobs, "並列数");

    // 手動実行用にポインタを返す
    return build_cmd;
}

} // namespace cli11_example
