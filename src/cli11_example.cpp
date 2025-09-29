#include "option_parsing/cli11_commands.hpp"
#include <CLI/CLI.hpp>
#include <iostream>

int main(int argc, char **argv) {
    CLI::App app{"CLI11 Example"};

    // コールバック方式のサブコマンド設定
    cli11_example::SetupCallbackCommands(app);

    // 手動実行方式のサブコマンド設定
    CLI::App *build_cmd = cli11_example::SetupManualCommands(app);

    // コマンドライン解析
    try {
        app.parse(argc, argv);
    } catch (const CLI::ParseError &e) {
        return app.exit(e);
    }

    // 手動実行方式の処理
    if (build_cmd->parsed()) {
        std::cout << "Build command executed manually\n";
    }

    // サブコマンドが指定されていない場合はヘルプを表示
    if (app.get_subcommands().empty()) {
        std::cout << app.help();
    }

    return 0;
}
